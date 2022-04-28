#define EN 8      
#define Y_DIR 3  
#define Y_STEP 6 

int ldr_IZQUIERDA_ARRIBA = A0;       //ldr settings
int ldr1_IZQUIERDA_ABAJO = A1;
int ldr2_DERECHA_ARRIBA = A2;
int ldr3_DERECHA_ABAJO = A3;
int ldrval = 0;
int ldrval_1 = 0;
int ldrval_2 = 0;
int ldrval_3 = 0;
int Avg_Righ = 0;
int Avg_Lef = 0;



////////////////////////////////////////////

float time;       //Variables for time control
float distance_previous_error, distance_error,MY_PID;
int period = 50;  //Refresh rate period of the loop is 50ms

///////////////////PID constants///////////////////////
float kp=24.0384;
float ki=0.011495; 
float kd=165.973;
float PID_p, PID_i, PID_d, PID_total;


void setup() {
  Serial.begin(9600);
  pinMode(ldr_IZQUIERDA_ARRIBA,INPUT);
  pinMode(ldr1_IZQUIERDA_ABAJO,INPUT);
  pinMode(ldr2_DERECHA_ARRIBA,INPUT);
  pinMode(ldr3_DERECHA_ABAJO,INPUT);

  
  pinMode(EN, OUTPUT); 
  pinMode(Y_DIR, OUTPUT); 
  pinMode(Y_STEP, OUTPUT);
  time = millis();
}

void loop() {
ldrval = analogRead(ldr_IZQUIERDA_ARRIBA);
ldrval_1 = analogRead(ldr1_IZQUIERDA_ABAJO);
ldrval_2 = analogRead(ldr2_DERECHA_ARRIBA);
ldrval_3 = analogRead(ldr3_DERECHA_ABAJO);

Avg_Righ = ldrval_2 + ldrval_3;
Avg_Lef = ldrval + ldrval_1;


if (millis() >  time+period){
  time = millis();
  distance_error = Avg_Righ - Avg_Lef;
  if (distance_error<0){distance_error = -distance_error;}

  PID_p = kp * distance_error;

  
  PID_d = kd*((distance_error - distance_previous_error)/period);

  
  if(0 < distance_error && distance_error < 80)
    {
     PID_i = 0; 
      
    }
    else
    {
      PID_i = PID_i + (ki * distance_error);    
    }


    
    PID_total = PID_p + PID_i + PID_d; 

    PID_total = map(PID_total, 400, 20000, 0,1000);
    MY_PID = map(PID_total, 400, 20000, 1000000000000,10000000000000);
    MY_PID = abs(MY_PID);
    

    
if (PID_total <= 200){
   digitalWrite(EN, HIGH);
   }
else {
  if (Avg_Righ < Avg_Lef){
   digitalWrite(EN, LOW);
   digitalWrite(Y_DIR, LOW);   // direccion de giro 1
   }

  else if (Avg_Righ > Avg_Lef){
   digitalWrite(EN, LOW);
   digitalWrite(Y_DIR, HIGH);   // direccion de giro 2
   }
}

    distance_previous_error = distance_error;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    digitalWrite(Y_STEP, HIGH);      
    delayMicroseconds(MY_PID);          
    digitalWrite(Y_STEP, LOW);       
    delayMicroseconds(MY_PID); 
}
}
