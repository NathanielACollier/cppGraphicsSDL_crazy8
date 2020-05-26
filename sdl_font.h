/*
 Nathaniel Collier
 9/20/2007
 Written using tutorials on the internet
 This is a font class for sdl_ttf
*/
#include <string>
#include <stdarg.h>

using namespace std;

#ifndef SDL_FONT_H
#define SDL_FONT_H


#include "rgb_color.h"



class sdl_font
{
   private:
   TTF_Font* font; 
   SDL_Color fgcolor;
   SDL_Color bgcolor; 
   SDL_Surface* textSurface;
   bool font_loaded;
   public:
   

   
   
   sdl_font();
   ~sdl_font();
   int init_sdl_ttf();
   void open_font( string filename, int pt_size );
   void set_fg_color( int red, int green, int blue );
   void set_bg_color( int red, int green, int blue );
   void set_fg_color( rgb_color fg );
   void set_bg_color( rgb_color bg );
   bool ready();
   void draw_text( SDL_Surface *& screen, int x, int y, const char* text, ... );
   
};


//////////// private functions /////////////




//////////// public functions //////////////

/*
 Constructor
*/
sdl_font::sdl_font()
{
   font_loaded = false;
   TTF_Init();
   fgcolor = rgb_to_sdlcolor( RGB_WHITE );
   bgcolor = rgb_to_sdlcolor( RGB_BLACK );
}


/* 
 Destructor
*/
sdl_font::~sdl_font()
{
   SDL_FreeSurface(textSurface);
   TTF_CloseFont(font);
   TTF_Quit();
}


int sdl_font::init_sdl_ttf()
{
  if (TTF_Init() == -1) 
  {
    printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
    return false;
  }
}

/*
  Load a font
*/
void sdl_font::open_font( string filename, int pt_size )
{
  font = TTF_OpenFont( filename.c_str() , pt_size );
  font_loaded = true;
}



void sdl_font::set_fg_color( int red, int green, int blue )
{
   fgcolor = rgb_to_sdlcolor( red,green,blue );
}


void sdl_font::set_bg_color( int red, int green, int blue )
{
   bgcolor = rgb_to_sdlcolor( red,green,blue );
}

void sdl_font::set_fg_color( rgb_color fg )
{
  fgcolor = rgb_to_sdlcolor( fg.red,fg.green,fg.blue );
}

void sdl_font::set_bg_color( rgb_color bg )
{
  bgcolor = rgb_to_sdlcolor( bg.red,bg.green,bg.blue );
}

/*
  If the font is loaded and ready to draw text on the screen then
    this function will return true.
*/
bool sdl_font::ready()
{
  return font_loaded;
}



#define MAX_E 1000000

void sdl_font::draw_text( SDL_Surface *& screen, int x, int y, const char* text, ... )
{
  va_list args;
  char buf[MAX_E];
  
  va_start( args, text );
  vsnprintf( buf, MAX_E, text, args );
  va_end( args );
  
  
  textSurface = TTF_RenderText_Shaded(font, buf, fgcolor, bgcolor );
  
  // height and width of 0 make it display the whole font
  SDL_Rect textLocation = { x,y, 0, 0 }; 
  
  SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
  
  SDL_Flip(screen);
  
}



#endif





