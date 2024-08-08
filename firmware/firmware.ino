#include <WiFi.h>
#include <mapper.h>
#include <Adafruit_MCP4728.h>
#include <Wire.h>

// For disabling power saving
#include "esp_wifi.h"
Adafruit_MCP4728 mcp;

const char* ssid = "*************";
const char* password = "*************";

mpr_dev dev = 0;
mpr_sig inputSignal0 = 0;
mpr_sig inputSignal1 = 0;
mpr_sig inputSignal2 = 0;
mpr_sig inputSignal3 = 0;

float receivedValue = 0;
int DEBUG = 1;

void setup() {  
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  
  // Disable WiFi power save (huge latency improvements)
  esp_wifi_set_ps(WIFI_PS_NONE);
  
  // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

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

  dev = mpr_dev_new("CVmapper", 0);

  Serial.println("Initialized libmapper device");

  inputSignal0 = mpr_sig_new(dev, MPR_DIR_IN, "channel1", 1, MPR_FLT, "V", &min_pw, &max_pw, 0, inputSignalHandler, MPR_SIG_UPDATE);
  inputSignal1 = mpr_sig_new(dev, MPR_DIR_IN, "channel2", 1, MPR_FLT, "V", &min_pw, &max_pw, 0, inputSignalHandler, MPR_SIG_UPDATE);           
  inputSignal2 = mpr_sig_new(dev, MPR_DIR_IN, "channel3", 1, MPR_FLT, "V", &min_pw, &max_pw, 0, inputSignalHandler, MPR_SIG_UPDATE);           
  inputSignal3 = mpr_sig_new(dev, MPR_DIR_IN, "channel4", 1, MPR_FLT, "V", &min_pw, &max_pw, 0, inputSignalHandler, MPR_SIG_UPDATE);           
           
  Serial.println("Initialized libmapper signals");

  for(int i=0; i<10; i++){
    mpr_dev_poll(dev, 500);
  }

  Serial.println("Done Polling");


}

void loop() {  
  mpr_dev_poll(dev, 100);
}



void inputSignalHandler(mpr_sig sig, mpr_sig_evt evt, mpr_id inst, int length,
                        mpr_type type, const void* value, mpr_time time) {
    float val = *((float*)value);

    // This is bad programming -- but I soldered the jacks to the wrong DAC pins in V0.1
    if(strcmp(mpr_obj_get_prop_as_str(sig, MPR_PROP_NAME, NULL), "channel1") == 0)
      mcp.setChannelValue(MCP4728_CHANNEL_C, (int)val);
    else if (strcmp(mpr_obj_get_prop_as_str(sig, MPR_PROP_NAME, NULL), "channel2") == 0)
        mcp.setChannelValue(MCP4728_CHANNEL_D, (int)val);
    else if (strcmp(mpr_obj_get_prop_as_str(sig, MPR_PROP_NAME, NULL), "channel3") == 0)
        mcp.setChannelValue(MCP4728_CHANNEL_B, (int)val);
    else if (strcmp(mpr_obj_get_prop_as_str(sig, MPR_PROP_NAME, NULL), "channel4") == 0)
        mcp.setChannelValue(MCP4728_CHANNEL_A, (int)val);
    else Serial.println("Error with signal names");
    
}
