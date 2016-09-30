#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo myservo;

#define CE_PIN 9
#define CSN_PIN 10
                                    
const int PinZero = A0 , PinOne = A1 , fanPin = 2 , OfficePin = 3;
int analog_value = 0 , reading , sensor_temp , sensor_light ;        
const uint64_t pipe[2] = {0xE8E8F0F0F2LL, 0xE8E8F0F0E1LL};
char income[32]=""; 
char input[32] ="";
float voltage;
bool tx_mode = false , ok;
char state_1c[13]="" ,state_2c[13]="", state_3c[14]="", state_4c[14]="" ;
String state_1 = "darkgood"  ;
String state_2 = "darkfire"  ;
String state_3 = "lightgood" ;
String state_4 = "lightfire" ;



// Declaration objects .
RF24 radio( CE_PIN, CSN_PIN );

void setup(){
  Serial.begin(115200);
  radio.begin();
  pinMode( 2,OUTPUT );
  pinMode( 3,OUTPUT );
  pinMode( 4,OUTPUT );
  pinMode( 5,OUTPUT );
  pinMode( 6,OUTPUT );
  pinMode( 7,OUTPUT );
  pinMode( 8,OUTPUT );
  pinMode(PinZero,INPUT);
  pinMode(PinOne,INPUT);
  myservo.attach(8);
  radio.setRetries( 15, 8 );
  radio.openWritingPipe(pipe[1]);
  radio.openReadingPipe( 1, pipe[0] );
  radio.setChannel(7);
  radio.startListening(); 
myservo.write(0);
  state_1.toCharArray(state_1c,sizeof(state_1c));
  state_2.toCharArray(state_2c,sizeof(state_2c));
  state_3.toCharArray(state_3c,sizeof(state_3c));
  state_4.toCharArray(state_4c,sizeof(state_4c));

}


void loop(){
  //Serial.print("here");
  tx_mode = true;  
  if( tx_mode==true ){
      radio.stopListening();
// *******  *********************************** *******      
      //delay(50);
      analog_value = analogRead(PinZero);  
      reading = analogRead(PinOne);
      voltage = reading*5.0/1024 ;          
      if (analog_value < 120)  
         { sensor_light = 0;  
         }
      else     
         { sensor_light = 1;    
         }
// *******  *********************************** *******
// *******  *********************************** *******
       if( voltage >= 0.38 ){  
          sensor_temp = 1;      
          //digitalWrite( BuzzerPin , HIGH );
       }else{  
          sensor_temp = 0;      
          //digitalWrite( BuzzerPin , LOW );
          }
// *******  *********************************** *******
        if( (sensor_light==0) && (sensor_temp==0) )   ok = radio.write(state_1c,sizeof(state_1c));    // "darkgood"  .
   else if( (sensor_light==0) && (sensor_temp==1) )   ok = radio.write(state_2c,sizeof(state_2c));    // "darkfire"  .
   else if( (sensor_light==1) && (sensor_temp==0) )   ok = radio.write(state_3c,sizeof(state_3c));    // "lightgood" .
   else if( (sensor_light==1) && (sensor_temp==1) )   ok = radio.write(state_4c,sizeof(state_4c));    // "lightfire" .
// *******  *********************************** *******
          
// *******  *********************************** *******

  if( ok ){      // if (ok)>> is it a perdefined condition in C 
          radio.startListening();
          tx_mode=false;
          }

    }
    
     if( tx_mode == false ){
      int count = 10000;
       // radio.startListening();
        while( (!radio.available()) && count-- );  //{
          //Serial.print("here");
            bool done = false;
            while(!done){
                done = radio.read( income, sizeof(income) );
              }
              
// *******  *********************************** *******
     if( (income[0] == 'l') && (income[1] == 'o') )     // Turn LED ON .
       { digitalWrite( OfficePin , HIGH ); 
       }
else if( (income[0] == 'l') && (income[1] == 'f') )     // Turn LED OFF .
       { digitalWrite( OfficePin , LOW ); 
       }
else if( (income[0] == 'f') && (income[1] == 'o') )     // Turn fan on .
       { digitalWrite( fanPin , HIGH ); 
       }
else if ( (income[0] == 'f') && (income[1] == 'f') )    // Turn fan off .
      {digitalWrite( fanPin , LOW );
      }
     if( income[0] == 'A' )   digitalWrite(4,HIGH);
else if( income[0] == 'a' )   digitalWrite(4,LOW);
else if( income[0] == 'B' )   digitalWrite(5,HIGH);
else if( income[0] == 'b' )   digitalWrite(5,LOW);
else if( income[0] == 'C' )   digitalWrite(6,HIGH);
else if( (income[0] == 'c')&&(income[1] != 'l') )   digitalWrite(6,LOW);
else if( income[0] == 'D' )   digitalWrite(7,HIGH);
else if( income[0] == 'd' )   digitalWrite(7,LOW);
     if( (income[0]=='1')&&(income[1]=='2')&&(income[2]=='3')&&(income[3]=='4')&&(income[4]=='5') )
                              myservo.write(80);
else if( (income[0]=='7')&&(income[1]=='8') )                                
                              myservo.write(-80);
     tx_mode == true;   
// *******  *********************************** *******
         
        
      }
}




