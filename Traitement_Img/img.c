#include <err.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "pixel_operations.c"
#include "List.h"
#include "preTraitement.h"

/*======================================*/
/*================Usefull===============*/
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
/*=============cutExtension=============*/
/*======================================*/

char *cutExtension(char *file){
	size_t len = strlen(file);
	char *dir = malloc(len * sizeof(char)-3);
	size_t i;
	for(i = 0; i < len -3; i ++)
		*(dir + i) = *(file + i);
	return dir;	
}

/*======================================*/
/*=============dlCharAsBmp==============*/
/*======================================*/

void dlCharAsBmp(SDL_Surface *img, struct l_list *lines, char *file, 
								 Uint32 rmask, Uint32 gmask, Uint32 bmask, Uint32 amask)
{
	SDL_Surface *surface;
	Uint8 r,g,b;
	Uint32 pixel;
	int charNb = 0;
		
	char *dir = cutExtension(file);
	printf("dir: %s \n",dir);

	mkdir(dir,0777);
	
	for(;lines; lines = lines->next){
		struct list *line = lines->value;
		line = line->next;

		for(;line;line = line->next){
			int *cooChar = line->value;

			char fileName[strlen(dir) + 8];
			sprintf(fileName,"%s/char%i.bmp",dir,charNb);	
		
			int width = (*(cooChar+2) - *cooChar)+1;
			int height = (*(cooChar+3) - *(cooChar+1))+1;
			
			surface = SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32,
																		 rmask,gmask,bmask,amask);
			if(!surface){
				fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
				exit(1);
			}

			for(int y = *(cooChar+1); y <= *(cooChar+3); y++){
				for(int x = *cooChar; x <= *(cooChar+2); x++){
					pixel = getpixel(img, x,y);
					SDL_GetRGB(pixel, img->format, &r, &g, &b);

					pixel = SDL_MapRGB(surface->format, r, g, b);
					putpixel(surface, x-(*cooChar), y-*(cooChar+1), pixel);
				}
			}

			if(SDL_SaveBMP(surface, fileName) < 0){
				fprintf(stderr, "SaveBMP failed: %s\n", SDL_GetError());
				exit(1);
			}
			
			charNb++;
			SDL_FreeSurface(surface);
		}
 	}
	free(dir);
}

/*======================================*/
/*=============grayScale================*/
/*======================================*/
/*

Uint8 grayscale(Uint8 r, Uint8 g, Uint8 b)
{
	return (r + g + b)/3;
}
*/
/*======================================*/
/*==============Binarize================*/
/*======================================*/
/*
void binarize(SDL_Surface *surface, int threshold){

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
			if(gray <= threshold)
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
*/
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
/*=============getCooLine===============*/
/*======================================*/

void getCooLine(int *cooLine, struct list *l){
	*cooLine = *(l->value);//Xmin

	for(;l->next;l = l->next){
		if(*((l->value)+1) < *(cooLine+1))//Ymin
			*(cooLine+1) = *((l->value)+1);
		if(*((l->value)+3) > *(cooLine+3))//Ymax
			*(cooLine+3) = *((l->value)+3);
	}
	if(*((l->value)+1) < *(cooLine+1))//Ymin
		*(cooLine+1) = *((l->value)+1);
	if(*((l->value)+3) > *(cooLine+3))//Ymax
		*(cooLine+3) = *((l->value)+3);
	
	*(cooLine+2) = *((l->value)+2);//Xmax
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
	int xMin = cooRect[0]-1; int yMin = cooRect[1]-1;
	int xMax = cooRect[2]+1; int yMax = cooRect[3]+1;
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
	g = 0;
	b = 0;

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

struct l_list *segmentation(SDL_Surface *surface, Uint8 mark){

	int x;
	int y;
	int new_verrou = 0;
	int old_verrou = 0;
	Uint32 pixel;
	Uint8 r,g,b,a;
	
	struct l_list *lines = empty_l_list(); //Creates a line list.
	struct list *line = empty_list(); //Create a char list.
	
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
				int *cooChar = malloc(4 * sizeof(int));
				*cooChar = x; *(cooChar+1) =y; *(cooChar+2) = x; *(cooChar+3) = y;
				dfs_surface(surface, x, y, mark, cooChar);

				line = insert(line,cooChar);//Insert char and sort it
				y = *(cooChar + 1);
			}
		}
	
		//Detects the passage from a black pixel to a white one.
		if (old_verrou && !new_verrou){
			int *cooLine = malloc(4 * sizeof(int));			
			
			for(int yolo = 0; yolo < 4; yolo++) //Need something to compare.
				*(cooLine + yolo) = *((line->value)+yolo);
			
			getCooLine(cooLine,line);
			line = add(line,cooLine);//Put on the top of the line its coo.

			lines = l_list_append(lines,line);//Put the char list in the line list.
			line = empty_list();
		}
	}
	return lines;
}

/*======================================*/
/*================main==================*/
/*======================================*/
int main(int argc, char *argv[])
{
	Uint32 rmask, gmask, bmask, amask; 

	if(argc > 2)
		printf("error: La fonction a trop de paramètres. \n");
	if(argc < 2)
		printf("error: Il faut un fichier à la fonction. \n");

	char *file = argv[1];
	
	SDL_Surface *surface;
		
	init_sdl();

	surface = load_image(file);
	
	SDL_LockSurface(surface);	

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

	Uint8 histo[256];
	histogram(surface,histo);

	int threshold = otsu(histo,surface->w * surface->h);
	binarize(surface, threshold);
	
	struct l_list *lines =	segmentation(surface, 1);
  dlCharAsBmp(surface,lines,file,rmask,gmask,bmask,amask);

	for(;lines;lines = lines->next){
		
		struct list *line2 = lines->value;
		for(;line2;line2 = line2->next)
			trace_rect(surface,line2->value);
	}

	l_list_destroy(lines);
	
	char *dir = cutExtension(file);
	char imgName[strlen(dir) + 8];
	sprintf(imgName,"%s/_seg.bmp",dir);	
	
	SDL_SaveBMP(surface,imgName);
	printf("%s\n",imgName);
	SDL_UnlockSurface(surface);

	display_image(surface);

	SDL_FreeSurface(surface);

	SDL_Quit();

	return 0;
}
