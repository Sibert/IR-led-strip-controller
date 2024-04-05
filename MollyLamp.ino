#include <Adafruit_NeoPixel.h>
#include <IRremote.hpp>
#include "DynamicAction.hpp"
#include "RainbowAction.hpp"
#include "BreathingAction.hpp"
#include "FadeAction.hpp"
#include "DiscoAction.hpp"

#define NEOPIXEL_PIN 4
#define NUM_PIXELS 39

Adafruit_NeoPixel pixels(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

enum Buttons {
  NO_CMD    = 0,
  ZERO      = 0x19,
  ONE       = 0x45,
  TWO       = 0x46,
  THREE     = 0x47,
  FOUR      = 0x44,
  FIVE      = 0x40,
  SIX       = 0x43,
  SEVEN     = 0x07,
  EIGHT     = 0x15,
  NINE      = 0x09,
  ASTERISK  = 0x16,
  HASHTAG   = 0x0D,
  OK        = 0x1C,
  LEFT      = 0x08,
  RIGHT     = 0x5A,
  UP        = 0x18,
  DOWN      = 0x52
};

void setup() {
  // put your setup code here, to run once:
  IrReceiver.begin(9, ENABLE_LED_FEEDBACK);

  Serial.begin(115200);
  Serial.println("Starting...");

  pixels.begin();
}

int lastCmd = 0;
int currentCmd = 0;
int luminance = 255;

DynamicAction * rainbow = new RainbowAction(&pixels, &luminance, NUM_PIXELS);
DynamicAction * breathing = new BreathingAction(&pixels, &luminance,NUM_PIXELS);
DynamicAction * fade = new FadeAction(&pixels, &luminance,NUM_PIXELS);
DynamicAction * disco = new DiscoAction(&pixels, &luminance,NUM_PIXELS);
DynamicAction * currentAction = nullptr;

struct GradientColor {
  uint8_t R;
  uint8_t G;
  uint8_t B;
};

bool settingGradientColor1 = false;
bool settingGradientColor2 = false;

GradientColor gradientColor1 = { 0, 0, 0 };
GradientColor gradientColor2 = { 0, 0, 0 };

unsigned long lastCmdRecv = 0;
unsigned long cmdTimeout = 200;

unsigned long gradientSelectionBlink = 0;
bool gradientBlinkState = false;

void loop() {

    if(millis() - gradientSelectionBlink > 500)
    {
      uint32_t c = pixels.Color(255, 255, 255);
      
      if(gradientBlinkState)
      {
        c = 0;        
      }

      gradientBlinkState = !gradientBlinkState;

      if(settingGradientColor1)
      {
        pixels.clear();
        pixels.setPixelColor(0, c);
        pixels.show();
      }
      else if(settingGradientColor2)
      {
        pixels.clear();
        pixels.setPixelColor(NUM_PIXELS - 1, c);
        pixels.show();
      }
      gradientSelectionBlink = millis();
    }

    if (IrReceiver.decode()) 
    {
        IrReceiver.resume(); // Enable receiving of the next value

        if(millis() - lastCmdRecv >= cmdTimeout)
        {
          Serial.print("Received 0x");
          currentCmd = IrReceiver.decodedIRData.command;
          Serial.println(lastCmd, HEX);
          lastCmdRecv = millis();
        }
        else
        {
          Serial.println("Command ignored due to timeout");
        }
    }

    if(IrReceiver.isIdle())
    {
      switch((Buttons)currentCmd)
      {
        case ZERO:
          Serial.println("Handling '0'");
          setColor(255, 255, 255);
          break;
        case ONE:
          Serial.println("Handling '1'");
          setColor(255, 0, 0);
          break;
        case TWO:
          Serial.println("Handling '2'");
          setColor(0, 255, 0);
          break;
        case THREE:
          Serial.println("Handling '3'");
          setColor(0, 0, 255);
          break;
        case FOUR:
          Serial.println("Handling '4'");
          setColor(255, 255, 0);
          break;
        case FIVE:
          Serial.println("Handling '5'");
          setColor(255, 0, 255);
          break;
        case SIX:
          Serial.println("Handling '6'");
          setColor(0, 255, 255);
          break;
        case SEVEN:
          Serial.println("Handling '7'");
          setAction(rainbow);
          break;
        case EIGHT:
          Serial.println("Handling '8'");
          setAction(breathing);
          break;
        case NINE:
          Serial.println("Handling '9'");
          setAction(fade);
          break;
        case OK:
          Serial.println("Handling '#'");
          setColor(0, 0, 0);
          break;
        case ASTERISK:
          Serial.println("Handling '*'");
          setAction(disco);
          break;
        case HASHTAG:
          Serial.println("Handling 'OK'");
          setAction(nullptr);
          setGradient(gradientColor1, gradientColor2);
          break;
        case UP:
          increaseLuminance();
          break;
        case DOWN:
          decreaseLuminance();
          break;
        case LEFT:
          Serial.println("Handling 'LEFT'");
          settingGradientColor1 = true;
          setAction(nullptr);
          break;
        case RIGHT:
          Serial.println("Handling 'RIGHT'");
          settingGradientColor2 = true;
          setAction(nullptr);
          break;
      }
      if(currentCmd != 0)
      {
        lastCmd = currentCmd;
        currentCmd = 0;
      }

      if(currentAction != nullptr)
      {
        currentAction->runAction();
      }
    }
}

int lastLuminanceChange = 0;

void increaseLuminance()
{
  changeLuminance(5);
}

void decreaseLuminance()
{
  changeLuminance(-5);
}

void changeLuminance(int change)
{
  if(millis() - lastLuminanceChange > 100)
  {
    if(luminance + change > 255)
    {
      luminance = 255;
    }
    else if(luminance + change < 0)
    {
      luminance = 0;
    }
    else
    {
      luminance += change;
    }
    
    Serial.print("Luminance set to ");
    Serial.println(luminance);

    pixels.setBrightness(luminance);
    pixels.show();
    
    lastLuminanceChange = millis();    
  }
}

void setAction(DynamicAction * action)
{
  if(currentAction == action)
  {
    return;
  }

  if(currentAction != nullptr)
  {
    currentAction->stopAction();
  }
  
  currentAction = action;

  if(currentAction != nullptr)
  {
    currentAction->startAction();
      
    settingGradientColor1 = false;
    settingGradientColor2 = false;
  }
}

void setColor(int r, int g, int b)
{
  setAction(nullptr);

  if(settingGradientColor1)
  {
    Serial.println("Setting gradient color 1");
    gradientColor1 = { r, g, b };
    settingGradientColor1 = false;
    setGradient(gradientColor1, gradientColor2);
  }
  else if(settingGradientColor2)
  {
    Serial.println("Setting gradient color 2");
    gradientColor2 = { r, g, b };
    settingGradientColor2 = false;
    setGradient(gradientColor1, gradientColor2);
  }
  else
  {
    pixels.fill(pixels.Color(r, g, b), 0, NUM_PIXELS);
    pixels.show();
  }
}

void setGradient(struct GradientColor c1, struct GradientColor c2)
{
  int gradientStep_R = (c2.R - c1.R) / NUM_PIXELS;
  int gradientStep_G = (c2.G - c1.G) / NUM_PIXELS;
  int gradientStep_B = (c2.B - c1.B) / NUM_PIXELS;

  for(int i = 0; i < NUM_PIXELS; i++)
  {
    pixels.setPixelColor(i, c1.R + (i * gradientStep_R), c1.G + (i * gradientStep_G), c1.B + (i * gradientStep_B));
  }

  pixels.show();
}
