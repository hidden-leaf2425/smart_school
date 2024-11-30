#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Thông tin WiFi
const char* ssid = "Phuong Dep Chai";
const char* password = "phuongdepchai";

// Thông tin MQTT
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_topic = "home/test";

// WiFi và MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  // Kết nối WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Kết nối MQTT
  client.setServer(mqtt_server, mqtt_port);
  while (!client.connected()) {
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
  client.subscribe(mqtt_topic);
}

void loop() {
    if (!client.connected()) {
    while (!client.connected()) {
      Serial.println("Reconnecting to MQTT...");
      if (client.connect("ESP8266Client")) {
        Serial.println("Reconnected");
        client.subscribe(mqtt_topic);
      } else {
        delay(2000);
      }
    }
  }
  client.loop();
  String payload = Serial.readStringUntil('\n');
    if (payload != "") {
        Serial.print("Sending payload: ");
        Serial.println(payload);
        client.publish(mqtt_topic, payload.c_str());
    }
}
