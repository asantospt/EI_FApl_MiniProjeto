/** 
 * @file EP5.ino
 * @brief EP5 – Abertura automática de estores para controlo de temperatura
 * @date 06/01/2018
 * @authors Afonso Santos (nr. 2130653) & Natanael Godinho (nr. 2110634)
 * @state INC  
 */

 /** 
  * LIGAÇÕES: @Arduino 'Mega 2560'
  * 5 V -> USB
  * GND -> breadboard
  * Pin 1 -> Servo/ Laranja(Sinal)
  * 
  * @Servomotor
  * Laranja(Sinal) -> Arduino/Pin 1
  * Vermelho (+) -> VCC
  * Castanho (-) -> GND
  */

#include <Servo.h>

Servo myservo;            // Criação de 1 objeto para controlar o Servo

// Atribuição dos pinos do Arduino
const int PIN_SERVO = 1;

// Valores pré-definidos no enunciado 

// Valores pré-definidos 
const int MIN_ANG_SERVO = 10;
const int MAX_ANG_SERVO = 170;
const float T_LIMIAR_1 = 15.0;              // Temperatura considerado como 'Frio'
const float T_LIMIAR_2 = 30.0;              // Temperatura considerado como 'Calor'
const float T_LIMIAR_LUZ = 80.0;            // Valor para incidência de luz solar
            // TODO: medir valores
//const int DELTA_T1 = 10000;
const int ANG_ESTORES_ABERTOS = 30; 
const int ANG_ESTORES_INT = 75;
const int ANG_ESTORES_FECHADOS = 120; 

// Variáveis globais do programa
int posServo = 0;                 // Variável que guarda a posição do servo

// Declaração de funções
void funcao_Ep5(float tempEp3);

void setup() {
  Serial.begin (9600);  
  myservo.attach(PIN_SERVO);  

  myservo.write(MIN_ANG_SERVO); 
  delay(15);  
  myservo.write(20); 
  delay(15);
  myservo.write(MIN_ANG_SERVO); 
  Serial.println("Teste servo - na posição 10º");   // debug
  delay(15);  
}

void loop() {
  unsigned long instanteAtual;
  float tempEp3 = 0.0;
  float luminosidade_ep4 = 0.0;

  /*tempEp3 = 9.0;
  funcao_Ep5(tempEp3);*/

  // ######################## testar - sweep
  // goes from 0 degrees to 170 degrees in steps of 1 degree
  for (posServo = MIN_ANG_SERVO; posServo <= MAX_ANG_SERVO; posServo += 1) { 
    myservo.write(posServo);              // tell servo to go to position in variable 'posServo'
    Serial.println(posServo); 
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (posServo = MAX_ANG_SERVO; posServo >= MIN_ANG_SERVO; posServo -= 1) { // goes from 170 degrees to 0 degrees
    myservo.write(posServo);              // tell servo to go to position in variable 'posServo'
    Serial.println(posServo); 
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void funcao_Ep5(float tempEp3) {
  // debug - valores para teste
             // TODO: receber valores do EP3, mas validá-los antes (dentro de 1 intervalo aceitável)
  float luminosidade_ep4 = 90.0;

  // Se estiver frio
  if ((tempEp3 < T_LIMIAR_1) && (luminosidade_ep4 > T_LIMIAR_LUZ))  {
    // angulo 30º - Estores abertos
    myservo.write(ANG_ESTORES_ABERTOS);
    Serial.print("há luz + está frio = ESTORES ABERTOS"); // debug
    Serial.println(ANG_ESTORES_ABERTOS);      
    delay(15); 
  }
  // Se estiver calor
  else if ((tempEp3 > T_LIMIAR_2) && (luminosidade_ep4 > T_LIMIAR_LUZ)) {
    // angulo 120º - Estores fechados
    myservo.write(ANG_ESTORES_FECHADOS);
    Serial.print("há luz + está calor = ESTORES FECHADOS"); // debug
    Serial.println(ANG_ESTORES_FECHADOS);     
    delay(15);
  }
  else {    // Se tempª estiver entre Min e Max, ou igual aos limites. Luz é indiferente 
    // não acionar servos
    // angulo 75º - Estores em posição intermédia
    myservo.write(ANG_ESTORES_INT);
    Serial.print("temp entre valores = Estores em posição intermédia"); // debug
    Serial.println(ANG_ESTORES_FECHADOS);     
    delay(15);
  }

  // Nota: em ambiente real, verificação poderia ser a cada 15 minutos
}