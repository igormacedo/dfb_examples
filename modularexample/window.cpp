#include "window.h"

void Window::initiate(int *argc, char ***argv){

	DFBCHECK(DirectFBInit(argc, argv));
	DFBCHECK(DirectFBCreate(&dfb));

	DFBCHECK(dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &layer));
	layer->SetCooperativeLevel(layer, DLSCL_ADMINISTRATIVE);

	DFBSurfaceDescription dsc;
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

	IDirectFBFont *fontbig;
	DFBFontDescription desc;
	desc.flags = DFDESC_HEIGHT;
	desc.height = (int)(screen_height * 0.05);
	DFBCHECK(dfb->CreateFont(dfb, "decker.ttf", &desc, &fontbig));

	DFBCHECK(surface->SetFont(surface, fontbig));
	surface->SetColor(surface, 0xff, 0xff, 0xff, 0xff);
	surface->DrawString(surface, "GAP 6 - Canvas", -1, (int)(screen_width * 0.36), (int)(screen_height * 0.11), (DFBSurfaceTextFlags)(DSTF_LEFT | DSTF_TOP));
	
	fontbig->Release(fontbig);

	// Flip to DFB base surface in the original layer
	DFBCHECK(surface->Flip(surface, NULL, (DFBSurfaceFlipFlags)0));
}

void Window::runWindow(){

	this->createMenuSelector();

	event = EventHandler();
	event.createEventHandler(dfb);

	bool quit = false;

	while(!quit){

		if (event.captureEventTimeout() == STATUS_HAS_EVENT){
			// If event is a mouse event
			if (event.type & (MOUSE_BUTTONDOWN | MOUSE_BUTTONUP | MOUSE_MOTION)){
				currentTool->drawAt(&canvas, &event);
			}
			// If event is a keyboard_down
			else if (event.type == KEYBOARD_KEYDOWN){
				switch (event.key){
					case KEY_UP:{
						updateMenuSelectorPosition(true); // move up
						break;
					}
					case KEY_DOWN:{
						updateMenuSelectorPosition(false); //move down
						break;
					}
					case KEY_ESC:{
						quit = true; //quit
						break;
					}
					default:
						break;
				}
			}
		}					
	}
}

void Window::createMenuSelector(){

	if (toolsMap.size() == 0) return;

	DFBWindowDescription swdesc;

	// Creating Window Select
	swdesc.flags = (DFBWindowDescriptionFlags)(DWDESC_POSX | DWDESC_POSY |
											   DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS);
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
	select_window->RequestFocus(select_window);

}

void Window::updateMenuSelectorPosition(bool up){
	int height, width;
	select_window->GetSize(select_window, &width, &height);

	if (up){
		if (position > 1){
			select_window->Move(select_window, 0, -height - 1);
			position--;
		}
		else{
			select_window->Move(select_window, 0, +height * (toolsMap.size()-1) + (toolsMap.size()-1));
			position = toolsMap.size();
		}
	}
	else{
		if (position < toolsMap.size()){
			select_window->Move(select_window, 0, height + 1);
			position++;
		}
		else{
			select_window->Move(select_window, 0, -height * (toolsMap.size()-1) - (toolsMap.size()-1));
			position = 1;
		}
	}
}

void Window::addTool(Tool *tool){
	// The map size will always increase after add the new tool,
	// and it it useful to have its first index being zero
	toolsMap[toolsMap.size()] = tool;
}

void Window::reloadToolMenu(){

	IDirectFBFont *fontsmall;
	DFBFontDescription desc;
	desc.flags = DFDESC_HEIGHT;
	desc.height = (int)(screen_height * 0.02);

	DFBCHECK(dfb->CreateFont(dfb, "decker.ttf", &desc, &fontsmall));
	DFBCHECK(surface->SetFont(surface, fontsmall));
	
	// Side Rectangle // Reloading Side rectangle with new tool
	DFBCHECK(surface->SetColor(surface, 0x00, 0x69, 0x5c, 0xff));
	DFBCHECK(surface->FillRectangle(surface, 0, (int)(screen_height * 0.2), (int)(screen_width * 0.25), screen_height - (int)(screen_height * 0.2)));

	DFBCHECK(surface->SetColor(surface, 0xff, 0xff, 0xff, 0xff));
	for (int i = 0; i < toolsMap.size(); i++)
	{	
		surface->DrawString(surface, toolsMap[i]->name.c_str(), -1, (int)(screen_width * 0.06), (int)(screen_height * 0.25) + (int)(screen_height * 0.07) * i, (DFBSurfaceTextFlags)(DSTF_LEFT | DSTF_TOP));
	}

	fontsmall->Release(fontsmall);

	// Flip to DFB base surface in the original layer
	DFBCHECK(surface->Flip(surface, NULL, (DFBSurfaceFlipFlags)0));

}

void Window::finalize(){
	canvas.finalize();
	event.finalize();

	surface->Release(surface);
	select_surface->Release(select_surface);
	select_window->Release(select_window);
	layer->Release(layer);
	dfb->Release(dfb);
}