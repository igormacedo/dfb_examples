#ifndef EVENT_H
#define EVENT_H

enum EventType {
    UNKOWN = 0x01,
    KEYBOARD_KEYDOWN = 0x02,
    KEYBOARD_KEYUP = 0x04,
    MOUSE_BUTTONDOWN = 0x08,
    MOUSE_BUTTONUP = 0x10,
    MOUSE_MOTION = 0x20
};

enum KeyboardKey {
    KEY_UNKNOWN = 0x01,
    KEY_UP = 0x02,
    KEY_DOWN = 0x04,
    KEY_ESC = 0x08
};

enum MouseButton {
    BUTTON_UNKNOWN = 0x01,
    BUTTON_LEFT = 0x02
};

enum EventStatus {
    STATUS_HAS_EVENT = 0x01,
    STATUS_EMPTY = 0x02
};

#include "directfb.h"
#include <iostream>

class EventHandler {
private:
public:
    EventType type;
    KeyboardKey key;
    MouseButton button;
    int mouse_px;
    int mouse_py;

    IDirectFBEventBuffer* buffer;

    void createEventHandler(IDirectFB* dfb);
    EventStatus captureEventTimeout();
    void finalize();
};

#endif //EVENT_H