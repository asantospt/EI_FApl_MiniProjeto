/** 
 * @file EP3-4-5.ino
 * @brief SMART HOME - EP3 + EP4 + EP5
 * @date 06/01/2018
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
float funcEp3();
void funcEp5(float Ep3Temp);


void setup() {
  Serial.begin (9600);

  pinMode(PIN_LED_EP3_VERDE, OUTPUT);
  pinMode(PIN_LED_EP3_AMARELO, OUTPUT);
  pinMode(PIN_LED_EP3_VERMELHO, OUTPUT);
  pinMode(PIN_LED_EP4, OUTPUT);

  digitalWrite(PIN_LED_EP3_VERMELHO, HIGH);
  digitalWrite(PIN_LED_EP3_AMARELO, HIGH);
  digitalWrite(PIN_LED_EP3_VERDE, HIGH);
  digitalWrite(PIN_LED_EP4, HIGH);
  delay(2000);
  digitalWrite(PIN_LED_EP3_VERMELHO, LOW);
  digitalWrite(PIN_LED_EP3_AMARELO, LOW);
  digitalWrite(PIN_LED_EP3_VERDE, LOW);
  digitalWrite(PIN_LED_EP4, LOW);
}

void loop() {

  float funcEp3Temp = 0.0;
  funcEp3Temp = funcEp3();
  funcEp5(funcEp3Temp);

  delay(100);

}

float funcEp3(){

  // Leitura do Vout do NTC (V_NTC)
  int sensorValue = analogRead(PIN_NTC);
  
  // Converter leitura analogica para voltagem
  float voltage = sensorValue * (5.0 / 1023.0);
  //para Debug
  //Serial.println(voltage); 

  //Calcular rNTC
  float rNTC = (( R1 * 5) / (voltage / GANHO_NTC)) - R1;
  //calcular temp cº
  float tNTC = 1.0 / ((1.0 / T0) + ((1.0 / BETA) * log(rNTC / R0))) - KELVIN + 20; 
  //para debug
  //Serial.println(tNTC);

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

void funcEp5(float Ep3Temp){

}