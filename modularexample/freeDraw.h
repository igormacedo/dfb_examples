#ifndef FREEDRAW_H
#define FREEDRAW_H

#include "canvas.h"
#include "event.h"
#include "tool.h"
#include <iostream>

using namespace std;

class FreeDraw : public Tool {
private:
public:
    FreeDraw(std::string str_name);
    ~FreeDraw();
    void drawAt(Canvas* canvas, EventHandler* evt);
    //void finalize();
};

#endif // FREEDRAW_H