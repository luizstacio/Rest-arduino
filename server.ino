#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,200);

EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  Ethernet.begin(mac, ip);
  server.begin();
}

char* getreponse (char* typeConnect) {
  char buffer[256];
  StaticJsonBuffer<200> jsonBuffer;
  
  // Object data  
  JsonObject& root = jsonBuffer.createObject();
  //JsonArray& data = root.createNestedArray("random");
  
  root["request_type"] = typeConnect;

  //Object parse
  root.printTo(buffer, sizeof(buffer));
  
  return buffer;
}

void get (EthernetClient client, char* url) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  client.print(getreponse("GET"));
}

void post (EthernetClient client, char* url) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  client.print(getreponse("POST"));
}

void put (EthernetClient client, char* url) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  client.print(getreponse("PUT"));
}

void destroy (EthernetClient client, char* url) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  client.print(getreponse("DELETE"));
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    char requestBuffer[1000];
    char requestType[7] = "      ";
    int indexChar = 0;
    int indexCharData = 0;
    int auxType = 0;
    boolean currentLineIsBlank = false;
    
    while (client.connected()) {
      if (client.available()) {
        char c =  client.read();

        requestBuffer[indexChar] = c;

        if ( c != ' ' && auxType == 0 ) {
          requestType[indexChar] = c;
        } else {
          auxType = 1;
        }
       
        indexChar += 1;
      } else {
        String typeReq = String(requestType);
        
        if (typeReq == "GET   ") {
          get(client, requestBuffer);
        } else if (typeReq == "POST  ") {
          post(client, requestBuffer);
        } else if (typeReq == "PUT   ") {
          put(client, requestBuffer);
        } else if (typeReq == "DELETE") {
          destroy(client, requestBuffer);
        }
        
        client.stop();
        break;
      }
    }
  }
}
