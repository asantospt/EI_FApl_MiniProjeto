/** 
 * @file EP4.ino
 * @brief EP4 – Iluminação noturna de presença
 * @date 14/12
 * @authors Afonso (nr. 2130653) & Natanael (nr. 2110634)
 * @state INC
 */

/** 
 * LIGAÇÕES: @Arduino 'Mega 2560'
 * 5V -> USB
 * GND -> breadboard
 * A0 -> LDR
 * Pin 8 -> LED
 */

// Constantes usadas no programa 
const int PIN_LDR = A0; 
const int PIN_LED = 8; 

// Valores pré-definidos no enunciado
const int TEMPO_LEDS_ARRANQUE = 2000;       // (milissegundos) 2 segundos
const int TOT_PERC_BRILHO = 255;            // Brilho máximo = 255
const int METADE_PERC_BRILHO = 128;         // Metade do brilho máximo = 127.5

// Declaração de funções
void atuarLed();

void setup() {
	pinMode(PIN_LED, OUTPUT);

  Serial.begin (9600);
}

void loop() {

}

/**
 * @descr 
 * @params 
 * @return 
 */
void atuarLed() {  

}
