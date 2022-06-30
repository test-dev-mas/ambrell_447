CC = avr-gcc
CFLAGS = -Os -DF_CPU=14745600UL -mmcu=atmega2560
CONF = "C:\Users\Kendell\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\etc\avrdude.conf"

USBPORT = usb

all: main.out

%.out: %.c
	$(CC) $(CFLAGS) $< -o  $@

%.hex: %.out
	avr-objcopy -O ihex -R .eeprom $< $@

# an Arduino Uno is used as the programmer
install.%: %.hex
	avrdude -F -V -D -v -C $(CONF) -p atmega2560 -c avrispmkii -P $(USBPORT)  -b 19200 -U flash:w:$<

clean:
	del *.out 