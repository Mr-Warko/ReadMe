#include <err.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.c"

/* Le tout effectue un binarize (supprime les couleurs) sur une image au format
	 bmp donnée en argument. */



/*======================================*/
/*==============Utilitaire==============*/
/*======================================*/


void wait_for_keypressed(void) {
  SDL_Event             event;
  // Infinite loop, waiting for event
  for (;;) {
    // Take an event
    SDL_PollEvent( &event );
    // Switch on event type
    switch (event.type) {
    // Someone pressed a key -> leave the function
    case SDL_KEYDOWN: return;
    default: break;
    }
  // Loop until we got the expected event
  }
}

void init_sdl(void) {
  // Init only the video part
  if( SDL_Init(SDL_INIT_VIDEO)==-1 ) {
    // If it fails, die with an error message
    errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
  }
  // We don't really need a function for that ...
}

SDL_Surface* load_image(char *path) {
  SDL_Surface          *img;
  // Load an image using SDL_image with format detection
  img = IMG_Load(path);
  if (!img)
    // If it fails, die with an error message
    errx(3, "can't load %s: %s", path, IMG_GetError());
  return img;
}

SDL_Surface* display_image(SDL_Surface *img) {
  SDL_Surface          *screen;
  // Set the window to the same size as the image
  screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
  if ( screen == NULL ) {
    // error management
    errx(1, "Couldn't set %dx%d video mode: %s\n",
         img->w, img->h, SDL_GetError());
  }
 
  /* Blit onto the screen surface */
  if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
    warnx("BlitSurface error: %s\n", SDL_GetError());
 
  // Update the screen
  SDL_UpdateRect(screen, 0, 0, img->w, img->h);
 
  // wait for a key
  wait_for_keypressed();
 
  // return the screen for further uses
  return screen;
}


/*======================================*/
/*=============grayScale================*/
/*======================================*/


Uint8 grayscale(Uint8 r, Uint8 g, Uint8 b)
{
	return (r + g + b)/3;
}

/*======================================*/
/*==============Binarize================*/
/*======================================*/

void binarize(SDL_Surface *surface){

	Uint32 pixel;
	Uint8 r,g,b,a;
	int x,y;

	for(y = 0; y < surface-> h; y++)
	{
		for(x = 0; x < surface -> w; x++)
		{
			pixel = getpixel(surface, x,y);
			SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
			
			Uint8 gray = grayscale(r,g,b);
			if(gray <= 127)
			{
				r = 0;
				g = 0;
				b = 0;
			}
			else
			{
				r = 255;
				g = 255;
				b = 255;
			}

			pixel = SDL_MapRGBA(surface->format, r, g, b, a);
			putpixel(surface, x, y, pixel);

		}
	}

}

/*======================================*/
/*============dfs_surface===============*/
/*======================================*/


void dfs_surface(SDL_Surface *surface, int x, int y, Uint8 mark){

	Uint32 pixel;
	Uint8 r,g,b,a;
	
	pixel = getpixel(surface, x,y);
	SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

	r = mark;
	g = 100;
	b = 100;

	pixel = SDL_MapRGBA(surface->format, r, g, b, a);
	putpixel(surface, x, y, pixel);
	
	if((0 <= x && x < surface-> w) && (0 <= y && y < surface-> h)){
		/*Left*/
		if(x > 0){
	
		pixel = getpixel(surface, x-1,y);
		SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
		
		if(r == 0)
			 dfs_surface(surface, x-1, y, mark);
		}
		/*Right*/
		if(x < (surface-> w)-1){
			
		pixel = getpixel(surface, x+1,y);
		SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
		
		if(r == 0)
			 dfs_surface(surface, x+1, y, mark);
		}
		/*Top*/
		if(y > 0){
		
		pixel = getpixel(surface, x,y-1);
		SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
		
		if(r == 0)
			 dfs_surface(surface, x, y-1, mark);
	
		}
		/*Bot*/
		if(y < (surface-> h)-1){
			
		pixel = getpixel(surface, x,y+1);
		SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
		
		if(r == 0)
			 dfs_surface(surface, x, y+1, mark);
		}
		
	}
}

/*======================================*/
/*=============segmentation=============*/
/*======================================*/

void segmentation(SDL_Surface *surface, Uint8 mark){

	Uint32 pixel;	
	Uint8 r,g,b,a;
	int x;
	int y;
	
	for(y = 0; y < surface-> h; y++){
		for(x = 0; x < surface -> w; x++){

		pixel = getpixel(surface, x,y);
		SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
	
		if(r == 0)
			dfs_surface(surface, x, y, mark);
		}
	}
}

/*======================================*/
/*================main==================*/
/*======================================*/


int main(int argc, char *argv[])
{ 
	if(argc > 2)
		printf("error: La fonction a trop de paramètres. \n");
	if(argc < 2)
		printf("error: Il faut un fichier à la fonction. \n");

	char *file = argv[1];
	
	SDL_Surface *surface;
		
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Erreur d'initialisation.");
		return 3;
	}

	surface = load_image(file);
	if (!surface){
		printf("Erreur de chargement dans l'image.");
		return 3;
	}
	
	SDL_LockSurface(surface);	
	
	binarize(surface);
	
	segmentation(surface, 255);

	SDL_UnlockSurface(surface);

	display_image(surface);

	SDL_FreeSurface(surface);

	SDL_Quit();

	return 0;
}

