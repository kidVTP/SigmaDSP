/***********************************************|
| SigmaDSP library                              |
| https://github.com/MCUdude/SigmaDSP           |
|                                               |
| 8_Compressor_RMS.ino                          |
| This example we use the Compressor RMS block  |
| To do various changes to the audio signal.    |                                      |    
|***********************************************/

// Include Wire and SigmaDSP library
#include <Wire.h>
#include <SigmaDSP.h>

// Include generated parameter file
#include "SigmaDSP_parameters.h"


// The first parameter is the Wire object we'll be using when communicating wth the DSP
// The second parameter is the DSP i2c address, which is defined in the parameter file
// The third parameter is the sample rate
// An optional fourth parameter is the pin to physically reset the DSP
SigmaDSP dsp(Wire, DSP_I2C_ADDRESS, 48000.00f /*,12*/);


// Only needed if an external i2c EEPROM is present + the DSP is in selfboot mode
// The first parameter is the Wire object we'll be using when communicating wth the EEPROM
// The second parameter is the EEPROM i2c address, which is defined in the parameter file
// The third parameter is the EEPROM size in kilobits (kb)
// An optional fourth parameter is the pin to toggle while writing content to EEPROM
//DSPEEPROM ee(Wire, EEPROM_I2C_ADDRESS, 256, LED_BUILTIN);

// Create a compressor instance
compressor comp;

void setup()
{
  Serial.begin(9600);
  Serial.println(F("8_Compressor_RMS example\n"));

  Wire.begin();
  dsp.begin();
  //ee.begin();

  delay(2000);


  Serial.println(F("Pinging i2c bus...\n0 -> deveice is present\n2 -> device is not present"));
  Serial.print(F("DSP response: "));
  Serial.println(dsp.ping());
  //Serial.print(F("EEPROM ping: "));
  //Serial.println(ee.ping());


  // Use this step if no EEPROM is present
  Serial.print(F("\nLoading DSP program... "));
  loadProgram(dsp);
  Serial.println("Done!\n");


  // Comment out the three code lines above and use this step instead if EEPROM is present
  // The last parameter in writeFirmware is the FW version, and prevents the MCU from overwriting on every reboot
  //ee.writeFirmware(DSP_eeprom_firmware, sizeof(DSP_eeprom_firmware), 0);
  //dsp.reset();
  //delay(2000); // Wait for the FW to load from the EEPROM
}


void loop()
{
  Serial.println(F("Threshold = +6dB,  Ratio = 50,  RMS TC = 72ms, Hold = 0ms,   Decay = 869ms, Post gain = 0dB"));
  comp.threshold = 6;
  comp.ratio     = 50;
  comp.rms_tc    = 72;
  comp.hold      = 0;
  comp.decay     = 869;
  comp.postgain  = 0;
  dsp.compressorRMS(MOD_COMPRESSOR_ALG0_TWOCHANNELSINGLEDETECTALGFIX20_ADDR, comp);
  delay(5000);

  Serial.println(F("Threshold = -40dB, Ratio = 100, RMS TC = 72ms, Hold = 100ms, Decay = 869ms, Post gain = 0dB"));
  comp.threshold = -40;
  comp.ratio     = 100;
  comp.rms_tc    = 72;
  comp.hold      = 100;
  comp.decay     = 869;
  comp.postgain  = 0;
  dsp.compressorRMS(MOD_COMPRESSOR_ALG0_TWOCHANNELSINGLEDETECTALGFIX20_ADDR, comp);
  delay(5000);
}