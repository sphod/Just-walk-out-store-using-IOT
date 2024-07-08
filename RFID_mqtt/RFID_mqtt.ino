#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_PN532.h>


const char *ssid = "phodkar";
const char *password = "Kenway25";


const char *mqtt_server = "192.168.29.55";
const int mqtt_port = 1883;
const char *mqtt_user = "your_mqtt_username";
const char *mqtt_password = "your_mqtt_password";
const char *mqtt_topic = "NFC";

WiFiClient espClient;
PubSubClient client(espClient);


#define SDA_PIN 0 
#define SCL_PIN 2 
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup_wifi() {
  delay(10);
  Serial.begin(115200);
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
    if (client.connect("ESPRFID", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Wire.begin();
  setup_wifi();
  
  client.setServer(mqtt_server, mqtt_port);
  
  
  nfc.begin();
  
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // Halt
  }
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  nfc.SAMConfig();

  Serial.println("Waiting for NFC tag...");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    Serial.print("Tag UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("Tag UID Value: ");
    for (uint8_t i=0; i < uidLength; i++) {
      Serial.print(" 0x");Serial.print(uid[i], HEX);
    }
    Serial.println("");


    String uidString = "";
    for (uint8_t i=0; i < uidLength; i++) {
      uidString += String(uid[i], HEX);
    }


    if (client.publish(mqtt_topic, uidString.c_str())) {
      Serial.println("Published UID to MQTT");
    } else {
      Serial.println("Failed to publish UID to MQTT");
    }

    delay(1000);
  }
}