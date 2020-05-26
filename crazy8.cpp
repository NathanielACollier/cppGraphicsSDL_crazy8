

/*
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
*/

// This stuff is for compiling with visual studio which is how the Windows Release of the game will need to be compiled
//      To produce the smallest executable possible
//winsock library
#pragma comment(lib, "ws2_32")
// Main SDL Libraries
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL.lib")
// Addons to SDL Library
#pragma comment(lib, "SDL_ttf.lib")
#pragma comment(lib, "SDL_net.lib")
#pragma comment(lib, "sdlgfx.lib")


/*
Nathaniel Collier
9/20/2007
SDL Test program without using opengl
  I want this program to use ttf fonts be able to load bitmaps and draw things on the screen
  and maybe some 2d rotation

  This project attempts to maintain compatibility with a windows version of the software.
  It will likely be opensource when it is released possibly.
*/




#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_net.h>

#include <SDL/SDL_gfxPrimitives.h>



//#include <unistd.h> // linux network stuff
#include <Winsock2.h> // windows network stuff


#include "sdl_font.h"

#include "card_type.h"

#include "rectangle.h"


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define CARD_W 71
#define CARD_H 96



vector<string> picture_list;
vector<card_type> card_list;
int pic_count = 0;

sdl_font arialblk_8pt;
sdl_font arialblk_12pt;

int last_mouse_x = 0,
     last_mouse_y = 0;

 rectangle_t mouse_loc( rect_dim_t( 600, 10, 60,10 ) ); // find out why this doesn't work right

 rectangle_t status_bar( rect_dim_t(0,580,500,20) );
 rectangle_t card_info( rect_dim_t(0,12,100,100) );
 rectangle_t top_card( rect_dim_t( 366,249,71,96 ) );
 rectangle_t main_window( rect_dim_t( 0,0,800,600 ) );

 rectangle_t player3_cards( rect_dim_t( 223,3,355,191 ) );




#define fill_rect(surface,x,y,width,height,rgb_color) boxRGBA( surface, x,y,x+width,y+height,rgb_color,255)
#define draw_rect(surface,x,y,width,height,rgb_color) rectangleRGBA( surface,x,y,x+width,y+height,rgb_color,255)



vector<card_type> generate_cards( int min, int max );
vector<string> load_file_list_from_file( string filename );
void draw_with_SDL_gfx( SDL_Surface*& screen );
int init_sdl( Uint32, SDL_Surface*&  );
int process_events(SDL_Event& event, SDL_Surface*& screen );
void quit();
char** resolve_hostname();

/*
Should be able to get rid of draw_bitmap and put it in the rectangle class pretty easily
*/
void draw_bitmap( SDL_Surface *& screen, string filename, int x, int y );


void display_card_info( SDL_Surface *& screen );


string prints( const char* text, ... );



int main ( int argc, char *argv[] )
{

  SDL_Event event;
  int gameover = 0;
  char** ip_host;
  SDL_Surface * screen;




  mouse_loc.load_font( "arialblack.ttf", 8, rgb_color(RGB_WHITE), rgb_color(RGB_BLACK) );
  status_bar.load_font( "arialblack.ttf" );
  card_info.load_font( "arialblack.ttf", 8, rgb_color(RGB_WHITE),rgb_color(RGB_BLACK) );


  init_sdl( SDL_DOUBLEBUF /*| SDL_FULLSCREEN */, screen);


  arialblk_8pt.open_font( "arialblack.ttf", 8 );


  arialblk_12pt.open_font( "arialblack.ttf", 12 );



  ip_host = resolve_hostname();

  status_bar.fill(screen,rgb_color(RGB_BLUE));
  mouse_loc.fill(screen,rgb_color(RGB_BLACK));

  char localhostname[256];


     picture_list = load_file_list_from_file( "pic_list.txt" );

   card_list = generate_cards(0,54);

   cout << "\nLoaded: " << card_list.size() << " cards" << endl;

   cout << "\nLoaded: " << picture_list.size() << " pictures" << endl;


  draw_with_SDL_gfx( screen );



      draw_bitmap( screen, "image/image.bmp", 0 , 0 );
    draw_bitmap( screen, "image/image.bmp", 128,128);
	draw_bitmap( screen, "image/image.bmp", 256,256);
	draw_bitmap( screen, "image/linux.bmp", 20, 300 );


   main_window.draw_bitmap( screen, "main_window.bmp" );

	 top_card.draw_bitmap( screen, picture_list[pic_count] );

	 // just drew a card set the cards coordinates
	 if( card_list.size() > pic_count  && pic_count >= 0 )
	 {
	   coordinate_t top_left = top_card.get_top_left();

       card_list[pic_count].set_coordinates( top_left.x, top_left.y );
     }


  gethostname( localhostname, 256 );

  printf( "\nLocal host name: %s", localhostname );


           arialblk_12pt.set_fg_color( RGB_YELLOW );
           arialblk_12pt.draw_text( screen, 600, 70 , "HOST: %s", ip_host[0] );

           arialblk_12pt.set_fg_color( RGB_GREEN );
           arialblk_12pt.draw_text( screen, 600, 100, "IP: %s", ip_host[1] );


  // main game loop
  while (!gameover)
  {

    gameover = process_events(event,screen);

  }


  return 0;
}



char** resolve_hostname()
{
  char localhostname[256];
  char** ip_host;
  Uint8 *ipaddr;
  IPaddress ip;


  //ip_host = new char[2][256];

  ip_host = new char* [2];

  ip_host[0] = new char [256];
  ip_host[1] = new char [256];





  //if( gethostname(localhostname, 256) > 0 )  // linux
  if( gethostname(localhostname, 256) >= 0 ) // windows
  {
    sprintf( ip_host[0], "%s", localhostname );

    if( SDLNet_ResolveHost( &ip, localhostname, 0 ) != -1 )
    {
      // turns the ip into an array of the octets
      ipaddr= (Uint8*) &ip.host;
      sprintf( ip_host[1], "%d.%d.%d.%d", ipaddr[0],ipaddr[1],ipaddr[2],ipaddr[3]);

    }

  }
  return ip_host;
}



void quit()
{
  SDLNet_Quit();
  SDL_Quit();
}



int init_sdl( Uint32 flags, SDL_Surface*& screen )
{

  // load sdl
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
     printf( "Unable to initialize SDL: %s\n", SDL_GetError());
     exit(1);
  }

  if(SDLNet_Init()==-1) {
    printf("SDLNet_Init: %s\n", SDLNet_GetError());
    exit(2);
  }

  atexit(quit); // this will call the sdl quit function when the program exits

  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, flags);

  if (screen == NULL)
  {
    printf( "Unable to set video mode: %s\n", SDL_GetError());
    return false;
  }

  /*
    There are other SDL_WM functions so you might want to look that up in the sdl
      reference manual
  */
  SDL_WM_SetCaption("NAC SDL Application","Icon Title");

  /*
   Clear the entire screen to black
  */
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, RGB_BLACK ));

}







int process_events(SDL_Event& event, SDL_Surface*& screen )
{
  int gameover = 0;
  bool screen_change = false;

    /* look for an event */
    if (SDL_PollEvent(&event))
    {

      /* an event was found */
      switch (event.type)
      {
        /* mouse button released */
	      case SDL_MOUSEBUTTONUP:


		    break;
	      /* mouse button pressed */
		    case SDL_MOUSEBUTTONDOWN:
                /* Display a message when the user clicks in the card box */
				status_bar.clear(screen); // clear whatever text was last displayed


				if( top_card.check_point( event.button.x,event.button.y)   )
				{
				
					status_bar.draw_text( screen, "Mouse clicked in player cards" );

				}
				else
				if( 1 )
				{
				
				}




		    break;
	      /* mouse moved */
		    case SDL_MOUSEMOTION:

			   mouse_loc.fill(screen, rgb_color(RGB_BLUE) );
              // mouse_loc.clear(screen); // clear previous mouse position
			   mouse_loc.draw_text( screen, prints( "Mouse x:%d y:%d", event.motion.x, event.motion.y) );


		        if( top_card.check_point( event.motion.x, event.motion.y ) )
				{
					status_bar.draw_text( screen,  "Mouse Within Top Card" );

					// now that we know the mouse is hoovering over a player card lets display information about the card.
					display_card_info( screen  );
				}
				else if( player3_cards.check_point( event.motion.x, event.motion.y ) )
				{
                    status_bar.draw_text( screen, "Mouse in Player 3 Card Section" );
				}
				else
				{
					status_bar.fill( screen ); // clear out the status bar
				}


           // this is for commands that depend only on the mouse being in specific coordinates
           // but not necesarily having to be moving
		   last_mouse_x = event.motion.x;
		   last_mouse_y = event.motion.y;


		  break;
        /* close button clicked */
        case SDL_QUIT:
          gameover = 1;
          break;

        /* handle the keyboard */
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym)
          {
            case SDLK_LEFT:
              pic_count = ((pic_count == 0) ? picture_list.size() - 1 : pic_count - 1);

               top_card.draw_bitmap( screen, picture_list[pic_count] );

                if( top_card.check_point(last_mouse_x,last_mouse_y ) )
                {
                  status_bar.draw_text( screen,  "Mouse Within Player Cards" );

                  // now that we know the mouse is hoovering over a player card lets display information about the card.
                  display_card_info( screen  );
                }
                else
                {
                  status_bar.fill( screen ); // clear out the status bar
                }

                break;
            case SDLK_RIGHT:
              pic_count = ((pic_count == picture_list.size() - 1) ? 0 : pic_count + 1);


              top_card.draw_bitmap( screen, picture_list[pic_count] );


              if( top_card.check_point( last_mouse_x, last_mouse_y ) )
				{
					status_bar.draw_text( screen,  "Mouse Within Player Cards" );

					// now that we know the mouse is hoovering over a player card lets display information about the card.
					display_card_info( screen  );
				}
				else
				{
					status_bar.fill( screen ); // clear out the status bar
				}

            break;
            case SDLK_ESCAPE:
            case SDLK_q:
              gameover = 1;
              break;
            case SDLK_f:
              /* F1 key was pressed
               * this toggles fullscreen mode
               */
              SDL_WM_ToggleFullScreen( screen );
              break;
          }
          break;
      }
    }


  return gameover;
}









void draw_with_SDL_gfx( SDL_Surface*& screen )
{
       pixelRGBA(screen,
                10, 15,
                255, 255, 255, 255);

      lineRGBA(screen,
               20, 10,
               70, 90,
               255, 0, 0, 255);

      trigonRGBA(screen,
                 500, 50,
                 550, 200,
                 600, 150,
                 0, 255, 255, 255);

      filledTrigonRGBA(screen,
                       200, 200,
                       300, 50,
                       400, 200,
                       0, 0, 255, 255);

      rectangleRGBA(screen,
                    -10, 300,
                    100, 380,
                    0, 255, 0, 255);

      boxRGBA(screen,
              210, 76,
              325, 300,
              255, 0, 0, 150);

      ellipseRGBA(screen,
                  600, 400,
                  50, 90,
                  255, 255, 0, 200);

      filledEllipseRGBA(screen,
                        600, 400,
                        25, 150,
                        0, 255, 0, 255);
}


void draw_bitmap( SDL_Surface *& screen, string filename, int x, int y )
{
  SDL_Surface *image,
              *temp;
  SDL_Rect src,dest;

  temp = SDL_LoadBMP(filename.c_str() );
  if( temp == NULL )
  {
    printf( "Unable to Load Bitmap: %s\n", SDL_GetError());
    return;
  }

  image = SDL_DisplayFormat(temp);
  SDL_FreeSurface(temp);

  src.x = 0;
  src.y = 0;
  src.w = image->w;
  src.h = image->h;

  dest.x = x;
  dest.y = y;
  dest.w = image->w;
  dest.h = image->h;

  SDL_BlitSurface( image, &src, screen, &dest );
  SDL_FreeSurface( image );

  SDL_Flip(screen);

}


vector<string> load_file_list_from_file( string filename )
{
  ifstream fin(filename.c_str());
  string line;
  vector<string> temp;

  if( !fin )
  {
     cout << "\nError occured loading picture file\"" << filename << "\".";
	 exit(0);
  }

  while( getline(fin,line) )
  {
    if( line.size() > 1 )
    temp.push_back( line );
  }

  return temp;
}


vector<card_type> generate_cards( int min, int max )
{
  vector<card_type> temp;
  card_type x;
  int cmin=0,cmax=0;
  if( max > 54 || max <= min ) // 52 + 2 jokers
  {
    cout << "\nBad Max | min to 54 |";
	exit(0);
  }

  if( min < 0 || min >= max )
  {
    cout << "\nBad Min | 0 to max |";
	exit(0);
  }

  // 4 suits 1 to 4
  // 14 faces 1 to 14


  for( int face = 1; face <= 14; ++face )
  {
    for( int suit = 1; suit <= 4; ++suit )
	{
      if( cmin == min )
	  {
        x.create_card( suit,face);
        temp.push_back( x );
	  }
	  else
	  {
        ++cmin;
	  }

	  if( cmax == max )
	  {
        return temp;
	  }
	  else
	  {
        ++cmax;
	  }
	}
  }
  return temp;
}







// needed for variable length arguments in the next function
#include <stdarg.h>

#define MAX_E 1000000


/*
 This function allows printing directly to a string like using
 sprintf to a character array very usefull function
 */
string prints( const char* text, ... )
{
  va_list args;
  char buf[MAX_E];

  va_start( args, text );
  vsnprintf( buf, MAX_E, text, args );
  va_end( args );

  return buf;
}




/*
  Displays all the information for the card inside a box
  */
void display_card_info( SDL_Surface *& screen )
{
  const int card_num = pic_count;

   if( card_num > card_list.size() )
   {
      // might want to indicate some kind of error message
     return; // the picture is not a card
   }



   card_info.fill( screen  );

   card_info.draw_text( screen, 0, 0*(8+2), prints("Pos: %d", card_num )  );
   card_info.draw_text( screen, 0, 1*(8+2), "Name: " + card_list[card_num].name );
   card_info.draw_text( screen, 0, 2*(8+2), "ABV Name: " + card_list[card_num].abv_name  );
   card_info.draw_text( screen, 0, 3*(8+2), "Face: " + card_list[card_num].face_to_string() );
   card_info.draw_text( screen, 0, 4*(8+2), "Suit: " + card_list[card_num].suit_to_string() );
   card_info.draw_text( screen, 0, 5*(8+2), prints("Value: %d", card_list[card_num].value )  );
   card_info.draw_text( screen, 0, 6*(8+2), prints("Coordinates: (%d,%d)",
	                                                   card_list[card_num].x,
													   card_list[card_num].y)   );



}








