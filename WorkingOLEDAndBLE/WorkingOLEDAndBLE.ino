/* By Michael Albinson
*
*
*
*/


#include "U8glib.h"
#include <SPI.h>
#include <boards.h>
#include <RBL_nRF8001.h>
#include <MemoryFree.h>

int CHARS_TO_READ = 14; // max of 16
int carrier, a=0;
String VIPTicket = "WdmMZkFsMooqcy", RegTicket = "75iGkxWAw695wp", VoidTicket = "igJ9PXHhlOVorC"; //Ticket types
char stringBase[14]; // used to extract the ticket code from the BLE shield -- Must array bound change to match CHARS_TO_READ
String finalString= "";//carries our incoming ticket code

U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);

void setup()
{  

  ble_begin();
  
  Serial.begin(57600);
  
  OLED_setup();
  delay (1000);

}


void loop()
{

  a=0;
  if ( ble_available() )
  {
    while ( ble_available() ){
      carrier =ble_read(); //parses the BLE input ticket status
      Serial.write (carrier); //prints it
      if (a<CHARS_TO_READ) // breaks the status into readable chars
      {
        stringBase[a] = carrier; 
      }
      a++;
    }
    for  (a=0; a<CHARS_TO_READ; a++){
      finalString = finalString + stringBase[a]; //combines the chars into comparable string
    }
    Serial.println("");

    // Serial.println (INCOMING_MSG); these two can be brought in to see the input ticket status
    //Serial.println();

    if (finalString == VIPTicket){ //VIP ticket status
      Serial.println("ITS A VIP");
      Serial.println("Good ticket mode begin *indicator on*");
      u8g.firstPage();  
      do {
        drawVIP();
      } 
      while( u8g.nextPage() );

      // rebuild the picture after some delay
      delay(5000);
      Serial.println("Good ticket mode end *indicator off*");    
    }

    else if (finalString == RegTicket){ // regular ticket status
      Serial.println ("Normal Ticket");
      Serial.println ("Bad ticket mode begin");
      u8g.firstPage();  
      do {
        drawNormal();
      } 
      while( u8g.nextPage() );
      delay(5000);
      Serial.println ("*indicator off*");
      Serial.println ("Bad ticket mode end");
    }

    else if (finalString == VoidTicket){ // Void ticket status
      Serial.println ("This is a void ticket");
      Serial.println ("Bad ticket mode begin");
      u8g.firstPage();  
      do {
        drawVoid();
      } 
      while( u8g.nextPage() );

      // rebuild the picture after some delay
      delay(5000);
      Serial.println ("Bad ticket mode end");
    }

    else { // All other unreadable types
      Serial.println ("Check Ticket");
      Serial.println ("Bad ticket mode begin");
      u8g.firstPage();  
      do {
        drawNull();
      } 
      while( u8g.nextPage() );

      // rebuild the picture after some delay
      delay(5000);
      Serial.println ("Bad ticket mode end");
    }
    Serial.println();
  }

  else {
    //Serial.println ("No Ticket Detected *No Indicator*");
    delay(1000);
  }

  if ( Serial.available() ) // allows the console to send messages back to the phone/bluetooth device
  {
    delay(5);

    while ( Serial.available() )
      ble_write( Serial.read() );
  }
  ble_do_events();

  for (a=0; a<CHARS_TO_READ; a++){
    stringBase[a] = 1;
  }
  finalString= "";

  u8g.firstPage();  
  do {
    drawTFly();
  } 
  while( u8g.nextPage() );

  delay(50);
}


void drawVIP() {
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 22,  "VIP");
}

void drawTFly() {
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 22,  "TFly");
}

void drawNormal() {
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 22,  "NRM Adimt");
}

void drawVoid() {
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 22,  "VOID");
}

void drawNull() {
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 22,  "NO TICKET");
}

void OLED_setup() 
{
  u8g.getMode() == U8G_MODE_BW;
  u8g.setColorIndex(1);
  
  u8g.firstPage();  
  do {
    u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 10,  "TFLY Beacon");
  u8g.drawStr(0, 20, "Setup");
  u8g.drawStr( 0, 30,  "Beacon Ready");
  } 
  while( u8g.nextPage() );
}
