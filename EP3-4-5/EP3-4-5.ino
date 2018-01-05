/** 
 * @file EP3-4-5.ino
 * @brief SMART HOME - EP3 + EP4 + EP5
 * @date 05/01/2018
 * @authors Afonso Santos (nr. 2130653) & Natanael Godinho (nr. 2110634)
 * @state inc
 */

// Atribuição dos pinos do Arduino 
const int PIN_NTC = A1; 
const int PIN_LDR = A2; 
const int PIN_SERVO = 1;
const int PIN_LED_EP3_VERDE = 6;      // verde
const int PIN_LED_EP3_AMARELO = 7;    // amarelo
const int PIN_LED_EP3_VERMELHO = 8;   // vermelho
const int PIN_LED_EP4 = 9;            // verde

// Valores pré-definidos no enunciado


// Variáveis globais do programa

// Declaração de funções
float funcEp3();

void setup() {
  Serial.begin (9600);

  pinMode(PIN_LED_EP3_VERDE, OUTPUT);
  pinMode(PIN_LED_EP3_AMARELO, OUTPUT);
  pinMode(PIN_LED_EP3_VERMELHO, OUTPUT);
  pinMode(PIN_LED_EP4, OUTPUT);

}

void loop() {



}

float funcEp3() {


}

