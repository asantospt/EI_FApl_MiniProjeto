/** 
 * @file EP4.ino
 * @brief EP4 – Iluminação noturna de presença
 * @date 06/01/2018
 * @authors @authors Afonso Santos (nr. 2130653) & Natanael Godinho (nr. 2110634)
 * @state INC
 */

/** 
 * LIGAÇÕES: @Arduino 'Mega 2560'
 * 5 V -> USB
 * GND -> breadboard
 * Pin A2 -> LDR + (A2) + Res. 22 kOhm
 * Pin 9 -> LED verde
 * 
 * @LDR - luminosidade
 * + -> 5V
 * - -> V_out, Res. 22 kOhm + Arduino/Pin A2
 * 
 * @LED verde
 * + -> Res. 220 Ohm + Arduino/Pin 9
 * - -> GND
 */

// Atribuição dos pinos do Arduino 
const int PIN_LDR = A2; 
const int PIN_LED_EP4 = 4;      // verde, Mega 9

// Valores pré-definidos no enunciado
const int MIN_PERC_BRILHO = 0;        // Brilho mín., quando "luz ambiente normal"
const int MAX_PERC_BRILHO = 255;      // Brilho máx., quando "ausência de luz"
const int MAX_BRILHO_MADRUGADA = 128;     // Metade do brilho máximo = 127.5
const int DELTA_T3 = 10000;               // 10 s para simular as 04:00h da madrugada [Segundos]  

// Variáveis globais do programa
const float VALOR_MIN_LUZ = 3.7;       // Valor min. de voltagem (LDR tapada) = 2.00   
const float VALOR_MAX_LUZ = 4.58;      // Valor máx. de voltagem (LDR a descoberta) = 4.50 
const int TEMPO_MADRUGADA = 10000;     // Simulação da 04:00 da madrugada. 10 s [Milissegundos]
unsigned long instanteAtual_Ep4 = 0;
unsigned long tRef3 = 0;              // Varíavel para guardar tempo de referência de contagem dos 10 s
boolean isMadrugada = false;             

// Declaração de funções
void funcao_Ep4(int maxBrilho);

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_LED_EP4, OUTPUT);

  // Acender o LED durante 2 s no arranque do sistema
  digitalWrite(PIN_LED_EP4, HIGH);   
  delay(2000);                  
  digitalWrite(PIN_LED_EP4, LOW);   
}

void loop() {
  instanteAtual_Ep4 = millis();       // Contar milisegundos desde o arranque do sistema

  if (((instanteAtual_Ep4 - tRef3) >= DELTA_T3) && isMadrugada == false) {        
    // Já passaram os 10 s desde o arranque do programa, significa que já é 'madrugada'
    tRef3 = instanteAtual_Ep4;
    isMadrugada = true;
      Serial.println("** passaram 10 s desde arranque. F-Madrugada!!");   // debug
    funcao_Ep4(MAX_BRILHO_MADRUGADA);
  } else {
    funcao_Ep4(MAX_PERC_BRILHO);
  }
}

/**
 * @brief Recebe o brilho máximo a aplicar, lê luminosidade existente e atua sobre o brilho de um LED
 * @params Valor máximo do brilho, em função da hora
 * @return -
 */
void funcao_Ep4(int maxBrilho) {
  int sensorValueLDR = 0;
  float voltageLDR = 0.0;
  float declive = 0.0;
  int maxBrilhoEntradaFuncao = 0;
  int brilhoLED = 0;

  maxBrilhoEntradaFuncao = maxBrilho; 
    //Serial.println(sensorValueLDR);     // debug
  sensorValueLDR = analogRead(PIN_LDR);
  //Serial.println(sensorValueLDR);      // debug

  voltageLDR = sensorValueLDR * (5.0 / 1023.0);     // Conversão da leitura analógica (0-1023) para uma voltagem (0-5 V) 
  Serial.println(voltageLDR);      // debug

  /* 
  x = luminosidade LDR (VALOR_MIN_LUZ, VALOR_MAX_LUZ)
  y = brilho LED (MIN_PERC_BRILHO, MAX_PERC_BRILHO ou MAX_BRILHO_MADRUGADA)
  Declive: m = (y1 - y0) / (x1 - x0) 
  Equação da reta: y = y0 + m(x - x0)   */
  /*declive = (float)(maxBrilho - MIN_PERC_BRILHO) / (VALOR_MAX_LUZ - VALOR_MIN_LUZ);
       // maxBrilho é o valor máx. da luminosidade, em função da hora do dia
  brilhoLED = (int) 0 + declive * (sensorValueLDR - VALOR_MIN_LUZ);*/

  brilhoLED = map(voltageLDR, VALOR_MIN_LUZ, VALOR_MAX_LUZ, MIN_PERC_BRILHO, maxBrilhoEntradaFuncao);
  if (brilhoLED > MIN_PERC_BRILHO || brilhoLED < maxBrilhoEntradaFuncao) {      // Só quando deve alterar brilho do LED
    analogWrite(PIN_LED_EP4, brilhoLED);
    // OU constrain(yout, y0, y1);
    // constrain(brilhoLED, y0, y1);
  }  

    //Serial.println(maxBrilho);     // debug
    //Serial.println("** luz a metade!");     // debug
}