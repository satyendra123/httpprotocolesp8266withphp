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

/* 
python manage.py createapp SchoolProject
cd SchoolProject
python manage.py startapp StudentApp
python manage.py startapp esp32_api

step-0 'corsheaders', 'rest_framework', 'StudentApp.apps.StudentappConfig', 'esp32_api.apps.Esp32ApiConfig', DATABASES = { 'default': { 'ENGINE': 'django.db.backends.mysql', 'NAME': 'sms','USER': 'root', 'PASSWORD': '',} }
        CORS_ORIGIN_ALLOW_ALL = True
        CORS_ALLOW_ALL_HEADERS=True

#this all is for the application StudentApp
Step-1 this is apps.py code

from django.apps import AppConfig
class StudentappConfig(AppConfig):
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'StudentApp'

Step-2 this is models.py code
from django.db import models

# Create your models here.
class Student(models.Model):
    name = models.CharField(max_length = 255)
    address = models.CharField(max_length = 255)
    fee = models.IntegerField()

Step-3 this is serializers.py
from rest_framework import serializers
from StudentApp.models import Student

class StudentSerializer(serializers.ModelSerializer):
    class Meta:
        model = Student
        fields = '__all__'

Step-4 this is urls.py code

from django.urls import path
from . import views

urlpatterns = [
    path('insert/', views.studentApi),
    path('view/', views.studentApi),
    path('delete/<int:id>/', views.studentApi),
    path('update/<int:id>/', views.studentApi),
    # Add other URL patterns specific to the StudentApp app here
]
Step-5 this is views.py file
from django.shortcuts import render

# Create your views here.
from django.views.decorators.csrf import csrf_exempt
from rest_framework.parsers import JSONParser
from django.http.response import JsonResponse
from StudentApp.serializers import StudentSerializer
from StudentApp.models import Student

@csrf_exempt
def studentApi(request,id=0):
    if request.method=='GET':
        student = Student.objects.all()
        student_serializer=StudentSerializer(student,many=True)
        return JsonResponse(student_serializer.data,safe=False)
    elif request.method=='POST':
        student_data=JSONParser().parse(request)
        student_serializer=StudentSerializer(data=student_data)
        if student_serializer.is_valid():
            student_serializer.save()
            return JsonResponse("Added Successfully",safe=False)
        return JsonResponse("Failed to Add",safe=False)
    elif request.method=='PUT':
        student_data=JSONParser().parse(request)
        student=Student.objects.get(id=id)
        student_serializer=StudentSerializer(student,data=student_data)
        if student_serializer.is_valid():
            student_serializer.save()
            return JsonResponse("Updated Successfully",safe=False)
        return JsonResponse("Failed to Update")
    elif request.method=='DELETE':
        student=Student.objects.get(id=id)
        student.delete()
        return JsonResponse("Deleted Successfully",safe=False)




#this all is for the application esp32_api
Step-1 python django code views.py code

from django.shortcuts import render
# Create your views here.
from rest_framework import status
from rest_framework.decorators import api_view
from rest_framework.response import Response
from .models import SensorData
from .serializers import SensorDataSerializer

@api_view(['POST'])
def sensor_data_view(request):
    serializer = SensorDataSerializer(data=request.data)
    if serializer.is_valid():
        serializer.save()
        return Response(serializer.data, status=status.HTTP_201_CREATED)
    return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)

@api_view(['GET'])
def get_sensor_data(request):
    sensor_data = SensorData.objects.all()
    serializer = SensorDataSerializer(sensor_data, many=True)
    return Response(serializer.data)

step-2 urls.py code

from django.urls import path
from . import views

urlpatterns = [
    path('api/sensor-data/', views.sensor_data_view, name='sensor_data_view'),
    path('get_sensor_data/', views.get_sensor_data, name='get_sensor_data'),
]

Step-3 main urls.py code
from django.contrib import admin
from django.urls import path, include

urlpatterns = [
    path('admin/', admin.site.urls),
    path('esp32/', include('esp32_api.urls')),  # Include esp32_api urls
    path('student/', include('StudentApp.urls')),  # Include StudentApp urls
]

Step-4 models.py code
from django.db import models

# Create your models here.

class SensorData(models.Model):
    device_id = models.CharField(max_length=50)
    sensor_type = models.CharField(max_length=50)
    value = models.FloatField()
    timestamp = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return f"{self.device_id} - {self.sensor_type}"

Step-5 this is serializers.py code

'''
from rest_framework import serializers
class SensorData(serializers.Serializer):
    device_id = serializers.CharField(max_length=50)
    sensor_type = serializers.CharField(max_length=50)
    value = serializers.FloatField()
    timestamp = serializers.DateTimeField(auto_now_add=True)
'''

from rest_framework import serializers
from .models import SensorData

class SensorDataSerializer(serializers.ModelSerializer):
    class Meta:
        model = SensorData
        fields = '__all__'
    
Step-6 this is apps.py code

from django.apps import AppConfig


class Esp32ApiConfig(AppConfig):
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'esp32_api'

  */
/* this is for the get the sensor data 
get_response = http://127.0.0.1:8000/esp32/get_sensor_data/
post_response = http://127.0.0.1:8000/esp32/api/sensor-data/     raw_data = { "device_id": "001", "sensor_type": "temperature", "value": 25.5 }

#this is the api request for the StudentApp
post_response = http://localhost:8000/student/insert/        raw_data = { "name": "John Doe", "address": "123 Main St", "fee": 5000 }
get_response = http://localhost:8000/student/view/
PUT_response = http://localhost:8000/student/update/4/        raw_data = { "name": "satyendra", "address": "123 Main St", "fee": 5000 }
DELETE_response = http://localhost:8000/student/delete/8/


*/
