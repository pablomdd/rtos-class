String inputString = "";
bool stringComplete = false;

// Entrada de la señal A del encoder (Cable amarillo).
const byte C1 = 34;
// Entrada de la señal B del encoder (Cable verde).
const byte C2 = 35;

//  Motor DC 1
const int in1 = 27;
const int in2 = 26;
const int ena = 14;
const int m1PwmChannel = 0;

// PWM config
const int freq = 30000;
const int resolution = 8;
int dutyCycle = 200;

// Pulsos
volatile int n = 0;
volatile byte ant = 0;
volatile byte act = 0;

// Revoluciones por minuto calculadas.
double N = 0.0;
// Variable de control (pwm)
int Cv = 0;
// Número de muescas que tiene el disco del encoder.
// TODO(@GustavoElizarraras): Actualizar numero de muescas.
unsigned int R = 1980;

unsigned int pulsosParaVuelta = 0;
bool darVueltas = false;

void setup() {
  // Start Serial port
  Serial.begin(115200);

  // Motor 1 config
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);
  digitalWrite(in1, false);
  digitalWrite(in2, false);
  analogWrite(ena, Cv);
  // configure LED PWM utility for writing PWM
  ledcSetup(m1PwmChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ena, m1PwmChannel);

  // Motor Encoder
  pinMode(C1, INPUT);
  // Encoder interrupt
  attachInterrupt(digitalPinToInterrupt(C1), encoder, CHANGE);
}

void loop() {
  // Si hay string completo, se activa.
  if (stringComplete) {
    Cv = (double)inputString.toInt();

    // Agregar dar X vueltas.
    pulsosParaVuelta = Cv * R;
    darVueltas = true;
    
    inputString = "";
    stringComplete = false;
  }

  if (darVueltas) {
    // Imprimir dir positiva, PWM 150,   
    setMotor(1, 150, m1PwmChannel, in1, in2);
  }

  if (n >= pulsosParaVuelta) {
    // Mandar 0 para parar el motor.
    setMotor(0, 150, m1PwmChannel, in1, in2);
    darVueltas = false;
    // Modificar una variable volatil puede ser peligroso
    n = 0;
  }
}

// Recibir buffer del puerto serial
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void setMotor(int dir, int pwmVal, int pwmChannel, int in1, int in2)
{
  ledcWrite(pwmChannel, pwmVal);
  if (dir == 1)
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else if (dir == -1)
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  else
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
}

void encoder(void)
{
  n++;
  // Serial.println(n);
}
