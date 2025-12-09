#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// --- UPDATE YOUR CREDENTIALS ---
const char *ssid = "Calamity_2.5Ghz";      // e.g. "iPhone"
const char *password = "P0n!1ooe"; // e.g. "password123"
// -------------------------------

const int led1 = 5; // D1
const int led2 = 4; // D2

ESP8266WebServer server(80);

void sendCORS() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  server.sendHeader("Access-Control-Allow-Private-Network", "true");
}

void handleRoot() { sendCORS(); server.send(200, "text/plain", "System Online"); }
void turnLed1On() { sendCORS(); digitalWrite(led1, HIGH); server.send(200, "text/plain", "LED 1 ON"); }
void turnLed2On() { sendCORS(); digitalWrite(led2, HIGH); server.send(200, "text/plain", "LED 2 ON"); }
void handleOptions() { sendCORS(); server.send(204); }

// --- NEW FUNCTION TO TURN OFF LEDS ---
void resetLeds() {
  sendCORS();
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  server.send(200, "text/plain", "LEDs OFF");
}

void setup() {
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println(WiFi.localIP()); 

  server.on("/", HTTP_GET, handleRoot);
  server.on("/led1", HTTP_GET, turnLed1On);
  server.on("/led2", HTTP_GET, turnLed2On);
  server.on("/reset", HTTP_GET, resetLeds); // <--- New Route
  server.onNotFound([]() { if(server.method() == HTTP_OPTIONS) handleOptions(); else server.send(404); });

  server.begin();
}

void loop() {
  server.handleClient();
}