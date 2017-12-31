/** 
 * @file EP2.ino
 * @brief SMART HOME: EP2
 * @date - 2017-12-18
 * @authors Afonso (2130653) & Natanael (2110634)
 * @state OK
 */

/** 
 * LIGAÇÕES: @Arduino 'Mega 2560'
 * 5V -> USB
 * GND -> breadboard 
 * A0 -> sensor QRE1113, pin3
 * pin 5 -> R1 (220 Ohm) + LED
 * 
 * @Sensor de reflexão/proximidade 'QRE1113'
 * pin 1 -> R1 (130 Ohm) + 5V
 * pin 2 -> GND
 * pin 3 -> A0 + R2 (10 kOhm) +5V 
 * pin 4 -> GND
 *
* @led
 * + -> R3 (220 Ohm) + pin 13
 * - -> GND
 */


// Constantes usadas no programa 
const int PIN_QRE = A0;
const int LED_PIN =  5; 


//Controle de tempos
//T1: Tempo para primeira identificação de reflexão - Milisegundos
const int DELTA_T1 = 2000;

//bool led = 0;
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

void setup() {
  Serial.begin (9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  delay(2000);
  digitalWrite(LED_PIN, LOW);


}

void loop() {
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
     digitalWrite(LED_PIN, LOW);    
  }else{
    //Por o led a low
    digitalWrite(LED_PIN, HIGH);
    }
  delay(100);
}