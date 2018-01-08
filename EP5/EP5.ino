/** 
 * @file EP5.ino
 * @brief EP5 – Abertura automática de estores para controlo de temperatura
 * @date 08/01/2018
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
const float T_LIMIAR_1 = 15.0;              // Temperatura considerado como 'Frio'
const float T_LIMIAR_2 = 28.0;              // Temperatura considerado como 'Calor'
const int T_LIMIAR_LUZ = 700;            // Valor para incidência de luz solar
int posServo = 0;                 // Variável que guarda a posição do Servo [Graus]

// Declaração de funções
void funcaoEp5(float temp_Ep3, float voltageLDR_Ep4);

void setup() {
  Serial.begin(9600);  
  myservo.attach(PIN_SERVO);  

  for (posServo = MIN_ANG_SERVO; posServo <= MAX_ANG_SERVO; posServo += 1) { 
    myservo.write(posServo);          
    Serial.println(posServo); 
    delay(15);                       
  }
  for (posServo = MAX_ANG_SERVO; posServo >= MIN_ANG_SERVO; posServo -= 1) { 
    myservo.write(posServo);            
    Serial.println(posServo); 
    delay(15);                        
  } 
}

void loop() {
  float temp_Ep3 = 0.0;
  int sensorValueLDR_Ep4 = 0;

  //delay(2000);

  // cond. 1
    temp_Ep3 = 9.0;          // debug
    sensorValueLDR_Ep4 = 701;

  // cond. 2
    temp_Ep3 = 29.0;          // debug
    sensorValueLDR_Ep4 = 701;

  // cond. 3
    temp_Ep3 = 20.0;          // debug
    sensorValueLDR_Ep4 = 701;
    
  funcaoEp5(temp_Ep3, sensorValueLDR_Ep4);     // EP5 precisa de receber valores lidos na EP3 e na EP4
}

/**
 * @brief Recebe a temperatura e a luminosidade da casa e altera a posição dos estores
 * @params Temperatura (EP3), Voltagem da LRD (EP4)
 * @return -
 */
void funcaoEp5(float temp_Ep3, int voltageLDR_Ep4) {
  float temp_Entrada = 0.0;
  int voltageLDR_Entrada = 0;
  // TODO: receber valores do EP3+EP4, mas validá-los antes (dentro de 1 intervalo aceitável)

  temp_Entrada = temp_Ep3;
  voltageLDR_Entrada = voltageLDR_Ep4;

  // Se estiver frio
  if ((temp_Entrada < T_LIMIAR_1) && (voltageLDR_Entrada > T_LIMIAR_LUZ))  {
    // angulo 30º - Estores abertos
    myservo.write(ANG_ESTORES_ABERTOS);
    Serial.print("cond. 1 - ESTORES ABERTOS");     // debug
    //Serial.println(ANG_ESTORES_ABERTOS);      
    delay(15); 
  }
  // Se estiver calor
  else if ((temp_Entrada > T_LIMIAR_2) && (voltageLDR_Entrada > T_LIMIAR_LUZ)) {
    // angulo 120º - Estores fechados
    myservo.write(ANG_ESTORES_FECHADOS);
    Serial.print("cond. 2 - ESTORES FECHADOS");   // debug
    //Serial.println(ANG_ESTORES_FECHADOS);     
    delay(15);
  }
  else {    // Se tempª estiver entre Min e Max, ou igual aos limites. Luz é indiferente 
    // não acionar servos
    // angulo 75º - Estores em posição intermédia
    myservo.write(ANG_ESTORES_INT);
    Serial.print("cond. 3 - Estores em posição intermédia");   // debug
    //Serial.println(ANG_ESTORES_INT);     
    delay(15);
  }

  // Nota: em ambiente real, verificação poderia ser a cada 15 minutos
}
