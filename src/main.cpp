#include <filesystem>

#define MIN_WIN_WIDTH 300u
#define MIN_WIN_HEIGHT 200u
#include "App.hpp"


#if !(defined(APP_ICON_PNG) && defined(APP_ICON_ICO))
#error "both FILE_ICON_PNG and FILE_ICON must be defined"
#endif


#ifdef WIN32
#define APP_ENTRY() WinMain()
#else
#define APP_ENTRY() main()
#endif


int APP_ENTRY() {
    App app(1280u, 720u, 60u, "Hello there");
	app.setupIcon(APP_ICON_PNG);

    return app.runUntilExit();
}

#define DARK_BLUE "#040714"