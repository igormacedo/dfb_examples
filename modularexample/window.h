#ifndef WINDOW_H
#define WINDOW_H

#include <directfb.h>
#include <map>
#include <string>
#include <iostream>
#include "tool.h"
#include "canvas.h"
#include "event.h"

#define DFBCHECK(x...)                                       \
{                                                            \
    DFBResult err = x;                                       \
                                                             \
    if (err != DFB_OK)                                       \
    {                                                        \
        fprintf(stderr, "%s <%d>:\n\t", __FILE__, __LINE__); \
        DirectFBErrorFatal(#x, err);                         \
    }                                                        \
}

class Window{
    private:
        IDirectFB *dfb = NULL;
        IDirectFBDisplayLayer *layer = NULL;
        IDirectFBSurface *surface = NULL;

        IDirectFBWindow *select_window = NULL;
	    IDirectFBSurface *select_surface = NULL;

        std::map<int, Tool*> toolsMap;
        Canvas canvas;
        EventHandler event;
        Tool *currentTool;
        int position = 1;

        void createMenuSelector();
        void updateMenuSelectorPosition(bool up);

    public:
        int screen_width;
	    int screen_height;

        void initiate(int *argc, char ***argv);
        void addTool(Tool *tool);
        void reloadToolMenu();
        void runWindow();
        void finalize();
};

#endif