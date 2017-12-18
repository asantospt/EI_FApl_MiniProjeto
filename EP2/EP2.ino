/** 
 * @file EP2.ino
 * @brief SMART HOME: EP1
 * @date - 2017-12-
 * @authors Afonso (2130653) & Natanael (2110634)
 * @state INC - Falta testar e acender e apagar led.
 */

/** 
 * LIGAÇÕES: @Arduino 'Mega 2560'
 * 5V -> USB
 * GND -> breadboard 
 * A0 -> sensor QRE1113
 * 
 * @Sensor de reflexão/proximidade 'QRE1113'
 * 1 -> 5V + R1 (130 kOhm)
 * 2 -> GND
 * 3 -> V_out + R2 (10 kOhm) 
 * 4 -> GND
 */


// Constantes usadas no programa 
const int PIN_QRE = A0;
int CONTAGEM = 0;

//Controle de tempos
//T1: Tempo entre identificação de reflexão - Milisegundos
const int DELTA_T1 = 2000;
unsigned long tRef1 = 0;
unsigned long tRef2 = 0;

bool LED = 0;

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

void setup() {
  Serial.begin (9600);
}

void loop() {
  //Contar milisegundos desde o arranque do sistema
  float instanteAtual = millis();

  //Accionar o led por 2 segundos inicialmente
  if ((instanteAtual - tRef1) >= DELTA_T1 ){
    //por o LED a HIGH

    
  }else if (LED == 0) {
    //por o led a low
    LED = 1;
  }

  // Ler reflexão a cada 2 segundos para estabilizar a contagem
  if ( (instanteAtual - tRef2) >= DELTA_T1 ){
    int sensorValue = analogRead(PIN_QRE);
    Serial.println(CONTAGEM);
    
    if(sensorValue > 700 && sensorValue < 800 ){
      CONTAGEM = CONTAGEM + 1;
    }
  }

  if ( (CONTAGEM % 2) == 0) {
    //Por o led a High

  }else{
    //Por o led a low

  }

  delay(100);
}