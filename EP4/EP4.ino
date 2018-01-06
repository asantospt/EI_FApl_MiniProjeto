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
const int PIN_LED_EP4 = 9;

// Valores pré-definidos no enunciado 
const int MAX_BRILHO_MADRUGADA = 128;     // Metade do brilho máximo = 127.5
const int DELTA_T3 = 10000;             // 10 s para simular as 04:00h da madrugada [Segundos]

// Variáveis globais do programa
const int MIN_PERC_BRILHO = 0;        // Brilho mínimo 
const int MAX_PERC_BRILHO = 255;      // Brilho máximo 
const float VALOR_MIN_LUZ = 2.00;     // valor min. de luminosidade lido (tapado) = 2.00    TODO:
const float VALOR_MAX_LUZ = 4.50;     // valor max. de luminosidade lido (a descoberto) = 4.50 TODO: test leitura
const int TEMPO_MADRUGADA = 10000;    // Simulação da 04:00 da madrugada. 10 s [Milissegundos]
unsigned long tRef3 = 0;              // Varíavel para guardar tempo de referência de contagem dos 10 s

// Declaração de funções
void funcao_Ep4(int maxBrilho);

void setup() {
  Serial.begin(9600);
  
  pinMode(PIN_LED_EP4, OUTPUT);

  // Acender o LED 2 segs. para teste
  digitalWrite(PIN_LED_EP4, HIGH);   
  delay(2000);                  
  digitalWrite(PIN_LED_EP4, LOW);     
}

void loop() {
  unsigned long instanteAtual = 0;
  boolean madrugada = false; 

  instanteAtual = millis();       // Contar milisegundos desde o arranque do sistema
  
  if ((instanteAtual - tRef3) >= DELTA_T3) {        
    // Já passaram os 10 s desde o arranque do programa, significa que já é madrugada
    tRef3 = instanteAtual;
    //madrugada = true;
    Serial.println("** passaram 10 s desde arranque. F-Madrugada!! ");
    funcao_Ep4(MAX_PERC_BRILHO);
  } else {
    //madrugada = false;
    funcao_Ep4(MAX_BRILHO_MADRUGADA);
  }
}

void funcao_Ep4(int maxBrilho) {
  int sensorValueLDR = 0;
  float voltageLDR = 0.0;
  float declive = 0.0;
  int brilhoLED = 0;

  sensorValueLDR = analogRead(PIN_LDR);
  Serial.println(sensorValueLDR);     // debug

  voltageLDR = sensorValueLDR * (5.0 / 1023.0);     // Conversão da leitura analógica (0-1023) para uma voltagem (0-5 V) 
  Serial.println(voltageLDR);      // debug

  /* 
  x = luminosidade LDR
  y = brilho LED
  Eq. da reta: y = y0 + m(x - x0)
  Declive: m = (y1 - y0) / (x1 - x0) */
  declive = (float)(maxBrilho - MIN_PERC_BRILHO) / (VALOR_MAX_LUZ - VALOR_MIN_LUZ);
                  // maxBrilho é o valor máx. para a luminosidade, seja 
  brilhoLED = 0 + declive * (sensorValueLDR - VALOR_MIN_LUZ);

  analogWrite(PIN_LED_EP4, brilhoLED);

  Serial.println("** luz a metade!");     // debug
  analogWrite(PIN_LED_EP4, MAX_BRILHO_MADRUGADA); 
}

/**
 * @descr 
 * @params 
 * @return 
 */
void lerLuminosidade() {  

}
