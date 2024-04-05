#ifndef DYNAMICACTION
#define DYNAMICACTION

#include <Adafruit_NeoPixel.h>

class DynamicAction
{
private:
  String m_actionName = "";
protected:
  virtual void onStart() = 0;
  virtual void onRunAction() = 0;

  int UPDATE_INTERVAL_MS = 100;
  bool m_started = false;
  unsigned long m_lastUpdate = 0;
  Adafruit_NeoPixel * m_pPixels;
  int * m_pLuminance = 255;
  int m_numLeds = 0;
public:
  struct ActionColor 
  {
    uint8_t R;
    uint8_t G;
    uint8_t B;
  };

  enum ColorChannel
  {
    Red,
    Green,
    Blue
  };
  
  DynamicAction(int interval_ms, String actionName, Adafruit_NeoPixel * pixels, int num_leds, int * luminance)
  {
     UPDATE_INTERVAL_MS = interval_ms;
     m_actionName = actionName;
     m_pPixels = pixels;
     m_pLuminance = luminance;
     m_numLeds = num_leds;
  }

  void runAction()
  {
      if(millis() - m_lastUpdate > UPDATE_INTERVAL_MS)
      {
        onRunAction();
        m_lastUpdate = millis(); 
      }
  }
  
  void startAction()
  {
    Serial.print("Starting action ");
    Serial.println(m_actionName);
    m_started = true;
    m_lastUpdate = 0;
    onStart();
  }
  
  void stopAction()
  {
    Serial.print("Stoppin action ");
    Serial.println(m_actionName);
    m_started = false;
  }
};

#endif
