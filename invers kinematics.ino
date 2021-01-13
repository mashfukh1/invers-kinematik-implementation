#include <Servo.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 
#define OLED_RESET     4 
#define NUMFLAKES     10 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define analogA PA0
#define analogB PA1
#define analogC PA2

#define push1 PA6
#define push2 PA7

#define buzzer PB5

#define led1 PA4
#define led2 PA5

double a2 = 5.2;  
double a4 = 7;  

Servo myservoA;
Servo myservoB;
Servo myservoC;

double r1,xA,yA,phi_1,phi_2,theta_1,phi_3,theta_2,j,k,theta_3;

 int A = 0;
 int B = 0;
 int C = 0;

 int sensorValueA;
 int sensorValueB;
 int sensorValueC;
 
 int resultA;
 int convertA;

 int resultB;
 int convertB;

 int resultC;
 int convertC;

void setup(){
  calibration();
  
  myservoA.attach(8);
  myservoB.attach(9);
  myservoC.attach(0);

  myservoA.write(0);
  myservoB.write(0);
  myservoC.write(0);
  
  pinMode(analogA,INPUT);
  pinMode(analogB,INPUT);
  pinMode(analogC,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(push1,INPUT_PULLUP);
  pinMode(push2,INPUT_PULLUP);
 
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);  
  buzzerStart();
 }
void loop() {
cartesianSpace();
}

void starting(){
   
  }

void cartesianSpace(void) {
int alpha1,alpha2,alpha3;

 alpha1=0;
 alpha2=0;
 alpha3=0;
 
 sensorValueA = analogRead(analogA);
 sensorValueB = analogRead(analogB);
 sensorValueC = analogRead(analogC);
 
 resultA = map(sensorValueA, 0, 4095, 0, 65535);
 convertA = map(resultA, 0, 65535, 0, 255);

 resultB = map(sensorValueB, 0, 4095, 0, 65535);
 convertB = map(resultB, 0, 65535, 0, 255);

 resultC = map(sensorValueC, 0, 4095, 0, 65535);
 convertC = map(resultC, 0, 65535, 0, 255);


 display.clearDisplay();
 display.setTextSize(1);         
 display.setTextColor(SSD1306_WHITE);  
  
 display.setCursor(20 ,0); 
 display.print("X: ");   
 display.print(convertA);

 display.setCursor(20,12); 
 display.print("Y: ");   
 display.print(convertB);

 display.setCursor(20,24); 
 display.print("Z: ");   
 display.print(convertC);

 display.setCursor(80,0); 
 display.print("Xf: ");   
 display.print(A);

 display.setCursor(80,12); 
 display.print("Yf: ");   
 display.print(B);

 display.setCursor(80,24);
 display.print("Zf: ");   
 display.print(C);
   display.display();
 if(digitalRead(push1)==LOW){
   A = convertA;
   B = convertB; 
   C = convertC;  
   digitalWrite(buzzer,HIGH);
 }
 else if(digitalRead(push2)==LOW){
  buzzerStart();
  while(1){
  inversKinematik(A,B);
  if(digitalRead(push1)==LOW){
    digitalWrite(buzzer,HIGH);
    break;
    }
   }
  }
 else{
  digitalWrite(buzzer,LOW);
  }
 delay(100);
}

void calibration() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000); 
  display.clearDisplay();
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.display();
  delay(2000);
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
}

void buzzerStart(){
  for(int i=0;i<100;i++){  
    digitalWrite(buzzer,HIGH);
    delay(10);
    digitalWrite(buzzer,LOW);
    delay(10);
    digitalWrite(led1,HIGH);
    display.clearDisplay();    
    drawProgressbar(0,10,120,10,i);
    display.display();
    delay(10);
    }
   for(int i=0;i<2;i++){
    digitalWrite(buzzer,HIGH);
    delay(80);
    digitalWrite(buzzer,LOW);
    delay(80);
    }
    digitalWrite(led1,LOW);
    digitalWrite(led2,HIGH);
  }
 double inversKinematik(double x, double y){ 
    display.clearDisplay();
    display.setTextSize(1);         
    display.setTextColor(SSD1306_WHITE);  
    
    r1 = sqrt((x*x)+(y*y)); 
    phi_1 = acos(((a4*a4)-(a2*a2)-r1*r1)/(-2*a2*r1));
    phi_2 = atan2(y, x);
    j = phi_1 - phi_2;
    theta_1 = j*180/3.14; 
    phi_3 = acos((r1*r1-a2*a2-a4*a4)/(-2*a2*a4));
    theta_2 = 180-(phi_3*180/3.14);
    theta_3 = atan(yA/xA);
    
    display.setCursor(10,0); 
    display.print("Theta 1: ");   
    display.print(theta_1);

    display.setCursor(10,12); 
    display.print("Theta 2: ");   
    display.print(theta_2);

    display.setCursor(10 ,24); 
    display.print("Theta 3: ");   
    display.print(theta_3);
    display.display();

    myservoA.write(theta_1);
    myservoB.write(theta_2);
    myservoC.write(theta_3);
  }
void drawProgressbar(int x,int y, int width,int height, int progress)
{
   float bar = ((float)(width-1) / 100) * progress;
   display.drawRect(x, y, width, height, WHITE);
   display.fillRect(x+2, y+2, bar , height-4, WHITE); // initailize the graphics fillRect(int x, int y, int width, int height)
  }
void progress() {
 for(int i=0;i<100;i++){
 display.clearDisplay();    
 drawProgressbar(0,10,120,10,i);
 display.display();
 delay(20);
 }
}
