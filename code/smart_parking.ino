#include <ESP8266WiFi.h>
#include <Servo.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "ThingSpeak.h"
#include <WiFiClientSecure.h>

// ------------------- Wi-Fi Credentials -------------------
const char *ssid = "WORKSPACE-2 2.4";
const char *password = "Admin.Tumkur@06";

// ------------------- ThingSpeak Credentials -------------------
unsigned long channelID = 3044249;
const char *writeAPIKey = "2Y68FBJNM6L6CY44";
WiFiClient client;

// ------------------- Firebase Credentials -------------------
const char* firebaseHost = "iot-based-smart-parkinge-d8e29-default-rtdb.firebaseio.com"; 
const char* firebaseAuth = "AIzaSyD-5gogb-3zoM7CgJTBl8IoXuPaoURYR8M";
const int firebasePort = 443;

// ------------------- NTP Client -------------------
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);

// ------------------- Servo Motors -------------------
Servo entryGateServo;
Servo exitGateServo;

// ------------------- Sensor Pins -------------------
int carEnter = 5;    // Entry sensor
int carExited = 4;   // Exit sensor
int slot3 = 13;      // Slot 3
int slot2 = 12;      // Slot 2
int slot1 = 0;       // Slot 1

// ------------------- Variables -------------------
int count = 0;
int CLOSE_ANGLE = 80;
int OPEN_ANGLE = 0;

String EntryTimeSlot1, ExitTimeSlot1, EntryTimeSlot2, ExitTimeSlot2, EntryTimeSlot3, ExitTimeSlot3;
boolean s1_occupied = false;
boolean s2_occupied = false;
boolean s3_occupied = false;

bool carEntryFlag = false;
int lastCarEnterState = -1;

// ------------------- Wi-Fi Setup -------------------
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
  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// ------------------- Firebase Send Function -------------------
void sendToFirebase(String path, String value) {
  WiFiClientSecure firebaseClient;
  firebaseClient.setInsecure();  // Not secure for production

  String url = path + ".json?auth=" + String(firebaseAuth);
  String jsonData = "\"" + value + "\"";

  if (firebaseClient.connect(firebaseHost, firebasePort)) {
    firebaseClient.print(String("PUT ") + url + " HTTP/1.1\r\n" +
                         "Host: " + firebaseHost + "\r\n" +
                         "Content-Type: application/json\r\n" +
                         "Content-Length: " + jsonData.length() + "\r\n\r\n" +
                         jsonData);

    Serial.println("Data sent to Firebase: " + path + " = " + value);

    unsigned long timeout = millis();
    while (firebaseClient.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println("Firebase Timeout");
        firebaseClient.stop();
        return;
      }
    }
    while (firebaseClient.available()) {
      String line = firebaseClient.readStringUntil('\r');
    }
    firebaseClient.stop();
  } else {
    Serial.println("Firebase Connection Failed");
  }
}

// ------------------- Setup -------------------
void setup() {
  delay(1000);
  Serial.begin(115200);

  entryGateServo.attach(D4);
  exitGateServo.attach(D5);

  pinMode(carEnter, INPUT);
  pinMode(carExited, INPUT);
  pinMode(slot1, INPUT);
  pinMode(slot2, INPUT);
  pinMode(slot3, INPUT);

  setup_wifi();

  ThingSpeak.begin(client);
  timeClient.begin();
}

// ------------------- Loop -------------------
void loop() {
  timeClient.update();
  String currentTime = String(timeClient.getHours()) + ":" + String(timeClient.getMinutes());

  int currentCarEnterState = digitalRead(carEnter);
  if (currentCarEnterState != lastCarEnterState) {
    Serial.print("carEnter Sensor Value: ");
    Serial.println(currentCarEnterState);
    lastCarEnterState = currentCarEnterState;
  }

  // Entry Detection
  if (currentCarEnterState == HIGH && !carEntryFlag) {
    Serial.println("Car Detected at Entry");
    entryGateServo.write(OPEN_ANGLE);
    delay(3000);
    entryGateServo.write(CLOSE_ANGLE);
    count++;
    updateThingSpeak();
    carEntryFlag = true;
  }
  if (currentCarEnterState == LOW) carEntryFlag = false;

  // Exit Detection
  if (digitalRead(carExited) == LOW) {
    Serial.println("Car Detected at Exit");
    exitGateServo.write(OPEN_ANGLE);
    delay(3000);
    exitGateServo.write(CLOSE_ANGLE);
    while (digitalRead(carExited) == LOW);
    count--;
    updateThingSpeak();
  }

  // Slot Monitoring
  checkSlotOccupancy(slot1, s1_occupied, 2, 3, "Slot1", EntryTimeSlot1, ExitTimeSlot1, currentTime);
  checkSlotOccupancy(slot2, s2_occupied, 4, 5, "Slot2", EntryTimeSlot2, ExitTimeSlot2, currentTime);
  checkSlotOccupancy(slot3, s3_occupied, 6, 7, "Slot3", EntryTimeSlot3, ExitTimeSlot3, currentTime);
}

// ------------------- ThingSpeak Update -------------------
void updateThingSpeak() {
  ThingSpeak.setField(1, count);
  ThingSpeak.writeFields(channelID, writeAPIKey);

  // Firebase Update
  sendToFirebase("/carCount", String(count));
}

// ------------------- Slot Occupancy Function -------------------
void checkSlotOccupancy(int slotPin, boolean &slotOccupied, int entryField, int exitField,
                        String slotName, String &entryTime, String &exitTime, String currentTime) {
  int sensorState = digitalRead(slotPin);

  if (sensorState == LOW && !slotOccupied) { // Slot now occupied
    Serial.println(slotName + " Occupied at " + currentTime);
    entryTime = currentTime;
    slotOccupied = true;

    ThingSpeak.setField(entryField, entryTime);
    ThingSpeak.writeFields(channelID, writeAPIKey);

    sendToFirebase("/" + slotName + "/entryTime", entryTime);
    sendToFirebase("/" + slotName + "/occupied", "true");

  } else if (sensorState == HIGH && slotOccupied) { // Slot now free
    Serial.println(slotName + " Available at " + currentTime);
    exitTime = currentTime;
    slotOccupied = false;

    ThingSpeak.setField(exitField, exitTime);
    ThingSpeak.writeFields(channelID, writeAPIKey);

    sendToFirebase("/" + slotName + "/exitTime", exitTime);
    sendToFirebase("/" + slotName + "/occupied", "false");
  }
}
