#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>


#include <SPI.h>


char ssid[] = "XXXXXX";
char pass[] = "F546F8E8F56ADD2B2618C313AE47F014022C43DAFF051E63AFCD8094881D1392";

int keyIndex = 0;

int status = WL_IDLE_STATUS;

WiFiServer server(80);

boolean alreadyConnected = false; // wheter or not the client was connected prevously

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi sheild not present");
    //dont continue
    while (true);
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);

    delay(10000);
  }

  server.begin();

  printWiFiStatus();
}


void loop() {
  // put your main code here, to run repeatedly:

  WiFiClient client = server.available();


  if (client) {
    if (!alreadyConnected) {
      // clean out the input buffer;
      client.flush();
      Serial.println("We have a new client");
      client.println("Hello, Hagin :) ");
      alreadyConnected = true;
    }

    if (client.available() > 0) {

      char thisChar = client.read();

      server.write(thisChar);

      Serial.write(thisChar);
    }
  }
}

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strenth (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}



