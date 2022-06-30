#include <cdcftdi.h>
#include <usbhub.h>

#include "pgmstrings.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

class FTDIAsync : public FTDIAsyncOper
{
public:
    uint8_t OnInit(FTDI *pftdi);
};

uint8_t FTDIAsync::OnInit(FTDI *pftdi)
{
    uint8_t rcode = 0;

    rcode = pftdi->SetBaudRate(115200);

    if (rcode)
    {
        ErrorMessage<uint8_t>(PSTR("SetBaudRate"), rcode);
        return rcode;
    }
    rcode = pftdi->SetFlowControl(FTDI_SIO_DISABLE_FLOW_CTRL);

    if (rcode)
        ErrorMessage<uint8_t>(PSTR("SetFlowControl"), rcode);

    return rcode;
}

USB              Usb;
//USBHub         Hub(&Usb);
FTDIAsync        FtdiAsync;
FTDI             Ftdi(&Usb, &FtdiAsync);

void setup()
{
  Serial.begin( 115200 );
// #if !defined(__MIPSEL__)
//   while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
// #endif
//   Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay( 200 );

  for (;;) {
    Usb.Task();

    if( Usb.getUsbTaskState() == USB_STATE_RUNNING ) {
        uint8_t  buf[4] = {0};
        uint16_t rcvd = 4;
        uint8_t rcode = Ftdi.RcvData(&rcvd, buf);
        if (rcvd > 2) {
            // Serial.print((char*)(buf+2));
            Ftdi.SndData(2, (uint8_t*)(buf+2));
        }
    }
  }

  
}

void loop() {
    //
}