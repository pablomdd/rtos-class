#define ENCA 34 // YELLOW
#define ENCB 35 // GREEN
#define PWM 32
#define IN2 33
#define IN1 25


// PWM config
const int m1PwmChannel = 0;
const int freq = 30000;
const int resolution = 8;
int dutyCycle = 200;

// encoder
volatile int aState;
volatile int aLastState;

volatile int pos = 0;

// times for pid
volatile long prevT = 0;
volatile float eprev = 0;
volatile float eintegral = 0;

// PID constants
float kp = 0.3;
float kd = 0.005;
float ki = 0.003;

TaskHandle_t Task1, Task2;

void setup() {
  Serial.begin(9600);
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);

  //pinMode(PWM,OUTPUT);
  ledcSetup(m1PwmChannel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(PWM, m1PwmChannel);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  
  aLastState = digitalRead(ENCA); 
  
  xTaskCreatePinnedToCore(
    control,   /* Task function. */
    "TaskOne", /* String with name of task. */
    10000,     /* Stack size in bytes. */
    NULL,      /* Parameter passed as input of the task */
    2,         /* Priority of the task. */
    &Task1,
    0);  

  xTaskCreatePinnedToCore(
    readEncoder,   /* Task function. */
    "TaskTwo", /* String with name of task. */
    10000,     /* Stack size in bytes. */
    NULL,      /* Parameter passed as input of the task */
    1,         /* Priority of the task. */
    &Task2,
    1);  // hilo
}


void loop() {

}

void control(void * parameter){
  for(;;){
    int target = 600*sin(prevT/1e6);
    for(int i=0; i<=50000; i++){
      if(i == 50000){
        // time difference
        long currT = micros();
        float deltaT = ((float) (currT - prevT))/( 1.0e6 );
        prevT = currT;
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
      
        setMotor(dir,pwr,PWM,IN1,IN2);

        // store previous error
        eprev = e;
        Serial.print(target);
        Serial.print(" ");
        Serial.print(pos);
        Serial.print(" ");;
        Serial.print(pwr);
        Serial.print(" ");
        Serial.println();
      }
    }
  }
}

void readEncoder(void * parameter){
  for(;;){
    for(int i=0; i<=10000; i++){
      if(i == 10000){
        aState = digitalRead(ENCA); // Reads the "current" state of the outputA
        // If the previous and the current state of the outputA are different, that means a Pulse has occured
        if (aState != aLastState){     
          // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
          if (digitalRead(ENCB) != aState) { 
            pos++;
          } else {
            pos--;
          }
        } 
        aLastState = aState; // Updates the previous state of the outputA with the current state
        //vTaskDelay( 500 / portTICK_PERIOD_MS );
      }
    }
  }
}

void setMotor(int dir, int pwmVal, int pwm, int in1, int in2){
  ledcWrite(m1PwmChannel,pwmVal);
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