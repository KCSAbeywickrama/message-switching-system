 #include <VirtualWire.h>

#include <SoftwareSerial.h>
const int rx=-1;
const int tx=4;

SoftwareSerial mser(rx,tx);

 uint8_t pn=245;
 uint8_t adr=0;
 uint8_t ON;
 uint8_t OFF;

 int relay=0;
 
 void setup()
 {

  ON=2*adr+1;
  OFF=2*adr;
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_set_rx_pin(3);
  vw_setup(300);  // Bits per sec
  pinMode(relay, OUTPUT);

  vw_rx_start();         // Start the receiver PLL running


pinMode(rx,INPUT);
pinMode(tx,OUTPUT);
mser.begin(9600);
mser.println("Begin");
 }
     void loop()
 {
     uint8_t buf[VW_MAX_MESSAGE_LEN];
     uint8_t buflen = VW_MAX_MESSAGE_LEN;

     if (vw_get_message(buf, &buflen)) // Non-blocking
     {

      if(buf[0]==pn){
        if(buf[1]==ON){
           digitalWrite(relay,1);
           mser.println("adr0-ON");
          }
         
        else if(buf[1]==OFF){
          digitalWrite(relay,0);
          mser.println("adr0-OFF"); 
          }
              
        }


        for (int i = 0; i < buflen; i++)
  {
      mser.print(buf[i]);
      mser.print(' ');
  }
  mser.println();
      

      }
 }
