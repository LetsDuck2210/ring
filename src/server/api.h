namespace WebApi {
  struct ApiResponse {
    uint8_t code;
    String message;
    String data;

    ApiResponse(uint8_t code, String message)
      : code(code), message(message), data("") {}
    ApiResponse(uint8_t code, String message, String data)
      :code(code), message(message), data(data) {}
    ApiResponse(const ApiResponse& other, String data)
      : code(other.code), message(other.message), data(data) {}
  };
  struct ApiEndpoint {
    String path;
    ApiResponse (*handler)(JsonVariant value);
  };

  const uint8_t ENDPOINT_COUNT = 2;
  extern const ApiEndpoint endpoints[ENDPOINT_COUNT];

  ApiEndpoint get_endpoint(String path);
}
