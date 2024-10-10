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
  void handle_not_found(AsyncWebServerRequest* request);
  String toString(unsigned char* cstr, unsigned int len, size_t start = 0);

  void setup() {
    server.on("/", handle_requests);
    for(auto file : files)
      server.on(file.filename.c_str(), handle_requests);

    server.onNotFound(handle_not_found);
    server.begin();
  }

  void handle_requests(AsyncWebServerRequest* request) {
    String url = request->url();

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

  // String toString(unsigned char* cstr, unsigned int len, size_t start = 0) {
  // }
}
