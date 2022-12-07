/* 
   Public Domain - 2019
   This example needs https://github.com/esp8266/arduino-esp8266fs-plugin
   Based on ESP8266 AWS IoT example by Evandro Luis Copercini
   It connects to AWS IoT server then:
  - publishes humidity and temperature reading to the topic "outTopic" every two seconds
 
*/

#include "FS.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "DHT.h"

// Update these with values suitable for your network.

const char *ssid="your-ssid"; 
const char *password="your-password";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const char* AWS_endpoint = "xxxxxxxxxxxxxxxxxx.iot.us-east-2.amazonaws.com"; //MQTT broker ip


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

}

WiFiClientSecure espClient;
PubSubClient client(AWS_endpoint, 8883, callback, espClient); //set  MQTT port number to 8883 as per //standard
long lastMsg = 0;
char msg[50];
int value = 0;

#define DHTTYPE DHT11   // DHT 11 
#define dht_dpin D6     // Signal pin of DHT 11 is connected to the Digital pin 6

DHT dht(dht_dpin, DHTTYPE); 

void setup_wifi() {
  
  delay(10);
  // We start by connecting to a WiFi network
  
  espClient.setBufferSizes(512, 512);
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

  timeClient.begin();
  
  while(!timeClient.update()){
    timeClient.forceUpdate();
  }

  espClient.setX509Time(timeClient.getEpochTime());

}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("TestThing")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic","hello");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      char buf[256];
      espClient.getLastSSLError(buf,256);
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  dht.begin();
  
  setup_wifi();
  delay(1000);
  
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());

  // Load certificate file
  File cert = SPIFFS.open("/cert.der", "r"); //replace cert.crt with your uploaded file name
  if (!cert) {
    Serial.println("Failed to open cert file");
  }
  else
    Serial.println("Success to open cert file");

  delay(1000);

  if (espClient.loadCertificate(cert))
    Serial.println("cert loaded");
  else
    Serial.println("cert not loaded");
    

  // Load private key file
  File private_key = SPIFFS.open("/key.der", "r"); //replace private with your uploaded file name
  if (!private_key) {
    Serial.println("Failed to open private cert file");
  }
  else
    Serial.println("Success to open private cert file");

  delay(1000);

  if (espClient.loadPrivateKey(private_key))
    Serial.println("private key loaded");
  else
    Serial.println("private key not loaded");
    

   // Load CA file
  File ca = SPIFFS.open("/ca.der", "r"); //replace ca eith your uploaded file name
  if (!ca) {
    Serial.println("Failed to open ca ");
  }
  else
    Serial.println("Success to open ca");

    delay(1000);

    if(espClient.loadCACert(ca))
      Serial.println("ca loaded");
    else
      Serial.println("ca failed");
      Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());
  }



void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    
    float h = dht.readHumidity();      // Get humidity reading
    float t = dht.readTemperature();   // Get temperature reading

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    String temp = "{\"temperature\":" + String(t) + ", \"humidity\":" + String(h) + "}";
    int len = temp.length();
    char msg[len+1];
    temp.toCharArray(msg, len+1);
 

    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
    Serial.print("Heap: "); Serial.println(ESP.getFreeHeap()); //Low heap can cause problems
  }
}

/*//==============================================================================//
 * SW-420 Vibration Sensor Module Interfacing with Arduino
 */ //=============================================================================//
#include <Arduino.h>
#include <stdio.h>

//define on/off logic symbols with name ON and OFF
#define ON HIGH
#define OFF LOW

#define Sensor_Out_Pin A5
#define LED_Pin  13
 
int present_condition = 0;
int previous_condition = 0;

void setup() {
pinMode(Sensor_Out_Pin, INPUT);
pinMode(LED_Pin, OUTPUT);
}
 

void LED_Pin_blink(void);
 
void loop() {
previous_condition = present_condition;
present_condition = digitalRead(Sensor_Out_Pin); // Reading digital data from the A5 Pin of the Arduino.
 
if (previous_condition != present_condition) {
LED_Pin_blink();
 
} else {
digitalWrite(LED_Pin, OFF);
}
}
 
void LED_Pin_blink(void) {
digitalWrite(LED_Pin, ON);
delay(250);
digitalWrite(LED_Pin, OFF);
delay(250);
digitalWrite(LED_Pin, ON);
delay(250);
digitalWrite(LED_Pin, OFF);
delay(250);
}

/*//==============================================================================//
 * LM393 Sensor Module Interfacing with Arduino
 */ //=============================================================================//

int vib_pin=7;
int led_pin=13;
void setup() {
pinMode(vib_pin,INPUT);
pinMode(led_pin,OUTPUT);
}

void loop() {
int val;
val=digitalRead(vib_pin);
if(val==1)
{
digitalWrite(led_pin,HIGH);
delay(1000);
digitalWrite(led_pin,LOW);
delay(1000);
}
else
digitalWrite(led_pin,LOW);
}