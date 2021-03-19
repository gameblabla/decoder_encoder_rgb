#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL/SDL.h>

#ifndef SDL_TRIPLEBUF
#define SDL_TRIPLEBUF SDL_DOUBLEBUF
#endif

SDL_Surface* sdl_screen;
SDL_Surface* backbuffer;
uint32_t key_pressed = 0;
uint8_t str[64];
uint8_t tmp_str[64];

int total_width = 512;
int total_height = 512;

void SDL_putpixel(SDL_Surface *surface, uint32_t x, uint32_t y, uint32_t pixel)
{
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
    *(Uint32 *)p = pixel;
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
	
	if (argc < 2)
	{
		printf("Binary to RGB encoder\n ./rgbtobinary file.raw\n");
		return 0;
	}
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(0);

	//backbuffer = SDL_LoadBMP("mire.bmp");
	
	fp = fopen(argv[1], "rb");
	fseek (fp , 0 , SEEK_END );
	filesize = ftell(fp);
	fseek (fp , 0 , SEEK_SET );
	buffer = malloc(filesize);
	fread( buffer, 1, filesize, fp );
	fclose(fp);
	
	if (argc == 4)
	{
		total_width = atoi(argv[2]);
		total_height = atoi(argv[3]);
	}
	else
	{
		total_width = (filesize / 256) + 2;
		total_height = (filesize / 256) + 2;
	}
	
	backbuffer = SDL_CreateRGBSurface(0, total_width, total_height, 32, 0, 0, 0, 0);
	
	if (filesize > ((total_width * total_height) * 2) + 1)
	{
		printf("Please increase the width or height\n");
		return 0;
	}
	
	inc = 0;
	for(a=0;a<total_height;a++)
	{
		for(i=0;i<total_width;i++)
		{
			retain = ((buffer[inc] << 8) | (buffer[inc+1] << 0));
			SDL_putpixel(backbuffer, i, a, retain);
			inc = inc + 2;
			if (inc >= filesize)
			{
				// Add +1 to total height to avoid cropping but also copying too much
				total_height = a + 1;
				
				a = total_height + 2;
				i = total_width + 2;
			}
		}
	}

	sdl_screen = SDL_SetVideoMode(total_width, total_height, 32, SDL_HWSURFACE | SDL_TRIPLEBUF);
	
	while(!quit)
	{
		while (SDL_PollEvent(&event)) 
		{
			switch(event.type) 
			{
				case SDL_KEYUP:
					switch(event.key.keysym.sym) 
					{
						/*
						 * HOME is for OpenDingux
						 * 3 is for RetroFW
						 * RCTRL is for PocketGo v2
						 * ESCAPE is mapped to Select
						*/
						case SDLK_ESCAPE:
							SDL_SaveBMP(sdl_screen, "save.bmp");
							quit = 1;
						break;
						default:
						break;
					}
				break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) 
					{
						default:
							key_pressed = event.key.keysym.sym;
						break;
					}
				break;
				case SDL_QUIT:
					quit = 1;
				break;
			}
		}
		
		//SDL_BlitSurface(backbuffer, NULL, sdl_screen, NULL);
		SDL_BlitSurface(backbuffer, NULL, sdl_screen, NULL);
		SDL_Flip(sdl_screen);
	}
	
	SDL_FreeSurface(sdl_screen);
	SDL_FreeSurface(backbuffer);
	SDL_Quit();
}
