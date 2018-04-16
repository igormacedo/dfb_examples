#ifndef FREEDRAW_H
#define FREEDRAW_H

#include "tool.h"
#include <iostream>

using namespace std;

class Canvas;
class EventHandler;

class FreeDraw : public Tool {
private:
public:
    FreeDraw(std::string str_name);
    ~FreeDraw();
    void drawAt(Canvas* canvas, EventHandler* evt);
    //void finalize();
};

#endif // FREEDRAW_H