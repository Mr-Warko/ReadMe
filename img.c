#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "SDL.h"

/* Le tout effectue un binarize (supprime les couleurs) sur une image au format
	 bmp donnée en argument. */

/*======================================*/
/*============obtenirPixel==============*/
/*======================================*/

Uint32 obtenirPixel(SDL_Surface *surface, int x, int y)
{
	/*nbOctetsParPixel représente le nombre d'octets utilisés pour stocker un 
	pixel.
	En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient 
	la profondeur de couleur de l'image : 8, 16, 24 ou 32 bits.*/
	
	int nbOctetsParPixel = surface->format->BytesPerPixel;

	/* Ici p est l'adresse du pixel que l'on veut connaitre */
	/*surface->pixels contient l'adresse du premier pixel de l'image*/
	
	Uint8 *p = (Uint8 *)surface->pixels 
						 + y * surface->pitch + x * nbOctetsParPixel;

	/*Gestion différente suivant le nombre d'octets par pixel de l'image*/
	switch(nbOctetsParPixel)
	{
			case 1:
					return *p;
			case 2:
					return *(Uint16 *)p;
			case 3:
					/*Suivant l'architecture de la machine*/
					if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
							return p[0] << 16 | p[1] << 8 | p[2];
					else
							return p[0] | p[1] << 8 | p[2] << 16;
			case 4:
					return *(Uint32 *)p;
			/*Ne devrait pas arriver, mais évite les erreurs*/
			default:
					return 0; 
	}
}

/*======================================*/
/*============definirPixel==============*/
/*======================================*/


void definirPixel(SDL_Surface *surface,	int x, int y, Uint32 pixel)
{
	int nbOctetsParPixel = surface->format->BytesPerPixel;
	
	Uint8 *p = (Uint8 *)surface->pixels 
						 + y * surface->pitch + x * nbOctetsParPixel;
	
	switch(nbOctetsParPixel)
	{
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p	= pixel;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			}
			else
			{
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

			case 4:
				*(Uint32 *)p = pixel;
				break;
	}
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
			pixel = obtenirPixel(surface, x,y);
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
			definirPixel(surface, x, y, pixel);

		}
	}

}

/*======================================*/
/*============dfs_surface===============*/
/*======================================*/


void dfs_surface(SDL_Surface *surface, int x, int y, Uint8 mark){

	Uint32 pixel;
	Uint8 r,g,b,a;
	
	pixel = obtenirPixel(surface, x,y);
	SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);

	r = mark;
	g = 100;
	b = 100;

	pixel = SDL_MapRGBA(surface->format, r, g, b, a);
	definirPixel(surface, x, y, pixel);
	
	if((0 <= x && x < surface-> w) && (0 <= y && y < surface-> h)){
		/*Left*/
		if(x > 0){
	
		pixel = obtenirPixel(surface, x-1,y);
		SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
		
		if(r == 0)
			 dfs_surface(surface, x-1, y, mark);
		}
		/*Right*/
		if(x < (surface-> w)-1){
			
		pixel = obtenirPixel(surface, x+1,y);
		SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
		
		if(r == 0)
			 dfs_surface(surface, x+1, y, mark);
		}
		/*Top*/
		if(y > 0){
		
		pixel = obtenirPixel(surface, x,y-1);
		SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
		
		if(r == 0)
			 dfs_surface(surface, x, y-1, mark);
	
		}
		/*Bot*/
		if(y < (surface-> h)-1){
			
		pixel = obtenirPixel(surface, x,y+1);
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

		pixel = obtenirPixel(surface, x,y);
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
	
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *surface;
	SDL_Texture *texture;
	SDL_Event event;

	Uint32 pixel;
	Uint8 r,g,b,a;
	int x,y;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s",
			SDL_GetError());
			return 3;
	}

	if (SDL_CreateWindowAndRenderer(320, 240, SDL_WINDOW_RESIZABLE, &window, 
																	&renderer)) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
									 "Couldn't create window and renderer: %s", SDL_GetError());
			return 3;
	}

	surface = SDL_LoadBMP(file);
	
	if (!surface) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
									 "Couldn't create surface from image: %s", SDL_GetError());
			return 3;
	}
	
	SDL_LockSurface(surface);	
	
	binarize(surface);
	
	SDL_UnlockSurface(surface);
	SDL_LockSurface(surface);

	segmentation(surface, 255);

	SDL_UnlockSurface(surface);

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	while (1) 
 {
			SDL_PollEvent(&event);
			if (event.type == SDL_QUIT) {
					break;
			}
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
	}
	
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}

