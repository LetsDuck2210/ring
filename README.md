# Ring

This project is made for the ESP8266-12F.

Current Features:

- Send notification to [ntfy](https://ntfy.sh) server when doorbell is pressed

- Doorbell presses which are passed through the controller will be limited

Planned Features:

- Enable/Disable doorbell through webinterface


## How to use

Copy the file `secrets\_template.h` to `secrets.h`, then fill out all fields inside `secrets.h`.

Connect the ESP8266 to the computer, compile and upload. Example using arduino-cli:

```bash
device=/dev/ttyUSB0
arduino-cli compile \
    && arduino-cli upload -p $device
    && arduino-cli monitor -p $device
```
