/*
  Welcome to JP Learning
*/
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#define DHT_PIN 5
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// Wi-Fi Credentials
const char* ssid = "Your SSID";
const char* password = "Your Password";

String HTTPS_POST_URL = "http://127.0.0.1:8000/esp32/api/sensor-data/";
String HTTPS_GET_URL = "http://127.0.0.1:8000/esp32/get_sensor_data/";

WiFiClientSecure client;
HTTPClient https;

String device_id = "Device0001";

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(5000);
  Serial.println("\n\nWelcome to JP Learning\n");
  setup_wifi();

  // Most important line
  client.setInsecure();
  HTTPS_GET(HTTPS_GET_URL);
}

void loop() {
  float DHT_Temperature = dht.readTemperature();
  float DHT_Humidity = dht.readHumidity();
  DHT_Temperature = 22.4;
  DHT_Humidity = 34;
//  if (isnan(DHT_Temperature) || isnan(DHT_Humidity)) {
//    Serial.println("\n\nFailed to read from DHT11 sensor!");
//    delay(1000);
//  } else 
  {
    Serial.println("\n\nDHT Temperature: " + String(DHT_Temperature) + " °C");
    Serial.println("DHT Humidity: " + String(DHT_Humidity) + " %");

    // POST Temperature Data
    String pkt = "{";
    pkt += "\"device_id\": \"" + device_id + "\", ";
    pkt += "\"type\": \"Temperature\", ";
    pkt += "\"value\": " + String(DHT_Temperature) + "";
    pkt += "}";
    HTTPS_POST(HTTPS_POST_URL, pkt);

    // POST Humidity Data
    String pkt2 = "{";
    pkt2 += "\"device_id\": \"" + device_id + "\", ";
    pkt2 += "\"type\": \"Humidity\", ";
    pkt2 += "\"value\": " + String(DHT_Humidity) + "";
    pkt2 += "}";
    HTTPS_POST(HTTPS_POST_URL, pkt2);
  }
  delay(1000);
}
void setup_wifi() {
  Serial.print("\nConnecting to ");
  Serial.println("\"" + String(ssid) + "\"");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void HTTPS_GET(String HTTPS_GET_URL) {
  Serial.println("\nGetting from: " + HTTPS_GET_URL);

  Serial.println("Conntecting to server..");
  if (https.begin(client, HTTPS_GET_URL)) {
    int httpsCode = https.GET();
    if (httpsCode > 0) {
      Serial.println("httpsCode: " + String(httpsCode));
      if (httpsCode == HTTP_CODE_OK) {
        String ServerResponse = https.getString();
        Serial.println("\nServerResponse: " + ServerResponse);
      }
    } else
      Serial.print("Failed to GET");
  }
  else
    Serial.print("Failed to connect to server");
  https.end();
}
void HTTPS_POST(String HTTPS_POST_URL, String PostPacket) {
  Serial.println("\nPosting to: " + HTTPS_POST_URL);
  Serial.println("PostPacket: " + PostPacket);

  Serial.println("Conntecting to server..");
  if (https.begin(client, HTTPS_POST_URL)) {
    https.addHeader("Content-Type", "application/json");
    int httpsCode = https.POST(PostPacket);
    if (httpsCode > 0) {
      Serial.println("httpsCode: " + String(httpsCode));
      if (httpsCode == HTTP_CODE_OK || httpsCode == HTTP_CODE_CREATED) {
        String ServerResponse = https.getString();
        Serial.println("\nServerResponse: " + ServerResponse);
      }
    } else
      Serial.print("Failed to POST");
  }
  else
    Serial.print("Failed to connect to server");
  https.end();
}

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
