#include "event.h"

void EventHandler::createEventHandler(IDirectFB* dfb)
{
    dfb->CreateInputEventBuffer(dfb, (DFBInputDeviceCapabilities)(DICAPS_KEYS | DICAPS_BUTTONS),
        DFB_TRUE, &this->buffer);
}

EventStatus EventHandler::captureEventTimeout()
{
    DFBEvent evt;
    buffer->WaitForEventWithTimeout(buffer, 0, 10);

    EventStatus evt_status;
    if (buffer->HasEvent(buffer) == DFB_OK) {
        evt_status = EventStatus::STATUS_HAS_EVENT;
    } else {
        this->type = EventType::UNKOWN;
        evt_status = EventStatus::STATUS_EMPTY;
    }

    if (buffer->GetEvent(buffer, DFB_EVENT(&evt)) == DFB_OK) {
        evt_status = EventStatus::STATUS_HAS_EVENT;
        switch (evt.clazz) {
        case DFEC_INPUT:
            switch (evt.input.type) {
            case DIET_KEYPRESS: {
                this->type = EventType::KEYBOARD_KEYDOWN;
                this->button = MouseButton::BUTTON_UNKNOWN;

                switch (evt.input.key_id) {
                case DIKI_DOWN: {
                    this->key = KEY_DOWN;
                    break;
                }
                case DIKI_UP: {
                    this->key = KEY_UP;
                    break;
                }
                case DIKI_ESCAPE: {
                    this->key = KEY_ESC;
                    break;
                }
                default:
                    this->key = KEY_UNKNOWN;
                    break;
                }
                break;
            }
            case DIET_BUTTONPRESS: {
                this->type = EventType::MOUSE_BUTTONDOWN;
                this->key = KeyboardKey::KEY_UNKNOWN;
                if (evt.input.button == DIBI_LEFT) {
                    this->button = MouseButton::BUTTON_LEFT;
                }
                break;
            }
            case DIET_BUTTONRELEASE: {
                this->type = EventType::MOUSE_BUTTONUP;
                this->key = KeyboardKey::KEY_UNKNOWN;
                if (evt.input.button == DIBI_LEFT) {
                    this->button = MouseButton::BUTTON_UNKNOWN;
                }
                break;
            }
            case DIET_AXISMOTION: {
                this->type = EventType::MOUSE_MOTION;

                switch (evt.input.axis) {
                case DIAI_X: {
                    mouse_px = evt.input.axisabs;
                    break;
                }
                case DIAI_Y: {
                    mouse_py = evt.input.axisabs;
                    break;
                }
                default:
                    break;
                }
                break;
            }
            }
            break;
        }
    }

    return evt_status;
}

void EventHandler::finalize()
{
    buffer->Release(buffer);
}