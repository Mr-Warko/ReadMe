#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <err.h>
#include <math.h>

//lock=1 -> linearized | lock=0 -> normal
void histogram(SDL_Surface *surface, Uint8 *histo){
	Uint8 r,g,b,grayValue;
	Uint32 pixel;

	for(int x = 0; x < surface->w; x++){
		for(int y = 0; y < surface->h; y++){
			
			pixel = getpixel(surface, x, y);
			SDL_GetRGB(pixel, surface->format, &r, &g, &b);

			grayValue = 0.59*r + 0.30*g + 0.11*b;
	
			r = grayValue;
			g = grayValue;
			b = grayValue;

			
			if(histo[grayValue])
				histo[grayValue] += 1;
			else
				histo[grayValue] = 1;

			pixel = SDL_MapRGB(surface->format, r, g, b);
			putpixel(surface, x, y, pixel);

		}
	}			
}

void linearized_histo(SDL_Surface *surface, Uint8 *histo){

	histogram(surface, histo);

	Uint8 min = 255;
	Uint8 max = 0;	
	Uint8 sum = 0;
	for(int i = 0; i < 256; i++){
		sum += histo[i];
		if(histo[i] < min)
			min = histo[i];
		else if (histo[i] > max)
			max = histo[i];
		histo[i] = 0;
	}

	Uint8 r,g,b,grayValue;
	Uint32 pixel;

	for(int x = 0; x < surface->w; x++){
		for(int y = 0; y < surface->h; y++){
			
			pixel = getpixel(surface, x, y);
			SDL_GetRGB(pixel, surface->format, &r, &g, &b);
			
			grayValue = 255/(max-min) * r - (255 * min / (max - min));
	
			r = grayValue;
			g = grayValue;
			b = grayValue;

			
			if(histo[grayValue])
				histo[grayValue] += 1;
			else
				histo[grayValue] = 1;

			pixel = SDL_MapRGB(surface->format, r, g, b);
			putpixel(surface, x, y, pixel);

		}
	}		
}

void binarize(SDL_Surface *surface, int threshold){

	Uint32 pixel;
	Uint8 r,g,b,gray;
	int x,y;

	for(y = 0; y < surface-> h; y++)
	{
		for(x = 0; x < surface -> w; x++)
		{
			pixel = getpixel(surface, x,y);
			SDL_GetRGB(pixel, surface->format, &r, &g, &b);
			
			gray = 0.59*r + 0.30*g + 0.11*b;
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

			pixel = SDL_MapRGB(surface->format, r, g, b);
			putpixel(surface, x, y, pixel);

		}
	}
}


Uint8 otsu(Uint8 *histo, int total){
	Uint8 sum = 0;
	for( int i = 1; i < 256; ++i)
		sum += i * histo[i];
	Uint8 sumB = 0;
	Uint8 wB = 0;
	Uint8 wF = 0;
	Uint8 mB;
	Uint8 mF;
	Uint8 max = 0.0;
	Uint8 between = 0.0;
	int threshold1 = 0;
	int threshold2 = 0;
	for (int i = 0; i < 256; ++i){
		wB += histo[i];
		if(wB == 0)
			continue;
		wF = total - wB;
		if (wF == 0)
			break;
		sumB += i * histo[i];
		mB = sumB / wB;
		if (wF == 0)
			break;
		sumB += i * histo[i];
		mB = sumB / wB;
		mF = (sum - sumB) / wF;
		between = wB * wF * (mB - mF) * (mB - mF);
		if (between >= max) {
			threshold1 = i;
			if (between > max){
				threshold2 = i;
			}
			max = between;
		}
	}
	return (threshold1 + threshold2) / 2.0;
}

