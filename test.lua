SSID="SSID"
Password="Password"
IP='192.168.0.2'
 
wifi.setmode(wifi.STATION)
wifi.sta.config(SSID,Password)
humi="XX"
temp="XX"
PIN = 4
 
function sendData()
dht22 = require("dht22")
dht22.read(PIN)
t = dht22.getTemperature()
h = dht22.getHumidity()
humi=(h/10).."."..(h%10)
temp=(t/10).."."..(t%10)
dht22 = nil
package.loaded["dht22"]=nil
conn=net.createConnection(net.TCP, 0) 
conn:on("receive", function(conn, payload) print(payload) end)
conn:connect(80,IP) 
conn:send("GET /dht22.php?temp="..temp.."&humi="..humi.." HTTP/1.1\r\n") 
conn:send("Host: IP\r\n") 
conn:send("Accept: */*\r\n") 
conn:send("User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n")
conn:send("\r\n")
conn:on("sent",function(conn)
conn:close()
end)
conn:on("disconnection", function(conn)
end)
end
tmr.alarm(2, 12000, 1, function() sendData() end )