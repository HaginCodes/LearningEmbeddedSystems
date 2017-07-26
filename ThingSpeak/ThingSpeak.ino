#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <ThingSpeak.h>


long unsigned long myChannelNumber = ; //Your channel num
const char * myWriteAPIKey = "; //Your API Key


#include <SPI.h>


#define USE_WIFI101_SHIELD
//#define USE_ETHERNET_SHIELD

#include <SPI.h>

char ssid[] = "";    //  your network SSID (name)
char pass[] = "";   // your network password
int status = WL_IDLE_STATUS;
WiFiClient  client;
WiFiServer server(80);
// Use wired ethernet shield


#ifdef ARDUINO_ARCH_AVR
// On Arduino:  0 - 1023 maps to 0 - 5 volts
#define VOLTAGE_MAX 5.0
#define VOLTAGE_MAXCOUNTS 1023.0
#elif ARDUINO_SAMD_MKR1000
// On MKR1000:  0 - 1023 maps to 0 - 3.3 volts
#define VOLTAGE_MAX 3.3
#define VOLTAGE_MAXCOUNTS 1023.0
#elif ARDUINO_SAM_DUE
// On Due:  0 - 1023 maps to 0 - 3.3 volts
#define VOLTAGE_MAX 3.3
#define VOLTAGE_MAXCOUNTS 1023.0
#elif ARDUINO_ARCH_ESP8266
// On ESP8266:  0 - 1023 maps to 0 - 1 volts
#define VOLTAGE_MAX 1.0
#define VOLTAGE_MAXCOUNTS 1023.0
#endif



void setup() {


  Serial.begin(9600);
  while(!Serial) {
    ;
  }
  WiFi.begin(ssid, pass);
  Serial.println("Wifi connected: ");
  Serial.println(ssid);

  //Start Server
  server.begin();

  //Start ThingSpeak
  ThingSpeak.begin(client);
  Serial.println();
  Serial.println("Set up complete");
}

void loop() {


  WiFiClient client = server.available();
  if(client) {
    Serial.println("Client successfully created");

    boolean currentLineIsBlank = true;
    while(client.connected()){
      if(client.available()){
        char c = client.read();
        Serial.write(c);

        if(c == '\n' && currentLineIsBlank){

          //send a response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          //print out the analog input pin on the arduino.. I think
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");       
          }
          client.println("</html>");
           break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          //character on that line bud
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    
    // close the connection:
  //  client.stop();
   // Serial.println("client disonnected");
    }

    // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading
  // On MKR1000,Due: 0 - 4095 maps to 0 - 3.3 volts
  float voltage = sensorValue * (VOLTAGE_MAX / VOLTAGE_MAXCOUNTS);
  Serial.println(voltage);
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
   ThingSpeak.writeField(myChannelNumber, 1, voltage, myWriteAPIKey);
  delay(15000); // ThingSpeak will only accept updates every 15 seconds.
   }

