#include "squareDraw.h"

SquareDraw::SquareDraw(std::string str_name)
{
    this->name = str_name;
}

SquareDraw::~SquareDraw()
{
}

void SquareDraw::drawAt(Canvas* canvas, EventHandler* evt)
{
    if (evt->type == MOUSE_BUTTONDOWN && evt->button == BUTTON_LEFT) {
        initialRecX = evt->mouse_px - canvas->px;
        initialRecY = evt->mouse_py - canvas->py;

    } else if (evt->type == MOUSE_MOTION && evt->button == BUTTON_LEFT) {
        int mousex = evt->mouse_px - canvas->px;
        int mousey = evt->mouse_py - canvas->py;

        canvas->draw_surface->Blit(canvas->draw_surface, canvas->draw_surface_tmp, NULL, 0, 0);
        canvas->draw_surface->Flip(canvas->draw_surface, NULL, (DFBSurfaceFlipFlags)0);
        canvas->draw_surface->DrawRectangle(canvas->draw_surface, initialRecX, initialRecY,
            abs(initialRecX - mousex), abs(initialRecY - mousey));
        canvas->draw_surface->Flip(canvas->draw_surface, NULL, (DFBSurfaceFlipFlags)0);

    } else if (evt->type == MOUSE_BUTTONUP && evt->button == BUTTON_LEFT) {
        finalRecX = evt->mouse_px - canvas->px;
        finalRecY = evt->mouse_py - canvas->py;

        canvas->draw_surface->DrawRectangle(canvas->draw_surface, initialRecX, initialRecY,
            initialRecX - finalRecX, initialRecY - finalRecY);

        canvas->draw_surface_tmp->Blit(canvas->draw_surface_tmp, canvas->draw_surface, NULL, 0, 0);
        canvas->draw_surface->Flip(canvas->draw_surface, NULL, (DFBSurfaceFlipFlags)0);
    }

    if (evt->type == MOUSE_BUTTONUP) {
        canvas->draw_surface_tmp->Blit(canvas->draw_surface_tmp, canvas->draw_surface, NULL, 0, 0);
    }
}
