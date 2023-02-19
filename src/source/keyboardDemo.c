#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include "./keyboardDemo.h"
#include "brew_bgr.h"

//static SwkbdCallbackResult MyCallback(void* user, const char** ppMessage, const char* text, size_t textlen)
//{
//	if (strstr(text, "lenny"))
//	{
//		*ppMessage = "Nice try but I'm not letting you use that meme right now";
//		return SWKBD_CALLBACK_CONTINUE;
//	}
//
//	if (strstr(text, "brick"))
//	{
//		*ppMessage = "~Time to visit Brick City~";
//		return SWKBD_CALLBACK_CLOSE;
//	}
//
//	return SWKBD_CALLBACK_OK;
//}

int keyboardDemo(int argc, char **argv)
{
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	printf("Software keyboard demo - by fincs\n");
	printf("A: text input\n");
	printf("B: numpad input\n");
	printf("X: western input\n");
	printf("Y: filtered input + HOME/POWER/reset handling\n");
	printf("START: exit\n");
//Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one

	//We don't need double buffering in this example. In this way we can draw our image only once on screen.
	gfxSetDoubleBuffering(GFX_BOTTOM, false);

	//Get the bottom screen's frame buffer
	u8* fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);

	//Copy our image in the bottom screen's frame buffer
	memcpy(fb, brew_bgr, brew_bgr_size);



	while (aptMainLoop())
	{
		hidScanInput();

		u32 kDown = hidKeysDown();

		if (kDown & KEY_START)
			break;

		static SwkbdState swkbd;
		static char mybuf[60];
		static SwkbdStatusData swkbdStatus;
		static SwkbdLearningData swkbdLearning;
		SwkbdButton button = SWKBD_BUTTON_NONE;
		bool didit = false;

		if (kDown & KEY_A)
		{
			didit = true;
			swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 3, -1);
			swkbdSetInitialText(&swkbd, mybuf);
			swkbdSetHintText(&swkbd, "Please enter the first number");
		//	swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Maybe Not", false);
		//	swkbdSetButton(&swkbd, SWKBD_BUTTON_MIDDLE, "~Middle~", true);
		//	swkbdSetButton(&swkbd, SWKBD_BUTTON_RIGHT, "Do It", true);
      swkbdSetFeatures(&swkbd, SWKBD_PREDICTIVE_INPUT);
			SwkbdDictWord words[2];
			swkbdSetDictWord(&words[0], "lenny", "( ͡° ͜ʖ ͡°)");
			swkbdSetDictWord(&words[1], "shrug", "¯\\_(ツ)_/¯");
			swkbdSetDictionary(&swkbd, words, sizeof(words)/sizeof(SwkbdDictWord));
			static bool reload = false;
			swkbdSetStatusData(&swkbd, &swkbdStatus, reload, true);
			swkbdSetLearningData(&swkbd, &swkbdLearning, reload, true);
			reload = true;
			button = swkbdInputText(&swkbd, mybuf, sizeof(mybuf));
		}

		if (didit)
		{
			if (button != SWKBD_BUTTON_NONE)
			{
				printf("You pressed button %d\n", button);
				printf("First number: %s\n", mybuf);
			} else
				printf("swkbd event: %d\n", swkbdGetResult(&swkbd));
		}

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		gspWaitForVBlank();
	}

	gfxExit();
	return 0;
}
