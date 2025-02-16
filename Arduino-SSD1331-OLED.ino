/*****************************************************************

 * Interfacing Arduino with SSD1331 color display (96x64 pixel).
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/

/*****************************************************************/

#include <Adafruit_GFX.h>       // Adafruit Graphics library
#include <Adafruit_SSD1331.h>   // Adafruit SSD1331 display driver
#include <SPI.h>                // Arduino SPI library

// Connection between Arduino and SSD1331 display
#define CS_PIN   10   // Chip select pin
#define DC_PIN   9    // Data/Command pin
#define RST_PIN  8    // Or set to -1 and connect to board RESET pin

// Option 1: Hardware SPI pins must be used (for UNO SCLK = 13 and MOSI = 11)
Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, CS_PIN, DC_PIN, RST_PIN);

// Option 2: Software SPI, comment the above line and uncomment the below lines, slower than hardware SPI
/*
#define SCLK_PIN  13   // Clock pin
#define MOSI_PIN  11   // Master-Out Slave-In pin
Adafruit_SSD1331 display = Adafruit_SSD1331(CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);
*/

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

float p = 3.1415926;
#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

void setup(void) {
  Serial.begin(9600);

  display.begin();

  Serial.println("init");
  uint16_t time = millis();
  display.fillScreen(BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  lcdTestPattern();
  delay(2000);

  display.fillScreen(BLACK);
  display.setCursor(0,0);
  display.print("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa");
  delay(2000);

  // tft print function!
  tftPrintTest();
  delay(4000);

  //a single pixel
  display.drawPixel(display.width()/2, display.height()/2, GREEN);
  delay(500);

  // line draw test
  testlines(YELLOW);
  delay(500);

  // optimized lines
  testfastlines(RED, BLUE);
  delay(500);

  testdrawrects(GREEN);
  delay(1000);

  testfillrects(YELLOW, MAGENTA);
  delay(1000);

  display.fillScreen(BLACK);
  testfillcircles(10, BLUE);
  testdrawcircles(10, WHITE);
  delay(1000);

  testroundrects();
  delay(500);

  testtriangles();

  Serial.println("done");
  delay(2000);

  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
}

void loop() {

}

void testlines(uint16_t color) {
   display.fillScreen(BLACK);
   for (int16_t x=0; x < display.width()-1; x+=6) {
     display.drawLine(0, 0, x, display.height()-1, color);
   }
   for (int16_t y=0; y < display.height()-1; y+=6) {
     display.drawLine(0, 0, display.width()-1, y, color);
   }

   display.fillScreen(BLACK);
   for (int16_t x=0; x < display.width()-1; x+=6) {
     display.drawLine(display.width()-1, 0, x, display.height()-1, color);
   }
   for (int16_t y=0; y < display.height()-1; y+=6) {
     display.drawLine(display.width()-1, 0, 0, y, color);
   }

   // To avoid ESP8266 watchdog timer resets when not using the hardware SPI pins
   delay(0);

   display.fillScreen(BLACK);
   for (int16_t x=0; x < display.width()-1; x+=6) {
     display.drawLine(0, display.height()-1, x, 0, color);
   }
   for (int16_t y=0; y < display.height()-1; y+=6) {
     display.drawLine(0, display.height()-1, display.width()-1, y, color);
   }

   display.fillScreen(BLACK);
   for (int16_t x=0; x < display.width()-1; x+=6) {
     display.drawLine(display.width()-1, display.height()-1, x, 0, color);
   }
   for (int16_t y=0; y < display.height()-1; y+=6) {
     display.drawLine(display.width()-1, display.height()-1, 0, y, color);
   }

}

void testdrawtext(char *text, uint16_t color) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }
}

void testfastlines(uint16_t color1, uint16_t color2) {
   display.fillScreen(BLACK);
   for (int16_t y=0; y < display.height()-1; y+=5) {
     display.drawFastHLine(0, y, display.width()-1, color1);
   }
   for (int16_t x=0; x < display.width()-1; x+=5) {
     display.drawFastVLine(x, 0, display.height()-1, color2);
   }
}

void testdrawrects(uint16_t color) {
 display.fillScreen(BLACK);
 for (int16_t x=0; x < display.height()-1; x+=6) {
   display.drawRect((display.width()-1)/2 -x/2, (display.height()-1)/2 -x/2 , x, x, color);
 }
}

void testfillrects(uint16_t color1, uint16_t color2) {
 display.fillScreen(BLACK);
 for (int16_t x=display.height()-1; x > 6; x-=6) {
   display.fillRect((display.width()-1)/2 -x/2, (display.height()-1)/2 -x/2 , x, x, color1);
   display.drawRect((display.width()-1)/2 -x/2, (display.height()-1)/2 -x/2 , x, x, color2);
 }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (uint8_t x=radius; x < display.width()-1; x+=radius*2) {
    for (uint8_t y=radius; y < display.height()-1; y+=radius*2) {
      display.fillCircle(x, y, radius, color);
    }
  }
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < display.width()-1+radius; x+=radius*2) {
    for (int16_t y=0; y < display.height()-1+radius; y+=radius*2) {
      display.drawCircle(x, y, radius, color);
    }
  }
}

void testtriangles() {
  display.fillScreen(BLACK);
  int color = 0xF800;
  int t;
  int w = display.width()/2;
  int x = display.height()-1;
  int y = 0;
  int z = display.width();
  for (t = 0 ; t <= 7; t+=1) {
    display.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  display.fillScreen(BLACK);
  int color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = display.width();
    int h = display.height();
    for(i = 0 ; i <= 8; i+=1) {
      display.drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
    }
    color+=100;
  }
}

void tftPrintTest() {
  display.fillScreen(BLACK);
  display.setCursor(0, 5);
  display.setTextColor(RED);
  display.setTextSize(1);
  display.println("Hello World!");
  display.setTextColor(YELLOW, GREEN);
  display.setTextSize(2);
  display.print("Hello Wo");
  display.setTextColor(BLUE);
  display.setTextSize(3);
  display.print(1234.567);
  delay(1500);
  display.setCursor(0, 5);
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  display.setTextSize(0);
  display.println("Hello World!");
  display.setTextSize(1);
  display.setTextColor(GREEN);
  display.print(p, 5);
  display.println(" Want pi?");
  display.print(8675309, HEX); // print 8,675,309 out in HEX!
  display.print(" Print HEX");
  display.setTextColor(WHITE);
  display.println("Sketch has been");
  display.println("running for: ");
  display.setTextColor(MAGENTA);
  display.print(millis() / 1000);
  display.setTextColor(WHITE);
  display.print(" seconds.");
}

/**************************************************************************/
/*!
    @brief  Renders a simple test pattern on the LCD
*/
/**************************************************************************/
void lcdTestPattern(void)
{
  uint8_t w,h;
  display.setAddrWindow(0, 0, 96, 64);

  for (h = 0; h < 64; h++) {
    for (w = 0; w < 96; w++) {
      if (w > 83) {
        display.writePixel(w, h, WHITE);
      } else if (w > 71) {
        display.writePixel(w, h, BLUE);
      } else if (w > 59) {
        display.writePixel(w, h, GREEN);
      } else if (w > 47) {
        display.writePixel(w, h, CYAN);
      } else if (w > 35) {
        display.writePixel(w, h, RED);
      } else if (w > 23) {
        display.writePixel(w, h, MAGENTA);
      } else if (w > 11) {
        display.writePixel(w, h, YELLOW);
      } else {
        display.writePixel(w, h, BLACK);
      }
    }
  }
  display.endWrite();
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2
#define DELTAT 3

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][4];
  uint16_t _color[NUMFLAKES];

  // Initialize 'snowflake' positions
  for(f = 0; f < NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
    icons[f][YPOS]   = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
    icons[f][DELTAT] = 0;
    _color[f] = random(BLUE, WHITE);
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  display.fillScreen(BLACK);   // Clear the display

  for(;;) { // Loop forever...
    // Draw each snowflake:
    for(f = 0; f < NUMFLAKES; f++) {
      icons[f][DELTAT] += 1;
      if (icons[f][DELTAT] >= icons[f][DELTAY]) {
        icons[f][YPOS] += 1;
        icons[f][DELTAT] = 0;
        display.drawBitmap(icons[f][XPOS], icons[f][YPOS] - 1, bitmap, w, h, BLACK);   // Erase the snowflake
        display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, _color[f]);   // Draw the snowflake at the new position
      }
    }

    delay(50);   // Wait 50 milliseconds

    // Then update coordinates of each flake...
    for(f = 0; f < NUMFLAKES; f++) {
      // If snowflake is off the bottom of the screen...
      if ( icons[f][YPOS] >= (display.height() + 4) ) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
        icons[f][DELTAT] = 0;
        _color[f] = random(BLUE, WHITE);
      }
    }
  }
}

// End of code.
// https://simple-circuit.com/


/***************************************************
  This is a example sketch demonstrating the graphics
  capabilities of the SSD1331 library  for the 0.96"
  16-bit Color OLED with SSD1331 driver chip

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/684

  These displays use SPI to communicate, 4 or 5 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/