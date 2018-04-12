#include "event.h"

void EventHandler::createEventHandler(IDirectFB *dfb){

	dfb->CreateInputEventBuffer(dfb, (DFBInputDeviceCapabilities)(DICAPS_KEYS | DICAPS_BUTTONS),
								DFB_TRUE, &this->buffer);
}

void EventHandler::captureEventTimeout(){

    DFBEvent evt;

    std::cerr << "windows is before Timeout" << std::endl;

	buffer->WaitForEventWithTimeout(buffer, 0, 10);

    std::cerr << "windows is after Timeout" << std::endl;

    if (buffer->GetEvent(buffer, DFB_EVENT(&evt)) == DFB_OK)
    {
        std::cerr << "windows is loopingInSwicth" << std::endl;
        switch(evt.clazz){
            case DFEC_INPUT:
                switch (evt.input.type)
				{
				case DIET_KEYPRESS:
				{
                    this->type = EventType::KEYBOARD_KEYDOWN;
                    this->button = MouseButton::BUTTON_UNKNOWN;

					switch (evt.input.key_id)
					{
					case DIKI_DOWN:{
                        this->key = KEY_DOWN;
						break;
                    }
					case DIKI_UP:{
                        this->key = KEY_UP;
						break;
                    }
					case DIKI_ESCAPE:{
                        this->key = KEY_ESC;
						break;
					}
					default:
                        this->key = KEY_UNKNOWN;
						break;
					}
					break;
				}
                }
        }
    }
}

void EventHandler::finalize(){
    buffer->Release(buffer);
}