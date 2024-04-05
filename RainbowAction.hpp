#ifndef RAINBOWACTION
#define RAINBOWACTION

class RainbowAction : public DynamicAction
{
private:
  uint16_t m_hueShift = 0;
 
  void onStart() override;
public:
  RainbowAction(Adafruit_NeoPixel * pixels, int * luminance, int num_leds) : DynamicAction(20, "Rainbow", pixels, num_leds, luminance)
  {
    
  }

  void onRunAction() override;
};

void RainbowAction::onStart()
{
  m_pPixels->clear();
  m_pPixels->show();
  m_hueShift = 0;
}

void RainbowAction::onRunAction()
{
  m_pPixels->rainbow(m_hueShift, 1, 255, *m_pLuminance, true);
  m_pPixels->show();
  m_hueShift += 65535 / 250;
}

#endif
