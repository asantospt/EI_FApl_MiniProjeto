/** 
 * @file EP4.ino
 * @brief EP4 – Iluminação noturna de presença
 * @date 08/01/2018
 * @authors @authors Afonso Santos (nr. 2130653) & Natanael Godinho (nr. 2110634)
 * @state OK com map(). ver 2 detalhes
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
 * + -> Res. 330 Ohm + Arduino/Pin 9
 * - -> GND
 */

// Atribuição dos pinos do Arduino 
const int PIN_LDR = A2; 
const int PIN_LED_EP4 = 5;      // verde, Pin com PWM (IMPORTANTE)    TODO: Mega/Pin 9

// Valores pré-definidos no enunciado
const int MIN_PERC_BRILHO = 0;        // Brilho mín., quando "luz ambiente normal"
const int MAX_PERC_BRILHO = 255;      // Brilho máx., quando "ausência de luz"
const int MAX_BRILHO_MADRUGADA = 128;     // Metade do brilho máximo = 127.5
const int DELTA_T4 = 10000;               // 10 s para simular as 04:00h da madrugada [Segundos]  

// Variáveis globais do programa
const int VALOR_MIN_LUZ = 50;        // Valor min. de voltagem (LDR tapada) =       
const int VALOR_MAX_LUZ = 1000;      // Valor máx. de voltagem (LDR a descoberta) =     
const int TEMPO_MADRUGADA = 10000;     // Simulação da 04:00 da madrugada. 10 s [Milissegundos]
unsigned long instanteAtual_Ep4 = 0;
boolean isMadrugada = false;           // Declara a variável com valor '0'          

// Declaração de funções
int funcaoEp4(int maxBrilho);

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_LED_EP4, OUTPUT);

  // Acender o LED durante 2 s no arranque do sistema
  digitalWrite(PIN_LED_EP4, HIGH);   
  delay(2000);                  
  digitalWrite(PIN_LED_EP4, LOW);   
}

void loop() {
  int sensorValueLDR_Ep4 = 0;

  instanteAtual_Ep4 = millis();       // Contar milisegundos desde o arranque do sistema
 
  if (instanteAtual_Ep4 >= DELTA_T4) {      
    // Já passaram os 10 s desde o arranque do programa, significa que já é 'madrugada'
    isMadrugada = true;
    //Serial.println(isMadrugada);
    Serial.println(">>> Passaram 10 s desde arranque. Madrugada!! <<<");   // debug
    sensorValueLDR_Ep4 = funcaoEp4(MAX_BRILHO_MADRUGADA);
  } else {
    isMadrugada = false;
    //Serial.println(isMadrugada);
    sensorValueLDR_Ep4 = funcaoEp4(MAX_PERC_BRILHO);
  }

  /*if ((instanteAtual_Ep4 >= DELTA_T4) && isMadrugada == false) {
    Serial.println(">>>>>>>>> HORA: 04:00h | É Madrugada! <<<<<<<<<");   // debug
    mensagemMadrugada = true;  
    // TODO: alterar qd tiver forma de sair da MADRUGADA
  } */

  delay(1000);
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

  /**
   * Valores lidos c/ LDR do Natanael:
   * sensorValueLDR: min: 50; max: 1000
   * voltageLDR: min: 2.1; max: 4.9
   */
  /** 
   * Valores lidos c/ LDR do Labº:
   * sensorValueLDR: min: ; max: 
   * voltageLDR: min: 2.00; max: 4.50   (no RP3-c)
   */
  maxBrilhoEntradaFuncao = maxBrilho;

  sensorValueLDR = analogRead(PIN_LDR);     
  Serial.print("sensorValueLDR: ");
  Serial.println(sensorValueLDR);      // debug

  /**
   * x = Luminosidade LDR (VALOR_MAX_LUZ, VALOR_MIN_LUZ)
   * y = Brilho LED (MIN_PERC_BRILHO, MAX_PERC_BRILHO ou MAX_BRILHO_MADRUGADA)
   * Declive: m = (y1 - y0) / (x1 - x0) 
   * Equação da reta: y = y0 + m(x - x0) 
   */ 
  // 1ª forma       // TODO: test: evita valores negativos? 
  declive = (MIN_PERC_BRILHO - maxBrilhoEntradaFuncao) / (VALOR_MAX_LUZ - VALOR_MIN_LUZ);
  brilhoLED = MIN_PERC_BRILHO + (declive * (sensorValueLDR - VALOR_MIN_LUZ));     
  

  //brilhoLED = (maxBrilhoEntradaFuncao / (VALOR_MIN_LUZ - VALOR_MAX_LUZ)) * (sensorValueLDR - VALOR_MAX_LUZ);
  //brilhoLED = constrain(brilhoLED, MIN_PERC_BRILHO, maxBrilhoEntradaFuncao);

  /* 2ª forma: OK
  brilhoLED = map(x_IN, x0, x1, y0, y1) 
  /*brilhoLED = map(sensorValueLDR, VALOR_MIN_LUZ, VALOR_MAX_LUZ, maxBrilhoEntradaFuncao, MIN_PERC_BRILHO);
      // brilhoLED = map(sensorValueLDR, 0, 1023, 255, 0);
  brilhoLED = constrain(brilhoLED, MIN_PERC_BRILHO, maxBrilhoEntradaFuncao);  */  

  analogWrite(PIN_LED_EP4, brilhoLED);
  delay(30);       // Esperar 30 ms para ser o efeito da alteração do brilho do LED [Milisegundos]
  Serial.print("brilhoLED: "); 
  Serial.println(brilhoLED);
  Serial.print(" ");

  return sensorValueLDR;
}