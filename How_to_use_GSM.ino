#include <SoftwareSerial.h>    //Create software serial object to communicate with SIM800L
SoftwareSerial GSM(8, 9); //SIM800L Tx & Rx is connected to Arduino #8 & #9

char phone_no[]="+923378655465"; //change +92 with country code and 3378655465 with phone number to sms

#define bt_M  A0 // Button1 Pin A0 use for Send Message
#define bt_C  A1 // Button2 Pin A1 use for Call

#define LED 2 // Led Pin D2 use for LED

char inchar; // Will hold the incoming character from the GSM shield

void setup(){// put your setup code here, to run once

Serial.begin(9600);//Begin serial communication with Arduino and Arduino IDE (Serial Monitor)  
GSM.begin(9600);   //Begin serial communication with Arduino and SIM800L

pinMode(bt_M, INPUT_PULLUP); // declare bt_M as input
pinMode(bt_C, INPUT_PULLUP); // declare bt_C as input

pinMode(LED,OUTPUT); // declare LED as output
  
Serial.println("Initializing....");
initModule("AT","OK",1000);                //Once the handshake test is successful, it will back to OK
initModule("ATE1","OK",1000);              //this command is used for enabling echo
initModule("AT+CPIN?","READY",1000);       //this command is used to check whether SIM card is inserted in GSM Module or not
initModule("AT+CMGF=1","OK",1000);         //Configuring TEXT mode
initModule("AT+CNMI=2,2,0,0,0","OK",1000); //Decides how newly arrived SMS messages should be handled  
Serial.println("Initialized Successfully"); 

}

void loop(){

if(GSM.available() >0){inchar=GSM.read(); Serial.print(inchar); 
 if(inchar=='R'){inchar=GSM.read(); 
 if(inchar=='I'){inchar=GSM.read();
 if(inchar=='N'){inchar=GSM.read();
 if(inchar=='G'){  
    initModule("ATH","OK",1000); // this command is used for Call busy
    }
   }
  }
 }

else if(inchar=='L'){delay(10); inchar=GSM.read(); 
     if(inchar=='E'){delay(10); inchar=GSM.read();
     if(inchar=='D'){delay(10); inchar=GSM.read();

     if(inchar=='1'){digitalWrite(LED, 1); sendSMS(phone_no,"LED is On");}
else if(inchar=='0'){digitalWrite(LED, 0); sendSMS(phone_no,"LED is Off");}
     }
    }
  }
}


if(digitalRead (bt_M) == 0){sendSMS(phone_no,"Hello Muhammad Ansar from GSM SMS");}
if(digitalRead (bt_C) == 0){callUp(phone_no);}

delay(5);
}



void sendSMS(char *number, char *msg){
GSM.print("AT+CMGS=\"");GSM.print(number);GSM.println("\"\r\n"); //AT+CMGS=”Mobile Number” <ENTER> - Assigning recipient’s mobile number
delay(500);
GSM.println(msg); // Message contents
delay(500);
GSM.write(byte(26)); //Ctrl+Z  send message command (26 in decimal).
delay(3000);  
}

void callUp(char *number){
GSM.print("ATD + "); GSM.print(number); GSM.println(";"); //Call to the specific number, ends with semi-colon,replace X with mobile number
delay(20000);       // wait for 20 seconds...
GSM.println("ATH"); //hang up
delay(100);  
}


void initModule(String cmd, char *res, int t){
while(1){
    Serial.println(cmd);
    GSM.println(cmd);
    delay(100);
    while(GSM.available()>0){
       if(GSM.find(res)){
        Serial.println(res);
        delay(t);
        return;
       }else{Serial.println("Error");}}
    delay(t);
  }
}
