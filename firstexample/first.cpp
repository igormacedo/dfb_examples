#include <directfb.h>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <unistd.h>

using namespace std;

static IDirectFB* dfb = NULL;
static IDirectFBDisplayLayer* layer = NULL;
static IDirectFBSurface* surface = NULL; //primary
static IDirectFBFont* fontbig;
static IDirectFBFont* fontsmall;
static int screen_width = 0;
static int screen_height = 0;

#define DFBCHECK(x...)                                           \
    {                                                            \
        DFBResult err = x;                                       \
                                                                 \
        if (err != DFB_OK) {                                     \
            fprintf(stderr, "%s <%d>:\n\t", __FILE__, __LINE__); \
            DirectFBErrorFatal(#x, err);                         \
        }                                                        \
    }

int main(int argc, char** argv)
{
    DFBSurfaceDescription dsc;

    DFBWindowDescription swdesc;
    IDirectFBWindow* select_window;
    IDirectFBSurface* select_surface;

    DFBWindowDescription dwdesc;
    IDirectFBWindow* draw_window;
    IDirectFBSurface* draw_surface;
    IDirectFBWindow* draw_window_temp;
    IDirectFBSurface* draw_surface_tmp;

    DFBCHECK(DirectFBInit(&argc, &argv));
    DFBCHECK(DirectFBCreate(&dfb));

    //DFBCHECK (dfb->SetCooperativeLevel (dfb, DFSCL_FULLSCREEN));

    // Getting Layer and configuring to create windows later

    DFBCHECK(dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &layer));
    layer->SetCooperativeLevel(layer, DLSCL_ADMINISTRATIVE);

    dsc.flags = DSDESC_CAPS;
    dsc.caps = (DFBSurfaceCapabilities)(DSCAPS_PRIMARY | DSCAPS_FLIPPING);

    DFBCHECK(dfb->CreateSurface(dfb, &dsc, &surface));
    DFBCHECK(surface->GetSize(surface, &screen_width, &screen_height));

    // Top rectangle
    DFBCHECK(surface->SetColor(surface, 0x00, 0x4d, 0x40, 0xff));
    DFBCHECK(surface->FillRectangle(surface, 0, 0, screen_width, (int)(screen_height * 0.2)));

    // Side Rectangle
    DFBCHECK(surface->SetColor(surface, 0x00, 0x69, 0x5c, 0xff));
    DFBCHECK(surface->FillRectangle(surface, 0, (int)(screen_height * 0.2), (int)(screen_width * 0.25), screen_height - (int)(screen_height * 0.2)));

    //Body Rectangle
    DFBCHECK(surface->SetColor(surface, 0x00, 0x96, 0x88, 0xff));
    DFBCHECK(surface->FillRectangle(surface, (int)(screen_width * 0.25), (int)(screen_height * 0.2), screen_width - (int)(screen_width * 0.25), screen_height - (int)(screen_height * 0.2)))

    DFBFontDescription desc;
    desc.flags = DFDESC_HEIGHT;
    desc.height = (int)(screen_height * 0.05);

    //Font big
    DFBCHECK(dfb->CreateFont(dfb, "decker.ttf", &desc, &fontbig));

    //Font small
    desc.height = (int)(screen_height * 0.02);
    DFBCHECK(dfb->CreateFont(dfb, "decker.ttf", &desc, &fontsmall));

    //Drawing title
    DFBCHECK(surface->SetFont(surface, fontbig));
    surface->SetColor(surface, 0xff, 0xff, 0xff, 0xff);
    surface->DrawString(surface, "GAP 6 - Canvas", -1, (int)(screen_width * 0.36), (int)(screen_height * 0.11), (DFBSurfaceTextFlags)(DSTF_LEFT | DSTF_TOP));

    //Drawing menu
    DFBCHECK(surface->SetFont(surface, fontsmall));

    std::map<int, std::string> title;

    title[0] = "Free Drawing";
    title[1] = "Draw Rectangle";
    title[2] = "Nome3";
    title[3] = "Nome4";
    title[4] = "Nome5";
    title[5] = "Nome6";

    for (int i = 0; i < title.size(); i++) {
        surface->DrawString(surface, title[i].c_str(), -1, (int)(screen_width * 0.06), (int)(screen_height * 0.25) + (int)(screen_height * 0.07) * i, (DFBSurfaceTextFlags)(DSTF_LEFT | DSTF_TOP));
    }

    // Creating evert buffer for mouse and keyboard events
    IDirectFBEventBuffer* buffer;
    dfb->CreateInputEventBuffer(dfb, (DFBInputDeviceCapabilities)(DICAPS_KEYS | DICAPS_BUTTONS),
        DFB_TRUE, &buffer);

    // Creating Window Select
    swdesc.flags = (DFBWindowDescriptionFlags)(DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS);
    swdesc.caps = DWCAPS_ALPHACHANNEL;
    swdesc.posx = (int)(screen_width * 0.04);
    swdesc.posy = (int)(screen_height * 0.23);
    swdesc.width = (int)(screen_width * 0.17);
    swdesc.height = (int)(screen_height * 0.069);

    DFBCHECK(layer->CreateWindow(layer, &swdesc, &select_window));
    select_window->GetSurface(select_window, &select_surface);
    select_window->SetOpacity(select_window, 0xFF);
    select_surface->Clear(select_surface, 0x00, 0x00, 0x00, 0x00);
    select_surface->SetColor(select_surface, 0xFF, 0xFF, 0xFF, 0x55);
    select_surface->DrawRectangle(select_surface, 0, 0, swdesc.width, swdesc.height);
    select_surface->Flip(select_surface, NULL, (DFBSurfaceFlipFlags)0);
    //select_window->AttachEventBuffer(select_window, buffer);
    select_window->RequestFocus(select_window);

    // Creating Window for drawing surface
    dwdesc.flags = (DFBWindowDescriptionFlags)(DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS);
    dwdesc.caps = DWCAPS_ALPHACHANNEL;
    dwdesc.posx = (int)(screen_width * 0.25);
    dwdesc.posy = (int)(screen_height * 0.2);
    dwdesc.width = screen_width - (int)(screen_width * 0.25);
    dwdesc.height = screen_height - (int)(screen_height * 0.2);

    DFBCHECK(layer->CreateWindow(layer, &dwdesc, &draw_window));
    draw_window->GetSurface(draw_window, &draw_surface);
    draw_window->SetOpacity(draw_window, 0xFF);
    draw_surface->Clear(draw_surface, 0x00, 0x00, 0x00, 0x00);
    draw_surface->SetColor(draw_surface, 0xFF, 0xFF, 0xFF, 0x55);
    draw_surface->DrawRectangle(draw_surface, 0, 0, dwdesc.width, dwdesc.height);
    draw_surface->Flip(draw_surface, NULL, (DFBSurfaceFlipFlags)0);
    draw_window->AttachEventBuffer(draw_window, buffer);

    //Creating temporary surface for drawing
    DFBCHECK(layer->CreateWindow(layer, &dwdesc, &draw_window_temp));
    draw_window_temp->GetSurface(draw_window_temp, &draw_surface_tmp);

    // Flip to DFB base surface in the original layer
    DFBCHECK(surface->Flip(surface, NULL, (DFBSurfaceFlipFlags)0));

    int quit = 0;
    int position = 1;
    bool button_right_pressed = 0;
    int px = 0, py = 0;

    bool rectangleDrawing = 0;
    int initialRecX = 0, initialRecY = 0;
    int finalRecX = 0, finalRecY = 0;

    while (!quit) {
        DFBEvent evt;
        buffer->WaitForEventWithTimeout(buffer, 0, 10);

        while (buffer->GetEvent(buffer, DFB_EVENT(&evt)) == DFB_OK) {
            cerr << evt.clazz << " - " << evt.window.window_id << endl;
            switch (evt.clazz) {
            case DFEC_INPUT:
                switch (evt.input.type) {
                case DIET_KEYPRESS: {
                    switch (evt.input.key_id) {
                    case DIKI_DOWN:
                        if (position < 6) {
                            select_window->Move(select_window, 0, swdesc.height + 1);
                            position++;
                        } else {
                            select_window->Move(select_window, 0, -swdesc.height * 5 - 5);
                            position = 1;
                        }
                        break;
                    case DIKI_UP:
                        if (position > 1) {
                            select_window->Move(select_window, 0, -swdesc.height - 1);
                            position--;
                        } else {
                            select_window->Move(select_window, 0, +swdesc.height * 5 + 5);
                            position = 6;
                        }
                        break;
                    case DIKI_ESCAPE: {
                        quit = 1;
                        break;
                    }
                    default:
                        break;
                    }
                    break;
                }
                case DIET_AXISMOTION: {
                    //If during mouse movement, the left button is pressed
                    if (evt.input.buttons == DIBM_LEFT) {
                        switch (evt.input.axis) {
                        case DIAI_X:

                            px = evt.input.axisabs;
                            break;

                        case DIAI_Y:
                            py = evt.input.axisabs;
                            break;
                        default:
                            break;
                            //return;
                        }

                        //int px = evt.input.
                        cerr << px << "-" << py << endl;
                    }
                }
                default:
                    break;
                }
            case DFEC_WINDOW:
                if (evt.window.type == DWET_BUTTONUP && evt.window.button == DIBI_LEFT) {
                    draw_surface_tmp->Blit(draw_surface_tmp, draw_surface, NULL, 0, 0);

                } else if (evt.window.buttons == DIBM_LEFT && evt.window.window_id == 3 && position == 1) {
                    DFBCHECK(draw_surface->SetColor(draw_surface, 0xFF, 0x00, 0x00, 0xff));
                    DFBCHECK(draw_surface->FillRectangle(draw_surface, evt.window.x, evt.window.y, 10, 10));
                    draw_surface->Flip(draw_surface, NULL, (DFBSurfaceFlipFlags)0);

                } else if (evt.window.type == DWET_BUTTONDOWN && evt.window.buttons == DIBM_LEFT && evt.window.window_id == 3 && position == 2) {
                    rectangleDrawing = true;
                    initialRecX = evt.window.x;
                    initialRecY = evt.window.y;

                } else if (evt.window.type == DWET_MOTION && evt.window.buttons == DIBM_LEFT && evt.window.window_id == 3 && position == 2 && rectangleDrawing) {

                    draw_surface->Blit(draw_surface, draw_surface_tmp, NULL, 0, 0);
                    draw_surface->Flip(draw_surface, NULL, (DFBSurfaceFlipFlags)0);
                    draw_surface->DrawRectangle(draw_surface, initialRecX, initialRecY,
                        abs(initialRecX - evt.window.x), abs(initialRecY - evt.window.y));
                    draw_surface->Flip(draw_surface, NULL, (DFBSurfaceFlipFlags)0);

                } else if (evt.window.type == DWET_BUTTONUP && evt.window.button == DIBI_LEFT && position == 2 && rectangleDrawing) {

                    rectangleDrawing = false;
                    finalRecX = evt.window.x;
                    finalRecY = evt.window.y;

                    draw_surface->DrawRectangle(draw_surface, initialRecX, initialRecY,
                        initialRecX - finalRecX, initialRecY - finalRecY);

                    draw_surface_tmp->Blit(draw_surface_tmp, draw_surface, NULL, 0, 0);
                    draw_surface->Flip(draw_surface, NULL, (DFBSurfaceFlipFlags)0);
                }

            default:
                break;
            }
        }
    }

    //test
    //getchar();

    buffer->Release(buffer);
    surface->Release(surface);
    select_surface->Release(select_surface);
    select_window->Release(select_window);
    draw_surface->Release(draw_surface);
    draw_window->Release(draw_window);
    draw_surface_tmp->Release(draw_surface_tmp);
    draw_window_temp->Release(draw_window_temp);
    fontbig->Release(fontbig);
    fontsmall->Release(fontsmall);
    layer->Release(layer);
    dfb->Release(dfb);

    return 23;
}