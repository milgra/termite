
#ifdef __cplusplus

// Steam

    #ifdef STEAM

	#include "../projects/steam/CInventory.hpp"

    #endif

extern "C"
{
#endif

    // OpenGL

#if defined(IOS)

    #include <OpenGLES/ES1/gl.h>
    #include <OpenGLES/ES1/glext.h>

#elif defined(ANDROID)

    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>

#endif

#include "SDL.h"
#include "bridge.h"

#include <stdio.h>
#include <time.h>

#include "bus.c"
#include "defaults.c"
#include "renderer.c"
#include "scene.c"
#include "settings.c"
#include "ui.c"
#include "zc_cstring.c"

    float scale = 1.0;

    int32_t width  = 1000;
    int32_t height = 750;

    SDL_Window*   window;
    SDL_GLContext context;

    char quit = 0;

    void main_onmessage(const char* name, void* data)
    {

	if (strcmp(name, "DONATE") == 0)
	{
#ifdef STEAM
	    steam_buy((char*) data);
#else
	bridge_buy((char*) data);
#endif
	}
	else if (strcmp(name, "FEEDBACK") == 0)
	{
	    bridge_open((char*) "http://www.milgra.com/termite.html");
	}
	else if (strcmp(name, "HOMEPAGE") == 0)
	{
	    bridge_open((char*) "http://www.milgra.com");
	}
	else if (strcmp(name, "FULLSCREEN") == 0)
	{
	    int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;

	    char fullscreen = SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP;

	    if (fullscreen == 1) SDL_SetWindowFullscreen(window, flags);
	    else SDL_SetWindowFullscreen(window, flags | SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else if (strcmp(name, "RESET") == 0)
	{
	    settings_reset();
	}
	else if (strcmp(name, "EXIT") == 0)
	{
	    quit = 1;
	}
    }

    void main_init()
    {

	srand((unsigned int) time(NULL));

	char* basepath = SDL_GetPrefPath("milgra", "termite");

	settings_init(basepath, (char*) "termite.state");
	defaults_init();

	bus_init();
	bus_subscribe("UI", main_onmessage);

	v2_t dimensions       = {.x = width * scale, .y = height * scale};
	defaults.display_size = dimensions;

#ifdef STEAM
	steam_init();
#endif

	bridge_init(); // request donation prices from app store, init text scaling, init glfw

	int framebuffer  = 0;
	int renderbuffer = 0;

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);
	glGetIntegerv(GL_RENDERBUFFER_BINDING, &renderbuffer);

	renderer_init(framebuffer, renderbuffer, basepath);
	scene_init();
	ui_init();

	bus_notify("CTL", "RESIZE", &dimensions);

	int level = 0;
	bus_notify("CTL", "LOAD", &level);

	SDL_free(basepath);
    }

    void main_free()
    {

	scene_free();
	renderer_free();
	ui_free();

	settings_free();
	bus_free();

	bridge_kill();

#ifdef STEAM
	SteamAPI_Shutdown();
#endif
    }

    void main_loop()
    {
	char      drag = 0;
	SDL_Event event;

	while (!quit)
	{

	    while (SDL_PollEvent(&event) != 0)
	    {

		if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEMOTION)
		{

		    int x = 0, y = 0;
		    SDL_GetMouseState(&x, &y);

		    v2_t dimensions = {.x = x * scale, .y = y * scale};

		    if (event.type == SDL_MOUSEBUTTONDOWN)
		    {
			drag = 1;
			bus_notify("CTL", "TOUCHDOWN", &dimensions);
		    }
		    else if (event.type == SDL_MOUSEBUTTONUP)
		    {
			drag = 0;
			bus_notify("CTL", "TOUCHUP", &dimensions);
		    }
		    else if (event.type == SDL_MOUSEMOTION && drag == 1)
		    {
			bus_notify("CTL", "TOUCHMOVE", &dimensions);
		    }
		}
		else if (event.type == SDL_QUIT)
		{
		    quit = 1;
		}
		else if (event.type == SDL_WINDOWEVENT)
		{

		    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
		    {

			width  = event.window.data1;
			height = event.window.data2;

			v2_t dimensions       = {.x = width * scale, .y = height * scale};
			defaults.display_size = dimensions;

			bus_notify("CTL", "RESIZE", &dimensions);
		    }
		}
		else if (event.type == SDL_KEYUP)
		{
		    switch (event.key.keysym.sym)
		    {
			case SDLK_f:
			    main_onmessage((char*) "FULLSCREEN", NULL);
			    break;

			case SDLK_ESCAPE:
			    main_onmessage((char*) "FULLSCREEN", NULL);
			    break;
		    }
		}
		else if (event.type == SDL_APP_WILLENTERFOREGROUND)
		{

		    bus_notify("CTL", "RESETTIME", NULL); // reset scene timer to avoid giant step
		}
	    }

	    // update simulation

	    uint32_t ticks = SDL_GetTicks();

	    bus_notify("CTL", "UPDATE", &ticks);
	    bus_notify("CTL", "RENDER", &ticks);

	    SDL_GL_SwapWindow(window);

#ifdef STEAM
	    SteamAPI_RunCallbacks();
#endif
	}
    }

    int main(int argc, char* argv[])
    {

	// enable high dpi

	SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");

	// init sdl

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0)
	{

	    // setup opengl version

	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	    // window size should be full screen on phones, scaled down on desktops

	    SDL_DisplayMode displaymode;
	    SDL_GetCurrentDisplayMode(0, &displaymode);

	    if (displaymode.w < 800 || displaymode.h < 400)
	    {
		width  = displaymode.w;
		height = displaymode.h;
	    }
	    else
	    {
		width  = displaymode.w * 0.8;
		height = displaymode.h * 0.8;
	    }

#if defined(IOS) || defined(ANDROID)
	    width  = displaymode.w;
	    height = displaymode.h;
#endif

	    // create window

	    window = SDL_CreateWindow("Termite", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN |
#if defined(IOS) || defined(ANDROID)
														      SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN_DESKTOP);
#else
														  SDL_WINDOW_RESIZABLE);
#endif

	    if (window != NULL)
	    {

		// create context

		context = SDL_GL_CreateContext(window);

		if (context != NULL)
		{

		    GLint GlewInitResult = glewInit();
		    if (GLEW_OK != GlewInitResult)
		    {
			printf("ERROR: %s", glewGetErrorString(GlewInitResult));
		    }
		    else printf("GLEW OKAY\n");

		    // calculate scaling

		    int nw;
		    int nh;

		    SDL_GL_GetDrawableSize(window, &nw, &nh);

		    scale = nw / width;

		    // try to set up vsync

		    if (SDL_GL_SetSwapInterval(1) < 0) printf("SDL swap interval error %s\n", SDL_GetError());

		    main_init();
		    main_loop();
		    main_free();

		    // cleanup

		    SDL_GL_DeleteContext(context);
		}
		else printf("SDL context creation error %s\n", SDL_GetError());

		// cleanup

		SDL_DestroyWindow(window);
	    }
	    else printf("SDL window creation error %s\n", SDL_GetError());

	    // cleanup

	    SDL_Quit();
	}
	else printf("SDL init error %s\n", SDL_GetError());

	return 0;
    }

#ifdef __cplusplus
}
#endif
