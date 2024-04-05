#ifndef DISCOACTION
#define DISCOACTION

class DiscoAction : public DynamicAction
{
private:
  
  void onStart() override;
public:
  DiscoAction(Adafruit_NeoPixel * pixels, int * luminance, int num_leds) : DynamicAction(300, "Fade", pixels, num_leds, luminance)
  {
  }
  void onRunAction() override;
};

void DiscoAction::onStart()
{
  m_pPixels->clear();
  m_pPixels->show();
}

void DiscoAction::onRunAction()
{
  uint8_t R = random(0,2) * 255;    
  uint8_t G = random(0,2) * 255;    
  uint8_t B = random(0,2) * 255;    

  uint32_t c = m_pPixels->Color(R, G, B);
  
  m_pPixels->fill(m_pPixels->gamma32(c));
  m_pPixels->show();
}

#endif
