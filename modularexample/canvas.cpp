#include "canvas.h"

Canvas::Canvas(IDirectFB* dfb, IDirectFBSurface* surface)
    : dfb(dfb)
    , surface(surface)
{
}

void Canvas::initiateCanvas(int px, int py, int h, int w)
{
    this->px = px;
    this->py = py;
    this->height = h;
    this->width = w;

    DFBWindowDescription dwdesc;

    // Creating Window for drawing surface
    dwdesc.flags = (DFBWindowDescriptionFlags)(DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS);
    dwdesc.caps = DWCAPS_ALPHACHANNEL;
    dwdesc.posx = px;
    dwdesc.posy = py;
    dwdesc.width = w;
    dwdesc.height = h;

    IDirectFBDisplayLayer* layer;
    dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &layer);
    layer->SetCooperativeLevel(layer, DLSCL_ADMINISTRATIVE);

    layer->CreateWindow(layer, &dwdesc, &draw_window);
    draw_window->GetSurface(draw_window, &draw_surface);
    draw_window->SetOpacity(draw_window, 0xFF);
    draw_surface->Clear(draw_surface, 0x00, 0x00, 0x00, 0x00);
    draw_surface->SetColor(draw_surface, 0xFF, 0xFF, 0xFF, 0x55);
    draw_surface->DrawRectangle(draw_surface, 0, 0, dwdesc.width, dwdesc.height);
    draw_surface->Flip(draw_surface, NULL, (DFBSurfaceFlipFlags)0);
    //draw_window->AttachEventBuffer(draw_window, buffer);

    //Creating temporary surface for drawing
    layer->CreateWindow(layer, &dwdesc, &draw_window_temp);
    draw_window_temp->GetSurface(draw_window_temp, &draw_surface_tmp);

    // Flip to DFB base surface in the original layer
    surface->Flip(surface, NULL, (DFBSurfaceFlipFlags)0);
    layer->Release(layer);
}

void Canvas::finalize()
{
    draw_surface_tmp->Release(draw_surface_tmp);
    draw_window_temp->Release(draw_window_temp);
    draw_surface->Release(draw_surface);
    draw_window->Release(draw_window);
}