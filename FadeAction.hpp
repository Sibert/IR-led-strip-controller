#ifndef FADEACTION
#define FADEACTION

class FadeAction : public DynamicAction
{
private:
  ColorChannel m_currentColorChannel = Red;
  int m_currentColorVal = 255;
  const uint8_t m_diff = 2;
  
  void onStart() override;
public:
  FadeAction(Adafruit_NeoPixel * pixels, int * luminance, int num_leds) : DynamicAction(20, "Fade", pixels, num_leds, luminance)
  {
  }
  void onRunAction() override;
};

void FadeAction::onStart()
{
  m_pPixels->clear();
  m_pPixels->show();

  m_currentColorChannel = Red;
  m_currentColorVal = 255;
}

void FadeAction::onRunAction()
{
  uint32_t c = 0;

  if(m_currentColorChannel == Red)
  {
    c = m_pPixels->Color(m_currentColorVal, 255 - m_currentColorVal, 0);
    m_currentColorVal -= m_diff;

    if(m_currentColorVal <= 0)
    {
      m_currentColorVal = 255;    
      m_currentColorChannel = Green;
      Serial.println("Jumping to blue");
    }
  }
  else if(m_currentColorChannel == Green)
  {
    c = m_pPixels->Color(0, m_currentColorVal, 255 - m_currentColorVal);
    m_currentColorVal -= m_diff;

    if(m_currentColorVal <= 0)
    {
      m_currentColorVal = 255;    
      m_currentColorChannel = Blue;
      Serial.println("Jumping to blue");
    }
  }
  else if(m_currentColorChannel == Blue)
  {
    c = m_pPixels->Color(255 - m_currentColorVal, 0, m_currentColorVal);
    m_currentColorVal -= m_diff;

    if(m_currentColorVal <= 0)
    {
      m_currentColorVal = 255;    
      m_currentColorChannel = Red;
      Serial.println("Jumping to red");
    }
  }

    
  m_pPixels->fill(m_pPixels->gamma32(c));
  m_pPixels->show();
}

#endif
