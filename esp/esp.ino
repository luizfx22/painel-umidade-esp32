#include "DHTesp.h"
#include <WiFi.h>
#include "PubSubClient.h"
#include <ArduinoJson.h>

const char* ssid = "Princesa Bean";
const char* password = "godblesskingzog";
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";
const char* mqttTopic = "5c4a1abe-e3cc-4a48-a350-5ecaa14ca9ef/confirmations";
const char* clientId = "ESP32-SOIL";

const int DHT_PIN = 22;
const int SOIL_PIN = 32;
const int LED_PIN = 16;

// Soil sensor calibration
const int AirValue = 3393;
const int WaterValue = 1296;

DHTesp dhtSensor;
WiFiClient espClient;
PubSubClient client(espClient);

void mqttSendJson(float temperature, float humidity, float soil) {
  DynamicJsonDocument doc(2048);
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["soil"] = soil;
  char JSONmessageBuffer[2048];
  serializeJson(doc, JSONmessageBuffer);
  client.publish("5c4a1abe-e3cc-4a48-a350-5ecaa14ca9ef/input-data", JSONmessageBuffer);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  dhtSensor.setup(DHT_PIN, DHTesp::DHT11);

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect(clientId, mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT");
      client.subscribe(mqttTopic);
    }
    else {
      Serial.print("MQTT connection failed, retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void loop() {
  client.loop();

  TempAndHumidity  data = dhtSensor.getTempAndHumidity();

  float soil = analogRead(SOIL_PIN);
  soil = map(soil, AirValue, WaterValue, 0, 100);

  mqttSendJson(data.temperature, data.humidity, soil);

  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  Serial.println("Soil Humidity: " + String(soil, 1) + "%");
  Serial.println("---");

  delay(2000);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String topicStr = topic;
  Serial.println(topicStr);
  if (topicStr.equals(mqttTopic)) {
    Serial.println("CONFIRMED!");
  }
}
