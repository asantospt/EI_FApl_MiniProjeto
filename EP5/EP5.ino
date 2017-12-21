/** 
 * @file EP5.ino
 * @brief EP5 – Abertura automática de estores para controlo de temperatura
 * @date 21/12
 * @authors Afonso Santos (nr. 2130653) & Natanael Godinho (nr. 2110634)
 * @state INC    //SWEEP
 */

 /** 
  * LIGAÇÕES: @Arduino 'Mega 2560'
  * 5V -> *USB
  * GND -> breadboard
  * Pin 10 -> Servo/ Laranja(Sinal)
  * 
  * @Servomotor
  * Laranja(Sinal) -> Arduino/Pin 10
  * Vermelho (+) -> VCC
  * Castanho (-) -> GND
  */

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

const int PIN_SERVO = 10;
const int MAX_ANG_SERVO = 170;
const int MIN_ANG_SERVO = 10;

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin (9600);
  
  myservo.attach(PIN_SERVO);  // attaches the servo on pin 9 to the servo object

  myservo.write(50); 
  Serial.println("Teste servo - na posição 50º");
  delay(15);  
  myservo.write(MIN_ANG_SERVO); 
  Serial.println("Teste servo - na posição 10º");
  delay(15);  
   
}

void loop() {
  for (pos = MIN_ANG_SERVO; pos <= MAX_ANG_SERVO; pos += 1) { // goes from 0 degrees to 170 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.println(pos); 
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = MAX_ANG_SERVO; pos >= MIN_ANG_SERVO; pos -= 1) { // goes from 170 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.println(pos); 
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

