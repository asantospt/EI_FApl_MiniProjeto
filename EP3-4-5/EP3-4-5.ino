/** 
 * @file EP3-4-5.ino
 * @brief SMART HOME - EP3 + EP4 + EP5
 * @date 10/01/2018
 * @authors Afonso Santos (nr. 2130653) & Natanael Godinho (nr. 2110634)
 * @state OK
 *      ver TODOs + apagar debugs
 */

// Declaração de bibliotecas extra
#include <Servo.h>

// Atribuição dos pinos do 'Arduino Mega 2560'
const int PIN_NTC = A1; 
const int PIN_LDR = A2; 
const int PIN_LED_EP3_VERMELHO = 6;   // Vermelho
const int PIN_LED_EP3_AMARELO = 7;    // amarelo
const int PIN_LED_EP3_VERDE = 8;      // Verde     
const int PIN_LED_EP4 = 9;            // Verde, Pin com PWM (IMPORTANTE) 
const int PIN_SERVO = 10;

// Declaração de valores pré-definidos no enunciado e/ou váriáveis globais  
// ---------------------- EP 3 ----------------------
const int R1 = 3600;            // Resistência a associar à NTC 
const int VIN = 5;               
const float T0 = 298.15;        // Valor de 25 ºC em Kelvin
const int BETA = 4090;          // Parâmetro do sensor de temperatura NTC
const int R0 = 3300;            // Parâmetro do sensor de temperatura NTC
const float KELVIN = 273.15;    // Valor para converter para Graus
const int TREF1 = 25;           // Temperatura mín. de referência     // TODO:
const int TREF2 = 28;           // Temperatura média de referência
const int TREF3 = 30;           // Temperatura máx. de referência

const float GANHO_NTC = 1.66;   // Valor do ganho calculado
const float TMPOSC = 1.0;       // Valor de variação de temperatura para impressão de temperatura no monitor série  
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
const float T_LIMIAR_1 = 15.0;              // Temperatura considerada como Frio
const float T_LIMIAR_2 = 28.0;              // Temperatura considerada como Calor

const int T_LIMIAR_LUZ = 700;         // Valor para incidência de luz solar
const int MIN_ANG_SERVO = 10;         // Definição do ângulo mín. a enviar para o Servo, por segurança
const int MAX_ANG_SERVO = 170;        // Definição do ângulo máx. a enviar para o Servo, por segurança
int posServo = 0;                     // Variável que guarda a posição do Servo [Graus]
const int DELTA_T5 = 2000;          
unsigned long tRef5 = 0;  

// Criação de um objeto para controlar o Servomotor
Servo myservo;     

// Declaração de funções
float funcaoEp3();
int funcaoEp4(int maxBrilho);
void funcaoEp5(float temp_Ep3, float voltageLDR_Ep4);

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
  
  // Ativação do Servo no arranque do sistema
  for (posServo = MIN_ANG_SERVO; posServo <= MAX_ANG_SERVO; posServo += 1) { 
    myservo.write(posServo);          
    delay(15);        // Espera de 15 ms para que Servo pare em cada posição (muito rapidamente)                
  }
  for (posServo = MAX_ANG_SERVO; posServo >= MIN_ANG_SERVO; posServo -= 1) { 
    myservo.write(posServo);            
    delay(15);                        
  } 
}

void loop() {
  float temp_Ep3 = 0.0;
  int sensorValueLDR_Ep4 = 0;

  instanteAtual_Ep45 = millis();       // Contar milisegundos desde o arranque do sistema      

  temp_Ep3 = funcaoEp3();        // Função com funcionalidades do EP3

  if (instanteAtual_Ep45 >= DELTA_T4) {        
    // Já passaram os 10 s desde o arranque do programa, significa que já é 'madrugada'
      //Serial.println(">>> Passaram 10 s desde arranque. Madrugada!! <<<");       // debug
      //Serial.println(" ");
    sensorValueLDR_Ep4 = funcaoEp4(MAX_BRILHO_MADRUGADA);         // Função com funcionalidades do EP4
  } else {
    sensorValueLDR_Ep4 = funcaoEp4(MAX_PERC_BRILHO);
  }

  if ((instanteAtual_Ep45 - tRef5) >= DELTA_T5) {        
    // Já passaram 2 s desde a última atuação do motor dos estores
      //Serial.println(">>> Passaram 2 s <<<");       // debug
      //Serial.println(" ");
    funcaoEp5(temp_Ep3, sensorValueLDR_Ep4);     // Função com funcionalidades do EP5
    tRef5 = instanteAtual_Ep45;
  } 

  delay(200);
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
  //Serial.println(voltage);    //para Debug

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
  //Serial.println(sensorValueLDR);      // debug
  //Serial.println(" ");

  // brilhoLED = map(x_IN, x0, x1, y0, y1) 
  // Cálculo do brilho do LED, de forma linear, mediante o valor de luminosidade existente
  brilhoLED = map(sensorValueLDR, VALOR_MIN_LUZ, VALOR_MAX_LUZ, maxBrilhoEntradaFuncao, MIN_PERC_BRILHO);
  // Evitar valores fora do limite do brilho pedido
  brilhoLED = constrain(brilhoLED, MIN_PERC_BRILHO, maxBrilhoEntradaFuncao);    

  analogWrite(PIN_LED_EP4, brilhoLED);    // Escrita para o LED do valor do brilho (com PWM)
  delay(30);                     // Espera de 30 ms para se ver o efeito da alteração do brilho do LED [Milisegundos]
  //Serial.print("brilhoLED: ");   // debug
  //Serial.println(brilhoLED);
  //Serial.println(" ");

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
    //Serial.println("cond. 1 - ESTORES ABERTOS");    // debug      
    //Serial.println(" ");
    delay(15);                 // TODO: e se não usarmos?
  }
  else if ((temp_Entrada > T_LIMIAR_2) && (voltageLDR_Entrada > T_LIMIAR_LUZ)) {    // Calor
    myservo.write(ANG_ESTORES_FECHADOS);          // Ângulo 120º - Estores fechados
    //Serial.println("cond. 2 - ESTORES FECHADOS");   // debug 
    //Serial.println(" ");
    delay(15);
  }
  else {              // Se temperatura estiver entre mín. e máx., ou igual aos limites. Luz é indiferente 
    myservo.write(ANG_ESTORES_INT);                  // Ângulo 75º - Estores em posição intermédia
    //Serial.println("cond. 3 - Estores em posição intermédia");    // debug
    //Serial.println(" "); 
    delay(15);
  }

  // Nota: em ambiente real, verificação poderia ser a cada 15 minutos
}
