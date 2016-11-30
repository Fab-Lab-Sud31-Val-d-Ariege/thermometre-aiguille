#include <Servo.h>;
#include "RunningMedian.h"

// Temperature minimale a afficher
#define MIN_TEMP 15
// Temperature maximal a afficher
#define MAX_TEMP 30

// Angle minimal du servo
#define MIN_SERVO_ANGLE 0
// Angle maximal du servo
#define MAX_SERVO_ANGLE 180

// Broche analogique a laquelle est branchee la sortie du MCP9701
#define TEMP_PIN A0
// Broche a laquelle est branche le servo
#define SERVO_PIN 9

// Decalage de tension a appliquer. La valeur par defaut est de 400 (mV).
// Il vous faudra jouer avec cette valeur si la temperature mesuree est
//   incorrecte. Diminuez cette valeur si la temperature mesuree est trop
//   basse, augmentez cette valeur si la temperature mesuree est trop haute.
#define VOLTAGE_OFFSET 244

// Si place a 1, le servo affichera chaque temperature (de MIN_TEMP a MAX_TEMP),
//   avec une pause de 2 secondes entre chaque temperature, ce qui vous permettra
//   de dessiner les graduation sur votre support.
#define DEBUG_MODE 0

Servo servo;

void setup() {
  servo.attach(SERVO_PIN);
}

void loop() {
  
  if (DEBUG_MODE) {
    for (int temperature = MIN_TEMP; temperature <= MAX_TEMP; temperature++) {
      // Calcule l'angle du servo a partir de la temperature.
      int servoAngle = map(temperature, MIN_TEMP, MAX_TEMP, MIN_SERVO_ANGLE, MAX_SERVO_ANGLE);
      servo.write(servoAngle);

      delay(2000);
    }
  } else {
    // Initialise un tableau permettant de stocker les temperature. Dix
    //   mesures seront prises d'affilee, puis la mediane sera utilisee
    //   afin d'eliminer les eventuelles mesures erronees (trop basses ou
    //   trop hautes)
    RunningMedian<float,10> samples;
  
    float temperature = 0.0;
    
    for (int i = 0; i < 10; i++) {
      // Lit la tension de sortie du MCP9701
      int analogData = analogRead(TEMP_PIN);
      int voltage = (5000 / 1024) * analogData;
      
      // Calcule la temperature a partir de la tension mesuree
      float currentTemp = (voltage - VOLTAGE_OFFSET) / 19.5;
      currentTemp = constrain(currentTemp, MIN_TEMP, MAX_TEMP);
      
      // Ajoute la temperature mesuree au tableau de mesures
      samples.add(currentTemp);
      delay(20);
    }
  
    // Recupere la temperature mediane des mesures effectuees
    samples.getMedian(temperature);
    
    // Calcule l'angle du servo a partir de la temperature.
    int servoAngle = map(temperature, MIN_TEMP, MAX_TEMP, MIN_SERVO_ANGLE, MAX_SERVO_ANGLE);
    servo.write(servoAngle);
    
    delay(100);
  }
}

