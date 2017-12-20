/** 
 * @file EP3.ino
 * @brief SMART HOME: EP3
 * @date - 2017-12-
 * @authors Afonso (2130653) & Natanael (2110634)
 * @state INC - falta testar e fazer comentarios da ligações do AD8032
 */

/** 
 * LIGAÇÕES: @Arduino 'Mega 2560'
 * 5V -> USB
 * GND -> breadboard 
 * A1 -> NTC + (A1) + R. 3.6 kOhm
 * 10 -> R. 330 Ohm + Led Vermelho
 * 11 -> R. 330 Ohm + Led Amarelo
 * 12 -> R. 330 Ohm + Led Verde
 * 
 * @NTC
 * + -> 5V
 * - -> V_out, Arduino/Pin A1
 *
 * @led vervelho
 * + -> R 330 Ohm, Arduino/pin 10
 * - -> GND
*
 * @led amarelo
 * + -> R 330 Ohm, Arduino/pin 11
 * - -> GND
 *
 * @led verde
 * + -> R 330 Ohm, Arduino/pin 12
 * - -> GND
 */

// Constantes usadas no programa 
const int PIN_NTC = A1;
const int LED_PIN_VERMELHO =  10;
const int LED_PIN_AMARELO =  11;
const int LED_PIN_VERDE =  12; 
const float GANHO_NTC = 1.66;
const int R1 = 3600;
const int VIN = 5;
const float T0 = 298.17;// 25ºC
const int BETA = 4090;
const int R0 = 3300;
const float KELVIN = 273.15;



//Controle de tempos
//T1: Tempo para primeiro registo de temperatura - Milisegundos
const int DELTA_T1 = 2000;

bool led = 0;

// Valores pré-definidos no enunciado
/**
Parâmetros do sensor: β = 4090 K; R0(t = 25 °C) = 3,3 kΩ.
R1(t = 30 °C) = ????? kΩ
*
Temperaturas:
• T ≤ 15 °C ⇒ apenas LED verde aceso;
• 15 °C < T ≤ 28 °C ⇒todos os LEDs apagados;
• 28 °C < T ≤ 30 °C ⇒apenas LED amarelo aceso;
• T > 30 °C ⇒ apenas LED vermelho aceso.
*/

// Valores de referência - Montagem Inicial
/**

*/

// Declaração de funções

void setup() {
  Serial.begin (9600);
  pinMode(LED_PIN_VERMELHO, OUTPUT);
  pinMode(LED_PIN_AMARELO, OUTPUT);
  pinMode(LED_PIN_VERDE, OUTPUT);
}

void loop() {

  //Contar milisegundos desde o arranque do sistema
  float instanteAtual = millis();

  //Accionar o led por 2 segundos inicialmente
  if ((instanteAtual) <= DELTA_T1 ){
    //por o LED a HIGH
    digitalWrite(LED_PIN_VERMELHO, HIGH);
    digitalWrite(LED_PIN_AMARELO, HIGH);
    digitalWrite(LED_PIN_VERDE, HIGH);
    
  }else if (led == 0) {
    //por o led a low
    digitalWrite(LED_PIN_VERMELHO, LOW);
    digitalWrite(LED_PIN_AMARELO, LOW);
    digitalWrite(LED_PIN_VERDE, LOW);
    led = 1;
  }

  // Leitura do Vout do NTC (V_NTC)
  int sensorValue = analogRead(PIN_NTC);
  
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);

  // voltagem do NTC. Ao aquecer o sensor, V0 vai aumentando
  Serial.println(voltage); 

  //Calcular rNTC
  float rNTC = (( R1 * VIN) / (voltage / GANHO_NTC)) - R1;
  //calcular temp cº
  float tNTC = 1.0 / ((1.0 / T0) + ((1.0 / BETA) * log(rNTC / R0))) - KELVIN; 

    // LED - Verde
  if (tNTC > 25) {
    digitalWrite(LED_PIN_VERDE, HIGH);
  } else {
      digitalWrite(LED_PIN_VERDE, LOW);
    }
  // LED - Amarelo
  if (tNTC > 28) {
    digitalWrite(LED_PIN_AMARELO, HIGH);
  } else {
      digitalWrite(LED_PIN_AMARELO, LOW);
    }
  // LED - Vermelho
  if (tNTC > 30) {
    digitalWrite(LED_PIN_VERMELHO, HIGH);
  } else {
      digitalWrite(LED_PIN_VERMELHO, LOW);
    } 
  delay(100);
}