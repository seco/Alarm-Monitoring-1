if wifi.sta.status() ~= 5 then tmr.alarm(6, 10000,0, function(d) dofile('init.lua') end) return end
if wifi.sta.status() == 5 then dofile('MQTT-alarm-ESP07.lua') return end
