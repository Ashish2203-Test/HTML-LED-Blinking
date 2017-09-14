#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetServer server(80);
int led = 4;

void setup() {
  pinMode(led, OUTPUT);
  Ethernet.begin(mac);
  server.begin();
  Serial.begin(9600);
  Serial.println("Server address:");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  if (client){
    boolean currentLineIsBlank = true;
    String buffer = "";  
    while (client.connected()){
      if (client.available()) {
        char c = client.read(); 
        buffer+=c;       
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();    
          client.print("<center><br><h1>Led Control</h1><br><br><br><FORM>");
          client.print("<P> <INPUT type=\"submit\" name=\"status\" value=\"Turn ON\">");
          client.print("<P> <INPUT type=\"submit\" name=\"status\" value=\"Turn OFF\">");
          client.print("</FORM></center>"); 
          break;
        }
        if (c == '\n') { 
          currentLineIsBlank = true;
          buffer="";       
        } 
        else 
          if (c == '\r') {     
          if(buffer.indexOf("GET /?status=Turn+ON")>=0){
            digitalWrite(led, HIGH);
            Serial.print("ON");
          }
          if(buffer.indexOf("GET /?status=Turn+OFF")>=0){
            digitalWrite(led, LOW);   
            Serial.print("OFF");
          }
        }
        else {
          currentLineIsBlank = false;
        }  
      }
    }
    client.stop();
  }
}

