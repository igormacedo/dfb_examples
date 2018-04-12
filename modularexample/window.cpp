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
	
	event = EventHandler();
	bool quit = false;

	std::cerr << "windows is running" << std::endl;

	while(!quit){

		std::cerr << "windows is looping" << std::endl;
		event.captureEventTimeout(); 

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

void Window::createMenuSelector(){

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
	select_window->AttachEventBuffer(select_window, event.buffer);
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
			select_window->Move(select_window, 0, +height * 5 + 5);
			position = 6;
		}
	}
	else{
		if (position < 6){
			select_window->Move(select_window, 0, height + 1);
			position++;
		}
		else{
			select_window->Move(select_window, 0, -height * 5 - 5);
			position = 1;
		}
	}
}

void Window::addTool(Tool *tool){
	toolsMap[toolsMap.size()+1] = tool;
}

void Window::reloadToolMenu(){
	
	// Side Rectangle // Reloading Side rectangle with new tool
	DFBCHECK(surface->SetColor(surface, 0x00, 0x69, 0x5c, 0xff));
	DFBCHECK(surface->FillRectangle(surface, 0, (int)(screen_height * 0.2), (int)(screen_width * 0.25), screen_height - (int)(screen_height * 0.2)));

	for (unsigned int i = 0; i < toolsMap.size(); i++)
	{	
		surface->DrawString(surface, toolsMap[i]->name.c_str(), -1, (int)(screen_width * 0.06), (int)(screen_height * 0.25) + (int)(screen_height * 0.07) * (signed) i, (DFBSurfaceTextFlags)(DSTF_LEFT | DSTF_TOP));
	}
}

void Window::finalize(){
	canvas.finalize();
	event.finalize();

	surface->Release(surface);
	layer->Release(layer);
	dfb->Release(dfb);
}