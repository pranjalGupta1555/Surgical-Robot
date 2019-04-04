/*
 * SURGICAL ROBOT - 1.0 (RECEIVER)
 * Developed by - Ravindra Toshnival, Sanjay Kumar Das, Pranjal Gupta, Karthikeya GS.
 * College - PESIT South Campus, Bangalore, Karnataka, India.
 * Occasion - Rajasthan IT Day Hackathon 4.0
 * 
 * Components Required -
 * 1. H-bridge (Driver Motor) L298n
 * 2. Digital Accelerometer ADXL345
 * 3. NodeMCU ESP8266
 * 4. DC MOTOR
 * 
 * Introduction:
 * The main objective of our project is to achieve maximum range in human operated robotic devices,
 * by MACHINE TO MACHINE(M2M) connection between devices over the Internet of Things (IoT).
 * The current functioning of human gesture operated devices are by Transmitters and IR blasters.
 * With the implementation of M2M communication over the internet in this field,
 *  we can control this robot from any part of the world. 
 */

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

const char* ssid     = "";                     //WiFi name 
const char* password = "";                    //WiFi password

int ENA = 12;
int ENB = 0;
int IN3 = 2;
int IN1 = 13;
int IN4 = 14;
int IN2 = 15;
int ENA1 = 16;
int IN11 = 5;
int IN21 = 4;

void setup() {

   Serial.begin(115200);
   WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
 
  while (WiFi.status() != WL_CONNECTED) {
 
    delay(500);
    Serial.print(".");
 
  }
  Serial.println();

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA1, OUTPUT);
  pinMode(IN11, OUTPUT);
  pinMode(IN21, OUTPUT);
   

}
void testOne(int x,int y,int z) {

// turn on motor
if((x>2.0)&&(z>5.0))
{
digitalWrite(ENA, HIGH); // set speed to 200 out of possible range 0~255
digitalWrite(IN1, LOW);  //To move motor down
digitalWrite(IN2, HIGH);
//digitalWrite(IN3, LOW);
//digitalWrite(IN4, LOW);
}
//delay(500); // now change motor directions
if((x<-1.0)&&(z>5.0))
{
digitalWrite(ENA, HIGH);
digitalWrite(IN1, HIGH);  //To move motor up
digitalWrite(IN2, LOW);
}
//digitalWrite(IN3, LOW);
//digitalWrite(IN4, LOW);

if(z<-4.0)
{                        
digitalWrite(ENB, HIGH);  //To release
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
}
if((z>-2.5)&&(z<2.5))
{
digitalWrite(ENB, HIGH);  //To grab
digitalWrite(IN3, LOW);
digitalWrite(IN4, HIGH);
}
if((y>4.0)&&(z>5.0))
{
 digitalWrite(ENA1, HIGH); 
 digitalWrite(IN11, HIGH);  //To move motor left
 digitalWrite(IN21, LOW);
}
if((y<-4.0)&&(z>5.0))
{
 digitalWrite(ENA1, HIGH);
 digitalWrite(IN11, LOW);  //To move motor right
 digitalWrite(IN21, HIGH);
}

if((x<1.0)&&(x>-1.0))
{
digitalWrite(IN1, LOW);
digitalWrite(IN2, LOW);
}
}

void loop() {

  float axis[3];
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    //sourcerobots.000webhostapp.com
    http.begin("sourcerobots.000webhostapp.com", 80, "/axis.csv");              //Specify request destination
    int httpCode = http.GET();                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();                                        //Get the request response payload
      //Serial.println(payload);                                                //Print the response payload


      int n = payload.length(); 
     
      // declaring character array
      char char_array[n+1]; 
     
      // copying the contents of the 
      // string to char array
      strcpy(char_array, payload.c_str()); 

      int i = 0;
      char * ptr;
      ptr = strtok (char_array, "\n");
      while (ptr != NULL)
      {
        String str = ptr;
        axis[i] = strtof((str).c_str(),0);                                         // string to float
        ptr = strtok (NULL, "\n");
        i++;
      }

      for(int j=0;j<3;j++)
        Serial.println(axis[j]);

      Serial.println();
 
    }

    testOne(axis[0],axis[1],axis[2]);
 
    http.end();                                                                  //Close connection
 
  }
 
 
}

