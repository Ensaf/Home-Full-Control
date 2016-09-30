 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define CE_PIN 9
#define CSN_PIN 10


const uint64_t pipe[2] = {0xE8E8F0F0F2LL, 0xE8E8F0F0E1LL};
char input[32] = "" ;  // receiveing from interrupt .

char current_income[32] = "", previous_income[32] = "" ;  


bool tx_mode=false, flag_1=false ;
int c = 1;
// Declaration objects .
RF24 radio( CE_PIN, CSN_PIN );

void setup(){
  Serial.begin(115200);
  radio.begin();
  radio.setRetries( 15, 8 );
  radio.openWritingPipe(pipe[0]);
  radio.openReadingPipe( 1, pipe[1] );
  radio.setChannel(7);
  radio.startListening(); 
}



void loop(){
  if(Serial.available()){
      delay(32);
      for(int i=0;i<32;i++){
          input[i] = Serial.read();
          if( input[i] == (char)-1 ){
              input[i]='\0';
              goto lable;
            }
      }
      lable:
      tx_mode=true;
    }  

  if( tx_mode ){
      radio.stopListening();
      bool ok = radio.write(input,sizeof(input));
      //if( ok ){
        
          radio.startListening();
          tx_mode=false;
      //  }
    }else if( tx_mode == false ){
      //Serial.print("here");
        if( radio.available() ){
            bool done = false;
            while(!done){
                done = radio.read( current_income, sizeof(current_income) );
              }
//...............................................................
       if( (previous_income[0]==current_income[0]) && (previous_income[5]==current_income[5]) ) flag_1=true;
       else flag_1=false;      


       
       previous_income[0] = current_income[0]; 
       previous_income[5] = current_income[5];
         
       if( (current_income!="") && (flag_1==false) ) { Serial.print(current_income);}
//............................................................
         
          }
      }
  
}


