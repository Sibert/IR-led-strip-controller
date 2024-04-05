#ifndef BREATHINGACTION
#define BREATHINGACTION

class BreathingAction : public DynamicAction
{
private:
  ColorChannel m_currentColorChannel = Red;
  uint8_t m_currentColorVal = 0;
  bool m_currentDirectionUp = true;
  uint8_t m_diff = 1;
  
  void onStart() override;
public:
  BreathingAction(Adafruit_NeoPixel * pixels, int * luminance, int num_leds) : DynamicAction(10, "Breathing", pixels, num_leds, luminance)
  {
  }
  void onRunAction() override;
};

void BreathingAction::onStart()
{
  m_pPixels->clear();
  m_pPixels->show();
}

void BreathingAction::onRunAction()
{
  /*m_pPixels->fill(m_pPixels->Color(m_currentColor.R, m_currentColor.G, m_currentColor.B));
  m_pPixels->show();*/
  uint32_t c = 0;

  if(m_currentDirectionUp)
  {
    if(m_currentColorVal + m_diff <= 255)
    {
      m_currentColorVal += m_diff;
    }
    else
    {
      m_currentColorVal = 255;
      m_currentDirectionUp = false;
    }
  }
  else
  {
    if(m_currentColorVal - m_diff >= 0)
    {
      m_currentColorVal -= m_diff;
    }
    else
    {
      m_currentColorVal = 0;
      m_currentDirectionUp = true;
    }
  }

  if(m_currentColorChannel == Red)
  {
    if(m_currentColorVal == 0 && m_currentDirectionUp)
    {
      m_currentColorChannel = Green;
    }
    else
    {
      c = m_pPixels->Color(m_currentColorVal, 0, 0);
    }
  }
  else if(m_currentColorChannel == Green)
  {
    if(m_currentColorVal == 0 && m_currentDirectionUp)
    {
      m_currentColorChannel = Blue;
    }
    else
    {
      c = m_pPixels->Color(0, m_currentColorVal, 0);
    }
  }
  else if(m_currentColorChannel == Blue)
  {
    if(m_currentColorVal == 0 && m_currentDirectionUp)
    {
      m_currentColorChannel = Red;
    }
    else
    {
      c = m_pPixels->Color(0, 0, m_currentColorVal);
    }
  }

  
  m_pPixels->fill(m_pPixels->gamma32(c));
  m_pPixels->show();
}

#endif
