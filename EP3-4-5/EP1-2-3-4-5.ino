/** 
 * @file EP3-4-5.ino
 * @brief SMART HOME - EP3 + EP4 + EP5
 * @date 06/01/2018
 * @authors Afonso Santos (nr. 2130653) & Natanael Godinho (nr. 2110634)
 * @state inc
 */

// Bibliotecas extra usadas
#include <SPI.h>
#include <Adafruit_GFX.h> 
#include <Adafruit_ILI9341.h>

// LCD: definir os terminais de DC e CS
#define TFT_DC 9   //49 mega    
#define TFT_CS 10  //53 mega

// criar um objeto tft com indicacao dos terminais DC e CS
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Atribuição dos pinos do Arduino 
const int PIN_NTC = A1; 
const int PIN_LDR = A2; 
const int PIN_SERVO = 1;
const int PIN_LED_EP3_VERDE = 6;      // verde
const int PIN_LED_EP3_AMARELO = 7;    // amarelo
const int PIN_LED_EP3_VERMELHO = 8;   // vermelho
const int PIN_LED_EP4 = 9;            // verde

//EP1 + 2
const int PIN_QRE = A0;
const int PIN_TRIG = 2;
const int PIN_ECHO = 3;
const int PIN_BUZZER = 4;
const int PIN_LED_EP2 = 5;

// Valores pré-definidos no enunciado
//EP1 + 2
const int FREQ_BUZZER = 880;            // Frequência [Hz]
const float DIST_MIN_ALARM = 61.0;      // Distância mínima [Centímetros]
const int DELTA_T2 = 10000;             // 10 s para acionar o alarme [Segundos]

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

//EP1 + 2
const int V_MIN_QRE = 400;          // Valor mínimo definido para identificação de reflexão
const int V_MAX_QRE = 850;          // Valor máximo definido para identificação de reflexão
unsigned long tRef2 = 0;                // Varíavel para guardar tempo de referência de contagem dos 10 s
const int DELTA_T1 = 1000;              // Tempo entre medições de distância [Milisegundos]
unsigned long tRef1 = 0;                // Varíavel para guardar tempo de referência entre medições de distância
const float VEL_ULTRASSONS_AR = 58.31;  // Velocidade de propagação dos ultrassons no ar = 343 m/s 
                                        // Formula Utilizada D[cm] = deltaT / 58,31 [cms/Microsegundos]
int alarm = 0;       // Varíavel para indicar acionamento do alarme
int intrusao = 0;    // Varíavel para escrever 1 única vez no ecrã
int SECRET = 1234;   // Código do Alarme
float instanteAtual = 0.0;    // Varíavel para guardar o tempo decorrido deste o arranque do Arduino [Milisegundos] 
int contagem = 0;       // Contador de ocorrências
int isLocked = 0;       // Varíavel para garantir 1 única ocorrência de cada vez que faz 1 leitura dentro do intervalo definido

// Declaração de funções
float funcEp3();
void funcEp5(float Ep3Temp);

//EP1 + 2
void funcEp1();
void funcEp2();

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

    // inicializar o tft
  tft.begin();
  // colocar fundo a preto
  tft.fillScreen(ILI9341_BLACK);
  // definir orientacao da escrita
  tft.setRotation(2);

  //EP1 +2
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_LED_EP2, OUTPUT);
  
  digitalWrite(PIN_TRIG, LOW);  // Ao inicializar o sistema o pin Trig deve estar em modo LOW
  delayMicroseconds(2);

  digitalWrite(PIN_LED_EP2, HIGH);

  tone(PIN_BUZZER, FREQ_BUZZER, 500);     
  delay(500);                       // Acionar o buzzer por 0,5 segundos inicialmente
  noTone(PIN_BUZZER);                    

  delay(1500);                      // Para completar os 2 s em que o LED do EP2 deve estar ligado no arranque
  digitalWrite(PIN_LED_EP2, LOW); 
}

void loop() {

  float funcEp3Temp = 0.0;
  funcEp3Temp = funcEp3();
  funcEp5(funcEp3Temp);

  //Ep1 + 2
  funcEp1();        // Função com funcionalidades do EP1
  funcEp2();        // Função com funcionalidades do EP2

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
    //escrever para o ecrã
    tft.fillScreen(ILI9341_BLACK); 
    tft.setCursor(0, 100);
    tft.setTextColor(ILI9341_RED); 
    tft.setTextSize(2);
    tft.print("Temperatura ");
    tft.print(tNTCp);
    tft.println(" C");

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

void funcEp1() {
  float distance = 0.0;
  long duration = 0;      // Registar a duração do pulso do echo [Microsegundos]
  int secret = 0;

  instanteAtual = millis();       // Contar milisegundos desde o arranque do sistema

  digitalWrite(PIN_TRIG, HIGH);     // Tratar dos Pinos do sensor Ultrasons
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW); 
  
  if ((instanteAtual - tRef1) >= DELTA_T1 ) {        // Ler distancia a cada segundo
    duration = pulseIn(PIN_ECHO, HIGH);
    distance = ((float)duration / VEL_ULTRASSONS_AR);  
      
    if (alarm == 0 && distance < DIST_MIN_ALARM) {
      Serial.print(distance);     // degug
      Serial.println(" cm");      // degug
      Serial.println("Insira codigo de Seguranca ");
      tRef1 = instanteAtual;
      tRef2 = instanteAtual;
      alarm = 1;
      intrusao = 0;         // Escrever para o ecrã 1 única vez
      tft.fillScreen(ILI9341_YELLOW);
      tft.setCursor(0, 0);
      tft.setTextColor(ILI9341_RED); 
      tft.setTextSize(2);
      tft.println("Insira codigo Seguranca ");
    }
  }

  secret = Serial.parseInt();       // Receber texto enviado pela consola da porta série
  if (alarm == 1 && SECRET == secret ){
    noTone(PIN_BUZZER);
    alarm = 0;
    tft.fillScreen(ILI9341_GREEN);      // Escrever para o ecrã 
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_BLACK); 
    tft.setTextSize(2);
    tft.println("OK");
  }

  if (alarm == 1 && (instanteAtual - tRef2) >= DELTA_T2) {
    if (intrusao == 0) {
      tft.setCursor(0, 0);        // Escrever para o ecrã 
      tft.fillScreen(ILI9341_RED); 
      tft.setTextColor(ILI9341_WHITE); 
      tft.setTextSize(3);
      tft.println("> INTRUSAO! <");

      Serial.println(">>>>>>>>> INTRUSÃO! <<<<<<<<<");
      intrusao = 1;
    }
    tone(PIN_BUZZER, FREQ_BUZZER, 0);       // 0, tocar indefinidamente
  }
}

void funcEp2 () {
  int sensorValue = 0;

  sensorValue = analogRead(PIN_QRE);        // Ler valor do Sensor
  Serial.println(sensorValue);   // degug
  //Serial.print("Num. Ocorrencias "); 
  //Serial.println(contagem);         // debug

  if (sensorValue > V_MIN_QRE && sensorValue < V_MAX_QRE) {
    if (isLocked == 0) {
      contagem = contagem + 1;
      isLocked = 1;
    }
  } else {
    isLocked = 0;
  }
    
  // Se o resto da divisão for zero, LED é apagado
  if ((contagem & 1) == 0) {
    digitalWrite(PIN_LED_EP2, LOW);        // Por o LED a Low
  } else {
    digitalWrite(PIN_LED_EP2, HIGH);       // Por o LED a High
  }
}

