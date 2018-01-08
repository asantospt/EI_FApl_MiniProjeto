/** 
 * @file EP5.ino
 * @brief EP5 – Abertura automática de estores para controlo de temperatura
 * @date 07/01/2018
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
const int PIN_SERVO = 10;

// Valores pré-definidos no enunciado 
const int ANG_ESTORES_ABERTOS = 30; 
const int ANG_ESTORES_INT = 75;
const int ANG_ESTORES_FECHADOS = 120; 

// Variáveis globais do programa
const int MIN_ANG_SERVO = 10;         // Definição do ângulo mín. a enviar para o Servo, por segurança
const int MAX_ANG_SERVO = 170;        // Definição do ângulo máx. a enviar para o Servo, por segurança
const float T_LIMIAR_1 = 10.0;              // Temperatura considerado como 'Frio'
const float T_LIMIAR_2 = 28.0;              // Temperatura considerado como 'Calor'
const int T_LIMIAR_LUZ = 700;            // Valor para incidência de luz solar
int posServo = 0;                 // Variável que guarda a posição do Servo [Graus]

// Declaração de funções
void funcao_Ep5(float temp_Ep3, float voltageLDR_Ep4);

void setup() {
  Serial.begin(9600);  
  myservo.attach(PIN_SERVO);  

  myservo.write(MIN_ANG_SERVO); 
  delay(15);  
  myservo.write(20); 
  delay(15);
  myservo.write(MIN_ANG_SERVO); 
  Serial.println("Teste servo - na posição 10º");   // debug
  delay(15);  

  delay(1000);
}

void loop() {
  float temp_Ep3 = 0.0;
  float voltageLDR_Ep4 = 0.0;

  temp_Ep3 = 9.0;          // 2x debug
  voltageLDR_Ep4 = 500.0;
  //funcao_Ep5(temp_Ep3, voltageLDR_Ep4);     // EP5 precisa de receber valores lidos na EP3 e na EP4

  // ######################## testar - sweep
  // goes from 0 degrees to 170 degrees in steps of 1 degree
  for (posServo = MIN_ANG_SERVO; posServo <= MAX_ANG_SERVO; posServo += 1) { 
    myservo.write(posServo);              // tell servo to go to position in variable 'posServo'
    Serial.println(posServo); 
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  /*for (posServo = MAX_ANG_SERVO; posServo >= MIN_ANG_SERVO; posServo -= 1) { // goes from 170 degrees to 0 degrees
    myservo.write(posServo);              // tell servo to go to position in variable 'posServo'
    Serial.println(posServo); 
    delay(15);                       // waits 15ms for the servo to reach the position
  }*/
}

/**
 * @brief Recebe a temperatura e a luminosidade da casa e altera a posição dos estores
 * @params Temperatura (EP3), Voltagem da LRD (EP4)
 * @return -
 */
void funcao_Ep5(float temp_Ep3, float voltageLDR_Ep4) {
  float temp_Entrada = 0.0;
  float voltageLDR_Entrada = 0.0;
  // TODO: receber valores do EP3+EP4, mas validá-los antes (dentro de 1 intervalo aceitável)

  temp_Entrada = temp_Ep3;
  voltageLDR_Entrada = voltageLDR_Ep4;

  // Se estiver frio
  if ((temp_Entrada < T_LIMIAR_1) && (voltageLDR_Entrada > T_LIMIAR_LUZ))  {
    // angulo 30º - Estores abertos
    myservo.write(ANG_ESTORES_ABERTOS);
    Serial.print("há luz + está frio = ESTORES ABERTOS"); // debug
    Serial.println(ANG_ESTORES_ABERTOS);      
    delay(15); 
  }
  // Se estiver calor
  else if ((temp_Entrada > T_LIMIAR_2) && (voltageLDR_Entrada > T_LIMIAR_LUZ)) {
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