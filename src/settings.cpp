#include <Arduino.h>

namespace Settings {
  bool bell_enabled = true;
  uint8_t bell_limit = 2;

  void set_bell_enabled(bool enabled) {
    Serial.println(String(enabled ? "enabled" : "disabled") + " bell");
    bell_enabled = enabled;
  }
  void set_bell_limit(uint8_t limit) {
    if(limit == 0) return;
    Serial.println("set bell limit to " + String(limit));
    bell_limit = limit;
  }

  bool is_bell_enabled() {
    return bell_enabled;
  }
  uint8_t get_bell_limit() {
    return bell_limit;
  }
}
