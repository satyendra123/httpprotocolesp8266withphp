// this is the working code for http post request in esp32 in micropython. and this code is in working condition. it sends the back the response in json format. and when relay: OPENEN is getting in the response the builtin led will be turned on
from machine import Pin
from time import sleep
import network
import urequests as requests
import ujson as json

device_id = "Device0001"
ssid = "Airtel_tejv_3002"
password = "air73137"

DHT_Temperature = 22.4
DHT_Humidity = 34

HTTPS_POST_URL = "http://192.168.1.16:1880/update-sensor/"
HTTPS_GET_URL = "http://192.168.1.16:1880/get-sensor/"

LED_PIN = 2 # GPIO 2 for the built-in LED

def setup():
    global led
    led = Pin(LED_PIN, Pin.OUT)
    setup_wifi()

def loop():
    jsonTemperature = {
        "device_id": device_id,
        "type": "Temperature",
        "value": DHT_Temperature
    }

    HTTPS_POST(HTTPS_POST_URL, jsonTemperature)

    jsonHumidity = {
        "device_id": device_id,
        "type": "Humidity",
        "value": DHT_Humidity
    }

    HTTPS_POST(HTTPS_POST_URL, jsonHumidity)

    HTTPS_GET(HTTPS_GET_URL)

    sleep(1)

def setup_wifi():
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        print('Connecting to network...')
        sta_if.active(True)
        sta_if.connect(ssid, password)
        while not sta_if.isconnected():
            pass
    print('Network config:', sta_if.ifconfig())

def HTTPS_POST(HTTPS_POST_URL, data):
    print("\nPosting to:", HTTPS_POST_URL)
    print("PostPacket:", data)

    print("Connecting to server...")
    try:
        response = requests.post(HTTPS_POST_URL, json=data)
        if response.status_code == 200:
            print("ServerResponse:", response.text)
            jsonResponse = response.json()
            if jsonResponse.get("relay") == "OPENEN":
                led.on()
                sleep(1)
                led.off()
        else:
            print("Failed to POST. Error:", response.text)
    except Exception as e:
        print("Exception occurred:", e)


def HTTPS_GET(HTTPS_GET_URL):
    print("\nGetting from:", HTTPS_GET_URL)

    print("Connecting to server...")
    try:
        response = requests.get(HTTPS_GET_URL)
        if response.status_code == 200:
            print("ServerResponse:", response.text)
        else:
            print("Failed to GET. Error:", response.text)
    except Exception as e:
        print("Exception occurred:", e)

setup()
while True:
    loop()

# this is the code for the raspberry pi with same working process
import RPi.GPIO as GPIO
import time
import requests
import json

device_id = "Device0001"
ssid = "Airtel_tejv_3002"
password = "air73137"

DHT_Temperature = 22.4
DHT_Humidity = 34

HTTPS_POST_URL = "http://192.168.1.16:1880/update-sensor/"
HTTPS_GET_URL = "http://192.168.1.16:1880/get-sensor/"

LED_PIN = 2 # GPIO 2 for the built-in LED

def setup():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(LED_PIN, GPIO.OUT)
    setup_wifi()

def loop():
    jsonTemperature = {
        "device_id": device_id,
        "type": "Temperature",
        "value": DHT_Temperature
    }

    HTTPS_POST(HTTPS_POST_URL, jsonTemperature)

    jsonHumidity = {
        "device_id": device_id,
        "type": "Humidity",
        "value": DHT_Humidity
    }

    HTTPS_POST(HTTPS_POST_URL, jsonHumidity)

    HTTPS_GET(HTTPS_GET_URL)

    time.sleep(1)

def setup_wifi():
    # You'll need to implement WiFi setup for Raspberry Pi.
    # This can be done using the `wifi` or `wpa_supplicant` libraries in Python.
    pass

def HTTPS_POST(HTTPS_POST_URL, data):
    print("\nPosting to:", HTTPS_POST_URL)
    print("PostPacket:", data)

    print("Connecting to server...")
    try:
        response = requests.post(HTTPS_POST_URL, json=data)
        if response.status_code == 200:
            print("ServerResponse:", response.text)
            jsonResponse = response.json()
            if jsonResponse.get("relay") == "OPENEN":
                GPIO.output(LED_PIN, GPIO.HIGH)
                time.sleep(1)
                GPIO.output(LED_PIN, GPIO.LOW)
        else:
            print("Failed to POST. Error:", response.text)
    except Exception as e:
        print("Exception occurred:", e)

def HTTPS_GET(HTTPS_GET_URL):
    print("\nGetting from:", HTTPS_GET_URL)

    print("Connecting to server...")
    try:
        response = requests.get(HTTPS_GET_URL)
        if response.status_code == 200:
            print("ServerResponse:", response.text)
        else:
            print("Failed to GET. Error:", response.text)
    except Exception as e:
        print("Exception occurred:", e)

setup()
while True:
    loop()

//EXAMPLE-1 in this we are using the arduinojson library for sending the pkt. this is our code and this is working also builtin led is turned on
//isme humne do response send kiya hai. agar relay: openen and message:success but we will only read the relay. and if it is OPENEN then turn on the relay
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

WiFiClient client;
HTTPClient http;

String device_id = "Device0001";
String ssid = "Airtel_tejv_3002";
String password = "air73137";

float DHT_Temperature = 22.4;
float DHT_Humidity = 34;

String HTTPS_POST_URL = "http://192.168.1.16:1880/update-sensor/";
String HTTPS_GET_URL = "http://192.168.1.16:1880/get-sensor/";

const int LED_PIN = 2; // GPIO 2 for the built-in LED

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  setup_wifi();
}

void loop() {

  // Create JSON object for temperature data
  StaticJsonDocument<200> jsonTemperature;
  jsonTemperature["device_id"] = device_id;
  jsonTemperature["type"] = "Temperature";
  jsonTemperature["value"] = DHT_Temperature;

  // Serialize JSON object to a String
  String pkt;
  serializeJson(jsonTemperature, pkt);

  HTTPS_POST(HTTPS_POST_URL, pkt);

  // Create JSON object for humidity data
  StaticJsonDocument<200> jsonHumidity;
  jsonHumidity["device_id"] = device_id;
  jsonHumidity["type"] = "Humidity";
  jsonHumidity["value"] = DHT_Humidity;

  // Serialize JSON object to a String
  String pkt2;
  serializeJson(jsonHumidity, pkt2);

  HTTPS_POST(HTTPS_POST_URL, pkt2);

  HTTPS_GET(HTTPS_GET_URL);

  delay(1000);
}

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void HTTPS_POST(String HTTPS_POST_URL, String PostPacket) 
{
  Serial.println("\nPosting to: " + HTTPS_POST_URL);
  Serial.println("PostPacket: " + PostPacket);

  Serial.println("Connecting to server..");
  if (http.begin(client, HTTPS_POST_URL)) {
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(PostPacket);
    if (httpCode > 0) {
      Serial.println("httpCode: " + String(httpCode));
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
        String ServerResponse = http.getString();
        Serial.println("ServerResponse: " + ServerResponse);
        // Parse the JSON response
        StaticJsonDocument<200> jsonResponse;
        deserializeJson(jsonResponse, ServerResponse);
        // Check if the relay is "OPENEN" and turn on the LED
        if (jsonResponse["relay"] == "OPENEN") {
          digitalWrite(LED_PIN, HIGH); // Turn on the LED
          delay(1000); // Wait for 1 second
          digitalWrite(LED_PIN, LOW); // Turn off the LED
        }
      }
    } else {
      Serial.print("Failed to POST. Error: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }
  } else {
    Serial.print("Failed to connect to server");
  }
  http.end();
}

void HTTPS_GET(String HTTPS_GET_URL) 
{
  Serial.println("\nGetting from: " + HTTPS_GET_URL);

  Serial.println("Connecting to server..");
  if (http.begin(client, HTTPS_GET_URL)) {
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println("httpCode: " + String(httpCode));
      if (httpCode == HTTP_CODE_OK) {
        String ServerResponse = http.getString();
        Serial.println("\nServerResponse: " + ServerResponse);
      }
    } else {
      Serial.print("Failed to GET. Error: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }
  } else {
    Serial.print("Failed to connect to server");
  }
  http.end();
}


/*
// EXAMPLE-2 ye code bhi humara work kar rha hai is code me humne arduinojson library ka use nahi kiya hai. aur apne se packet banaya hai jaise hum postman se send karte hai waise.
#include <WiFi.h>
#include <HTTPClient.h>

WiFiClient client;
HTTPClient http;

String device_id = "Device0001";
String ssid = "Airtel_tejv_3002";
String password = "air73137";

float DHT_Temperature = 22.4;
float DHT_Humidity = 34;

String HTTPS_POST_URL = "http://192.168.1.16:1880/update-sensor/";
String HTTPS_GET_URL = "http://192.168.1.16:1880/get-sensor/";

const int LED_PIN = 2; // GPIO 2 for the built-in LED

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  setup_wifi();
}

void loop() {

  String pkt = "{";
  pkt += "\"device_id\": \"" + device_id + "\", ";
  pkt += "\"type\": \"Temperature\", ";
  pkt += "\"value\": " + String(DHT_Temperature) + "";
  pkt += "}";
  HTTPS_POST(HTTPS_POST_URL, pkt);

  String pkt2 = "{";
  pkt2 += "\"device_id\": \"" + device_id + "\", ";
  pkt2 += "\"type\": \"Humidity\", ";
  pkt2 += "\"value\": " + String(DHT_Humidity) + "";
  pkt2 += "}";
  HTTPS_POST(HTTPS_POST_URL, pkt2);

  HTTPS_GET(HTTPS_GET_URL);

  delay(1000);
}

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void HTTPS_POST(String HTTPS_POST_URL, String PostPacket) 
{
  Serial.println("\nPosting to: " + HTTPS_POST_URL);
  Serial.println("PostPacket: " + PostPacket);

  Serial.println("Connecting to server..");
  if (http.begin(client, HTTPS_POST_URL)) {
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(PostPacket);
    if (httpCode > 0) {
      Serial.println("httpCode: " + String(httpCode));
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
        String ServerResponse = http.getString();
        Serial.println("ServerResponse: " + ServerResponse);
        // Check if the response contains "success" and turn on the LED
        if (ServerResponse.indexOf("success") != -1) {
          digitalWrite(LED_PIN, HIGH); // Turn on the LED
          delay(1000); // Wait for 1 second
          digitalWrite(LED_PIN, LOW); // Turn off the LED
        }
      }
    } else {
      Serial.print("Failed to POST. Error: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }
  } else {
    Serial.print("Failed to connect to server");
  }
  http.end();
}

void HTTPS_GET(String HTTPS_GET_URL) 
{
  Serial.println("\nGetting from: " + HTTPS_GET_URL);

  Serial.println("Connecting to server..");
  if (http.begin(client, HTTPS_GET_URL)) {
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println("httpCode: " + String(httpCode));
      if (httpCode == HTTP_CODE_OK) {
        String ServerResponse = http.getString();
        Serial.println("\nServerResponse: " + ServerResponse);
      }
    } else {
      Serial.print("Failed to GET. Error: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }
  } else {
    Serial.print("Failed to connect to server");
  }
  http.end();
}

*/
/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
/*
// this is the working code
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Airtel_tejv_3002";
const char* password = "air73137";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://192.168.1.16:1880/update-sensor";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
    
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
      
      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";
      String httpRequestData = "sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";             
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      
      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
*/

// this is my flask code which receives the data
'''
from flask import Flask, request

app = Flask(__name__)

@app.route('/update-sensor/', methods=['POST'])
def update_sensor():
    data = request.form
    print("Received data:")
    for key, value in data.items():
        print(f"{key}: {value}")
    return "Data received", 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=1880)

'''

from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/update-sensor/', methods=['POST'])
def update_sensor():
    data = request.json
    device_id = data.get('device_id')
    sensor_type = data.get('type')
    value = data.get('value')

    temperature = None
    humidity = None

    if device_id and sensor_type and value:
        if sensor_type == 'Temperature':
            temperature = value
        elif sensor_type == 'Humidity':
            humidity = value

        print(f"Received {sensor_type} data from device {device_id}: {value}")

        return jsonify({
            'message': 'Data received',
            'temperature': temperature,
            'humidity': humidity
        }), 200
    else:
        return jsonify({'error': 'Invalid data format'}), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=1880)
