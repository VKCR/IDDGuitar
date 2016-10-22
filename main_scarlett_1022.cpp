/*
 * This code generate the figure for the projector to display
 * 1. display six strings(white line) and six frets(yellow line) for use to calibrate; parameters being modified in code
 * 2. detect keyboard events to get which chord user want to learn when user pressing the corresponding key
 * 3. display light blue rectangulars indicating the points to press of the chord
 * this code refers to Lazy Foo' Tutorial: 
 * http://lazyfoo.net/tutorials/SDL/08_geometry_rendering/index.php
 * and
 * http://lazyfoo.net/tutorials/SDL/18_key_states/index.php
 */

//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>

//Screen dimension constants
const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 700;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

/*
 * following functions are for guitar projecter display
 */
/*
 * display the 6 trings with calicration parameters
 * width - half of the distance between middle two strings
 * short - the shrinking side
 * long - the streching side
 */
int disp_strings(int short_width, int long_width)
{
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
	SDL_RenderClear( gRenderer );

	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );		
	SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2 - short_width * 5, SCREEN_WIDTH, SCREEN_HEIGHT / 2 - long_width * 5);
	SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2 - short_width * 3, SCREEN_WIDTH, SCREEN_HEIGHT / 2 - long_width * 3);
	SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2 - short_width, SCREEN_WIDTH, SCREEN_HEIGHT / 2 - long_width);
	SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2 + short_width, SCREEN_WIDTH, SCREEN_HEIGHT / 2 + long_width);
	SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2 + short_width * 3, SCREEN_WIDTH, SCREEN_HEIGHT / 2 + long_width * 3);
	SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2 + short_width * 5, SCREEN_WIDTH, SCREEN_HEIGHT / 2 + long_width * 5);

	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
	for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
	{
		SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 7, i );
		SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 7 * 2, i );
		SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 7 * 3, i );
		SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 7 * 4, i );
		SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 7 * 5, i );
		SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 7 * 6, i );
	}

	SDL_RenderPresent( gRenderer );

	return 0;
}

/*
 * generate the vertical coordinate on the screen 
 * given the string and fret number, and the calibration parameter
 * from top to bottom in the display window: string number 1 - 6
 * from right to left in the display window: fret number 1 - 6
 * the 6 frets(yeallow dotted line is the 6 horizontal lines in guitar chords note)
 */
int y_coordinate (int string_num, int fret_num, int short_width, int long_width)
{
	int y;
	fret_num = 7 - fret_num;
	string_num = 7 - string_num;
	switch(string_num) 
	{
	case 1:
		y = SCREEN_HEIGHT / 2 - short_width * 5 - (long_width * 5 - short_width * 5) * fret_num / 7;
		break;
	case 2:
		y = SCREEN_HEIGHT / 2 - short_width * 3 - (long_width * 3 - short_width * 3) * fret_num / 7;
		break;
	case 3:
		y = SCREEN_HEIGHT / 2 - short_width - (long_width - short_width) * fret_num / 7;
		break;
	case 4:
		y = SCREEN_HEIGHT / 2 + short_width + (long_width - short_width) * fret_num / 7;
		break;
	case 5:
		y = SCREEN_HEIGHT / 2 + short_width * 3 + (long_width * 3 - short_width * 3) * fret_num / 7;
		break;
	case 6:
		y = SCREEN_HEIGHT / 2 + short_width * 5 + (long_width * 5 - short_width * 5) * fret_num / 7;
		break;
	default:
		y = 0;
	}
	return y;
}

/*
 * display a rectangular point to indicate the point to press
 * for now we display the frets in certain width
 */
int x_coordinate (int fret_num)
{
	return (7 - fret_num) *  SCREEN_WIDTH / 7;
}

/*
 * display a rectangular point to indicate the point to press
 * given the coordinates
 */
int disp_point( int x, int y)
{
    SDL_Rect fillRect = {x, y, SCREEN_HEIGHT / 40, SCREEN_HEIGHT / 40 };
    SDL_SetRenderDrawColor( gRenderer, 135, 206, 250, 0xFF );        
    SDL_RenderFillRect( gRenderer, &fillRect );
	
	return 0;
}

enum Chords
{
    C, E, G, B, 
	Cm, Em, Gm, Bm, 
	C7, E7, G7, B7, 
	D, F, A,
	Dm, Fm, Am, 
	D7, F7, A7
};

/*
 * display a chord pressing points
 * given the chord number, and calicration parameters
 */
int disp_chords( int chords, int short_width, int long_width )
{
	switch(chords)
	{
		case C :
			disp_point( x_coordinate(2), y_coordinate(2, 2, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(4, 3, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(5, 4, short_width, long_width));
			break;
		
		case E :
			disp_point( x_coordinate(2), y_coordinate(3, 2, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(4, 3, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(5, 3, short_width, long_width));
			break;

		case G :
			disp_point( x_coordinate(4), y_coordinate(1, 4, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(5, 3, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(6, 4, short_width, long_width));
			break;

		case B :
			disp_point( x_coordinate(3), y_coordinate(1, 3, short_width, long_width));
			disp_point( x_coordinate(5), y_coordinate(2, 5, short_width, long_width));
			disp_point( x_coordinate(5), y_coordinate(3, 5, short_width, long_width));
			disp_point( x_coordinate(5), y_coordinate(4, 5, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(5, 3, short_width, long_width));
			break;

		case Cm :
			disp_point( x_coordinate(4), y_coordinate(1, 4, short_width, long_width));
			disp_point( x_coordinate(5), y_coordinate(2, 5, short_width, long_width));
			disp_point( x_coordinate(5), y_coordinate(2, 5, short_width, long_width));
			disp_point( x_coordinate(6), y_coordinate(3, 6, short_width, long_width));
			disp_point( x_coordinate(6), y_coordinate(4, 6, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(5, 4, short_width, long_width));
			break;

		case Em :
			disp_point( x_coordinate(3), y_coordinate(4, 3, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(5, 3, short_width, long_width));
			break;

		case Bm :
			disp_point( x_coordinate(4), y_coordinate(1, 4, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(2, 4, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(3, 4, short_width, long_width));
			disp_point( x_coordinate(6), y_coordinate(4, 6, short_width, long_width));
			disp_point( x_coordinate(6), y_coordinate(5, 6, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(6, 4, short_width, long_width));
			break;

		case Gm :
			disp_point( x_coordinate(3), y_coordinate(1, 3, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(2, 4, short_width, long_width));
			disp_point( x_coordinate(5), y_coordinate(3, 5, short_width, long_width));
			disp_point( x_coordinate(5), y_coordinate(4, 5, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(5, 3, short_width, long_width));
			break;

		case C7 :
			disp_point( x_coordinate(2), y_coordinate(2, 2, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(3, 4, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(4, 3, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(5, 4, short_width, long_width));
			break;

		case E7 :
			disp_point( x_coordinate(2), y_coordinate(3, 2, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(5, 3, short_width, long_width));
			break;

		case B7 :
			disp_point( x_coordinate(2), y_coordinate(1, 2, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(5, 3, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(6, 4, short_width, long_width));
			break;

		case G7 :
			disp_point( x_coordinate(3), y_coordinate(1, 3, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(3, 3, short_width, long_width));
			disp_point( x_coordinate(2), y_coordinate(4, 2, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(5, 3, short_width, long_width));
			break;

		case D :
			disp_point( x_coordinate(3), y_coordinate(1, 3, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(2, 4, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(3, 3, short_width, long_width));
			break;

		case F :
			disp_point( x_coordinate(2), y_coordinate(1, 2, short_width, long_width));
			disp_point( x_coordinate(2), y_coordinate(2, 2, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(3, 3, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(4, 4, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(5, 4, short_width, long_width));
			disp_point( x_coordinate(2), y_coordinate(6, 2, short_width, long_width));
			break;

		case A :
			disp_point( x_coordinate(3), y_coordinate(3, 2, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(3, 2, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(3, 3, short_width, long_width));
			break;

		case Dm :
			disp_point( x_coordinate(2), y_coordinate(1, 2, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(2, 4, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(3, 3, short_width, long_width));
			break;

		case Fm :
			disp_point( x_coordinate(2), y_coordinate(1, 2, short_width, long_width));
			disp_point( x_coordinate(2), y_coordinate(2, 2, short_width, long_width));
			disp_point( x_coordinate(2), y_coordinate(3, 2, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(4, 4, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(5, 4, short_width, long_width));
			disp_point( x_coordinate(2), y_coordinate(6, 2, short_width, long_width));
			break;

		case Am :
			disp_point( x_coordinate(2), y_coordinate(2, 2, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(3, 3, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(4, 3, short_width, long_width));
			break;

		case D7 :
			disp_point( x_coordinate(3), y_coordinate(1, 3, short_width, long_width));
			disp_point( x_coordinate(2), y_coordinate(2, 2, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(3, 3, short_width, long_width));
			break;

		case F7 :
			disp_point( x_coordinate(2), y_coordinate(1, 2, short_width, long_width));
			disp_point( x_coordinate(2), y_coordinate(2, 2, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(3, 3, short_width, long_width));
			disp_point( x_coordinate(2), y_coordinate(4, 2, short_width, long_width));
			disp_point( x_coordinate(4), y_coordinate(5, 4, short_width, long_width));
			disp_point( x_coordinate(2), y_coordinate(6, 2, short_width, long_width));
			break;

		case A7 :
			disp_point( x_coordinate(3), y_coordinate(2, 3, short_width, long_width));
			disp_point( x_coordinate(3), y_coordinate(4, 3, short_width, long_width));
			break;

		default:
			break;
}
	SDL_RenderPresent( gRenderer );
	return 0;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					/*
					 * guitar controller
					 */
					int short_width = SCREEN_HEIGHT / 40, long_width = SCREEN_HEIGHT / 10;
					disp_strings(short_width, long_width);
					/* display the guitar chords while user pressing the corresponding key
					 * A: C; B: E; C: G; D: B; 
					 * E: Cm; F: Em; G: Gm; H: Bm; 
					 * I: C7; J: E7; K: G7; L: B7; 
					 * M: D; N: F; O: A; 
					 * P: Dm; Q: Fm; R: Am; 
					 * S: D7; T: F7; U: A7;
					 */
					const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
					if( currentKeyStates[ SDL_SCANCODE_A ] )
					{
						disp_chords(C, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_B ] )
					{
						disp_chords(E, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_C ] )
					{
						disp_chords(G, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_D ] )
					{
						disp_chords(B, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_E ] )
					{
						disp_chords(Cm, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_F ] )
					{
						disp_chords(Em, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_G ] )
					{
						disp_chords(Gm, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_H ] )
					{
						disp_chords(Bm, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_I ] )
					{
						disp_chords(C7, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_J ] )
					{
						disp_chords(E7, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_K ] )
					{
						disp_chords(G7, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_L ] )
					{
						disp_chords(B7, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_M ] )
					{
						disp_chords(D, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_N ] )
					{
						disp_chords(F, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_O ] )
					{
						disp_chords(A, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_P ] )
					{
						disp_chords(Dm, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_Q ] )
					{
						disp_chords(Fm, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_R ] )
					{
						disp_chords(Am, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_S ] )
					{
						disp_chords(D7, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_T ] )
					{
						disp_chords(F7, short_width, long_width);
					}
					else if( currentKeyStates[ SDL_SCANCODE_U ] )
					{
						disp_chords(A7, short_width, long_width);
					}
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}