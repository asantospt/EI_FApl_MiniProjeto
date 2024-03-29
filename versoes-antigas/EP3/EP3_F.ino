/** 
 * @file EP3.ino
 * @brief SMART HOME: EP3
 * @date - 2017-12-
 * @authors Afonso (2130653) & Natanael (2110634)
 * @state OK
 */

/** 
 * LIGAÇÕES: @Arduino 'Mega 2560'
 * 5V -> USB
 * GND -> breadboard 
 * A1 -> AmpOp8032 pin 1
 * pin 6 -> R. 330 Ohm + Led Vermelho
 * pin 7 -> R. 330 Ohm + Led Amarelo
 * pin 8 -> R. 330 Ohm + Led Verde
 * 
 * @NTC
 * + -> 5V
 * - -> AmpOp8032 p3 + R 3600 Ohn + GND
 *
 * AmpOp8032
 * 1 -> A1
 * 2 -> R1 + GND e R2 + AmpOp8032 p1
 * 3 -> - NTC + R 3600 Ohn, GND
 * 4 -> GND
 * 8 -> 5V
 * 
 *  * Ecrã
 * 1 -> A1
 * 2 -> R1 + GND e R2 + AmpOp8032 p1
 * 3 -> - NTC + R 3600 Ohn, GND
 * 4 -> GND
 * 8 -> 5V
 * 
 * @led vervelho
 * + -> R 330 Ohm + pin 8
 * - -> GND
*
 * @led amarelo
 * + -> R 330 Ohm + pin 7
 * - -> GND
 *
 * @led verde
 * + -> R 330 Ohm + pin 6
 * - -> GND
 */

// Constantes usadas no programa 
const int PIN_NTC = A1;
const int PIN_LED_EP3_VERMELHO =  6;
const int PIN_LED_EP3_AMARELO =  7;
const int PIN_LED_EP3_VERDE =  8;

// Valores pré-definidos no enunciado
/**
Parâmetros do sensor: β = 4090 K; R0(t = 25 °C) = 3,3 kΩ.
R1(t = 30 °C) = ????? kΩ
*
Temperaturas:
• T ≤ 15 °C ⇒ apenas LED verde aceso;
• 15 °C < T ≤ 28 °C ⇒todos os LEDs apagados;
• 28 °C < T ≤ 30 °C ⇒apenas LED amarelo aceso;
• T > 30 °C ⇒ apenas LED vermelho aceso.
*/
const float GANHO_NTC = 1.75;
const int R1 = 3600;
const int VIN = 5;
const float T0 = 298.15;// 25ºC
const int BETA = 4090;
const int R0 = 3300;
const float KELVIN = 273.15;
const int TREF1 = 15;
const int TREF2 = 28;
const int TREF3 = 30;
const float TMPOSC = 0.5;
//variavel para print de temperatura no ecrã e serial
float tNTCp = 0.0;

// Declaração de funções
float funcEp3 ();

void setup() {
  Serial.begin (9600);
  pinMode(PIN_LED_EP3_VERMELHO, OUTPUT);
  pinMode(PIN_LED_EP3_AMARELO, OUTPUT);
  pinMode(PIN_LED_EP3_VERDE, OUTPUT);

  digitalWrite(PIN_LED_EP3_VERMELHO, HIGH);
  digitalWrite(PIN_LED_EP3_AMARELO, HIGH);
  digitalWrite(PIN_LED_EP3_VERDE, HIGH);
  delay(2000);
  digitalWrite(PIN_LED_EP3_VERMELHO, LOW);
  digitalWrite(PIN_LED_EP3_AMARELO, LOW);
  digitalWrite(PIN_LED_EP3_VERDE, LOW);
}

void loop() {
float funcEp3Temp = 0.0;
funcEp3Temp = funcEp3();
//para debug
//Serial.println(funcEp3); 

delay(100);

}

float funcEp3(){

  // Leitura do Vout do NTC (V_NTC)
  int sensorValue = analogRead(PIN_NTC);
  
  // Converter leitura analogica para voltagem
  float voltage = sensorValue * (5.0 / 1023.0);
  //para Debug
  Serial.println(voltage); 

  //Calcular rNTC
  float rNTC = (( R1 * 5) / (voltage / GANHO_NTC)) - R1;
  //calcular temp cº
  float tNTC = 1.0 / ((1.0 / T0) + ((1.0 / BETA) * log(rNTC / R0))) - KELVIN; 
  //para debug
  Serial.println(tNTC);

  //Fazer print da temperatura se alteraç\ao de 0,5 graus
  if(tNTC > (tNTCp + TMPOSC) || tNTC < (tNTCp - TMPOSC) ){
    tNTCp = tNTC;
    Serial.print("Temperatura Cº: ");
    Serial.println(tNTCp);
  } 

    // LED - Verde
  if (tNTC <= TREF1) {
    digitalWrite(PIN_LED_EP3_VERDE, HIGH);
  } else {
      digitalWrite(PIN_LED_EP3_VERDE, LOW);
    }
  // LED - Amarelo
  if (tNTC > TREF2 && tNTC <= TREF3) {
    digitalWrite(PIN_LED_EP3_AMARELO, HIGH);
  } else {
      digitalWrite(PIN_LED_EP3_AMARELO, LOW);
    }
  // LED - Vermelho
  if (tNTC > TREF3) {
    digitalWrite(PIN_LED_EP3_VERMELHO, HIGH);
  } else {
      digitalWrite(PIN_LED_EP3_VERMELHO, LOW);
    } 
  return tNTC;
}