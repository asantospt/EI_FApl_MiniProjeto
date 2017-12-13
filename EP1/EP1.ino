/** 
 * @file EP1.ino
 * @brief EP1 – Sistema de alarme anti-intrusão 
 * @date 10/12
 * @author Afonso (2130653) & Natanael (2110634)
 * @state INC
 */

 /** 
  * LIGAÇÕES: @Arduino 'Mega 2560'
  * 5V -> USB
  * GND -> breadboard
  * A0 -> sensor QRE1113, Pin 3/V_out
  * Pin 3 -> Sensor ultrassons, Trig
  * Pin 4 -> Sensor ultrassons, Echo
  * Pin 8 -> Buzzer
  * Pin 9 -> Servo, Laranja (Sinal)
  */

// Bibliotecas extra usadas 
#include <Servo.h>

// Constantes usadas no programa 
const int PIN_LED = 9; 

// Valores pré-definidos no enunciado
const float PERC_BRILHO_LED = 25.5;

// Declaração de funções
void abrirCancela();

void setup() {
	pinMode(PIN_LED, OUTPUT);

  Serial.begin (9600);

}

void loop() {
	
Serial.println("inicio LOOP...");



Serial.println("fim LOOP...");
  

}

/**
 * @descr 
 * @params 
 * @return 
 */
void atuarLED() {  
  // Quando não é detetado nenhum veículo > LED c/ 10% do brilho 
  if (voltage < 2.50) { 
    analogWrite(PIN_LED, PERC_BRILHO_LED);
    delay(500);   
  } else {
    // Veículo detetado > LED c/ 100% do brilho  
    analogWrite(PIN_LED, 255);
    delay(500);
  }
}
