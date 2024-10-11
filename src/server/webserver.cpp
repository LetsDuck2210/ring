#include <Arduino.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "../www/index.html.h"
#include "../www/style.css.h"

namespace WebServer {
  AsyncWebServer server(80);

  struct {
    String filename;
    String mimetype;
    unsigned char* content;
    size_t size;
  } files[] = {
    { .filename = "/index.html", .mimetype = "text/html", .content = index_html, .size = index_html_len },
    { .filename = "/style.css", .mimetype = "text/css", .content = style_css, .size = style_css_len }
  };
  uint8_t default_file = 0;

  void handle_requests(AsyncWebServerRequest* request);
  void handle_api_request(AsyncWebServerRequest* request);
  void handle_not_found(AsyncWebServerRequest* request);

  void setup() {
    server.on("/", HTTP_GET, handle_requests);
    for(auto file : files)
      server.on(file.filename.c_str(), HTTP_GET, handle_requests);
    server.on("^\\/api\\/([a-zA-Z]+)$", HTTP_GET, handle_api_request);

    server.onNotFound(handle_not_found);
    server.begin();
  }

  void handle_requests(AsyncWebServerRequest* request) {
    String url = request->url();
    Serial.println("requested: " + url);

    if(url == "/") {
      auto file = files[default_file];
      request->send_P(200, file.mimetype, (const uint8_t*) file.content, file.size);
      return;
    }

    for(auto file : files) {
      if(file.filename == url) {
        request->send_P(200, file.mimetype, (const uint8_t*) file.content, file.size);
      }
    }
  }
  void handle_not_found(AsyncWebServerRequest* request) {
    request->send(404, "text/html", request->url() + " not found");
  }

  void handle_api_request(AsyncWebServerRequest* request) {
    Serial.println("api call: " + request->url());

    request->send(200, "application/json", "{\"code\":1,\"message\":\"not implemented\"}");
  }
}
