# SSwetha
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <DHTesp.h>


Adafruit_MPU6050 m_p_u;
const int DHT_PIN = 15;
DHTesp dhtSensor;
int a = 27;


void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  while (!Serial); // Wait for the serial port to open
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(a, OUTPUT);


  // Initialize the MPU6050 sensor
  if (!m_p_u.begin()) {
    Serial.println("MPU6050 not found. Please check wiring.");
    while (1) {
      delay(20);
    }
  }
}


void loop() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  
  Serial.print("Temperature: ");
  Serial.print(data.temperature, 2);
  Serial.println(" °C");


  if (data.temperature <= 40) {
    digitalWrite(a, HIGH);
  } else {
    digitalWrite(a, LOW);
  }


  Serial.print("Humidity: ");
  Serial.print(data.humidity, 1);
  Serial.println(" %");


  Serial.println("---");


  sensors_event_t acc, gcc, temp;
  m_p_u.getEvent(&acc, &gcc, &temp);


  Serial.print("Acceleration on X axis: ");
  Serial.println(acc.acceleration.x);
  delay(1000);


  Serial.print("Acceleration on Y axis: ");
  Serial.println(acc.acceleration.y);
  delay(1000);


  Serial.print("Acceleration on Z axis: ");
  Serial.println(acc.acceleration.z);
  delay(1000);


  Serial.print("Rotation on X axis: ");
  Serial.println(gcc.gyro.x * 180.0 / M_PI);
  delay(1000);
}
