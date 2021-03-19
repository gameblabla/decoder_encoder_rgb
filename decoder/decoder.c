#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#ifndef SDL_TRIPLEBUF
#define SDL_TRIPLEBUF SDL_DOUBLEBUF
#endif

SDL_Surface* sdl_screen;
SDL_Surface* tmp;
SDL_Surface* backbuffer;
uint32_t key_pressed = 0;
uint8_t str[64];
uint8_t tmp_str[64];

int total_width = 512;
int total_height = 512;

Uint32 SDL_getpixel(SDL_Surface *surface, uint32_t x, uint32_t y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	return *(Uint32 *)p;
}

int main (int argc, char *argv[]) 
{
	SDL_Event event;
	int quit = 0;
	int brightness = 50;
	FILE* fp;
	int filesize;
	int inc;
	int inc2;
	int a, i;
	int a_s, i_s;
	unsigned char* buffer;
	uint16_t retain;
	uint16_t fre;
	if (argc < 3)
	{
		printf("RGB to binary decoder\n ./rgbtobinary a.png a.raw\n");
		return 0;
	}
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(0);
	
	tmp = IMG_Load(argv[1]);
	sdl_screen = SDL_SetVideoMode(tmp->w, tmp->h, 32, SDL_HWSURFACE | SDL_TRIPLEBUF);
	backbuffer = SDL_DisplayFormatAlpha(tmp);
	SDL_FreeSurface(tmp);
	
	if (!sdl_screen)
	{
		printf("Can't set video mode\n");
		return 0;
	}
	
	filesize = (backbuffer->w * backbuffer->h) * 2;
	buffer = malloc(filesize);
	
	printf("Initial filesize is %d bytes\n", filesize);
	
	inc = 0;
	for(a=0;a<backbuffer->h;a++)
	{
		for(i=0;i<backbuffer->w;i++)
		{
			retain = SDL_getpixel(backbuffer, i, a);
			fre = __builtin_bswap16(retain);
			//buffer[inc] = SDL_getpixel(backbuffer, i, a);
			memcpy(buffer + inc, &fre, 2);
			inc = inc + 2;
		}
	}
	
	fp = fopen(argv[2], "wb");
	fwrite( buffer, 1, filesize, fp );
	fclose(fp);
	
	SDL_FreeSurface(sdl_screen);
	SDL_FreeSurface(backbuffer);
	SDL_Quit();
}
