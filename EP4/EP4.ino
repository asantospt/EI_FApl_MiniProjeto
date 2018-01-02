/** 
 * @file EP4.ino
 * @brief EP4 – Iluminação noturna de presença
 * @date 31/12
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
 * @LDR
 * + -> 5V
 * - -> V_out, Arduino/Pin A2
 * 
 * @LED verde
 * + -> Res. 220 Ohm + Pin 9
 * - -> GND
 */

// Atribuição dos pinos do Arduino 
const int PIN_LDR = A2; 
const int PIN_LED_EP4 = 9;

// Valores pré-definidos 
const float VALOR_MIN_LDR = 4.50;     // valor max. lido (a descoberto) = 4.50 TODO: test leitura
const float VALOR_MAX_LDR = 2.00;     // valor min. lido (tapado) = 2.00
const int METADE_PERC_BRILHO = 128;   // Metade do brilho máximo = 127.5
const int TOT_PERC_BRILHO = 255;      // Brilho máximo = 255
const int TEMPO_MADRUGADA = 10000;    // (em milissegundos, 10 segundos) Simulação da 04:00 da madrugada

// Declaração de funções
void atuarLed();

void setup() {
  Serial.begin (9600);
  
  pinMode(PIN_LED_EP4, OUTPUT);

  // Acender o LED 2 segs. para teste
  digitalWrite(PIN_LED_EP4, HIGH);   
  delay(2000);                  
  digitalWrite(PIN_LED_EP4, LOW);     
}

void loop() {
  unsigned long instanteAtual = 0;
  int sensorValueLDR = 0;
  float voltageLDR = 0.0;

  instanteAtual = millis();
  if ((instanteAtual - tRef) >= deltaT1) {
      //ação
      tRef1 = instanteAtual;
  }


  
  // read the input on analog pin X:
  sensorValueLDR = analogRead(PIN_LDR);
  Serial.println(sensorValueLDR);

  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  voltageLDR = sensorValueLDR * (5.0 / 1023.0);
  Serial.println(voltageLDR);

  if (voltageLDR >= 3.25) {      // valor méd. calculado
    digitalWrite(PIN_LED_EP4, LOW);  
    Serial.println("MUITA LUZ!"); 
  }
  else {
    digitalWrite(PIN_LED_EP4, HIGH);
    Serial.println("POUCA LUZ!");
  }

  delay(1000);    // TODO: tirar
}

/**
 * @descr 
 * @params 
 * @return 
 */
void lerLuminosidade() {  

}

/**
 * @descr 
 * @params valor da luminosidade
 * @return -
 */
void atuarLed() {  

}
