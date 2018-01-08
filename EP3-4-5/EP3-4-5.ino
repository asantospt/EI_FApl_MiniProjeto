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
const int DELTA_T3 = 10000;               // 10 s para simular as 04:00h da madrugada [Segundos]  

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
const float VALOR_MIN_LUZ = 3.7;       // Valor min. de voltagem (LDR tapada) = 2.00   
const float VALOR_MAX_LUZ = 4.58;      // Valor máx. de voltagem (LDR a descoberta) = 4.50 
const int TEMPO_MADRUGADA = 10000;     // Simulação da 04:00 da madrugada. 10 s [Milissegundos]
unsigned long instanteAtual_Ep4 = 0;
boolean isMadrugada = false; 

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
float funcao_Ep4(int maxBrilho);
void funcao_Ep5(float temp_Ep3, int voltageLDR_Ep4);

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
  float voltageLDR_Ep4 = 0.0;

  funcEp3Temp = funcEp3();       // Função com funcionalidades do EP3

  // TODO: implementar EP4
  voltageLDR_Ep4 = funcao_Ep4(MAX_PERC_BRILHO);   // Função com funcionalidades do EP4

  // TODO: implementar EP5
  funcao_Ep5(funcEp3Temp, voltageLDR_Ep4);    // Função com funcionalidades do EP5

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

float funcao_Ep4(int maxBrilho) {

  return voltageLDR;
}

void funcao_Ep5(float temp_Ep3, int voltageLDR_Ep4) {
  float temp_Entrada = 0.0;
  int voltageLDR_Entrada = 0;

  temp_Entrada = temp_Ep3;
  voltageLDR_Entrada = voltageLDR_Ep4;


  // Nota: em ambiente real, verificação poderia ser a cada 15 minutos
}