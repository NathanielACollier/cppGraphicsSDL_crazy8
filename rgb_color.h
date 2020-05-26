/*
 Nathaniel Collier
 9/20/2007
 RGB Color definitions
 and a structure to hold the colors
*/


#ifndef RGB_COLOR_H
#define RGB_COLOR_H

#define RGB_RED      255,   0,   0 
#define RGB_GREEN    0,   255,   0 
#define RGB_BLUE     0,     0, 255
#define RGB_WHITE    255, 255, 255
#define RGB_CYAN     0,   255, 255
#define RGB_BLACK    0,     0,   0
#define RGB_YELLOW   255, 255,   0
#define RGB_PURPLE   255,   0, 255

#define RGB_CLEAR	RGB_BLACK


struct rgb_color
{
   int red;
   int green;
   int blue;
   
   rgb_color( int r=0, int g=0, int b=0 )
   {
      red = r;
      green = g;
      blue = b;
   }

   rgb_color  operator=( rgb_color  color )
   {
      red = color.red;
	  green = color.green;
	  blue = color.blue;
	  return color;
   }
};


SDL_Color rgb_to_sdlcolor( int red, int green, int blue )
{
  SDL_Color c;
  c.r = red;
  c.g = green;
  c.b = blue;
  return c;
}



#endif
















