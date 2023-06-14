#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <aesndlib.h>
#include <gcmodplay.h>
#include <string.h>
#include <math.h>

#include "music_mod.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;
static MODPlay play;

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();

	// Initialise the audio subsystem
	AESND_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();


	// The console understands VT terminal escape codes
	// This positions the cursor on row 2, column 0
	// we can use variables for this with format codes too
	// e.g. printf ("\x1b[%d;%dH", row, column );
	printf("\x1b[2;0H");

	// blue background
	//printf("\x1b[44;37m");
	printf("\x1b[40;37m");
	printf("\x1b[2J");

	MODPlay_Init(&play);
	MODPlay_SetMOD(&play,music_mod);
	MODPlay_SetVolume(&play,63,63);
	MODPlay_Start(&play);

	float A = 0, B = 0;
	float i, j;
	int k;
	float z[1760];
	char b[1760];
	for(;;) {
		WPAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		if ( pressed & WPAD_BUTTON_HOME ) {
			exit(0);
		}
		memset(b,32,1760);
			memset(z,0,7040);
			for(j=0; j < 6.28; j += 0.07) {
				for(i=0; i < 6.28; i += 0.02) {
					float c = sin(i);
					float d = cos(j);
					float e = sin(A);
					float f = sin(j);
					float g = cos(A);
					float h = d + 2;
					float D = 1 / (c * h * e + f * g + 5);
					float l = cos(i);
					float m = cos(B);
					float n = sin(B);
					float t = c * h * g - f * e;
					int x = 40 + 30 * D * (l * h * m - t * n);
					int y= 12 + 15 * D * (l * h * n + t * m);
					int o = x + 80 * y;
					int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
					if(22 > y && y > 0 && x > 0 && 80 > x && D > z[o]) {
						z[o] = D;
						b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
					}
				}
			}
		printf("\x1b[H");
		for(k = 0; k < 1761; k++) {
			//printf("\x1b[40;37m");
			putchar(k % 80 ? b[k] : 10);
			A += 0.00004;
			B += 0.00002;
		}
		usleep(30000);

		printf(" \x1b[44;37m.----------------------------------------------------------------------------.\x1b[40;37m\n");
		printf(" \x1b[44;37m|  Wii Donut v1.0                                     (Press HOME to quit.)  |\x1b[40;37m\n");
		printf(" \x1b[44;37m|  Based on the original donut.c by Andy Sloane <andy@a1k0n.net>             |\x1b[40;37m\n");
		printf(" \x1b[44;37m|  Ported (copied and pasted) by jornmann <jornmann@duck.com>                |\x1b[40;37m\n");
		printf(" \x1b[44;37m|  Music by Jogeir Liljedahl                                                 |\x1b[40;37m\n");
		printf(" \x1b[44;37m'----------------------------------------------------------------------------'\x1b[40;37m");
		VIDEO_WaitVSync();
	}

	return 0;
}
