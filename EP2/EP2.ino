/** 
 * @file EP2.ino
 * @brief SMART HOME: EP2
 * @date - 2017-12-18
 * @authors Afonso (2130653) & Natanael (2110634)
 * @state INC - Falta esclarecer detecçâo Ocorrência
 */

/** 
 * LIGAÇÕES: @Arduino 'Mega 2560'
 * 5V -> USB
 * GND -> breadboard 
 * A0 -> sensor QRE1113
 * 13 -> R1 (220 Ohm) + LED
 * 
 * @Sensor de reflexão/proximidade 'QRE1113'
 * 1 -> 5V + R1 (130 Ohm)
 * 2 -> GND
 * 3 -> V_out + R2 (10 kOhm) 
 * 4 -> GND
 *
* @led
 * + -> 5V + R1 (220 Ohm)
 * - -> GND
 */


// Constantes usadas no programa 
const int PIN_QRE = A0;
const int LED_PIN =  13; 
int CONTAGEM = 2;

//Controle de tempos
//T1: Tempo entre identificação de reflexão - Milisegundos
const int DELTA_T1 = 2000;

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
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  //Contar milisegundos desde o arranque do sistema
  float instanteAtual = millis();

  //Accionar o led por 2 segundos inicialmente
  if ((instanteAtual) <= DELTA_T1 ){
    //por o LED a HIGH
    digitalWrite(LED_PIN, HIGH);
    
  }else if (LED == 0) {
    //por o led a low
    digitalWrite(LED_PIN, LOW);
     Serial.println("LED IS LOW");
    LED = 1;
  }

  //Iniciar detecção animal domestico
  if ((instanteAtual) >= DELTA_T1 ){
    int sensorValue = analogRead(PIN_QRE);
    Serial.println(sensorValue);
    Serial.println(CONTAGEM);

    if(sensorValue > 850 && sensorValue < 900 ){
      CONTAGEM = CONTAGEM + 1;
    }
      if ( (CONTAGEM & 1) == 0) {
        //Por o led a High
        digitalWrite(LED_PIN, LOW);    
        }else{
          //Por o led a low
          digitalWrite(LED_PIN, HIGH);
        }
      }

  delay(100);
}