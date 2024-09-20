#error please edit secrets.h and remove the first line

// WiFi SSID and password
inline const char* ssid = "";
inline const char* pass = "";

namespace NtfySec {
	// ntfy user credentials
	inline const char* user = "";
	inline const char* pass = "";

	// ntfy topic
	inline const char* topic = "";

	// ntfy server ip and port
	inline const IPAddress server(0, 0, 0, 0);
	inline const uint16_t port = 80;
}
