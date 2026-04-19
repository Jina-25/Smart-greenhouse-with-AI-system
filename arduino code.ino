// Include necessary libraries
#include <DHT.h> // DHT sensor library
#include <Servo.h> // Servo motor library

// Pin connections
#define DHT_PIN 2 // DHT22 sensor connected to digital pin 2
#define SOIL_PIN A0 // Soil moisture sensor connected to analog pin A0
#define LDR_PIN A1 // LDR sensor connected to analog pin A1
#define RELAY_PIN 3 // Relay module connected to digital pin 3
#define SERVO_PIN 4 // Servo motor connected to digital pin 4
#define FAN_PIN 5 // Fan connected to digital pin 5
#define LAMP_PIN 6 // Lamp connected to digital pin 6

// Define DHT sensor type and initialize sensor object
#define DHT_TYPE DHT22 // DHT22 sensor type
DHT dht(DHT_PIN, DHT_TYPE); // Initialize DHT sensor object

// Define servo motor object
Servo servo; // Create servo motor object

// Define plant profiles as enum
enum PlantProfile {
  STEVIA,
  BASIL,
  SPINACH
};

// Define variables for sensor readings
float temperature; // Temperature in degrees Celsius
float humidity; // Relative humidity in percentage
int soilMoisture; // Soil moisture in percentage
int lightIntensity; // Light intensity in lux

// Define variables for setpoints
float tempSetpoint; // Temperature setpoint in degrees Celsius
float humSetpoint; // Humidity setpoint in percentage
int soilSetpoint; // Soil moisture setpoint in percentage
int lightSetpoint; // Light intensity setpoint in lux

void loadPlantSettings(PlantProfile plant) {

  switch (plant) {

    case STEVIA:
      tempSetpoint = 23.5;
      humSetpoint = 50;
      soilSetpoint = 60;
      lightSetpoint = 4283;
      break;

    case BASIL:
      tempSetpoint = 22;
      humSetpoint = 65;
      soilSetpoint = 60;
      lightSetpoint = 15400;
      break;

    case SPINACH:
      tempSetpoint = 20;
      humSetpoint = 70;
      soilSetpoint = 70;
      lightSetpoint = 5555;
      break;
  }
}

// Define variable for current plant profile:
PlantProfile currentPlant;

// Define function to read sensors:
void readSensors() {
  // Read temperature and humidity from DHT sensor:
  temperature = dht.readTemperature(); // Read temperature in degrees Celsius
  humidity = dht.readHumidity(); // Read humidity in percentage

  // Read soil moisture from analog sensor:
  soilMoisture = map(analogRead(SOIL_PIN), 0, 1023, 0, 100); // Map analog reading to percentage

  // Read light intensity from LDR sensor:
  lightIntensity = map(analogRead(LDR_PIN), 0, 1023, 0, 20000);
  //lightIntensity = analogRead(LDR_PIN); // Read analog value

  // Print sensor readings to Serial Monitor:
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.println(" %");
  Serial.print("Light Intensity: ");
  Serial.print(lightIntensity);
  Serial.println(" lux");
}

// Define function to control system:
void controlSystem() {
  // Check if temperature is below setpoint:
  if (temperature > tempSetpoint + 1) {
  digitalWrite(FAN_PIN, HIGH);
  } 
  else {
  digitalWrite(FAN_PIN, LOW);
  }

  // Check if humidity is below setpoint:
  if (temperature > tempSetpoint + 1 || humidity > humSetpoint + 5) {
  servo.write(90);
  } 
  else {
  servo.write(0);
  }


  // Check if soil moisture is below setpoint:
  if (soilMoisture < soilSetpoint) {
    digitalWrite(RELAY_PIN, HIGH);
  }
  else {
    digitalWrite(RELAY_PIN, LOW);
  }

  // Check if light intensity is below setpoint:
  if (lightIntensity < lightSetpoint) {
    digitalWrite(LAMP_PIN, HIGH);
  }
  else {
    digitalWrite(LAMP_PIN, LOW);
  }

  // Print system status to Serial Monitor:
  Serial.println("System Status:");
  Serial.print("Fan: ");
  Serial.println(digitalRead(FAN_PIN) ? "ON" : "OFF");
  Serial.print("Vents: ");
  Serial.println(servo.read() == 90 ? "OPEN" : "CLOSED");
  Serial.print("Water Valve: ");
  Serial.println(digitalRead(RELAY_PIN) ? "ON" : "OFF");
  Serial.print("Lamp: ");
  Serial.println(digitalRead(LAMP_PIN) ? "ON" : "OFF");
}
void setup() {
  Serial.begin(9600);
  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LAMP_PIN, OUTPUT);

  servo.attach(SERVO_PIN);

  dht.begin();

  currentPlant = STEVIA;
  loadPlantSettings(currentPlant);
}

void loop() {

  readSensors();
  controlSystem();

  delay(2000);
  if (Serial.available()) {
  String msg = Serial.readStringUntil('\n');

  if (msg == "PLANT_HEALTH:DRY") {
    loadPlantSettings(STEVIA); 
    digitalWrite(RELAY_PIN, HIGH);
  }

  if (msg == "PLANT_HEALTH:HEALTHY") {
    digitalWrite(RELAY_PIN, LOW);
  }

  if (msg == "PLANT_HEALTH:STRESSED") {
    digitalWrite(FAN_PIN, HIGH);
    servo.write(90);
  }
  }
}