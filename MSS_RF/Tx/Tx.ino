#include <VirtualWire.h>
 
 char controller[2]={245,0};
boolean led=true;
 int cnt=0;
 void setup() {
   pinMode(13,OUTPUT);
 vw_set_ptt_inverted(true); //
 vw_set_tx_pin(7);
 vw_setup(300);// speed of data transfer Kbps
 }

 void loop(){
 controller[1]=cnt  ;

 vw_send((uint8_t *)controller, 2);
 vw_wait_tx(); // Wait until the whole message is gone
 
 delay(2000);
 digitalWrite(13,led);
 
 led=!led;
 cnt++;
 if(cnt>10)
 cnt=0;

 }
