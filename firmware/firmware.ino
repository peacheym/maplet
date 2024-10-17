#include <Adafruit_NeoPixel.h>

#include <WiFi.h>
#include <mapper.h>
#include <Adafruit_MCP4728.h>
#include <Wire.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

Adafruit_NeoPixel pixels(1, 4, NEO_RGB + NEO_KHZ800);
#define DELAYVAL 500


// For disabling power saving
#include "esp_wifi.h"
Adafruit_MCP4728 mcp;

const char* ssid = "GemLab";
const char* password = "gemlabdal";

mpr_dev dev = 0;
mpr_sig inputSignal0 = 0;
mpr_sig inputSignal1 = 0;
mpr_sig inputSignal2 = 0;
mpr_sig inputSignal3 = 0;

float receivedValue = 0;
int DEBUG = 1;

void setup() {  

  int led_status_ind = 0;

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(155, 0, 0));
  pixels.show();

  WiFi.begin(ssid, password);
  Serial.begin(115200);
  
  // Disable WiFi power save (huge latency improvements)
  esp_wifi_set_ps(WIFI_PS_NONE);
  
  // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (led_status_ind==0){
      pixels.setPixelColor(0, pixels.Color(0,0,155));
      led_status_ind=1;
    }
    else{
      pixels.setPixelColor(0, pixels.Color(155, 0, 0));
      led_status_ind=0;
    }
    pixels.show();

  }
  Serial.println("Connected to WiFi");

//  pixels.setPixelColor(0, pixels.Color(0, 155, 0));
//  pixels.show();
//  pixels.setPixelColor(0, pixels.Color(0, 0, 155));
//  pixels.show();


  if(!mcp.begin(0x64)){
    Serial.println("Failed to find MCP4728");
    while(1){
      delay(10);
    }
  }

  Serial.println("Initialized DAC");


  // Initalize libmapper goodies
  float min_pw = 0.0f;
  float max_pw = 4095.0f;  

  dev = mpr_dev_new("Maplet", 0);

  Serial.println("Initialized libmapper device");

  inputSignal0 = mpr_sig_new(dev, MPR_DIR_IN, "channel1", 1, MPR_FLT, "V", &min_pw, &max_pw, 0, inputSignalHandler, MPR_SIG_UPDATE);
  inputSignal1 = mpr_sig_new(dev, MPR_DIR_IN, "channel2", 1, MPR_FLT, "V", &min_pw, &max_pw, 0, inputSignalHandler, MPR_SIG_UPDATE);           
  inputSignal2 = mpr_sig_new(dev, MPR_DIR_IN, "channel3", 1, MPR_FLT, "V", &min_pw, &max_pw, 0, inputSignalHandler, MPR_SIG_UPDATE);           
  inputSignal3 = mpr_sig_new(dev, MPR_DIR_IN, "channel4", 1, MPR_FLT, "V", &min_pw, &max_pw, 0, inputSignalHandler, MPR_SIG_UPDATE);           
           
  Serial.println("Initialized libmapper signals");

  for(int i=0; i<10; i++){
    mpr_dev_poll(dev, 500);

    if (led_status_ind==0){
      pixels.setPixelColor(0, pixels.Color(0,155,0));
      led_status_ind=1;
    }
    else{
      pixels.setPixelColor(0, pixels.Color(0, 0, 155));
      led_status_ind=0;
    }
    pixels.show();
  }

  Serial.println("Done Polling");

  pixels.setPixelColor(0, pixels.Color(0, 155, 0));
  pixels.show();

}

void loop() {  
  mpr_dev_poll(dev, 100);

//  int num_maps = mpr_obj_get_prop_as_int32(inputSignal0, MPR_PROP_NUM_MAPS_IN, NULL);
//
//  if(num_maps > 0){
//    pixels.setPixelColor(0, pixels.Color(0, 155, 67));
//  }
//  else{
//    pixels.setPixelColor(0, pixels.Color(255,0,0));
//  }
//  pixels.show();

}



void inputSignalHandler(mpr_sig sig, mpr_sig_evt evt, mpr_id inst, int length,
                        mpr_type type, const void* value, mpr_time time) {
    float val = *((float*)value);

    if(strcmp(mpr_obj_get_prop_as_str(sig, MPR_PROP_NAME, NULL), "channel1") == 0)
      mcp.setChannelValue(MCP4728_CHANNEL_A, (int)val);
    else if (strcmp(mpr_obj_get_prop_as_str(sig, MPR_PROP_NAME, NULL), "channel2") == 0)
        mcp.setChannelValue(MCP4728_CHANNEL_B, (int)val);
    else if (strcmp(mpr_obj_get_prop_as_str(sig, MPR_PROP_NAME, NULL), "channel3") == 0)
        mcp.setChannelValue(MCP4728_CHANNEL_C, (int)val);
    else if (strcmp(mpr_obj_get_prop_as_str(sig, MPR_PROP_NAME, NULL), "channel4") == 0)
        mcp.setChannelValue(MCP4728_CHANNEL_D, (int)val);
    else Serial.println("Error with signal names");
    
}
