/** 
 * @file EP_solucao-final.ino
 * @brief SMART HOME: EP1 + EP2 + EP3 + EP4 + EP5
 * @date 30/12
 * @authors Afonso Santos (nr. 2130653) & Natanael Godinho (nr. 2110634)
 * @state INC
 */

 /** 
  * LIGAÇÕES: @Arduino 'Mega 2560'
  * 5V -> *USB
  * GND -> breadboard
  * Pin A2 -> LDR + (A2) + R. 22 kOhm     // EP4
  * Pin 8 -> LED verde                    // EP4
  * Pin 10 -> Servo/ Laranja(Sinal)       // EP5
  * 
  */

// Bibliotecas extra usadas
#include <Servo.h>

// Constantes usadas no programa 
const int PIN_LED_4 = 8; 

// Valores pré-definidos no enunciado


// Declaração de funções


void setup() {
  Serial.begin (9600);
  
  pinMode(PIN_LED_4, OUTPUT);

}

void loop() {


}

/**
 * @descr 
 * @params 
 * @return 
 */
void *atuarLED() {  

}


