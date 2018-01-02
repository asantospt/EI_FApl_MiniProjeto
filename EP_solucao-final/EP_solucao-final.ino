/** 
 * @file EP_solucao-final.ino
 * @brief SMART HOME - EP1 + EP2
 * @date 01/01/2018
 * @authors Afonso Santos (nr. 2130653) & Natanael Godinho (nr. 2110634)
 * @state rever código/ testar arranque + loop
 */

 /** 
  * LIGAÇÕES: @Arduino 'Mega 2560'
  * 5 V -> USB
  * GND -> breadboard
  * Pin A0 -> Sensor reflexão QRE1113/ Pin 3     // EP2
  * Pin 0 -> 
  * Pin 2 -> Sensor ultrassons/ Echo       // EP1
  * Pin 3 -> Sensor ultrassons/ Trig       // EP1
  * Pin 4 -> Buzzer/ +                     // EP1
  * Pin 5 -> LED vermelho                  // EP2
  * Pin 10 -> TODO:
  * Pin 11 -> 
  * Pin 12 -> 
  * Pin 13 -> 
  *
  * @Sensor reflexão 'QRE1113'
  * 1 -> 5 V + R1 (130 kOhm)
  * 2 -> GND
  * 3 -> V_out + R2 (10 kOhm) 
  * 4 -> GND
  *
  * @Sensor ultrassons 'HC-SR04'
  * VCC -> 5 V
  * GND -> GND 
  * Echo -> Arduino/ Pin 2
  * Trig -> Arduino/ Pin 3
  * 
  * @Buzzer 
  * + -> Arduino/ Pin 4 + Res. 10 kOhm
  * - -> GND
  *
  * @ecrã LCD TFT  TODO:
  * VCC -> 3.3 V
  * GND -> 0 V
  * CS -> 
  * DC/RS -> 
  * MOSI -> 
  * MISO -> 
  * SCK -> 
  * LED -> Res. 10 kOhm + VCC
  * RESET -> VCC
*/

// Bibliotecas extra usadas
#include <SPI.h>
#include <Adafruit_GFX.h> 
#include <Adafruit_ILI9341.h>

// LCD: definir os terminais de DC e CS
#define TFT_DC 9      // TODO: conferir com Natanael, terminal=0 (trig e echo tb estava trocado) 
#define TFT_CS 10

// Atribuição dos pinos do Arduino 
const int PIN_ECHO = 2;
const int PIN_TRIG = 3;
const int PIN_BUZZER = 4;
const int FREQ_BUZZER = 880;

// Valores pré-definidos

// Variáveis globais do programa



// criar um objeto tft com indicacao dos terminais DC e CS
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Valores pré-definidos no enunciado
/* Distancia Minima para iniciar procedimento de alarme
60 cm
Tempo para introduzir código de segurança e desligar alarme
10 Segundos */

// vel. propagação dos ultrassons no ar = 343 m/s 
// Formula Utilizada Dcm = deltaT / 58,31 cm/MicroSegundo
const float VEL_ULTRASSONS_AR = 58.31;  

//Centimetros
const int DIST_MIN_ALARM = 60;
//Segundos
const int TIME_TO_SHUTDOWN = 10;

//Controle de tempos
//T1: Tempo entre medição de distancia - Milisegundos
//T2: Tempo até acionamento do alarme - Milisegundos
const int DELTA_T1 = 1000;
unsigned long tRef1 = 0;

const int DELTA_T2 = 10000;
unsigned long tRef2 = 0;

//Alarme accionado
bool alarm = 0;

//PRINT INTRUSÃO
bool intrusao = 0;

//Codigo Alarme
int SECRET = 1234;

//EP2
// Constantes usadas no programa 
const int PIN_QRE = A0;
const int PIN_LED_EP2 = 5;

//bool led = 0;       // TODO: usado? n pode ser zero
int contagem = 0;
//Variavel para implementar funcionalidade de apenas uma ocorrencia por reflexão
//ao entrar no range definido só volta a contar depois de sair do range indicado
bool toogle = 0;

// Valores pré-definidos no enunciado
/**
Valor impar se animal está na rua e valor par se estiver em casa
*/

// Valores de referência - Montagem Inicial
/**
sensorValueHIGH: 1000
sensorValueLOW: 500
Formula para identificar passagem de animal:
Valor médio:
(100+500)/2=750
Tolerancia = 50 + -
*/

// Declaração de funções
void funcEp1();
void funcEp2();

void setup() {
  Serial.begin (9600);

  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_LED_EP2, OUTPUT);

  tft.begin();    // inicializar o tft
  tft.fillScreen(ILI9341_BLACK);    // colocar fundo a preto
  tft.setRotation(2);       // definir orientacao da escrita
  
  digitalWrite(PIN_TRIG, LOW);  // Ao inicializar o sistema o pin Trig deve estar em modo LOW
  delayMicroseconds(2);

  digitalWrite(PIN_LED_EP2, HIGH);

  tone(PIN_BUZZER, FREQ_BUZZER, 500);      // TODO: var p/ tempos buzzer? falta calar o buzzer no Loop
  delay(500);                  // Acionar o buzzer por 0,5 segundos inicialmente
  noTone(PIN_BUZZER);                    

  delay(1500);                 // Para completar os 2 s em que o LED do EP2 deve estar ligado no arranque
  digitalWrite(PIN_LED_EP2, LOW);   // TODO: testar se LED só acende 2 segs
}

void loop() {
  funcEp1();        // Função com funcionalidades do EP1
  funcEp2();        // Função com funcionalidades do EP2

  //para garantir bom funcionamento do sensor ultrasons
  delay(100);
}

void funcEp1(){
  //Contar milisegundos desde o arranque do sistema
  float instanteAtual = millis();

  //Registar a duração do pulso do echo - Microsegundos
  long duration;

  //Tratar dos Pinos do sensor Ultrasons
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW); 
  

// Ler distancia a cada segundo
  if ( (instanteAtual - tRef1) >= DELTA_T1 ){
    duration = pulseIn(PIN_ECHO, HIGH);
    float distance = ((float)duration / VEL_ULTRASSONS_AR);  
      
      if (alarm == 0 && distance < 61){
        Serial.print(distance); 
        Serial.println(" cm");
        Serial.println("Insira código de Segurança");
        tRef1 = instanteAtual;
        tRef2 = instanteAtual;
        alarm = 1;
        intrusao = 0;
        //escrever para o ecrã
        tft.fillScreen(ILI9341_YELLOW);
        tft.setCursor(0, 0);
        tft.setTextColor(ILI9341_RED); 
        tft.setTextSize(2);
        tft.println("Insira Codigo Seguranca");
  

      }
  }

  int secret;
  //receber texto enviado pela consola da porta serie
  secret = Serial.parseInt();

  if (alarm == 1 && SECRET == secret ){
    tone(PIN_BUZZER, FREQ_BUZZER, 1);
    alarm = 0;
    //escrever no ecrã
    tft.fillScreen(ILI9341_GREEN);
    tft.setCursor(0, 0);
    tft.setTextColor(ILI9341_BLACK); 
    tft.setTextSize(2);
    tft.println("OK");
  }

  if (alarm == 1 && (instanteAtual - tRef2) >= DELTA_T2 ){
    if(intrusao == 0){
      //escrever no ecrã
      tft.setCursor(0, 0);
      tft.fillScreen(ILI9341_RED); 
      tft.setTextColor(ILI9341_WHITE); 
      tft.setTextSize(3);
      tft.println("INTRUSAO");
      
      Serial.println("INTRUSÃO!");
      intrusao = 1;
    }
    tone(PIN_BUZZER, FREQ_BUZZER, 0);
  }
}

void funcEp2 (){
    //Ler valor do Sensor
  int sensorValue = analogRead(PIN_QRE);
  //degug
  Serial.println(sensorValue);
  //debug
  Serial.println(contagem);

  if(sensorValue > 400 && sensorValue < 850){
    if(toogle==0){
        contagem = contagem + 1;
        toogle = 1;
    }
  }else{
      toogle = 0;
    }
    
  if ( (contagem & 1) == 0) {
     //Por o led a High
     digitalWrite(PIN_LED_EP2, LOW);    
  }else{
    //Por o led a low
    digitalWrite(PIN_LED_EP2, HIGH);
    }
}