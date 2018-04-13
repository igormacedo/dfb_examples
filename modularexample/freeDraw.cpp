#include "freeDraw.h"

FreeDraw::FreeDraw(std::string str_name)
{
    this->name = str_name;
}

FreeDraw::~FreeDraw()
{
}

void FreeDraw::drawAt(Canvas* canvas, EventHandler* evt)
{
    if (evt->type == EventType::MOUSE_BUTTONDOWN) {
        canvas->draw_surface->SetColor(canvas->draw_surface, 0xFF, 0x00, 0x00, 0xff);
        canvas->draw_surface->FillRectangle(canvas->draw_surface, evt->mouse_px, evt->mouse_py, 10, 10);
        canvas->draw_surface->Flip(canvas->draw_surface, NULL, (DFBSurfaceFlipFlags)0);
    }
}
