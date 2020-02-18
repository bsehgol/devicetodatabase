// IoT Workshop
// Send temperature and humidity data to MQTT
//
// WiFiNINA https://www.arduino.cc/en/Reference/WiFiNINA (MKR WiFi 1010)
// Arduino MKR ENV https://www.arduino.cc/en/Reference/ArduinoMKRENV
// Arduino MQTT Client  https://github.com/arduino-libraries/ArduinoMqttClient

#include <WiFiNINA.h>
#include <Arduino_MKRENV.h>
#include <ArduinoMqttClient.h>

#include "config.h"

WiFiSSLClient net;
MqttClient mqtt(net);

String temperatureTopic = "itp/" + DEVICE_ID + "/temperature";
String humidityTopic = "itp/" + DEVICE_ID + "/humidity";
String ledTopic = "itp/" + DEVICE_ID + "/led";
String soilTopic = "itp/" + DEVICE_ID + "/soil";

String pressureTopic = "itp/" + DEVICE_ID + "/pressure";
String illuminanceTopic = "itp/" + DEVICE_ID + "/illuminance";
String uvaTopic = "itp/" + DEVICE_ID + "/uva";
String uvbTopic = "itp/" + DEVICE_ID + "/uvb";




// Publish every 10 seconds for the workshop. Real world apps need this data every 5 or 10 minutes.
unsigned long publishInterval = 10 * 1000;
unsigned long lastMillis = 0;
const int ledPin = 5;


void setup() {
  Serial.begin(9600);
  

  // Wait for a serial connection
  while (!Serial) { }
  
  // initialize the shield
  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }

  // initialize ledPin as an output.
  pinMode(ledPin, OUTPUT);
 
  Serial.println("Connecting WiFi");
  connectWiFi();

  // define function for incoming MQTT messages
  mqtt.onMessage(messageReceived);
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqtt.connected()) {
    connectMQTT();
  }
  
  // poll for new MQTT messages and send keep alives
  mqtt.poll();

  if (millis() - lastMillis > publishInterval) {
    lastMillis = millis();
    
    int soil = analogRead(A0);
    float temperature = ENV.readTemperature(FAHRENHEIT);
    float humidity = ENV.readHumidity();
    float pressure = ENV.readPressure();
    float illuminance = ENV.readIlluminance();
    float uva = ENV.readUVA();
    float uvb = ENV.readUVB();

    Serial.print(temperature);
    Serial.print("°F ");
    Serial.print(humidity);
    Serial.println("% RH");
    Serial.print(soil);
    Serial.println("% soil moisture");

    Serial.print(pressure);
    Serial.println("kPa");

    Serial.print(illuminance);
    Serial.println("lux");

    Serial.print(uva);
    Serial.println("μW/cm2");

    Serial.print(uvb);
    Serial.println("μW/cm2");
    
    mqtt.beginMessage(temperatureTopic);
    mqtt.print(temperature); 
    mqtt.endMessage();

    mqtt.beginMessage(humidityTopic);
    mqtt.print(humidity); 
    mqtt.endMessage();

    
    mqtt.beginMessage(soilTopic);
    mqtt.print(soil);
    mqtt.endMessage();

   mqtt.beginMessage(pressureTopic);
   mqtt.print(pressure);
   mqtt.endMessage();

   mqtt.beginMessage(illuminanceTopic);
   mqtt.print(illuminance);
   mqtt.endMessage();

   mqtt.beginMessage(uvaTopic);
   mqtt.print(uva);
   mqtt.endMessage();

   mqtt.beginMessage(uvbTopic);
   mqtt.print(uvb);
   mqtt.endMessage();
  }  
}



void connectWiFi() {
  // Check for the WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  Serial.print("WiFi firmware version ");
  Serial.println(WiFi.firmwareVersion());
  
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(WIFI_SSID);
  Serial.print(" ");

  while (WiFi.begin(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(3000);
  }

  Serial.println("Connected to WiFi");
  printWiFiStatus();

}

void connectMQTT() {
  Serial.print("Connecting MQTT...");
  mqtt.setId(DEVICE_ID);
  mqtt.setUsernamePassword(MQTT_USER, MQTT_PASSWORD);

  while (!mqtt.connect(MQTT_BROKER, MQTT_PORT)) {
    Serial.print(".");
    delay(5000);
  }

  mqtt.subscribe(ledTopic);
  Serial.println("connected.");
}

void printWiFiStatus() {
  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void messageReceived(int messageSize) {
  String topic = mqtt.messageTopic();
  String payload = mqtt.readString();
  Serial.println("incoming: " + topic + " - " + messageSize + " bytes ");
  Serial.println(payload);
  if (payload.equalsIgnoreCase("ON")) {
    analogWrite(ledPin, 255);  // need analog write for older firmware
  } else if (payload.equalsIgnoreCase("OFF")) {
    analogWrite(ledPin, LOW);    
  } else {
    
    // see if we have a brightness value
    int percent = payload.toInt();
    // check the range
    if (percent < 0) { 
      percent = 0; 
    } else if (percent > 100) { 
      percent = 100; 
    }
    // map brightness of 0 to 100 to 1 byte value 0x00 to 0xFF
    int brightness = map(percent, 0, 100, 0, 255);
    analogWrite(ledPin, brightness);
  }
}
