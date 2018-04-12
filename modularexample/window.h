#ifndef WINDOW_H
#define WINDOW_H

#include <directfb.h>
#include <map>
#include <string>
#include "tool.h"

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
        std::map<std::string, Tool> toolsMap;

    public:
        int screen_width;
	    int screen_height;

        void initiate(int *argc, char ***argv);
        void runCanvas();
        void finalize();
        void addTool(Tool tool);
        void reloadToolMenu();
};

#endif