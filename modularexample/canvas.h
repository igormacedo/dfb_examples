#ifndef CANVAS_H
#define CANVAS_H

#include <directfb.h>

class Canvas {
private:
    IDirectFBWindow* draw_window;
    IDirectFBWindow* draw_window_temp;
    int px, py, height, width;

    IDirectFB* dfb;
    IDirectFBSurface* surface;

public:
    IDirectFBSurface* draw_surface;
    IDirectFBSurface* draw_surface_tmp;

    Canvas(IDirectFB* dfb, IDirectFBSurface* surface);
    void initiateCanvas(int px, int py, int h, int w);
    void finalize();
};

#endif