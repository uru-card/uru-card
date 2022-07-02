
#include "hidusb.h"

#pragma once
#if defined(FIDO2_TRANSPORT_USB)

class CTAPHID : public HIDusb
{
public:
    CTAPHID(uint8_t id = 1);
    bool begin(char* str = nullptr);
};

#define TUD_HID_REPORT_DESC_CTAP_INOUT(report_size, ...) \
    HID_USAGE_PAGE_N ( 0xF1D0, 2                  ),\
    HID_USAGE        ( 0x01                       ),\
    HID_COLLECTION   ( HID_COLLECTION_APPLICATION ),\
      /* Report ID if any */\
      __VA_ARGS__ \
      /* Input */ \
      HID_USAGE       ( 0x02                                   ),\
      HID_LOGICAL_MIN ( 0x00                                   ),\
      HID_LOGICAL_MAX_N ( 0xff, 2                              ),\
      HID_REPORT_SIZE ( 8                                      ),\
      HID_REPORT_COUNT( report_size                            ),\
      HID_INPUT       ( HID_DATA | HID_ABSOLUTE | HID_VARIABLE ),\
      /* Output */ \
      HID_USAGE       ( 0x03                                   ),\
      HID_LOGICAL_MIN ( 0x00                                   ),\
      HID_LOGICAL_MAX_N ( 0xff, 2                              ),\
      HID_REPORT_SIZE ( 8                                      ),\
      HID_REPORT_COUNT( report_size                            ),\
      HID_OUTPUT      ( HID_DATA | HID_ABSOLUTE | HID_VARIABLE ),\
    HID_COLLECTION_END \

#endif
