/** 
 * @file EP1-2-3-4-5.ino
 * @brief SMART HOME: implementação das 5 EP's em simultâneo. LCD a funcionar com EP1
 * @date 08/01/2018
 * @authors Afonso Santos (nr. 2130653) & Natanael Godinho (nr. 2110634)
 * @state cod. da AULA1 é o que foi entregue - A FUNCIONAR
 *      falta colocar cód. da AULA2 - já pus tudo até setup()
 *      NOTA: código da AULA1, não teve nenhuma alteração
 */

// Bibliotecas extra usadas
#include <SPI.h>
#include <Adafruit_GFX.h> 
#include <Adafruit_ILI9341.h>
#include <Servo.h>

// LCD: definir os terminais de DC e CS
#define TFT_DC 49       
#define TFT_CS 53

// Atribuição dos pinos do Arduino 
const int PIN_QRE = A0;
const int PIN_NTC = A1; 
const int PIN_LDR = A2; 
const int PIN_TRIG = 2;
const int PIN_ECHO = 3;
const int PIN_BUZZER = 4;
const int PIN_LED_EP2 = 5;
const int PIN_LED_EP3_VERDE = 6;      // Verde
const int PIN_LED_EP3_AMARELO = 7;    // Amarelo
const int PIN_LED_EP3_VERMELHO = 8;   // Vermelho
const int PIN_LED_EP4 = 5;            // Verde, Pin com PWM (IMPORTANTE)    TODO: Mega/Pin 9 
const int PIN_SERVO = 10;

// Declaração de valores pré-definidos no enunciado e/ou váriáveis globais
// ---------------------- EP 1 ----------------------
const int FREQ_BUZZER = 880;            // Frequência [Hz]
const float DIST_MIN_ALARM = 61.0;      // Distância mínima [Centímetros]
const int DELTA_T2 = 10000;             // 10 s para acionar o alarme [Segundos]
const float VEL_ULTRASSONS_AR = 58.31;  // Velocidade de propagação dos ultrassons no ar = 343 m/s 
                                        // Formula Utilizada D[cm] = deltaT / 58,31 [cms/Microsegundos]

const int DELTA_T1 = 1000;              // Tempo entre medições de distância [Milisegundos]
unsigned long tRef1 = 0;                // Varíavel para guardar tempo de referência entre medições de distância
unsigned long tRef2 = 0;                // Varíavel para guardar tempo de referência de contagem dos 10 s
float instanteAtual = 0.0;    // Varíavel para guardar o tempo decorrido deste o arranque do Arduino [Milisegundos]
int alarm = 0;       // Varíavel para indicar acionamento do alarme
int intrusao = 0;    // Varíavel para escrever 1 única vez no ecrã
int SECRET = 1234;   // Código do Alarme
// ---------------------- EP 2 ----------------------
const int V_MIN_QRE = 400;          // Valor mínimo definido para identificação de reflexão
const int V_MAX_QRE = 850;          // Valor máximo definido para identificação de reflexão
int contagem = 0;       // Contador de ocorrências
int isLocked = 0;       // Varíavel para garantir 1 única ocorrência de cada vez que faz 1 leitura dentro do intervalo definido
// ---------------------- EP 3 ----------------------
const int R1 = 3600;            // Resistência a associar à NTC 
const int VIN = 5;               
const float T0 = 298.15;        // Valor de 25 ºC em Kelvin
const int BETA = 4090;          // Parâmetro do sensor de temperatura NTC
const int R0 = 3300;            // Parâmetro do sensor de temperatura NTC
const float KELVIN = 273.15;    // Valor para converter para Graus
const int TREF1 = 15;           // Temperatura mín. de referência
const int TREF2 = 28;           // Temperatura média de referência
const int TREF3 = 30;           // Temperatura máx. de referência

const float GANHO_NTC = 1.66;   // Valor do ganho calculado
const float TMPOSC = 0.5;       // Valor de variação de temperatura para impressão de temperatura no monitor série  
float tNTCp = 0.0;              // Variável para impressão de temperatura no monitor série
// ---------------------- EP 4 ----------------------
const int MIN_PERC_BRILHO = 0;        // Brilho mín., quando "luz ambiente normal"
const int MAX_PERC_BRILHO = 255;      // Brilho máx., quando "ausência de luz"
const int MAX_BRILHO_MADRUGADA = 128;     // Metade do brilho máximo = 127.5
const int DELTA_T4 = 10000;               // 10 s para simular as 04:00h da madrugada [Milissegundos]

const int VALOR_MIN_LUZ = 50;        // Valor mín. de voltagem (LDR sem receber luz)   
const int VALOR_MAX_LUZ = 1000;      // Valor máx. de voltagem (LDR a receber muita luz)      
unsigned long instanteAtual_Ep4 = 0;      // Varíavel para guardar o tempo decorrido desde o arranque do Arduino [Milisegundos] 
// ---------------------- EP 5 ----------------------
const int ANG_ESTORES_ABERTOS = 30;         // Ângulo para estores abertos
const int ANG_ESTORES_INT = 75;             // Ângulo para estores em posição intermédia
const int ANG_ESTORES_FECHADOS = 120;       // Ângulo para estores fechados
const float T_LIMIAR_1 = 15.0;              // Temperatura considerada como Frio
const float T_LIMIAR_2 = 28.0;              // Temperatura considerada como Calor

const int T_LIMIAR_LUZ = 700;         // Valor para incidência de luz solar
const int MIN_ANG_SERVO = 10;         // Definição do ângulo mín. a enviar para o Servo, por segurança
const int MAX_ANG_SERVO = 170;        // Definição do ângulo máx. a enviar para o Servo, por segurança
int posServo = 0;                     // Variável que guarda a posição do Servo [Graus]

// Criar um objeto tft com indicacao dos terminais DC e CS; objeto para controlar o Servomotor 
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Servo myservo;    

// Declaração de funções
void funcEp1();
void funcEp2();
float funcaoEp3();
int funcaoEp4(int maxBrilho);
void funcaoEp5(float temp_Ep3, float voltageLDR_Ep4);

void setup() {
  Serial.begin (9600);

  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_LED_EP2, OUTPUT);
  pinMode(PIN_LED_EP3_VERDE, OUTPUT);
  pinMode(PIN_LED_EP3_AMARELO, OUTPUT);
  pinMode(PIN_LED_EP3_VERMELHO, OUTPUT);
  pinMode(PIN_LED_EP4, OUTPUT);
  myservo.attach(PIN_SERVO);

  tft.begin();    // inicializar o tft
  tft.fillScreen(ILI9341_BLACK);    // colocar fundo a preto
  tft.setRotation(2);               // definir orientacao da escrita
  
  digitalWrite(PIN_TRIG, LOW);  // Ao inicializar o sistema o pin Trig deve estar em modo LOW
  delayMicroseconds(2);

  digitalWrite(PIN_LED_EP2, HIGH);
  digitalWrite(PIN_LED_EP3_VERDE, HIGH);
  digitalWrite(PIN_LED_EP3_AMARELO, HIGH);
  digitalWrite(PIN_LED_EP3_VERMELHO, HIGH);
  digitalWrite(PIN_LED_EP4, HIGH);

  tone(PIN_BUZZER, FREQ_BUZZER, 500);     
  delay(500);                       // Acionar o buzzer por 0.5 s inicialmente
  noTone(PIN_BUZZER);                    

  delay(1500);                      // Para completar os 2 s em que o LED do EP2 deve estar ligado no arranque
  digitalWrite(PIN_LED_EP2, LOW);   
  digitalWrite(PIN_LED_EP3_VERDE, LOW);
  digitalWrite(PIN_LED_EP3_AMARELO, LOW);
  digitalWrite(PIN_LED_EP3_VERMELHO, LOW);
  digitalWrite(PIN_LED_EP4, LOW);

  // Ativação do Servo no arranque do sistema
  for (posServo = MIN_ANG_SERVO; posServo <= MAX_ANG_SERVO; posServo += 1) { 
    myservo.write(posServo);          
    delay(15);                       
  }
  for (posServo = MAX_ANG_SERVO; posServo >= MIN_ANG_SERVO; posServo -= 1) { 
    myservo.write(posServo);            
    delay(15);                        
  } 
}

void loop() {
  funcEp1();        // Função com funcionalidades do EP1
  funcEp2();        // Função com funcionalidades do EP2

  // Para garantir bom funcionamento do sensor ultrasons
  delay(100);
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
