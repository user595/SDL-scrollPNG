/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The headers
#include "SDL.h"
#include "SDL_image.h"
#include <string>

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces
SDL_Surface *image = NULL;
SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

Uint32 get_pixel32( SDL_Surface *surface, int x, int y )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    //Get the requested pixel
    return pixels[ ( y * surface->w ) + x ];
}

void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = pixel;
}

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image using SDL_image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );
    }

    //Return the optimized image
    return optimizedImage;
}

SDL_Surface *screen_shift( SDL_Surface* source, int shift_x, int shift_y )
{
	SDL_Surface* shifted = source;

	for (int y = 1; y < SCREEN_HEIGHT; y++ )
	for (int x = 1; x < SCREEN_WIDTH; x++)
	{
		int pixel = get_pixel32( shifted, x, y );
		int mod_x = x;
		int mod_y = y;
		if (x > shift_x && x < (SCREEN_WIDTH) )
			mod_x = x - shift_x;
		if (y > shift_y && y < (SCREEN_HEIGHT) )
			mod_y = y - shift_y;
		if ( (mod_x < 0 || mod_x >= SCREEN_WIDTH) ||
			(mod_y < 0 || mod_y >= SCREEN_WIDTH) )
			continue;
		put_pixel32( shifted, mod_x, mod_y, pixel );

	}

	return shifted;

	SDL_FreeSurface( shifted );

}


void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Rectangle to hold the offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "PNG test", NULL );

    //If everything initialized fine
    return true;
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( image );

    //Quit SDL
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the image
    image = load_image( "look.png" );

    //If there was a problem in loading the image
    if( image == NULL )
    {
        return 1;
    }

    //Apply the surface to the screen
    apply_surface( 0, 0, image, screen );

	//screen = screen_shift( screen, 100,0 );

    //Update the screen
    if( SDL_Flip( screen ) == -1 )
    {
        return 1;
    }

    //Wait 2 seconds
    //SDL_Delay( 200000 );//2000 milliseconds = 2 seconds

	//Quit flag
    bool quit = false;

    //While the user hasn't quit
    while( quit == false )
    {
        //Start the frame timer
        //fps.start();

        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }

		//screen = screen_shift( screen, 1,0 );
		//screen = screen_shift( screen, 0,1 );
		//screen = screen_shift( screen, -1,0 );//negative numbers clear the screen
		screen = screen_shift( screen, 1,1 );

		//Update the screen
		if( SDL_Flip( screen ) == -1 )
		{
			return 1;
		}

		//Wait 2 seconds
		SDL_Delay( 500 );//2000 milliseconds = 2 seconds



	}

    //Free the surface and quit SDL
    clean_up();

    return 0;
}
