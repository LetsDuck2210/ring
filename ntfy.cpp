#include <Arduino.h>
#include <arduino_base64.hpp>
#include <ESP8266WiFi.h>

#if __has_include("secrets.h")
  #include "secrets.h"
#else
  #error please create a secrets.h file
#endif

namespace Ntfy {
  const uint32_t MIN_RING_DELAY = 10000;

  String notificationTitle = "Klingel";
  String notificationBody = "Es hat geklingelt";

  String generateRequest();
  String generateAuthHeader();
  void clientPrintln(String line);

  WiFiClient client;

  uint32_t lastRing = 0;
  void ring() {
    if(millis() - lastRing < MIN_RING_DELAY && lastRing != 0) {
      Serial.printf("Last ntfy ring was %.2fs ago. Not sending notification\n", (millis() - lastRing) / 1000.0);
      return;
    }

    Serial.println("connecting to server...");
    if(!client.connect(NtfySec::server, NtfySec::port)) {
      Serial.println("Failed to connect to ntfy server");
      return;
    }

    String req = generateRequest();
    String headers[] = {
      "Title: " + notificationTitle,
      "Host: " + NtfySec::server.toString(),
      "Content-Length: " + String(notificationBody.length()),
      generateAuthHeader(),
    };

    clientPrintln(req);
    for(uint8_t i = 0; i < sizeof(headers) / sizeof(String); i++)
      clientPrintln(headers[i]);
    clientPrintln("");
    clientPrintln(notificationBody);
    client.flush();
    Serial.println("sent notification!");

    Serial.println("response: ");
    while(client.available()) {
      Serial.write(client.read());
    }
    Serial.println();

    client.stop();

    lastRing = millis();
  }

  void clientPrintln(String line) {
    Serial.println("\t" + line);
    client.println(line);
  }
  String generateRequest() {
      const uint16_t BUF_SIZE = 256;
      char buf[BUF_SIZE];
      snprintf(buf, BUF_SIZE, 
          "POST http://%s:%d/%s HTTP/1.1", 
          NtfySec::server.toString().c_str(), 
          NtfySec::port, 
          NtfySec::topic);

      return String(buf);
  }
  String generateAuthHeader() {
    String input = String(NtfySec::user) + ":" + String(NtfySec::pass);
    char output[base64::encodeLength(input.length())];
    base64::encode((uint8_t*) input.c_str(), input.length(), output);

    return "Authorization: Basic " + String(output);
  }
}
