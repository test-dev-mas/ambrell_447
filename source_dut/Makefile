CC = avr-gcc
CFLAGS = -Os -DF_CPU=14745600UL -mmcu=atmega2560
CONF = "C:\Users\Kendell\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\etc\avrdude.conf"

USBPORT = usb

all: main.out

main.out: main.o hx8357.o sd.o spi.o uart.o
	$(CC) $(CFLAGS) main.o hx8357.o sd.o spi.o uart.o -o main.out

%.o: %.c
	$(CC) $(CFLAGS) -c $^

%.hex: %.out
	avr-objcopy -O ihex -R .eeprom $< $@

%.elf: 
	$(CC) $(CFLAGS) main.o hx8357.o sd.o spi.o uart.o -o main.elf
# an Arduino Uno is used as the programmer
install.%: %.hex
	avrdude -F -V -C $(CONF) -p atmega2560 -c avrispmkii -P $(USBPORT)  -b 19200 -U lfuse:w:0xff:m -U hfuse:w:0xd0:m -U efuse:w:0xfc:m -U flash:w:$<

clean:
	del *.out *.o *.elf