// Neste projeto o motor tem a fase e o neutro ligado e desligado juntos
int porta_motor_primeira = 10;
int porta_motor_segunda = 11;

// Portas dos sensores
int sen_poco_baixo = 5;
int sen_poco_alto = 6;
int sen_caixa_baixo = 7;
int sen_caixa_alto = 8;

int led = 9; // Saída do led que é ativado quando o poço está desativado

//Poço começa ativado
bool poco = true;

// Motor começa desligado
bool motor = false;

bool status_poco_baixo = false;
bool status_poco_alto = false;
bool status_caixa_baixo = false;
bool status_caixa_alto = false;

// Regulagem da boia 
bool boia_baixa = true;
bool boia_alta = false;

void setup() {
  pinMode(porta_motor_primeira, OUTPUT);
  pinMode(porta_motor_segunda, OUTPUT);
  pinMode(sen_poco_baixo, INPUT);
  pinMode(sen_poco_alto, INPUT);
  pinMode(sen_caixa_baixo, INPUT);
  pinMode(sen_caixa_alto, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  // Leitura das portas
  status_poco_baixo = digitalRead(sen_poco_baixo);
  status_poco_alto = digitalRead(sen_poco_alto);
  status_caixa_baixo = digitalRead(sen_caixa_baixo);
  status_caixa_alto = digitalRead(sen_caixa_alto);

  if(!poco && status_poco_alto == boia_alta && status_poco_baixo == boia_alta){ //Ativa o poço
    poco = true;
    digitalWrite(led, LOW);
  }
  if(poco && status_poco_baixo == boia_baixa) { // desativa o poço
    poco = false;
    digitalWrite(led, HIGH);
    digitalWrite(porta_motor_primeira, LOW);
    digitalWrite(porta_motor_segunda, LOW);
    motor = false;
  }
  if(status_caixa_baixo == boia_baixa && poco) { // liga o motor
    digitalWrite(porta_motor_primeira, HIGH);
    digitalWrite(porta_motor_segunda, HIGH);
    motor = true;
  }
  if(status_caixa_alto == boia_alta && motor) { // desliga o motor quando a caixa estiver cheia
    digitalWrite(porta_motor_primeira, LOW);
    digitalWrite(porta_motor_segunda, LOW);
    motor = false;
  }
  delay(1000);
}
