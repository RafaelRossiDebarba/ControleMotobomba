// Neste projeto o motor tem a fase e o neutro ligado e desligado juntos
#define porta_motor 9

// Portas dos sensores
#define  sen_poco_baixo 2
#define sen_poco_alto 3
#define sen_caixa_baixo 4
#define sen_caixa_alto 5

// Portas indicaçao dos leds
#define led_poco_baixo 6
#define led_poco_alto 7
#define led_caixa_baixo 8
#define led_caixa_alto 10

// Led de problema
#define led 11

#define max_unsigned_long 4294967295

bool led_pisca = false;

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

bool debug = true;

bool protecao = false;

// time
unsigned long tempo_ligado_motor = 0;
const unsigned long tempo_maximo_motor_ligado = 300000; // Em milissegundos

void setup() {
  pinMode(porta_motor, OUTPUT);
  pinMode(sen_poco_baixo, INPUT);
  pinMode(sen_poco_alto, INPUT);
  pinMode(sen_caixa_baixo, INPUT);
  pinMode(sen_caixa_alto, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led_poco_baixo, OUTPUT);
  pinMode(led_poco_alto, OUTPUT);
  pinMode(led_caixa_baixo, OUTPUT);
  pinMode(led_caixa_alto, OUTPUT);

  digitalWrite(led, HIGH);

  Serial.begin(9600);
}

void loop() {
  // Leitura das portas
  status_poco_baixo = digitalRead(sen_poco_baixo);
  status_poco_alto = digitalRead(sen_poco_alto);
  status_caixa_baixo = digitalRead(sen_caixa_baixo);
  status_caixa_alto = digitalRead(sen_caixa_alto);

  if(!poco) {
    digitalWrite(led, LOW);
    delay(100);
    digitalWrite(led, HIGH);
  }

  statusLed();

  tempoMaximoMotor();

  if(debug) {
    printStatus();
  }

  if(!poco && status_poco_alto == boia_alta && status_poco_baixo == boia_alta && !protecao){ //Ativa o poço
    poco = true;
  }
  if(poco && status_poco_baixo == boia_baixa) { // desativa o poço
    poco = false;
    desligarMotor();
  }
  if(status_caixa_baixo == boia_baixa && poco && !protecao) { // liga o motor
    ligarMotor();
  }
  if(status_caixa_alto == boia_alta && motor) { // desliga o motor quando a caixa estiver cheia
    desligarMotor();
  }
  delay(1000);
}

void ligarMotor() {
  digitalWrite(porta_motor, HIGH);
  if(!motor) {
    tempo_ligado_motor = millis(); 
  }
  motor = true;
  if(debug){
    Serial.println("Motor Ligado"); 
  }
}

void desligarMotor() {
  digitalWrite(porta_motor, LOW);
  motor = false;
  tempo_ligado_motor = 0;
  testeMotorPosDesl();
  if(debug){
    Serial.println("Motor desligado"); 
  }
}

void tempoMaximoMotor() {
  if(motor) {
    if(millis() > tempo_ligado_motor) {
      if((millis() - tempo_ligado_motor) >= tempo_maximo_motor_ligado) {
        desligarMotor();
      }
    }
    else {
      if(((max_unsigned_long - tempo_ligado_motor) + millis()) >= tempo_maximo_motor_ligado) {
        desligarMotor();
      }
    }
  }
}

void testeMotorPosDesl() {
  if(status_caixa_baixo == boia_baixa) {
    protecao = true;
  }
}

void printStatus() {
  if(status_poco_baixo) {
    Serial.print("Poço Baixo TRUE   ");  
  }
  else {
    Serial.print("Poço Baixo FALSE  ");
  }
  if(status_poco_alto) {
    Serial.print("Poço Alto True   "); 
  }
  else {
    Serial.print("Poço Alto False  ");
  }
  if(status_caixa_baixo) {
    Serial.print("Caixa Baixo True   ");
  }
  else {
    Serial.print("Caixa Baixo False  ");
  }
  if(status_caixa_alto) {
    Serial.print("Caixa Alto True   ");
  }
  else {
    Serial.print("Caixa Alto False  ");
  }
  if(motor) {
    Serial.print("Motor Ligado      "); 
  }
  else {
    Serial.print("Motor Desligado   ");
  }
  if(protecao) {
    Serial.println("Protecao ligada       ");
  }
  else {
    Serial.println("Protecao desligada    ");
  }
  Serial.println(millis());
}

void statusLed() {
  if(status_poco_baixo) {
    digitalWrite(led_poco_baixo, HIGH);
  }
  else {
    digitalWrite(led_poco_baixo, LOW);
  }
  if(status_poco_alto) {
    digitalWrite(led_poco_alto, HIGH);
  }
  else {
    digitalWrite(led_poco_alto, LOW);
  }
  if(status_caixa_baixo) {
    digitalWrite(led_caixa_baixo, HIGH);
  }
  else {
    digitalWrite(led_caixa_baixo, LOW);
  }
  if(status_caixa_alto) {
    digitalWrite(led_caixa_alto, HIGH);
  }
  else {
    digitalWrite(led_caixa_alto, LOW);
  }
}
