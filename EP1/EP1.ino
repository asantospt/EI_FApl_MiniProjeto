/** 
 * @file EP1.ino
 * @brief SMART HOME: EP1
 * @date - 2017-12-17
 * @authors Afonso (2130653) & Natanael (2110634)
 * @state OK
 */

 /** 
  * LIGAÇÕES: @Arduino 'Mega 2560'
  * 5V -> USB
  * GND -> breadboard
  * Pin 3 -> Sensor ultrassons, Trig
  * Pin 2 -> Sensor ultrassons, Echo
  * Pin 4 -> Buzzer

  * @Sensor ultrassons 'HC-SR04'
  * VCC -> 5V
  * GND -> GND 
  * Trig -> Arduino, Pin 3
  * Echo -> Arduino, Pin 2
  *
  * @Buzzer 
  * + -> Arduino, Pin 8 + R. 10 kOhm
  * - -> GND
*/

// Valores pré-definidos no enunciado
/** 
Distancia Minima para iniciar procedimento de alarme
60 cm
Tempo para introduzir código de segurança e desligar alarme
10 Segundos
*/

// Constantes usadas no programa 
const int PIN_TRIG = 3;
const int PIN_ECHO = 2;
const int PIN_BUZZER = 4;
// vel. propagação dos ultrassons no ar = 343 m/s 
// Formula Utilizada Dcm = deltaT / 58,31 cm/MicroSegundo
const float VEL_ULTRASSONS_AR = 58.31;  

//Centimetros
const int DIST_MIN_ALARM = 60;
//Segundos
const int TIME_TO_SHUTDOWN = 10;

//Controle de tempos
//T1: Tempo entre medição de distancia - Milisegundos
//T2: Tempo até acionamento do alarme - Milisegundos
const int DELTA_T1 = 1000;
unsigned long tRef1= 0;

const int DELTA_T2 = 10000;
unsigned long tRef2= 0;

//Alarme accionado
bool alarm = 0;

//PRINT INTRUSÃO
bool intrusao = 0;

//Codigo Alarme
int SECRET = 1234;

// Declaração de funções

void setup() {
  Serial.begin (9600);
  
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  
  // Ao inicializar o sistema o pin Trig deve estar em modo LOW
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  //Acionar o buzzer por 0,5 segundos inicialmente
  tone(PIN_BUZZER, 880,500); 
 
}

void loop() {
  //Contar milisegundos desde o arranque do sistema
  float instanteAtual = millis();

  //Registar a duração do pulso do echo - Microsegundos
  long duration;

  //Tratar dos Pinos do sensor Ultrasons
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW); 
  

// Ler distancia a cada segundo
  if ( (instanteAtual - tRef1) >= DELTA_T1 ){
    duration = pulseIn(PIN_ECHO, HIGH);
    float distance = ((float)duration / VEL_ULTRASSONS_AR);  
      
      if (alarm == 0 && distance < 61){
        Serial.print(distance); 
        Serial.println(" cm");
        Serial.println("Insira código de Segurança");
        tRef1 = instanteAtual;
        tRef2 = instanteAtual;
        alarm = 1;
        intrusao = 0;  

      }
  }

  int secret;
  //receber texto enviado pela consola da porta serie
  secret = Serial.parseInt();

  if (alarm == 1 && SECRET == secret ){
    tone(PIN_BUZZER, 880, 1);
    alarm = 0;
  }

  if (alarm == 1 && (instanteAtual - tRef2) >= DELTA_T2 ){
    if(intrusao == 0){
      Serial.println("INTRUSÃO!");
      intrusao = 1;
    }
    tone(PIN_BUZZER, 880 , 0); 
  }
  //Pequeno delay para garantir bom funcionamento do sensor ultrasons
  delay(100);
}