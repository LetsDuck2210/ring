#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "api.h"
#include "../settings.h"

namespace WebApi {
  const ApiResponse RESP_OK = ApiResponse { .code = 0, .message = "OK" };
  const ApiResponse RESP_INVALID_REQUEST = ApiResponse { .code = 1, .message = "Invalid Request" };

  ApiResponse bell_enabled(JsonVariant value) {
    if(value.isNull())
      return ApiResponse(RESP_OK, String(Settings::is_bell_enabled())); 
    if(!value.is<bool>())
      return RESP_INVALID_REQUEST;

    Settings::set_bell_enabled(value.as<bool>());
    return RESP_OK;
  }
  ApiResponse bell_limit(JsonVariant value) {
    if(value.isNull())
      return ApiResponse(RESP_OK, String(Settings::get_bell_limit()));
    if(!value.is<uint8_t>())
      return RESP_INVALID_REQUEST;

    uint8_t limit = value.as<uint8_t>();
    if(limit == 0)
      return ApiResponse(1, "Limit must be greater than 0");

    Settings::set_bell_limit(value.as<uint8_t>());
    return RESP_OK;
  }

  const ApiEndpoint endpoints[ENDPOINT_COUNT] = {
    { .path = "/bell/enabled", .handler = bell_enabled },
    { .path = "/bell/limit", .handler = bell_limit },
  };
  ApiEndpoint get_endpoint(String path) {
    for(auto endpoint : endpoints) {
      if(endpoint.path == path)
        return endpoint;
    }
    return ApiEndpoint { path };
  }
}
