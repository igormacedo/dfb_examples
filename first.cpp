#include <stdio.h>
#include <unistd.h>
#include <directfb.h>
#include <map>
#include <string>
#include <iostream>

using namespace std;

static IDirectFB *dfb = NULL;
static IDirectFBDisplayLayer *layer = NULL;
static IDirectFBSurface *surface = NULL; //primary
static IDirectFBFont *fontbig;
static IDirectFBFont *fontsmall;
static int screen_width = 0;
static int screen_height = 0;

#define DFBCHECK(x...)                                           \
	{                                                            \
		DFBResult err = x;                                       \
                                                                 \
		if (err != DFB_OK)                                       \
		{                                                        \
			fprintf(stderr, "%s <%d>:\n\t", __FILE__, __LINE__); \
			DirectFBErrorFatal(#x, err);                         \
		}                                                        \
	}

int main(int argc, char **argv)
{
	DFBSurfaceDescription dsc;

	DFBWindowDescription wdesc;
	IDirectFBWindow *select_window;
	IDirectFBSurface *select_surface;

	IDirectFBWindow *draw_window;
	IDirectFBSurface *draw_surface;

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

	title[0] = "Nome1";
	title[1] = "Nome2";
	title[2] = "Nome3";
	title[3] = "Nome4";
	title[4] = "Nome5";
	title[5] = "Nome6";

	for (int i = 0; i < title.size(); i++)
	{
		surface->DrawString(surface, title[i].c_str(), -1, (int)(screen_width * 0.06), (int)(screen_height * 0.25) + (int)(screen_height * 0.07) * i, (DFBSurfaceTextFlags)(DSTF_LEFT | DSTF_TOP));
	}

	//layer->SetBackgroundImage( layer, surface );

	wdesc.flags = (DFBWindowDescriptionFlags)(DWDESC_POSX | DWDESC_POSY |
											  DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS);
	wdesc.caps = DWCAPS_ALPHACHANNEL;

	wdesc.posx = (int)(screen_width * 0.04);
	wdesc.posy = (int)(screen_height * 0.23);
	wdesc.width = (int)(screen_width * 0.17);
	wdesc.height = (int)(screen_height * 0.069);

	DFBCHECK(layer->CreateWindow(layer, &wdesc, &select_window));
	select_window->GetSurface(select_window, &select_surface);
	select_window->SetOpacity(select_window, 0xFF);
	select_surface->Clear(select_surface, 0x00, 0x00, 0x00, 0x00);
	select_surface->SetColor(select_surface, 0xFF, 0xFF, 0xFF, 0x55);
	select_surface->DrawRectangle(select_surface, 0, 0, wdesc.width, wdesc.height);
	select_surface->Flip(select_surface, NULL, (DFBSurfaceFlipFlags)0);
	DFBWindowID id1;
	select_window->GetID(select_window,&id1);

	DFBCHECK(surface->Flip(surface, NULL, (DFBSurfaceFlipFlags)0));

	IDirectFBEventBuffer *buffer;
	dfb->CreateInputEventBuffer(dfb, (DFBInputDeviceCapabilities) 
								(DICAPS_KEYS | DICAPS_BUTTONS),
								DFB_TRUE, &buffer);
	select_window->AttachEventBuffer(select_window, buffer);
	select_window->RequestFocus(select_window);

	IDirectFBWindow *bg_window = NULL;

	int quit = 0;
	int position = 1;
	bool button_right_pressed = 0;
	int px = 0, py = 0;

	while (!quit)
	{
		DFBEvent evt;
		buffer->WaitForEventWithTimeout(buffer, 0, 10);

		while (buffer->GetEvent(buffer, DFB_EVENT(&evt)) == DFB_OK)
		{
			cerr << evt.clazz << " - " << evt.window.window_id << endl;
			if(evt.clazz == DFEC_INPUT)
			{
				switch (evt.input.type)
				{
					case DIET_KEYPRESS:{
						switch (evt.input.key_id)
						{
						case DIKI_DOWN:
							if (position < 6)
							{
								select_window->Move(select_window, 0, wdesc.height + 1);
								position++;
							}
							else
							{
								select_window->Move(select_window, 0, -wdesc.height * 5 - 5);
								position = 1;
							}
							break;
						case DIKI_UP:
							if (position > 1)
							{
								select_window->Move(select_window, 0, -wdesc.height - 1);
								position--;
							}
							else
							{
								select_window->Move(select_window, 0, +wdesc.height * 5 + 5);
								position = 6;
							}
							break;
						case DIKI_ESCAPE:
							quit = 1;
						default:
							break;
						}
						break;
					}
					case DIET_AXISMOTION:{
						//If during mouse movement, the left button is pressed
						if (evt.input.buttons == DIBM_LEFT)
						{
							switch(evt.input.axis)
							{
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
							cerr << evt.input.min << "=" << evt.input.max << endl;
						}
						
					}
					default:
						break;
				}
			}
		}
	}

	//getchar();

	buffer->Release(buffer);
	surface->Release(surface);
	select_surface->Release(select_surface);
	select_window->Release(select_window);
	fontbig->Release(fontbig);
	fontsmall->Release(fontsmall);
	layer->Release(layer);
	dfb->Release(dfb);

	return 23;
}