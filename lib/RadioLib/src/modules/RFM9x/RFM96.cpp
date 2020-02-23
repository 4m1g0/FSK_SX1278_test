#include "RFM96.h"

RFM96::RFM96(Module* mod) : SX1278(mod) {

}

int16_t RFM96::begin(float freq, float bw, uint8_t sf, uint8_t cr, uint8_t syncWord, int8_t power, uint8_t currentLimit, uint16_t preambleLength, uint8_t gain) {
  // execute common part
  int16_t state = SX127x::begin(RFM9X_CHIP_VERSION, syncWord, currentLimit, preambleLength);
  RADIOLIB_ASSERT(state);

  // configure settings not accessible by API
  state = config();
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  state = setBandwidth(bw);
  RADIOLIB_ASSERT(state);

  state = setSpreadingFactor(sf);
  RADIOLIB_ASSERT(state);

  state = setCodingRate(cr);
  RADIOLIB_ASSERT(state);

  state = setOutputPower(power);
  RADIOLIB_ASSERT(state);

  state = setGain(gain);

  return(state);
}

int16_t RFM96::setFrequency(float freq) {
  // check frequency range
  if((freq < 433.0) || (freq > 470.0)) {
    return(ERR_INVALID_FREQUENCY);
  }

  // set frequency
  return(SX127x::setFrequencyRaw(freq));
}
