/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings


#define BLYNK_TEMPLATE_ID "TMPLRE0PlNsg"
#define BLYNK_TEMPLATE_NAME "Earthquake and Water Level Detector"
#define BLYNK_AUTH_TOKEN "uTZSm-N8bY5fn7_I6ts0NvIVEN24mjgP"


#define BLYNK_PRINT Serial

//set water level in cm
int emptyTankDistance = 70;
int fullTankDistance = 30;

const int trigPin = 18;
const int echoPin = 19;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

#define pinBuzzer 2
#define pinLED1 5
#define pinLED2 4

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;


char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

BlynkTimer timer;

void myTimerEvent()
{
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinLED1, OUTPUT);
  pinMode(pinLED2, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  

  timer.setInterval(1000L, myTimerEvent); //Staring a timer
  // Try to initialize! MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
  delay(100);
}


void loop()
{
  deteksigempa();
  timer.run();
}

void deteksigempa(){

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  int acX = a.acceleration.x;
  int acY = a.acceleration.y;
  int acZ = a.acceleration.z;

  String v = ",";
  String x = String(acX);
  String y = String(acY);
  String z = String(acZ);

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  delay(1000);

  if(distanceInch > 70){
    tone(pinBuzzer, 1000);
    digitalWrite(pinLED2, HIGH);
    Serial.println("SITUASI LEVEL AIR");
    Serial.println("Level Air Tinggi !!!");
    Serial.print("Level Air (inch): ");
    Serial.println(distanceInch);
    Serial.println("");
    Blynk.virtualWrite(V6, distanceInch);
    Blynk.virtualWrite(V7, LOW);
    Blynk.virtualWrite(V8, HIGH);
  }
  else{
    noTone(pinBuzzer);
    digitalWrite(pinLED2, LOW);
    Serial.println("SITUASI LEVEL AIR");
    Serial.println("Level Air Aman");
    Serial.print("Level Air (inch): ");
    Serial.println(distanceInch);
    Serial.println("");
    Blynk.virtualWrite(V6, distanceInch);
    Blynk.virtualWrite(V7, HIGH);
    Blynk.virtualWrite(V8, LOW);
  }

  if(acX > 4 | acY > 4 | acZ > 13 ){
    tone(pinBuzzer, 1000);
    digitalWrite(pinLED1, HIGH);
    Serial.println("SITUASI GEMPA");
    Serial.println("AWAS GEMPA BUMI !!! "+ x + v + y + v + z);
    Serial.println("");
    Blynk.virtualWrite(V0, "AWAS !! GEMPA BUMI");
    Blynk.virtualWrite(V1, x);
    Blynk.virtualWrite(V4, y);
    Blynk.virtualWrite(V5, z);
    Blynk.virtualWrite(V2, LOW);
    Blynk.virtualWrite(V3, HIGH);

  }else{
    noTone(pinBuzzer);
    digitalWrite(pinLED1, LOW);
    Serial.println("SITUASI GEMPA");
    Serial.println("AMAN "+ x + v + y + v + z);
    Serial.println("");
    Blynk.virtualWrite(V0, "AMAN");
    Blynk.virtualWrite(V1, x);
    Blynk.virtualWrite(V4, y);
    Blynk.virtualWrite(V5, z);
    Blynk.virtualWrite(V2, HIGH);
    Blynk.virtualWrite(V3, LOW);
  }
  delay(1500);
  Blynk.run();
}
