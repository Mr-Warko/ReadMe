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
    case SDL_MOUSEBUTTONUP: return;
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
/*==============test_dfs================*/
/*======================================*/

int test_dfs(SDL_Surface *surface, int x, int y, Uint8 colorTest){
	
	Uint32 pixel;
	Uint8 r,g,b,a;
	
	pixel = getpixel(surface, x,y);
	SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

	return (r == colorTest)? 1: 0;
}

/*======================================*/
/*==========actualize_cooLine===========*/
/*======================================*/

void actualize_cooLine(int cooLine[], int cooChar[]){
	if (*cooLine == -1){
		*cooLine = *cooChar;
		*(cooLine + 1) = *(cooChar + 1);
		*(cooLine + 2) = *(cooChar + 2);
	}
	else{
		if (*(cooChar) < *(cooLine))
			*(cooLine) = *(cooChar);
		if (*(cooChar + 2) > *(cooLine + 2))
			*(cooLine + 2) = *(cooChar + 2);

		if (*(cooChar + 1) < *(cooLine + 1))
			*(cooLine + 1) = *(cooChar + 1);
		if (*(cooChar + 3) > *(cooLine + 3))
			*(cooLine + 3) = *(cooChar + 3);
	}
}

/*======================================*/
/*==========actualize_cooChar===========*/
/*======================================*/

void actualize_cooChar(int cooChar[], int x, int y){
	if (x < *(cooChar))
		*(cooChar) = x;
	else if (x > *(cooChar + 2))
		*(cooChar + 2) = x;

	if (y < *(cooChar + 1))
		*(cooChar + 1) = y;
	else if(y > *(cooChar + 3))
		*(cooChar + 3) = y;
}

/*======================================*/
/*=============trace_rect===============*/
/*======================================*/

void trace_rect(SDL_Surface *surface, int cooRect[]){

	Uint32 pixel;
	Uint8 r,g,b,a;
	int xMin = cooRect[0]; int yMin = cooRect[1];
	int xMax = cooRect[2]; int yMax = cooRect[3];
	/*LEFT and RIGHT*/
	for	(int y = yMin; y < yMax; ++y){
		/*LEFT*/
		pixel = getpixel(surface, xMin, y);
		SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

		r = 100;
		g = 100;
		b = 255;

		pixel = SDL_MapRGBA(surface->format, r, g, b, a);
		putpixel(surface, xMin, y, pixel);
		
		/*RIGHT*/
		pixel = getpixel(surface, xMax, y);
		SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

		r = 100;
		g = 100;
		b = 255;

		pixel = SDL_MapRGBA(surface->format, r, g, b, a);
		putpixel(surface, xMax, y, pixel);
	}
	/*TOP and BOT*/
	for (int x = xMin; x < xMax; ++x){
		/*TOP*/
		pixel = getpixel(surface, x,yMin);
		SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

		r = 100;
		g = 100;
		b = 255;

		pixel = SDL_MapRGBA(surface->format, r, g, b, a);
		putpixel(surface, x, yMin, pixel);
		
		/*BOT*/
		pixel = getpixel(surface, x,yMin);
		SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

		r = 100;
		g = 100;
		b = 255;

		pixel = SDL_MapRGBA(surface->format, r, g, b, a);
		putpixel(surface, x, yMax, pixel);
	}
}

/*======================================*/
/*============dfs_surface===============*/
/*======================================*/


void dfs_surface(SDL_Surface *surface, int x, int y, Uint8 mark, int cooChar[]){

	Uint32 pixel;
	Uint8 r,g,b,a;

	actualize_cooChar(cooChar, x, y);
		
	pixel = getpixel(surface, x,y);
	SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

	r = mark;
	g = 100;
	b = 100;

	pixel = SDL_MapRGBA(surface->format, r, g, b, a);
	putpixel(surface, x, y, pixel);
	
	if((0 <= x && x < surface-> w) && (0 <= y && y < surface-> h)){
		/*Left*/
		if(x > 0)
				if(test_dfs(surface,x-1,y,0))
				 dfs_surface(surface, x-1, y, mark,cooChar);

		/*Right*/
		if(x < (surface-> w)-1)
			if(test_dfs(surface,x+1,y,0))
				 dfs_surface(surface, x+1, y, mark,cooChar);
		
		/*Top*/
		if(y > 0)
			if(test_dfs(surface,x,y-1,0))
				 dfs_surface(surface, x, y-1, mark,cooChar);

		/*Bot*/
		if(y < (surface-> h)-1)
			if(test_dfs(surface,x,y+1,0))
				 dfs_surface(surface, x, y+1, mark,cooChar);
	
	}
}

/*======================================*/
/*=============segmentation=============*/
/*======================================*/

void segmentation(SDL_Surface *surface, Uint8 mark){

	int x;
	int y;
	int new_verrou = 0;
	int old_verrou = 0;
	Uint32 pixel;
	Uint8 r,g,b,a;

	int cooLine[] = {-1,0,0,0}; //Le -1 sert de marqueur pour actualize_cooLine.
	for(y = 0; y < surface-> h; ++y){
		old_verrou = new_verrou; new_verrou = 0;
		for(x = 0; x < surface -> w; ++x){
			
			pixel = getpixel(surface, x,y);
			SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

			if(g != 255){
				old_verrou = new_verrou; new_verrou = 1;
			}
			if(test_dfs(surface,x,y,0)){
				/* {xMin, yMin, xMax, yMax} */
				int cooChar[] = {x,y,x,y};			
				dfs_surface(surface, x, y, mark, cooChar);
				actualize_cooLine(cooLine, cooChar);
				y = *(cooChar + 1); //Optimisation de la boucle.
				trace_rect(surface,cooChar);
			}
		}
		//old verrou $$ !new_verrou => fin de ligne
		if (old_verrou && !new_verrou){
			trace_rect(surface,cooLine);
			*cooLine = -1;
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
		
	SDL_Init(SDL_INIT_VIDEO);

	surface = load_image(file);
	
	SDL_LockSurface(surface);	
	
	binarize(surface);
	
	segmentation(surface, 255);

	SDL_UnlockSurface(surface);

	display_image(surface);

	SDL_FreeSurface(surface);

	SDL_Quit();

	return 0;
}

