#define ENCA 34 // YELLOW
#define ENCB 35 // GREEN
#define PWM 32
#define IN2 33
#define IN1 25

volatile int pos = 0;

long prevT = 0;
float eprev = 0;
float eintegral = 0;

// PID constants
float kp = 0.085;
float kd = 0.00001;
float ki = 0.003;

void setup() {
  Serial.begin(9600);
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  // attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
  
  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);

  xTaskCreate(
    control,   /* Task function. */
    "TaskOne", /* String with name of task. */
    10000,     /* Stack size in bytes. */
    NULL,      /* Parameter passed as input of the task */
    1,         /* Priority of the task. */
    NULL);  

  xTaskCreate(
    readEncoder,   /* Task function. */
    "TaskTwo", /* String with name of task. */
    10000,     /* Stack size in bytes. */
    NULL,      /* Parameter passed as input of the task */
    1,         /* Priority of the task. */
    NULL);  
}


void loop() {

}

void control(void * parameter){
  for(;;){
    // time difference
    long currT = micros();
    float deltaT = ((float) (currT - prevT))/( 1.0e6 );
    prevT = currT;
    
    int target = 7000*sin(prevT/1e6);

    // error
    int e = pos - target;
  
    // derivative
    float dedt = (e-eprev)/(deltaT);
  
    // integral
    eintegral = eintegral + e*deltaT;
  
    // control signal
    float u = kp*e + kd*dedt + ki*eintegral;
    
    // motor power
    float pwr = fabs(u);
    if( pwr > 255 ){
      pwr = 255;
    }
  
    // motor direction
    int dir = 1;
    if(u<0){
      dir = -1;
    }
  
    // signal the motor
    setMotor(dir,pwr,PWM,IN1,IN2);
  
    // store previous error
    eprev = e;
    Serial.print(target);
    Serial.print(" ");
    Serial.print(pos);
    Serial.print(" ");
    Serial.print(-1 * e);
    Serial.print(" ");
    Serial.println();
    vTaskDelay( 5 / portTICK_PERIOD_MS );
  }
}

void readEncoder(void * parameter){
  for(;;){
    // TODO: Update correctly the position variable (pos)
    // that comes from reading the encoder signals
    int a = digitalRead(ENCA);
    int b = digitalRead(ENCB);
    
    if(b > 0 && a == 0){
      pos++;
    }
    else if(b == 0 && a > 0){
      pos--;
    }
    
    vTaskDelay( 5 / portTICK_PERIOD_MS );
  }
}

void setMotor(int dir, int pwmVal, int pwm, int in1, int in2){
  analogWrite(pwm,pwmVal);
  if(dir == 1){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  else if(dir == -1){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else{
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
  }
}