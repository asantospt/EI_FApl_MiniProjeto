/** 
 * @file EP_solucao-final.ino
 * @brief SMART HOME: EP1 + EP2 + EP3 + EP4 + EP5
 * @date 31/12
 * @authors Afonso Santos (nr. 2130653) & Natanael Godinho (nr. 2110634)
 * @state INC
 */

 /** 
  * LIGAÇÕES: @Arduino 'Mega 2560'
  * 5 V -> USB
  * GND -> breadboard
  * Pin A0 -> QRE1113/ Pin 3              // EP2
  * Pin A1 -> Ampop/ Pin 1/OUT1           // EP3
  * Pin A2 -> LDR + (A2) + Res. 22 kOhm   // EP4
  * Pin 1 -> Servo/ Laranja(Sinal)        // EP5
  * Pin 2 -> HC-SR04/ Echo                // EP1
  * Pin 3 -> HC-SR04/ Trig                // EP1
  * Pin 4 -> Buzzer/ +                    // EP1
  * Pin 5 -> LED vermelho                 // EP2
  * Pin 6 -> LED verde                    // EP3
  * Pin 7 -> LED amarelo                  // EP3
  * Pin 8 -> LED vermelho                 // EP3
  * Pin 9 -> LED verde                    // EP4
  */

// Bibliotecas extra usadas


// Atribuição dos pinos do Arduino  
const int PIN_QRE = A0;            
const int PIN_NTC = A1; 
const int PIN_LDR = A2; 
const int PIN_SERVO = 1;
const int PIN_ECHO = 2;
const int PIN_TRIG = 3;
const int PIN_BUZZER = 4;
const int PIN_LED_EP2 = 5;            // vermelho
const int PIN_LED_EP3_VERDE = 6;      // verde
const int PIN_LED_EP3_AMARELO = 7;    // amarelo
const int PIN_LED_EP3_VERMELHO = 8;   // vermelho
const int PIN_LED_EP4 = 9;            // verde

// Valores pré-definidos

// Variáveis globais do programa

// Declaração de funções


void setup() {
  Serial.begin (9600);
  
  pinMode(PIN_LED_EP2, OUTPUT);
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


