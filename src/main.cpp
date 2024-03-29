#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <server-handler.h>

ESP8266WebServer server(80);
uint8_t LIGHT_PIN = D3;
int LIGHT_STATE = LOW;

void set() {
  String value = server.arg("value");
  if (value == "true") {
    digitalWrite(LIGHT_PIN, HIGH);
    // Don't really understand how this works but LOW, turns the built in led ON
    digitalWrite(LED_BUILTIN, LOW);
    LIGHT_STATE = LOW;
  }
  if (value == "false") {
    digitalWrite(LIGHT_PIN, LOW);
    // HIGH turns built in led OFF
    digitalWrite(LED_BUILTIN, HIGH);
    LIGHT_STATE = HIGH;
  }
  // Serial.println(value);
  server.send(200, "text/plain", "true");
}

void getState() {
  server.send(200, "text/plain", String(LIGHT_STATE));
}

void init_server() {
  server.on("/set", set);
  server.on("/get-state", getState);
  server.begin();
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LIGHT_PIN, OUTPUT);

  digitalWrite(LIGHT_PIN, LOW);

  wifiConnect();
  ping();

  init_server();

  digitalWrite(LED_BUILTIN, HIGH);
}

unsigned long previousMillis = 0;
unsigned long interval = 10 * 1000;
void loop() {
  unsigned long currentMills = millis();

  server.handleClient();

  if (currentMills - previousMillis >= interval) {
    previousMillis = currentMills;
    ping();
  }
} 