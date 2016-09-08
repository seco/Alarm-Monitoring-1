//----------------------------------------------------------------
// NetworX Security System Monitoring with ESP8266
// Tested on ESP-07, NodeMCU v1.0 and WeMos Mini D1
// February 15, 2015 tgmaxx
// August 23, 2016 - onAux delays to 10msec
//
// by tgmaxx  <tgmaxx [at] gmail [dot] com>
//----------------------------------------------------------------

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define ledPin 5
#define aux1Pin 12
#define aux2Pin 13
#define aux3Pin 14
#define aux4Pin 4

const char* wifi_ssid = "your router ssid";
const char* wifi_password = "your router password";
const char* mqtt_server = "your mqtt server ip address";
const char* mqtt_topic = "NX8V2";

WiFiClient espClient;
PubSubClient client(espClient);

char msgBuff[10];

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// handles messages arriving on subscribed topic
void callback(char* topic, byte* payload, unsigned int length) {
  int i;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msgBuff[i] = payload[i];
  }
  Serial.println();
  msgBuff[i] = '\0';
  if (!(strncmp(msgBuff, "On", 2))) {
    digitalWrite(ledPin, LOW);   // Turn the LED on
  } else if (!(strncmp(msgBuff, "Off", 3))){
    digitalWrite(ledPin, HIGH);  // Turn the LED off
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("espAlarm", mqtt_topic, 0, 0, "offline")) { 
      // client.publish(mqtt_topic, "Connected to MQTT broker");
      Serial.println("connected");
      if(client.subscribe("espLED")) {
        // client.publish(mqtt_topic, "subscribed to espLED topic");
        Serial.println("Subscribed to espLED topic");
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void msDelay(int x)   {
  for(unsigned int i=0; i<=x; i++)   
  {
    delayMicroseconds(1000);
  }
}

void onAux1() {
  client.publish(mqtt_topic, "AUX1"); // Burglar Alarm
  msDelay(10);
}

void onAux2() {
  client.publish(mqtt_topic, "AUX2"); // Fire Alarm
  msDelay(10);
}

void onAux3() {
  client.publish(mqtt_topic, "AUX3"); // System Armed
  msDelay(10);
}

void onAux4() {
  client.publish(mqtt_topic, "AUX4"); // System Disarmed
  msDelay(10);
}

void setup() {
    pinMode(ledPin,OUTPUT);
    digitalWrite(ledPin, HIGH);  // Turn the LED off
    pinMode(aux1Pin,INPUT_PULLUP);
    pinMode(aux2Pin,INPUT_PULLUP);
    pinMode(aux3Pin,INPUT_PULLUP);
    pinMode(aux4Pin,INPUT_PULLUP);
    attachInterrupt(aux1Pin, onAux1, FALLING);
    attachInterrupt(aux2Pin, onAux2, FALLING);
    attachInterrupt(aux3Pin, onAux3, FALLING);
    attachInterrupt(aux4Pin, onAux4, FALLING);
    Serial.begin(57600);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
