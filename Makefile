PORT=/dev/ttyUSB0
BAUDRATE=9600

EXTRA_FLAGS=-DASYNCWEBSERVER_REGEX

all: build upload monitor

build: clean compile

compile:
	./convert_web_to_include.sh
	arduino-cli compile --build-property "compiler.cpp.extra_flags=\"${EXTRA_FLAGS}\""

upload:
	arduino-cli upload -p ${PORT}

monitor:
	arduino-cli monitor -p ${PORT} -c baudrate=${BAUDRATE}

clean:
	rm -rf src/www
