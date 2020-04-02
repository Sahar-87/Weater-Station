#include <Arduino.h>
#include <ESP8266WebServer.h> /*Arduino library.Supports only one simultaneous client, knows how to handle GET and POST*/
#include <ESP8266WiFi.h>/* esp8266 Wifi support Based on WiFi.h from Arduino WiFi shield library.*/
#include "dht.h"/*support sensors */
#define DHT11_PIN D4 // Defining pin D4 as sensor data pin
ESP8266WebServer server; // Creating object from webserver class
dht DHT; // Creating object from dht class

// Declaring functions //

float fetchSensorTempData()
{
  int chk = DHT.read11(DHT11_PIN); // Read data from the sensor and check for errors if any.
  switch (chk)
  {
    case DHTLIB_OK:  
		Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.print("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.print("Time out error,\t"); 
		break;
    case DHTLIB_ERROR_CONNECT:
        Serial.print("Connect error,\t");
        break;
    case DHTLIB_ERROR_ACK_L:
        Serial.print("Ack Low error,\t");
        break;
    case DHTLIB_ERROR_ACK_H:
        Serial.print("Ack High error,\t");
        break;
    default: 
		Serial.print("Unknown error,\t"); 
		break;
  }
  Serial.println(DHT.temperature, 2);
  return DHT.temperature;
}

float fetchSensorHumidityData()
{
  int chk = DHT.read11(DHT11_PIN); // Read data from the sensor and check for errors if any.
  switch (chk)
  {
    case DHTLIB_OK:  
		//Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.print("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.print("Time out error,\t"); 
		break;
    case DHTLIB_ERROR_CONNECT:
        Serial.print("Connect error,\t");
        break;
    case DHTLIB_ERROR_ACK_L:
        Serial.print("Ack Low error,\t");
        break;
    case DHTLIB_ERROR_ACK_H:
        Serial.print("Ack High error,\t");
        break;
    default: 
		Serial.print("Unknown error,\t"); 
		break;
  }
  
  return DHT.humidity;
}


void getTemperature()
{
  server.send(200, "text/plain", String(fetchSensorTempData()));
}

void getHumidity()
{
  server.send(200, "text/plain", String(fetchSensorHumidityData()));
}


// Webpage
char HOME_PAGE[] PROGMEM = R"=====(
  <!DOCTYPE html>
<html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    
    <meta name="viewport" content="width=device-width,initial-scale=1">
    <meta http-equiv="Cache-Control" content="no-cache, no-store, must-revalidate">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Expires" content="0">
    <title>Weather Station</title>
    <style type="text/css">
   body {
       background-color: gray;
       font-family: Arial, Helvetica, sans-serif;
   }
   .mainview {
    text-align: center
   }
   .content {
       display: inline-block;
       width: 400px;
       height: 250px;
       position: relative;
       right: 0px;
       left: -2.5px;
       top: 50px;
       border-radius: 20px;
       background-color: white;
   }

   </style>
   </head>
   <body onload="load()" cz-shortcut-listen="true">
        <div class="mainview">            
            <div class="content" id="content">
                <div id="title" style="position: fixed; top: 60px; width: 400px; height: 60px; z-index: 3;">             
                    <h3>Weather Station</h3>    
                    <hr width="398px" class="anchor" style="color: rgb(0, 0, 0); top: 50px;">               
                </div>
                <div>
                    <h3 style="position: absolute; top: 90px; left: 30px; color: gray;">Temperature: </h3>
                    <h3 style="position: absolute; top: 90px; right: 100px; color: rgb(66, 66, 66);" id="temp">00</h3>
                    <h3 style="position: absolute; top: 90px; right: 82px; color: rgb(66, 66, 66);">C</h3>
                    <h3 style="position: absolute; top: 150px; left: 30px; color: gray;">Humidity: </h3>
                    <h3 style="position: absolute; top: 150px; right: 100px; color: rgb(66, 66, 66);" id="humidity">00</h3>
                    <h3 style="position: absolute; top: 150px; right: 80px; color: rgb(66, 66, 66);">%</h3>
                       
                </div>
                
            </div>
        </div>
    <script>
    function load() {
        getValues();
        setInterval(getValues(), 10000);
    }
    
    function getValues() {
        var temp = document.getElementById("temp");
        var humidity = document.getElementById("humidity");
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            temp.innerText = parseInt(this.responseText, 10);
        }
        };
        xhttp.open("GET", "/gettemperature", true);
        xhttp.send();
        setTimeout(function(){

  var xhttp_ = new XMLHttpRequest();
        xhttp_.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            humidity.innerText = parseInt(this.responseText, 10);
        }
        };
        xhttp_.open("GET", "/gethumidity", true);
        xhttp_.send();
            
        }, 2000);
        
    }
    </script>       
</body>
</html>
)=====";

void setup()//debug function
{
  Serial.begin(9600);
  delay(5000);//delay 5 second 

  server.on("/",[](){server.send_P(200,"text/html",HOME_PAGE);});//this root to back homepage that store in flash if connect 200 its ok.
  server.on("/gettemperature", getTemperature);
  server.on("/gethumidity", getHumidity);
  server.begin();
  //WiFi.begin("Dr. Mohammed - Gazalnet", "muh171956");
  WiFi.begin("weatherap", "12345678");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  delay(200);
}

void loop()
{
  server.handleClient();
}
