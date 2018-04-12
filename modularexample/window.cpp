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

void Window::runCanvas(){
	getchar();
}

void Window::addTool(Tool tool){
	toolsMap[tool.name] = tool;
}

void Window::reloadToolMenu(){
	
	// Side Rectangle // Reloading Side rectangle with new tool
	DFBCHECK(surface->SetColor(surface, 0x00, 0x69, 0x5c, 0xff));
	DFBCHECK(surface->FillRectangle(surface, 0, (int)(screen_height * 0.2), (int)(screen_width * 0.25), screen_height - (int)(screen_height * 0.2)));

	int i = 0;
	for (std::map<std::string,Tool>::iterator it=toolsMap.begin();
		 it != toolsMap.end(); ++it)
	{	
		surface->DrawString(surface, it->first.c_str(), -1, (int)(screen_width * 0.06), (int)(screen_height * 0.25) + (int)(screen_height * 0.07) * i, (DFBSurfaceTextFlags)(DSTF_LEFT | DSTF_TOP));
		i++;
	}
}

void Window::finalize(){

	surface->Release(surface);
	layer->Release(layer);
	dfb->Release(dfb);
}