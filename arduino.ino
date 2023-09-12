
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
//---------------------------------GPS
SoftwareSerial mySerial(10, 11);
TinyGPS gps;
void gpsdump(TinyGPS &gps);
void printFloat(double f, int digits = 2);
//---------------------------------MPU

Adafruit_MPU6050 mpu;
//---------------------------------SONAR
int LED = 46;
int BUZZER = 44; 
int TAPBUT = 42; 
int REL=40; 
const int pingPin1 = 50;   // Trigger Pin Digital of Ultrasonic Sensor 1
const int echoPin1 = 2;   // Echo Pin PWM of Ultrasonic Sensor 1
const int pingPin2 = 48;   // Trigger Pin Digital of Ultrasonic Sensor 2
const int echoPin2 = 4;   // Echo Pin PWM of Ultrasonic Sensor 2

//---------------------------------MQ6 SENSOR
int ALCOHOL_sensor = 13;  // MQ-6 SENSOR
int ALCOHOL_detected;
int ACCIDENT;
int ACCIDENT_detected;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(1000);
  while (!Serial);
  
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 sensor!");
    while (1)
    {
      delay(100);
      if (mpu.begin())
      {
        Serial.println("-MPU FOUND-\n-Starting the program-\n");
        break;
      }
    }
  }
  
  pinMode(LED, OUTPUT);
  pinMode(ALCOHOL_sensor, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(TAPBUT, INPUT_PULLUP);
  pinMode(REL, OUTPUT);
}

void loop() {
 //----------------------------------------MPU---------------------------------------------------------------
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float roll  = atan2(a.acceleration.y, a.acceleration.z) * 180.0 / PI;
  float pitch = atan(-a.acceleration.x / sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180.0 / PI;
  Serial.print("AccX:");
  Serial.print(a.acceleration.x);
  Serial.print("\n");
  Serial.print("AccY:");
  Serial.print(a.acceleration.y);
  Serial.print("\n");
  Serial.print("AccZ:");
  Serial.print(a.acceleration.z);
  Serial.print("\n");
  //Serial.println(" m/s^2");
  Serial.print("Roll:");
  Serial.print(roll);
  Serial.print("\n");
  Serial.print("Pitch:");
  Serial.print(pitch);
  Serial.print("\n");
  //Serial.println(" degrees");//
 //----------------------------------------MPU ENDS---------------------------------------------------------------
 //--------------------------------------Buzzer for MPU-------------------------------
 if(roll<-150||roll>-20)
  {
    //Serial.println(" yessssss");
    digitalWrite(BUZZER, HIGH);
    delay(100);
    
    digitalWrite(BUZZER, LOW); 
    delay(50);

    digitalWrite(BUZZER, HIGH);
    delay(100);
    
    digitalWrite(BUZZER, LOW); 
    delay(50);

    digitalWrite(BUZZER, HIGH);
    delay(100);
    
    digitalWrite(BUZZER, LOW); 
    delay(50);

    delay(3000);
    ACCIDENT_detected=digitalRead(TAPBUT);
    //Serial.print(ACCIDENT_detected);
    if(ACCIDENT_detected==0)
    { 
        ACCIDENT=0;
    }
    else
    {
        ACCIDENT =1;
    }
      
    
  }
  else
  {
    
    digitalWrite(BUZZER, LOW); 
    ACCIDENT =0;
    
  }
  
  Serial.print("ACCIDENT:");
  Serial.print(ACCIDENT);
  Serial.print("\n");
  
  //---------------------------------------Buzzer End for MPU----------------------------------
  
  //-----------------------------------------Alchohol Sensor-----------------------------------


/* Read ALCOHOL sensor */
  ALCOHOL_detected = digitalRead(ALCOHOL_sensor);
  //Serial.println(ALCOHOL_detected);

  if (ALCOHOL_detected == 1) {
    Serial.println("ALC:1");
    digitalWrite(REL,HIGH);
    
  } else
  {
    digitalWrite(REL,LOW);
    Serial.println("ALC:0");
  }
 //-----------------------------------------Alchohol Sensor END-----------------------------------

Ang, [9/12/2023 10:35 PM]
//-----------------------------------------SONAR START-----------------------------------
 
  /* Read Sonar sensor */
  long duration1, inches1, cm1;
  pinMode(pingPin1, OUTPUT);
  digitalWrite(pingPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin1, LOW);
  pinMode(echoPin1, INPUT);
  duration1 = pulseIn(echoPin1, HIGH);
  inches1 = microsecondsToInches(duration1);
  cm1 = microsecondsToCentimeters(duration1);
  if(cm1<50)
  {
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW); 
    delay(100);
  }
  else if(cm1<100)
  {
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, LOW); 
    
  }
  
  else
  {
    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW); 
  }
  Serial.print("cm1:");
  //Serial.print(inches1);
  //Serial.print("in, ");
  Serial.print(cm1);
  //Serial.print("cm");
  Serial.println();

  
  /* Read Sonar sensor */
  long duration2, inches2, cm2;
  pinMode(pingPin2, OUTPUT);
  digitalWrite(pingPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin2, LOW);
  pinMode(echoPin2, INPUT);
  duration2 = pulseIn(echoPin2, HIGH);
  inches2 = microsecondsToInches(duration2);
  cm2 = microsecondsToCentimeters(duration2);
  if(cm2<50)
  {
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW); 
    delay(100);
  }
  else if(cm2<100)
  {
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, LOW); 
    
  }
  
  else
  {
    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW); 
  }
  Serial.print("cm2:");
  //Serial.print(inches2);
  //Serial.print("in, ");
  Serial.print(cm2);
  //Serial.print("cm");
  Serial.println();

  //delay(500);
  //static String cc;
  //cc = String(cm2)+'\n';
  //bluetooth.println(String(cm2)+'\n');

 

  //-----------------------------------------SONAR END------------------------------------------


  bool newdata = false;
  unsigned long start = millis();
  while (millis() - start < 500) 
  {
    //-----------------------------This is the code for GPS----------------------
    if (mySerial.available()) 
    
    {
      char c = mySerial.read();
      //Serial.print(c);  // uncomment to see raw GPS data
      //Serial.print(gps.encode(c));
      if (gps.encode(c)) 
      {
        newdata = true;
        break;  // uncomment to print new data immediately!
      }
    }

  }
    if (newdata) 
  {
    gpsdump(gps);
  }
    else
    {
      int flatt = 0;
      int flonn = 0;
      Serial.print("Lat:"); 
      printFloat(flatt, 5); 
      Serial.print("\n");
      Serial.print("Long:"); 
      printFloat(flonn, 5);
      Serial.print("\n");
      
    }

  
   digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW); 
  delay(300);
}






//-----------------------------------------Functions----------------------------------

long microsecondsToInches(long microseconds) 
{
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) 
{
  return microseconds / 29 / 2;
}


void gpsdump(TinyGPS &gps)
{
  long lat, lon;
  float flat, flon;
  unsigned long age; 
  int year;
  unsigned short sentences, failed;
  gps.f_get_position(&flat, &flon, &age);
  Serial.print("Lat:"); 
  printFloat(flat, 5); 
  Serial.print("\n");
  Serial.print("Long:"); 
  printFloat(flon, 5);
  Serial.print("\n");
    

  
}

void printFloat(double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0) 
  {
     Serial.print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  Serial.print(int_part);

Ang, [9/12/2023 10:35 PM]
// Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    Serial.print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0) 
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    Serial.print(toPrint);
    remainder -= toPrint;
  }
}
