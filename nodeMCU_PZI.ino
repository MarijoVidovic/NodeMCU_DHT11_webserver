#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "DHT.h"

#define DHTPIN 4     // what digital pin the DHT11 is conected to
#define DHTTYPE DHT11   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);
 
// Replace with your network credentials
const char* ssid = "Your SSID";
const char* password = "Your wifi password";

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
 
String page = "";
String text = "";
String data;
float temp_c;
float humidity;

void setup(void){
 delay(1000);
 Serial.begin(115200);
 WiFi.begin(ssid, password); //begin WiFi connection
 Serial.println("");
 
 // Wait for connection
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
}
 
 Serial.println("");
 Serial.print("Connected to ");
 Serial.println(ssid);
 Serial.print("IP address: ");
 Serial.println(WiFi.localIP());
 server.on("/data.txt", [](){
   text = data;
   server.send(200, "text/html", text);
 });
 server.on("/", [](){
   page = "<!DOCTYPE html> <html>\n";
   page +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
   page +="<title>ESP8266 Weather Report</title>\n";
   page +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
   page +="body{margin-top: 50px; background-color: lightblue;} h1 {color: #444444;margin: 50px auto 30px;}\n";
   page +="h3 {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
   page +="</style>\n";
   page +="</head>\n";
   page +="<body>\n";
   page +="<div id=\"webpage\">\n";
   page +="<h1>ESP8266 NodeMCU Weather Report</h1>\n";
   page += "<h3 id=\"data\">""</h3>\r\n";
   page += "<script>\r\n";
   page += "var x = setInterval(function() {loadData(\"data.txt\", updateData)}, 4000);\r\n"; // Javascript set interval function
   page += "function loadData(url, callback){\r\n";  // Start of AJAX
   page += "var xhttp = new XMLHttpRequest();\r\n"; // Create XMLHttpRequest object
   page += "xhttp.onreadystatechange = function(){\r\n"; 
   page += " if(this.readyState == 4 && this.status == 200){\r\n"; // Waiting 4(done) and 200(OK) before updating elements via callback updateData
   page += " callback.apply(xhttp);\r\n"; // To make web page responsive while there is panding request
   page += " }\r\n";
   page += "};\r\n";
   page += "xhttp.open(\"GET\", url, true);\r\n";
   page += "xhttp.send();\r\n";
   page += "}\r\n";
   page += "function updateData(){\r\n";
   page += " document.getElementById(\"data\").innerHTML = this.responseText;\r\n"; // Updating elements 
   page += "}\r\n"; // End of AJAX
   page += "</script>\r\n"; 
   page +="</div>\n";
   page +="</body>\n";
   page +="</html>\n";
   server.send(200, "text/html", page);
});
 
 server.begin();
 Serial.println("Web server started!");
}
 
void loop(void){
 temp_c = dht.readTemperature();
 humidity = dht.readHumidity();
 data = "Temperature: " + (String)temp_c + " ℃ Humidity: " + (String)humidity + " %";
 Serial.print(data);
 delay(1000);
 server.handleClient();
}
