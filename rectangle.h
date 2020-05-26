#pragma once
/*
Nathaniel Collier
12/15/2007
This file contains the dim_type class
*/
#include "rgb_color.h"
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>



struct coordinate_t
{
  int x;
  int y;
  int z;

  // default constructor with an initalization list
  coordinate_t( int x_=0, int y_=0, int z_=0 ) : x(x_), y(y_), z(z_) {}

};

struct rect_dim_t
{ 
  int x;
  int y;
  int width;
  int height;

  rect_dim_t( int x_=0, int y_=0, int w_=0, int h_=0 ) : x(x_), y(y_), width(w_), height(h_) {}

};

struct rect_coord_t
{
  int top_right_xpos;
  int top_right_ypos;
  int bottom_left_xpos;
  int bottom_left_ypos;

  rect_coord_t( int u_r_x=0, int u_r_y=0, int b_l_x=0, int b_l_y=0 ) : top_right_xpos( u_r_x ),
	                                                                   top_right_ypos( u_r_y ),
                                                                       bottom_left_xpos( b_l_x ),
																	   bottom_left_ypos( b_l_y ) {}

};




class rectangle_t
{
  private:
     
     rect_dim_t rect_dim;
     rect_coord_t rect_coord;

	 rgb_color foreground; // this is the color used when drawing a rectangle
	 rgb_color background; // this is the color used when filling a rectangle
	 sdl_font font;
  public:
     rectangle_t( rect_coord_t c );
     rectangle_t( rect_dim_t d );
	 void common_init();
	 void load_font( string filename, int size, rgb_color foreground_, rgb_color background_ );
     SDL_Rect get_sdl_rect();
	 void draw( SDL_Surface *& screen, rgb_color color );
	 void fill( SDL_Surface *& screen, rgb_color color );
	 void draw_text( SDL_Surface *& screen, string text, rgb_color text_color );
	 void draw_text( SDL_Surface *& screen, int x_offset, int y_offset, string text, rgb_color text_color );
	 void draw_bitmap( SDL_Surface *& screen, string filename, int x_scale, int y_scale );
	 void clear( SDL_Surface *& screen );
	 void set_coordinates( int u_r_x, int u_r_y, int b_l_x, int b_l_y );
     void set_dimensions( int x_, int y_, int w_, int h_ );
	 bool check_point( int x, int y );
	 coordinate_t get_center();
	 coordinate_t get_top_left(); // gives the top left corner
	 void hide( SDL_Surface *& screen );
	 void show( SDL_Surface *& screen );

};


/////////////// Private Meathods ///////////////




/////////////// Public Meathods ////////////////

// Constructors
rectangle_t::rectangle_t( rect_coord_t c )
{
	set_coordinates( c.top_right_xpos,c.top_right_ypos,c.bottom_left_xpos,c.bottom_left_ypos );
	set_dimensions(c.top_right_xpos,c.top_right_ypos,(c.bottom_left_xpos-c.top_right_xpos),(c.bottom_left_ypos-c.top_right_ypos));

	common_init();
}

rectangle_t::rectangle_t( rect_dim_t d )
{
	set_coordinates(d.x,d.y,(d.width+d.x),(d.height+d.y));
	set_dimensions(d.x,d.y,d.width,d.height);

	common_init();
}

void rectangle_t::common_init()
{
  
  
}


// Destructors


// Other Public Meathods

/*
  Not all rectangles will need a font so we load it for those that do
     uses default arguments incase just filename and size are used
*/
void rectangle_t::load_font( string filename, int size=0, rgb_color foreground_=rgb_color(RGB_WHITE), rgb_color background_=rgb_color(RGB_BLACK) )
{
  if( size < 8 )
  {
    // calculate the size based on the height of the box since
	  // either the user did not specify a size or the size was too small to be displayed and it look right
    size = rect_dim.height - 6;
	if( size < 8 ) // if its still less than 8 after this then there is going to be a problem with how small the box is
	{
      size = 8; // for now lets just set it to be 8 
	}
  }
  foreground = foreground_;
  background = background_;

  font.open_font( filename.c_str(), size );
  font.set_fg_color( foreground );
  font.set_bg_color( background );
}

SDL_Rect rectangle_t::get_sdl_rect()
{
  SDL_Rect sdl_rectangle;

  sdl_rectangle.x = rect_dim.x;
  sdl_rectangle.y = rect_dim.y;
  sdl_rectangle.w = rect_dim.width;
  sdl_rectangle.h = rect_dim.height;
  
  return sdl_rectangle;
}


/*
  Draw the outline of the rectangle
*/
void rectangle_t::draw( SDL_Surface *& screen, rgb_color color=rgb_color(-1,-1,-1) )
{
	if( color.red < 0 )
	{
      color = foreground;
	}
	else
	{
     foreground = color; // save this color for latter use
	}
	rectangleRGBA( screen,rect_coord.top_right_xpos,rect_coord.top_right_ypos,
	                      rect_coord.bottom_left_xpos,rect_coord.bottom_left_ypos,
	                      color.red,color.green,color.blue,255);
	SDL_Flip(screen);
}

/*
 fill the rectangle with a color
*/
void rectangle_t::fill( SDL_Surface *& screen, rgb_color color=rgb_color(-1,-1,-1) )
{
	if( color.red < 0 )
	{
       color = background;
	}
	else
	{
	   background = color; // save this color so we can use it latter
	}
	boxRGBA( screen,rect_coord.top_right_xpos,rect_coord.top_right_ypos,
	                      rect_coord.bottom_left_xpos,rect_coord.bottom_left_ypos,
	                      color.red,color.green,color.blue,255);
	SDL_Flip(screen);
}

/*
 This function should clear the rectangle with the clear color
 */
void rectangle_t::clear( SDL_Surface *& screen )
{
	fill( screen, rgb_color( RGB_CLEAR ) );
}

/*
  Draw text inside the rectangles
*/
void rectangle_t::draw_text( SDL_Surface *& screen , string text , rgb_color text_color=rgb_color(-1,-1,-1) )
{
	if( font.ready() == false ) return;

	if( text_color.red < 0 )
	{
      text_color = foreground;
	}
	// might want to do some calculations to make sure text stays in the box
    // but for now we will leave it up to the person that calls the function

	font.set_fg_color(text_color.red,text_color.green,text_color.blue);
	font.set_bg_color(background.red,background.green,background.blue);
	font.draw_text( screen, rect_dim.x,rect_dim.y,text.c_str());
}


/*
  Draw text offset allows for text to be positioned within the rectangle
*/
void rectangle_t::draw_text( SDL_Surface *& screen, int x_offset, int y_offset, string text, rgb_color text_color=rgb_color(-1,-1,-1) )
{
  if( font.ready() == false ) return;
  if( text_color.red < 0 )
  {
    text_color = foreground;
  }

  // might want to do some calculations to make sure text stays in the box
  // but for now we will leave it up to the person that calls the function
  	font.set_fg_color(text_color.red,text_color.green,text_color.blue);
	font.set_bg_color(background.red,background.green,background.blue);
	// this is the same as the draw_text function except for right here we add in the offsets
	font.draw_text( screen, rect_dim.x + x_offset ,rect_dim.y + y_offset ,text.c_str());

}



/*
 Draws a picture inside the rectangle
    This function receives a scalling factor which tells it how much to adjust the
	 position of the image from the top right of the box in the x and y directions

	uses default arguments so that the image will be in position (0,0) of the box
	if no scalling factors are sent
*/
void rectangle_t::draw_bitmap( SDL_Surface *& screen, string filename, int x_scale=0, int y_scale=0 )
{
	int image_x_pos=rect_dim.x + x_scale,
		image_y_pos=rect_dim.y + y_scale;
    
	SDL_Surface *image;
	SDL_Rect src,dest;

   
    if( !check_point( image_x_pos , image_y_pos )  )
	{
       // the scalling factors are not valid so do something
	   // produce error (Possibly adjust scalling factor to try and make it work)

	}
    

	// so far so good lets load the image and see what happens
	image = SDL_LoadBMP(filename.c_str() );

	if( !image )
	{
       // can't load the image something is probably wrong with the filename 
	   // produce error
	}

	image = SDL_DisplayFormat( image );

	// check the height and width of the image to see if it will fit in the box
	if( !check_point( image_x_pos + image->w,image_y_pos + image->h ) )
	{
      // the lower right point of the image is not in the box so it won't fit
	  // produce error (Possible scalling of image would fix this )
	  
	}

	src.x = 0;
    src.y = 0;
    src.w = image->w;
    src.h = image->h;
  
    dest.x = image_x_pos;
    dest.y = image_y_pos;
    dest.w = image->w;
    dest.h = image->h;

	SDL_BlitSurface( image, &src, screen, &dest );
    SDL_FreeSurface( image );
  
    SDL_Flip(screen);

}


void rectangle_t::set_coordinates( int u_r_x, int u_r_y, int b_l_x, int b_l_y )
{
     rect_coord.top_right_xpos = u_r_x;
	 rect_coord.top_right_ypos = u_r_y;
	 rect_coord.bottom_left_xpos = b_l_x;
	 rect_coord.bottom_left_ypos = b_l_y;
}

void rectangle_t::set_dimensions( int x_, int y_, int w_, int h_ )
{
     rect_dim.x = x_;
	 rect_dim.y = y_;
	 rect_dim.width = w_;
	 rect_dim.height = h_;
}



/*
  Check Point
  Determines if the point  (x,y) is located inside the rectangle on the coordinate plane
*/
bool rectangle_t::check_point( int x, int y )
{
  bool x_range = false,
	   y_range = false;

  // is the x coordinate in range
  if( x >= rect_dim.x && x <= (rect_dim.x + rect_dim.width)  )
	   x_range = true;
 
  if( y >= rect_dim.y && y <= (rect_dim.y + rect_dim.height)  )
	  y_range = true;

  // since both have to be true for the point to be inside the rectangle
  // we do this
  return x_range && y_range;
}

/*
 Might be usefull at some point to find the center point of a rectangle
*/
coordinate_t rectangle_t::get_center()
{
  coordinate_t center();

  // There may not be an easy way to do this since the resulting coordinates would be floats in
  // many cases
  //  x= x/2;  y=y/2;  z=z/2;  This should be right but as you see it may result in a floating point
  //                           coordinate which this class is not setup to do yet.
  

}


coordinate_t rectangle_t::get_top_left()
{
  return coordinate_t( rect_dim.x,rect_dim.y,0);
}

/*
 This will save a copy of the rectangular section of the image
 then call the clear meathod to clear the image off the screen
 */
void rectangle_t::hide( SDL_Surface *& screen )
{
  // need to figure out some kind of sdl function to save sections of the screen as bmp so that we can redisplay it
  // when the show function is executed
  SDL_SaveBMP( screen, "screen.bmp");
}

/*
 This will write the saved image back to the screen
 */
void rectangle_t::show( SDL_Surface *& screen)
{

}


