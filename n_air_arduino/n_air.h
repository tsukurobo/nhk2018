#ifndef NAir_h
#define NAir_h
#include "ti2c.h"
#include "Arduino.h"

class NAir {
  public:
    NAir(uint8_t i2caddr);
    void toggle(int port);
    void init();
  private:
    uint8_t addr;
    Ti2c ti2c;
};
#endif

