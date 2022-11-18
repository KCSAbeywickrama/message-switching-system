

////////////////////////////////////////////////////////////////////////////////////////////////
//pass word and permited users                                                                //
String pw="adg";                                                                              //
String own[]={"+94714491576","+94715925658"};                                                                              //
                                                                                              //
//objects                                                                                     //
int pin[]={4,5,6,8,9,10,11,12,13};                                                        //
String obj[10]={"b1","b2","b3","bulb4","plug1","p2","p3","p4","l13"};                                  //
////////////////////////////////////////////////////////////////////////////////////////////////



// include the GSM library
#include <GSM.h>
#include <EEPROM.h>

// PIN Number for the SIM
#define PINNUMBER ""

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

// Array to hold the number a SMS is retreived from
char senderNumber[20];


//Variable declaretion
String inputString = "";        
boolean stringComplete = false; 
//int swPin[2][6]={{A0,A1,A2,A3,A4,A5},{0,0,0,0,0,0}};
int swPin[2][4]={{A0,A1,A2,A3},{0,0,0,0}};
boolean sig=false;




void setup() {

for(int i=0;i<(sizeof(pin)/sizeof(*pin));i++){
  pinMode(pin[i],OUTPUT);
  digitalWrite(pin[i],EEPROM.read(i));
  }

for(int i=0;i<(sizeof(swPin)/4);i++){
  pinMode(swPin[0][i],INPUT);
  swPin[1][i]=EEPROM.read(100+i);
  } 

for(int i=0;i<(sizeof(swPin)/4);i++){

  if(digitalRead(swPin[0][i])!=swPin[1][i]){
    digitalWrite(pin[i],digitalRead(swPin[0][i]));
    swPin[1][i]=digitalRead(swPin[0][i]);
    EEPROM.update(i,digitalRead(pin[i]));     
    EEPROM.update((i+100),swPin[1][i]);
   
    }
  }

  /*
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("MSS");
  */

  
  // connection state
  boolean notConnected = true;

  // Start GSM connection
  
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {     
      notConnected = false;
    } else {
      delay(1000);
    }
  } 
 



 attachInterrupt(digitalPinToInterrupt(2), intrpt, CHANGE);

pinMode(7,OUTPUT);
digitalWrite(7,1);
delay(50);
digitalWrite(7,0); 

}

void loop() {
  
for(int i=0;i<(sizeof(swPin)/4);i++){

  if(digitalRead(swPin[0][i])!=swPin[1][i]){
    digitalWrite(pin[i],digitalRead(swPin[0][i]));
    swPin[1][i]=digitalRead(swPin[0][i]);
    EEPROM.update(i,digitalRead(pin[i]));     
    EEPROM.update((i+100),swPin[1][i]);
   
    }
  }

if(sig)
inSig();

}

///////////////////////////////////////////////
void inSig(){
  
   char c;

  // If there are any SMSs available()
  if (sms.available()) {
    Serial.println("Mf:");

    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    Serial.println(senderNumber);


    // An example of message disposal
    // Any messages starting with # should be discarded
    /*if (sms.peek() == '#') {
      Serial.println("DM");
      sms.flush();
    }*/

    // Read message bytes and print them
    String cmd="";
    while (c = sms.read()) {
      //Serial.print(c);
      cmd=cmd + c;
    }

    process(cmd);


//---------------------------------------------
   

    // Delete message from modem memory
    sms.flush();
   
  }

  sig=false;

 // delay(1000);
  
  
  
}


////////////////////////////////////////////////////////////////////////////////////

void process(String str){


String repMsg="";  
   


    
//============Words count===============


int w=0;
for(int i=0;i<str.length();i++){

  if(str.substring(i,i+1)==" ")
    w++;
    }
w++;


if(w<4){

//=========Words to array==============

String cmd[w];
short counter = 0;

str.toLowerCase();

for(short i=0;i<str.length();i++){
    if(str[i] == ' '){
        counter++;
        i++;
    }
    cmd[counter] += str[i];
}
for(int j=0;j<w;j++){

  }
  

   

//Permission=========================

boolean pmsn=false;
boolean RCmd=false;
boolean ext=false;
boolean sndRep=false;
 
for(int i=0;i<(sizeof(own)/sizeof(*own));i++){
  if(String(senderNumber)==own[i])
  pmsn=true;
  }

  if(!pmsn){
    if(cmd[2]==pw)
    pmsn=true;
    else
    repMsg="Access denied";
    }

if(pmsn){
  sndRep=true;
  //Process================================
  int objNum;
   
  for(int i=0;i<10;i++){
      if(cmd[0]==obj[i]){
        objNum=i; 
        ext=true;
      }
    }


  
  if(ext){
      if(cmd[1]=="on"){
        digitalWrite(pin[objNum],1);
        EEPROM.update((objNum),digitalRead(pin[objNum]));
        RCmd=true;
        repMsg=repMsg +"Success";
      }
      else if(cmd[1]=="off"){
        digitalWrite(pin[objNum],0);
        EEPROM.update((objNum),digitalRead(pin[objNum]));
        RCmd=true;
        repMsg=repMsg +"Success";
      }
      else if(cmd[1]=="state"){       
        RCmd=true;
       // repMsg=repMsg +"Now State";
       repMsg=repMsg +"Current state";
      }
      else{
         repMsg=repMsg +"Incorrect command";
           RCmd=false;
      }
  
           
    
  
  
      if(RCmd){
          String objState;
          
          if(digitalRead(pin[objNum])==1)
            objState="on";        
          else if(digitalRead(pin[objNum])==0)
            objState="off";
                    
           repMsg=repMsg +"\n"+obj[objNum]+" is "+objState;
    
      }
  
       
    }
    
  else
    {
       repMsg=repMsg +"No object as "+cmd[0];
    }


   
  }
  else
  {
    //else of pmsn

     int objNum;
   
  for(int i=0;i<10;i++){
      if(cmd[0]==obj[i]){
        objNum=i; 
        ext=true;
      }
    }


  
  if(ext){
    if(cmd[1]=="on"){
       RCmd=true;
        }
    else if(cmd[1]=="off"){
       RCmd=true;
       }
       else if(cmd[1]=="state"){
       RCmd=true;
       }
    }
    if(RCmd && ext)
      sndRep=true;
    
  }

  
  if((!ext) || (!pmsn)){

  if(cmd[0]=="help" && (pmsn || cmd[1]==pw)){
     
    // send the message
    sms.beginSMS(senderNumber);
    
    sms.print("User Guide\n");
   
    sms.print("\nTurn on/off,get state of a object");
    sms.print("\n:<OBJECT_NAME>(SPACE)on/off/state(space)<PASS_WORD>\n");
    
 
    
    sms.print("\nGet state of all objects");
    sms.print("\n:All(SPACE)state(space)<PASS_WORD>\n");      
    /*
    sms.print("\nIf your number is registered, no need to type <PASS_WORD>");
    */
    
    sms.endSMS();  
                           
    sndRep=false;
  }  


   if(cmd[0]=="all" && cmd[1]=="state"){
       if(pmsn){
         sms.beginSMS(senderNumber);
                
         sms.print("State of all objects\n");
                     
         String objState;
         for(int i=0;i<(sizeof(pin)/sizeof(*pin));i++){                 
            if(digitalRead(pin[i])==1)
              objState="on";        
            else if(digitalRead(pin[i])==0)
              objState="off"; 
                     
           
            sms.print("\n"+obj[i]+" - "+objState);
          }
         sms.endSMS(); 
         sndRep=false;
        }
        else{
          repMsg= "Access denied";
          sndRep=true;
          }
    
                        
        
     }

  }

  if(sndRep){
      sms.beginSMS(senderNumber);               
      sms.print(repMsg);
      sms.endSMS(); 
  }

 } //(w<4) 

    
}

///////////////////////////////////////////

void intrpt(){
  sig=true;
  }



