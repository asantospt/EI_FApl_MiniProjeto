/** 
 * @file PL_PL02_G08.ino
 * @brief SMART HOME - 5 EPs
 * @date 10/01/2018
 * @authors Afonso Santos (nr. 2130653) & Natanael Godinho (nr. 2110634) - Grupo 8
 * @state OK 
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
const int PIN_LED_EP2 = 11;
const int PIN_LED_EP3_VERMELHO = 6;   // Vermelho
const int PIN_LED_EP3_AMARELO = 7;    // amarelo
const int PIN_LED_EP3_VERDE = 8;      // Verde     
const int PIN_LED_EP4 = 9;            // Verde, Pin com PWM (IMPORTANTE) 
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
unsigned long instanteAtual_Ep45 = 0;   // Variável para guardar o tempo decorrido desde o arranque do Arduino [Milisegundos] 
// ---------------------- EP 5 ----------------------
const int ANG_ESTORES_ABERTOS = 30;         // Ângulo para estores abertos
const int ANG_ESTORES_INT = 75;             // Ângulo para estores em posição intermédia
const int ANG_ESTORES_FECHADOS = 120;       // Ângulo para estores fechados
const float T_LIMIAR_1 = 26.0;              // Temperatura considerada como Frio
const float T_LIMIAR_2 = 28.0;              // Temperatura considerada como Calor

const int T_LIMIAR_LUZ = 700;         // Valor para incidência de luz solar
const int MIN_ANG_SERVO = 10;         // Definição do ângulo mín. a enviar para o Servo, por segurança
const int MAX_ANG_SERVO = 170;        // Definição do ângulo máx. a enviar para o Servo, por segurança
int posServo = 0;                     // Variável que guarda a posição do Servo [Graus]
const int DELTA_T5 = 2000;          
unsigned long tRef5 = 0;  

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
  float temp_Ep3 = 0.0;
  int sensorValueLDR_Ep4 = 0;

  funcEp1();        // Função com funcionalidades do EP1
  funcEp2();        // Função com funcionalidades do EP2

  instanteAtual_Ep45 = millis();       // Contar milisegundos desde o arranque do sistema      

  temp_Ep3 = funcaoEp3();        // Função com funcionalidades do EP3

  if (instanteAtual_Ep45 >= DELTA_T4) {        
    // Já passaram os 10 s desde o arranque do programa, significa que já é 'madrugada'
      /*Serial.println(">>> Passaram 10 s desde arranque. Madrugada!! <<<");      
      Serial.println(" ");*/
    sensorValueLDR_Ep4 = funcaoEp4(MAX_BRILHO_MADRUGADA);         // Função com funcionalidades do EP4
  } else {
    sensorValueLDR_Ep4 = funcaoEp4(MAX_PERC_BRILHO);
  }

  if ((instanteAtual_Ep45 - tRef5) >= DELTA_T5) {        
    // Já passaram 2 s desde a última atuação do motor dos estores
      /*Serial.println(">>> Passaram 2 s <<<");      
      Serial.println(" ");*/
    funcaoEp5(temp_Ep3, sensorValueLDR_Ep4);     // Função com funcionalidades do EP5
    tRef5 = instanteAtual_Ep45;
  } 

  // Para garantir bom funcionamento do sensor ultrasons
  delay(100);
}

// **************************** Funções para as funcionalidades ****************************
/**
 * @brief Lê valores de distância do sensor de ultrassons. Se detetar 1 objeto e ninguém inserir um código válido, ativar alarme
 * @params - 
 * @return -
 */
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

/**
 * @brief Lê valores lidos pelo QRE. Identifica se existiu algum contato (passagem de animal)
 * @params - 
 * @return -
 */
void funcEp2 () {
  int sensorValue = 0;

  sensorValue = analogRead(PIN_QRE);        // Ler valor do Sensor
  //Serial.println(sensorValue);   
  //Serial.print("Num. Ocorrencias "); 
  //Serial.println(contagem);        

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

/**
 * @brief Lê a temperatura ambiente e devolve à função 'loop()'
 * @params -
 * @return Valor da temperatura (float)
 */
float funcaoEp3() {
  int sensorValueNTC = 0;
  float voltageNTC = 0.0;
  float rNTC = 0.0;
  float tNTC = 0.0;

  // Leitura do Vout do NTC (V_NTC)
  sensorValueNTC = analogRead(PIN_NTC);
  
  // Converter leitura analógica para voltagem
  voltageNTC = sensorValueNTC * (5.0 / 1023.0);
  //Serial.println(voltage); 

  // Calcular rNTC
  rNTC = ((R1 * 5) / (voltageNTC / GANHO_NTC)) - R1;
  // Calcular temperatura [Graus]
  tNTC = 1.0 / ((1.0 / T0) + ((1.0 / BETA) * log(rNTC / R0))) - KELVIN;    

  // Imprimir temperatura se existir alteração de 0,5 graus
  if (tNTC > (tNTCp + TMPOSC) || tNTC < (tNTCp - TMPOSC) ){
    tNTCp = tNTC;
    Serial.print("Temperatura: ");
    Serial.print(tNTCp);
    Serial.println(" [ºC]");
  } 

  if (tNTC <= TREF1) {                      // Temp. <= 15 ºC
    digitalWrite(PIN_LED_EP3_VERDE, HIGH);  // LED - Verde
  } else {
    digitalWrite(PIN_LED_EP3_VERDE, LOW);
  }
  if (tNTC > TREF2 && tNTC <= TREF3) {          // Temp. > 28 ºC e <= 30 ºC
    digitalWrite(PIN_LED_EP3_AMARELO, HIGH);    // LED - Amarelo
  } else {
    digitalWrite(PIN_LED_EP3_AMARELO, LOW);
  }
  if (tNTC > TREF3) {                           // Temp. > 30 ºC
    digitalWrite(PIN_LED_EP3_VERMELHO, HIGH);   // LED - Vermelho
  } else {
    digitalWrite(PIN_LED_EP3_VERMELHO, LOW);
  } 

  return tNTC;      // Devolve valor ao 'loop()'
}

/**
 * @brief Recebe o brilho máximo, lê a luminosidade existente, atua sobre o sistema de iluminação e devolve o valor à função 'loop()'
 * @params Valor máximo do brilho (int)
 * @return Valor da luminosidade (int)
 */
int funcaoEp4(int maxBrilho) {
  int sensorValueLDR = 0;
  int declive = 0;
  int maxBrilhoEntradaFuncao = 0;
  int brilhoLED = 0;

  maxBrilhoEntradaFuncao = maxBrilho;

  sensorValueLDR = analogRead(PIN_LDR);     
  //Serial.print("sensorValueLDR: ");
  //Serial.println(sensorValueLDR);      

  // brilhoLED = map(x_IN, x0, x1, y0, y1) 
  // Cálculo do brilho do LED, de forma linear, mediante o valor de luminosidade existente
  brilhoLED = map(sensorValueLDR, VALOR_MIN_LUZ, VALOR_MAX_LUZ, maxBrilhoEntradaFuncao, MIN_PERC_BRILHO);
  // Evitar valores fora do limite do brilho pedido
  brilhoLED = constrain(brilhoLED, MIN_PERC_BRILHO, maxBrilhoEntradaFuncao);    

  analogWrite(PIN_LED_EP4, brilhoLED);    // Escrita para o LED do valor do brilho (com PWM)
  delay(30);                     // Espera de 30 ms para se ver o efeito da alteração do brilho do LED [Milisegundos]
  //Serial.print("brilhoLED: ");   
  //Serial.println(brilhoLED);

  return sensorValueLDR;      // Devolve valor ao 'loop()'
}

/**
 * @brief Recebe a temperatura e a luminosidade da casa e altera a posição dos estores
 * @params Temperatura (float), Voltagem da LRD (int)
 * @return -
 */
void funcaoEp5(float temp_Ep3, int voltageLDR_Ep4) {
  float temp_Entrada = 0.0;
  int voltageLDR_Entrada = 0;

  temp_Entrada = temp_Ep3;
  voltageLDR_Entrada = voltageLDR_Ep4;

  // Situações possíveis para alteração da posição dos estores
  if ((temp_Entrada < T_LIMIAR_1) && (voltageLDR_Entrada > T_LIMIAR_LUZ))  {    // Frio
    myservo.write(ANG_ESTORES_ABERTOS);           // Ângulo 30º - Estores abertos
    //Serial.println("Cond. 1 - ESTORES ABERTOS");          
    //Serial.println(" ");
    delay(15);                 
  }
  else if ((temp_Entrada > T_LIMIAR_2) && (voltageLDR_Entrada > T_LIMIAR_LUZ)) {    // Calor
    myservo.write(ANG_ESTORES_FECHADOS);          // Ângulo 120º - Estores fechados
    //Serial.println("Cond. 2 - ESTORES FECHADOS");    
    //Serial.println(" ");
    delay(15);
  }
  else {              // Se temperatura estiver entre mín. e máx., ou igual aos limites. Luz é indiferente 
    myservo.write(ANG_ESTORES_INT);                  // Ângulo 75º - Estores em posição intermédia
    //Serial.println("Cond. 3 - Estores em posição intermédia");    
    //Serial.println(" "); 
    delay(15);
  }

  // Nota: em ambiente real, verificação poderia ser a cada 15 minutos, estamos a usar 2 s
}

