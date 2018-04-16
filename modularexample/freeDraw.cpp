#include "freeDraw.h"
#include "canvas.h"
#include "event.h"

FreeDraw::FreeDraw(std::string str_name)
{
    this->name = str_name;
}

FreeDraw::~FreeDraw()
{
}

void FreeDraw::drawAt(Canvas* canvas, EventHandler* evt)
{
    if ((evt->type & MOUSE_MOTION) && (evt->button == BUTTON_LEFT)) {
        int px = evt->mouse_px - canvas->px;
        int py = evt->mouse_py - canvas->py;

        canvas->draw_surface->SetColor(canvas->draw_surface, 0xFF, 0x00, 0x00, 0xff);
        canvas->draw_surface->FillRectangle(canvas->draw_surface, px, py, 10, 10);
        canvas->draw_surface->Flip(canvas->draw_surface, NULL, (DFBSurfaceFlipFlags)0);
    }

    if (evt->type == MOUSE_BUTTONUP) {
        canvas->draw_surface_tmp->Blit(canvas->draw_surface_tmp, canvas->draw_surface, NULL, 0, 0);
    }
}
