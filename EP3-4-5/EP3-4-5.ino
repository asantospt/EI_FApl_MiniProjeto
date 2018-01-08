/** 
 * @file EP3-4-5.ino
 * @brief SMART HOME - EP3 + EP4 + EP5
 * @date 07/01/2018
 * @authors Afonso Santos (nr. 2130653) & Natanael Godinho (nr. 2110634)
 * @state inc
 */

// Declaração de bibliotecas extra
#include <Servo.h>

// Atribuição dos pinos do 'Arduino Mega 2560'
const int PIN_NTC = A1; 
const int PIN_LDR = A2; 
const int PIN_LED_EP3_VERDE = 6;      // verde
const int PIN_LED_EP3_AMARELO = 7;    // amarelo
const int PIN_LED_EP3_VERMELHO = 8;   // vermelho
const int PIN_LED_EP4 = 5;            // verde, Pin com PWM (IMPORTANTE)    TODO: Mega/Pin 9 
const int PIN_SERVO = 10;

// Declaração de valores pré-definidos no enunciado
const int MIN_PERC_BRILHO = 0;        // Brilho mín., quando "luz ambiente normal"
const int MAX_PERC_BRILHO = 255;      // Brilho máx., quando "ausência de luz"
const int MAX_BRILHO_MADRUGADA = 128;     // Metade do brilho máximo = 127.5
const int DELTA_T4 = 10000;               // 10 s para simular as 04:00h da madrugada [Segundos] 

// Declaração de váriáveis globais       // TODO: org
// ---------------- EP 3 ----------------
const float GANHO_NTC = 1.66;
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

// ---------------- EP 4 ----------------
const int VALOR_MIN_LUZ = 50;       // Valor min. de voltagem (LDR sem receber luz)   
const int VALOR_MAX_LUZ = 1000;      // Valor máx. de voltagem (LDR a receber muita luz)      
const int TEMPO_MADRUGADA = 10000;     // Simulação da 04:00 da madrugada. 10 s [Milissegundos]
unsigned long instanteAtual_Ep4 = 0;

// ---------------- EP 5 ----------------
const int MIN_ANG_SERVO = 10;
const int MAX_ANG_SERVO = 170;
const float T_LIMIAR_1 = 10.0;              // Temperatura considerada como 'Frio'
const float T_LIMIAR_2 = 28.0;              // Temperatura considerada como 'Calor'
const float T_LIMIAR_LUZ = 700.0;            // Valor para incidência de luz solar
int posServo = 0;                 // Variável que guarda a posição do Servo [Graus]

// Criação de um objeto para controlar o Servomotor
Servo myservo;     

// Declaração de funções
float funcEp3();
float funcaoEp4(int maxBrilho);

void setup() {
  Serial.begin (9600);  

  pinMode(PIN_LED_EP3_VERDE, OUTPUT);
  pinMode(PIN_LED_EP3_AMARELO, OUTPUT);
  pinMode(PIN_LED_EP3_VERMELHO, OUTPUT);
  pinMode(PIN_LED_EP4, OUTPUT);
  myservo.attach(PIN_SERVO);

  // Acender os LED's durante 2 s no arranque do sistema
  digitalWrite(PIN_LED_EP3_VERDE, HIGH);
  digitalWrite(PIN_LED_EP3_AMARELO, HIGH);
  digitalWrite(PIN_LED_EP3_VERMELHO, HIGH);
  digitalWrite(PIN_LED_EP4, HIGH);
  delay(2000);  
  digitalWrite(PIN_LED_EP3_VERDE, LOW);
  digitalWrite(PIN_LED_EP3_AMARELO, LOW);
  digitalWrite(PIN_LED_EP3_VERMELHO, LOW);
  digitalWrite(PIN_LED_EP4, LOW);
}

void loop() {
  float funcEp3Temp = 0.0;
  int sensorValueLDR_Ep4 = 0;

  instanteAtual_Ep4 = millis();       // Contar milisegundos desde o arranque do sistema

  funcEp3Temp = funcEp3();        // Função com funcionalidades do EP3

  if (instanteAtual_Ep4 >= DELTA_T4) {      
    // Já passaram os 10 s desde o arranque do programa, significa que já é 'madrugada'
    Serial.println(">>> Passaram 10 s desde arranque. Madrugada!! <<<");       // debug
    sensorValueLDR_Ep4 = funcaoEp4(MAX_BRILHO_MADRUGADA);         // Função com funcionalidades do EP4
  } else {
    sensorValueLDR_Ep4 = funcaoEp4(MAX_PERC_BRILHO);
  }

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
  float tNTC = 1.0 / ((1.0 / T0) + ((1.0 / BETA) * log(rNTC / R0))) - KELVIN; 
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

/**
 * @brief Recebe o brilho máximo a aplicar, lê a luminosidade existente e atua sobre o sistema de iluminação
 * @params Valor máximo do brilho, em função da hora
 * @return Valor da luminosidade
 */
int funcaoEp4(int maxBrilho) {
  int sensorValueLDR = 0;
  int declive = 0;
  int maxBrilhoEntradaFuncao = 0;
  int brilhoLED = 0;

  maxBrilhoEntradaFuncao = maxBrilho;

  sensorValueLDR = analogRead(PIN_LDR);     
  Serial.print("sensorValueLDR: ");
  Serial.println(sensorValueLDR);      // debug

  //brilhoLED = map(x_IN, x0, x1, y0, y1) 
  brilhoLED = map(sensorValueLDR, VALOR_MIN_LUZ, VALOR_MAX_LUZ, maxBrilhoEntradaFuncao, MIN_PERC_BRILHO);
  brilhoLED = constrain(brilhoLED, MIN_PERC_BRILHO, maxBrilhoEntradaFuncao);  */  

  analogWrite(PIN_LED_EP4, brilhoLED);
  delay(30);                // Esperar 30 ms para ser o efeito da alteração do brilho do LED [Milisegundos]
  Serial.print("brilhoLED: ");      // debug
  Serial.println(brilhoLED);
  Serial.print(" ");

  return sensorValueLDR;
}