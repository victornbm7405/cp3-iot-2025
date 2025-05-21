#include <WiFi.h>
#include <PubSubClient.h>

const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";

const char* BROKER_MQTT  = "broker.hivemq.com";
const int   BROKER_PORT  = 1880;
const char* mqttUser     = "admlnx";
const char* mqttPassword = "senha";

#define TOPICO_PUBLISH  "cp3/iot/dados"
#define ID_MQTT "esp32cp3"

WiFiClient espClient;
PubSubClient MQTT(espClient);

void conectaWiFi() {
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void conectaMQTT() {
  while (!MQTT.connected()) {
    MQTT.connect(ID_MQTT, mqttUser, mqttPassword);
    if (!MQTT.connected()) {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  conectaWiFi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
}

void loop() {
  if (!MQTT.connected()) {
    conectaMQTT();
  }
  MQTT.loop();

  float temperatura = random(200, 350) / 10.0;
  float umidade = random(400, 800) / 10.0;
  float pressao = random(9800, 10500) / 10.0;
  float altitude = random(0, 500);

  String payload = "{";
  payload += "\"temperatura\": " + String(temperatura) + ",";
  payload += "\"umidade\": " + String(umidade) + ",";
  payload += "\"pressao\": " + String(pressao) + ",";
  payload += "\"altitude\": " + String(altitude);
  payload += "}";

  Serial.println(payload);
  MQTT.publish(TOPICO_PUBLISH, payload.c_str());

  delay(10000);
}
