#include <Arduino.h>
#include <ESP32QRCodeReader.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Wi-Fi credentials
const char *ssid = "phodkar";
const char *password = "Kenway25";

// MQTT Broker settings
const char *mqtt_server = "192.168.29.55";
const int mqtt_port = 1883;
const char *mqtt_user = "your_mqtt_username";
const char *mqtt_password = "your_mqtt_password";
const char *mqtt_topic = "QRIN";

WiFiClient espClient;
PubSubClient client(espClient);

// NTP settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

ESP32QRCodeReader reader(CAMERA_MODEL_AI_THINKER);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

String getFormattedDate() {
  time_t rawTime = timeClient.getEpochTime();
  struct tm * timeInfo = localtime(&rawTime);

  char buffer[11]; // Buffer for the date string "YYYY-MM-DD"
  strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeInfo);

  return String(buffer);
}

void onQrCodeTask(void *pvParameters) {
  struct QRCodeData qrCodeData;

  while (true) {
    if (reader.receiveQrCode(&qrCodeData, 100)) {
      if (qrCodeData.valid) {
        String payload = (const char *)qrCodeData.payload;
        String date = getFormattedDate();
        String time = timeClient.getFormattedTime();
        String message = "QR Code: " + payload + " Date: " + date + " Time: " + time;
        Serial.println(message);
        
        if (!client.connected()) {
          reconnect();
        }
        client.publish(mqtt_topic, message.c_str());
      } else {
        Serial.println("invalid");
      }
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  
  client.setServer(mqtt_server, mqtt_port);

  timeClient.begin();
  timeClient.update();

  reader.setup();
  reader.beginOnCore(1);

  xTaskCreate(onQrCodeTask, "onQrCode", 4 * 1024, NULL, 4, NULL);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  timeClient.update();
  delay(100);
}