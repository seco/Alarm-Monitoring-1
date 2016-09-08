-- NetworX Security System Monitoring with ESP-07
-- February 11, 2015 tgmaxx
-- August 23, 2016 - onAux delay at 10msec

print("LUA Interpreter started")

broker = "192.168.0.252"  
port = 1883

gpio.mode(1,gpio.OUTPUT)
gpio.mode(6,gpio.INT,gpio.PULLUP)
gpio.mode(7,gpio.INT,gpio.PULLUP)
gpio.mode(5,gpio.INT,gpio.PULLUP)
gpio.mode(2,gpio.INT,gpio.PULLUP)

chipid = node.chipid()
m = mqtt.Client(chipid, 120, "user", "password")
m:lwt("NX8V2", "offline", 0, 0)

m:on("offline", function(con)   
    print ("MQTT reconnecting...")   
    tmr.alarm(1, 10000, 1, function()  
      m:connect(broker, port, 0, function(conn)   
        tmr.stop(1)
		print("MQTT connected to " .. broker)  
        mqtt_sub()
      end)  
    end)  
end)

-- on message receive event
m:on("message", function(conn, topic, data)
  print(topic .. ":" )
  if data ~= nil then
    print(data)
  end
  if  data == "On" then
     gpio.write(1,gpio.LOW)
  end
  if  data == "Off" then
     gpio.write(1,gpio.HIGH)
  end
 
end)

function onAux(index, level)
  tmr.alarm(2, 10, tmr.ALARM_SINGLE, function()
    m:publish("NX8V2", "AUX" .. index, 0, 0)
  end)
end

gpio.trig(6, "down", function(level) onAux(1,level) end)
gpio.trig(7, "down", function(level) onAux(2,level) end)
gpio.trig(5, "down", function(level) onAux(3,level) end)
gpio.trig(2, "down", function(level) onAux(4,level) end)

function mqtt_sub()  
    m:subscribe("espLED/#",0, function(conn) print("Subscribe success") end) 
end

tmr.alarm(0, 1000, 1, function()
	if wifi.sta.status() == 5 then  -- status 5 means connected
		tmr.stop(0)
		m:connect(broker, port, 0, function(conn)
			print("MQTT connected to " .. broker)
            mqtt_sub()
		end)
	end
end)