#if __has_include("secrets.h")
  #include "secrets.h"
#else
  #error please create a secrets.h file
#endif

#include <Arduino.h>

#include <ESP8266WiFi.h>

#include "ntfy.h"
#include "src/server/webserver.h"
#include "src/settings.h"

const uint8_t PIN_RING_BUTTON = D0;
const uint8_t PIN_RING_BELL = D6;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_RING_BUTTON, INPUT);
  pinMode(PIN_RING_BELL, OUTPUT);

  Serial.println("connecting to " + String(ssid));
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  WiFi.setHostname("ESPRing");
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  WebServer::setup();
}

uint32_t lastButtonChange = 0;
uint32_t lastRing = 0;
uint8_t prevValue = 0, ringCount = 0;
void loop() {
  if(millis() - lastButtonChange < 200) return;
  uint8_t value = digitalRead(PIN_RING_BUTTON);
  if(value != prevValue) {
    Serial.printf("[%ld] ring(%d) ... ", millis(), value);
    lastButtonChange = millis();
    prevValue = value;

    if(value == 0) {
      Serial.println();
      return;
    }

    if(!Settings::is_bell_enabled()) {
      Serial.println("Bell is disabled. Not ringing");
      return;
    }

    const uint8_t bell_limit = Settings::get_bell_limit();
    if(millis() - lastRing < 5000 && ringCount >= bell_limit) {
      Serial.printf("Last ring was %.2fs ago. Not ringing\n", (millis() - lastRing) / 1000.0);
      return;
    }
    if(ringCount >= bell_limit) ringCount = 0;

    ringCount++;
    lastRing = millis();

    digitalWrite(PIN_RING_BELL, HIGH);
    delay(200);
    digitalWrite(PIN_RING_BELL, LOW);
    Ntfy::ring();
  }
}
