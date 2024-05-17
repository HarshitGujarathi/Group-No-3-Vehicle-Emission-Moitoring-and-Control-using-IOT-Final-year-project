#include <Arduino.h>
#include <WiFi.h>
#include <ThingSpeak.h>

// WiFi settings
const char* ssid = "Vivo T1";
const char* password = "Aasan he";

// ThingSpeak settings
unsigned long channelID = 2485160;
const char *apiKey = "3XAKXBD77NWY3K82";

// Define the analog pins connected to the gas sensors
const int mq135Pin = 35; // MQ-135
const int mq7Pin = 34;   // MQ-7
const int mq2Pin = 32;   // MQ-2

WiFiClient client;

void setup() {
  Serial.begin(9600);
  // Connect to WiFi
  Serial.println();
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
}

void loop() {
  // Read values from sensors
  int mq2Value = analogRead(mq2Pin);
  int mq7Value = analogRead(mq7Pin);
  int mq135Value = analogRead(mq135Pin);

  // Convert raw values to appropriate units
  float mq2Concentration = MQ2convertToPPM(mq2Value); // Hydrocarbons
  float mq7Concentration = MQ7convertToPPM(mq7Value); // CO
  float mq135Concentration = MQ135convertToPPM(mq135Value); // CO2

  // Print values to serial monitor
  Serial.print("MQ2: ");
  Serial.print(mq2Concentration);
  Serial.print(" ppm\tMQ7: ");
  Serial.print(mq7Concentration);
  Serial.print(" %\tMQ135: ");
  Serial.print(mq135Concentration);
  Serial.println(" ppm");

  // Update ThingSpeak
  ThingSpeak.begin(client);
  ThingSpeak.setField(1, mq135Concentration);
  ThingSpeak.setField(2, mq7Concentration);
  ThingSpeak.setField(3, mq2Concentration);

  // Service message
    const char *Service;
  if (mq7Concentration>0.5 && mq2Concentration>500)      //CO2 does not need in vehicle service 
  {
    Service = "Service required";
  }
  else
  {
    Service = "Service not required";
  }

  ThingSpeak.setField(4, Service);
  ThingSpeak.writeFields(channelID, apiKey);
  
  // int updateResult1 = ThingSpeak.writeField(channelID, 1, mq135Concentration, apiKey);
  // int updateResult2 = ThingSpeak.writeField(channelID, 2, mq7Concentration, apiKey);
  // int updateResult3 =ThingSpeak.writeField(channelID, 3, mq2Concentration, apiKey);
  
  // Serial.print("Update Result 1: ");
  // Serial.println(updateResult1);
  // Serial.print("Update Result 2: ");
  // Serial.println(updateResult2);
  // Serial.print("Update Result 3: ");
  // Serial.println(updateResult3);
  delay(15000); // Adjust delay as needed
}

// Function to convert raw sensor values to PPM
float MQ2convertToPPM(int rawValue) {
  // Replace these values with appropriate calibration values for your sensors
  // These are just placeholders
  float calibrationFactor = 1.55; // Modify according to calibration
  float voltage = rawValue * (5.0 / 1023.0); // Convert raw value to voltage
  float concentration = calibrationFactor * voltage; // Convert voltage to concentration (PPM)
  return concentration;
}

float MQ7convertToPPM(int rawValue) {
  // Replace these values with appropriate calibration values for your sensors
  // These are just placeholders
  float calibrationFactor = 0.015; // Modify according to calibration
  float voltage = rawValue * (5.0 / 1023.0); // Convert raw value to voltage
  float concentration = calibrationFactor * voltage; // Convert voltage to concentration (PPM)
  return concentration;
}
float MQ135convertToPPM(int rawValue) {
  // Replace these values with appropriate calibration values for your sensors
  // These are just placeholders
  float calibrationFactor = 243; // Modify according to calibration
  float voltage = rawValue * (5.0 / 1023.0); // Convert raw value to voltage
  float concentration = calibrationFactor * voltage; // Convert voltage to concentration (PPM)
  return concentration;
}