/** 
 * @file EP5.ino
 * @brief EP5 – Abertura automática de estores para controlo de temperatura
 * @date 21/12
 * @authors Afonso Santos (nr. 2130653) & Natanael Godinho (nr. 2110634)
 * @state INC  
 */

 /** 
  * LIGAÇÕES: @Arduino 'Mega 2560'
  * 5V -> *USB
  * GND -> breadboard
  * Pin 10 -> Servo/ Laranja(Sinal)
  * 
  * @Servomotor
  * Laranja(Sinal) -> Arduino/Pin 10
  * Vermelho (+) -> VCC
  * Castanho (-) -> GND
  */

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

const int PIN_SERVO = 10;
const int MAX_ANG_SERVO = 170;
const int MIN_ANG_SERVO = 10;

const int T_LIMIAR_1 = 15;        // Temperatura considerado como 'Frio'
const int T_LIMIAR_2 = 30;        // Temperatura considerado como 'Calor'
const float T_LIMIAR_LUZ = 80.0;        // Valor para incidência de luz solar
            // TODO: medir valores

int pos = 0;    // variable to store the servo position

// Valores pré-definidos no enunciado
const int ANG_ESTORES_ABERTOS = 30; 
const int ANG_ESTORES_FECHADOS = 120; 
const int ANG_ESTORES_INTERM = 75;

void setup() {
  Serial.begin (9600);
  
  myservo.attach(PIN_SERVO);  // attaches the servo on pin 9 to the servo object

  myservo.write(50); 
  Serial.println("Teste servo - na posição 50º");   // debug
  delay(15);  
  myservo.write(MIN_ANG_SERVO); 
  Serial.println("Teste servo - na posição 10º");   // debug
  delay(15);  
}

void loop() {
  int temp_ep3 = 0;
  float luminosidade_ep4 = 0.0;

  // valores para teste
  temp_ep3 = 9;
  luminosidade_ep4 = 90.0;

  // Se estiver frio
  if ((temp_ep3 < T_LIMIAR_1) && (luminosidade_ep4 > T_LIMIAR_LUZ))  {
    // angulo 30º - Estores abertos
    myservo.write(ANG_ESTORES_ABERTOS);
    Serial.print("há luz + está frio = ESTORES ABERTOS"); // debug
    Serial.println(ANG_ESTORES_ABERTOS);      
    delay(15); 
  }
  // Se estiver calor
  else if ((temp_ep3 > T_LIMIAR_2) && (luminosidade_ep4 > T_LIMIAR_LUZ)) {
    // angulo 120º - Estores fechados
    myservo.write(ANG_ESTORES_FECHADOS);
    Serial.print("há luz + está calor = ESTORES FECHADOS"); // debug
    Serial.println(ANG_ESTORES_FECHADOS);     
    delay(15);
  }
  else {    // Se tempª estiver entre Min e Max, ou igual aos limites. Luz é indiferente 
    // não acionar servos
    // angulo 75º - Estores em posição intermédia
    myservo.write(ANG_ESTORES_INTERM);
    Serial.print("temp entre valores = Estores em posição intermédia"); // debug
    Serial.println(ANG_ESTORES_FECHADOS);     
    delay(15);
  }


  // TODO: verificar alterações a cada x segs.
  // Em ambiente real, verificação poderia ser a cada 15 mins 


  // ######################## sweep
  for (pos = MIN_ANG_SERVO; pos <= MAX_ANG_SERVO; pos += 1) { // goes from 0 degrees to 170 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.println(pos); 
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = MAX_ANG_SERVO; pos >= MIN_ANG_SERVO; pos -= 1) { // goes from 170 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.println(pos); 
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}