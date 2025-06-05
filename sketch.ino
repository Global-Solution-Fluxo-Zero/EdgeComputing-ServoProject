#include <Servo.h>

#define LED 13
#define BUZZER 12
#define TRIG 3
#define ECHO 2
#define SERVO 5
#define BUTTON 7
#define RAIN_CM_DANGER 5 //especifica em CM o nível que deve acionar a segurança

//
Servo servo;

int servo_position = 0; //0 - 360
int lastinput = 0;
float floor_limit = 0.0;

float readDistanceCM() { //realiza o calculo de distância do chão até o sensor
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  int duration = pulseIn(ECHO, HIGH);
  return duration * 0.03435 / 2; //0.03435 = velocidade do som em cm/microsegundos aproximado
}

void openTrapdoor() {
  while (servo_position != 90){
    servo.write(servo_position);
    delay(15);
    servo_position++;
  }
}

void closeTrapdoor() {
  while (servo_position != 0){
    servo.write(servo_position);
    delay(15);
    servo_position--;
  }
}

void setup() {
  Serial.begin(115200);
  float height;
  while (true) { //Wokwi bugando no sensor. Fiz isso para garantir que o nunca venha 0
    height = readDistanceCM();
    if (height != 0){
      break;
    } 
  }
  float danger_mm = (float)RAIN_CM_DANGER;
  floor_limit = height - danger_mm; //Calcula a altura que o sensor está - a camada perigosa
  pinMode(LED, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  servo.attach(SERVO);
  pinMode(BUTTON, INPUT);
}

void loop() {
  delay(15); //Responsividade para o Botão
  if(digitalRead(BUTTON) == LOW){ //Se botão não for apertado, entra nesse if
    if (readDistanceCM() < floor_limit){ //Se ele atinge esse valor menor, quer dizer que ele nível crítico, significa que altura diminuiu porque a água subiu
      tone(BUZZER, 131, 1000); //Emite alerta
      digitalWrite(LED, HIGH); //Liga led
      closeTrapdoor();
    } else {
      digitalWrite(LED, LOW); //Desliga led
      openTrapdoor();
    }
    lastinput = 0; //Reseta botão
  } else {
    digitalWrite(LED, HIGH); //Led ligado, e sem aviso porque entrou no modo manual.
    //Quando o botão manual é pressionado, ele força o Servo mudar a posição, se estava pra cima, ele obriga a descer.
    if (servo_position == 90 && lastinput == 0) {
      closeTrapdoor();
    } else if (servo_position == 0 && lastinput == 0) {
      openTrapdoor();
    }
    lastinput = 1;// Mudar para 1, assim o modo manual acontece 1 vez até o usuário sair dele

  }

  Serial.print("Measured distance: ");
  Serial.println(readDistanceCM());

  delay(100);
}
