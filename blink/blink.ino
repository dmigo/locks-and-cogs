

#include <ESP8266WiFi.h>

const int ledPin = 5;
const int btnPin = 4;
int keyIndex = 0;

WiFiServer server(80);

void setup() {
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  startServer();
}
void loop() { 
  serve();
}

void startServer(){
 // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.println(WiFi.localIP());
}

void serve(){
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  if (req.indexOf("/blink") != -1)
    blink();
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  
  client.flush();
 
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nBlinking in progress, please stay tuned. ";
  s += "</html>\n";
 
  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
 
  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

void blink(){
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
  Serial.println("blink");  
}

