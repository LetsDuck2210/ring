#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>

#include "api.h"

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
  void handle_api_get(AsyncWebServerRequest* request);
  void handle_api_post(AsyncWebServerRequest* request, JsonVariant data);
  void handle_not_found(AsyncWebServerRequest* request);

  const String api_prefix = "/api";
  void setup() {
    server.on("/", HTTP_GET, handle_requests);
    for(auto file : files)
      server.on(file.filename.c_str(), HTTP_GET, handle_requests);
    for(auto endpoint : WebApi::endpoints) {
      String api_path = api_prefix + endpoint.path;
      server.on(api_path.c_str(), HTTP_GET, handle_api_get);
      server.addHandler(new AsyncCallbackJsonWebHandler(api_path.c_str(), handle_api_post));
    }

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

  void handle_api_get(AsyncWebServerRequest* request) {
    JsonDocument doc;
    handle_api_post(request, doc.as<JsonVariant>());
  }
  void handle_api_post(AsyncWebServerRequest* request, JsonVariant data) {
    Serial.println("api request: " + request->url());

    String api_path = request->url().substring(api_prefix.length());
    auto endpoint = WebApi::get_endpoint(api_path);
    if(endpoint.handler == nullptr) {
      request->send(500);
      return;
    }

    if(!data.isNull())
      data = data["data"];
    auto response = endpoint.handler(data);
    JsonDocument json_resp;
    json_resp["code"] = response.code;
    json_resp["message"] = response.message;
    if(response.data != "")
      json_resp["data"] = response.data;

    String out;
    serializeJson(json_resp, out);

    request->send(200, "application/json", out);
    Serial.println("api request complete");
  }
}
