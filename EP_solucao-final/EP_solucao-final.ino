/** 
 * @file EP_solucao-final.ino
 * @brief SMART HOME: EP1 + EP2 + EP3 + EP4 + EP5
 * @date -
 * @authors Afonso (2130653) & Natanael (2110634)
 * @state INC
 */

 /** 
  * LIGAÇÕES: @Arduino 'Mega 2560'
  * 5V -> USB
  * GND -> breadboard
  */

// Bibliotecas extra usadas
#include <Servo.h>

// Constantes usadas no programa 
const int PIN_LED = 9; 

// Valores pré-definidos no enunciado


// Declaração de funções

void setup() {
  Serial.begin (9600);
  
  pinMode(PIN_LED, OUTPUT);

}

void loop() {


}

/**
 * @descr 
 * @params 
 * @return 
 */
void atuarLED() {  

}


