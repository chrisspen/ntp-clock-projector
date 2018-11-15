/*
 * Select board NodeMCU 1.0 (ESP-12E Module)
*/
#include <ESP8266WiFi.h>

#define LED_PIN 13

#define DISCONNECTED_T 250
#define CONNECTED_T 2000

unsigned long last_blink_time = 0;

const char* ssid = "dd-wrt2"; //dd-wrt1 does not work
const char* password = "password";

//int ledPin = 13; // GPIO13
WiFiServer server(80);

void blink_wifi_status(){
    if (WiFi.status() != WL_CONNECTED){
        if (millis() - last_blink_time >= DISCONNECTED_T){
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            last_blink_time = millis();
        }
    }else{
        if (millis() - last_blink_time >= CONNECTED_T){
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            last_blink_time = millis();
        }
    }
}

void setup() {
    last_blink_time = 0;

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    
    Serial.begin(115200);
    delay(10);

    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println(".");
      blink_wifi_status();
    }
    Serial.println("");
    Serial.println("WiFi connected");

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Print the IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");

}
 
void loop() {
    
    blink_wifi_status();

    // Check if a client has connected
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
  //String request = client.readStringUntil('\r');
  //Serial.println(request);
  //client.flush();
 
  // Match the request
  //int value = LOW;
  //if (request.indexOf("/LED=ON") != -1)  {
    //digitalWrite(ledPin, HIGH);
    //value = HIGH;
  //}
  //if (request.indexOf("/LED=OFF") != -1)  {
    //digitalWrite(ledPin, LOW);
    //value = LOW;
  //}
 
  // Set ledPin according to the request
  //digitalWrite(ledPin, value);
 
  // Return the response
  //client.println("HTTP/1.1 200 OK");
  //client.println("Content-Type: text/html");
  //client.println(""); //  do not forget this one
  //client.println("<!DOCTYPE HTML>");
  //client.println("<html>");
 
  //client.print("Led pin is now: ");
 
  //if(value == HIGH) {
    //client.print("On");
  //} else {
    //client.print("Off");
  //}
  //client.println("<br><br>");
  //client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  //client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  
  //client.println("</html>");
 
  //delay(1);
  //Serial.println("Client disonnected");
  //Serial.println("");
 
}
 
