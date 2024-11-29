#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Thông tin WiFi
const char* ssid = "Tang1-Hieu01";
const char* password = "123123a@";

// Thông tin MQTT
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "home/test";

// WiFi và MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial1.begin(9600);
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
}

void loop() {
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    Serial.print("From Arduino: ");
    Serial.println(message);
    client.publish(mqtt_topic, message.c_str());

    // Gửi phản hồi lại Arduino Mega
    String response = "Hello from ESP8266\n";
    Serial1.print(response);  // Phản hồi lại qua TX (GPIO1)
    delay(3000);
    client.loop();  
  }
}
