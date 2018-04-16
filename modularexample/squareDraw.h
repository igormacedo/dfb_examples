#ifndef SQUAREDRAW_H
#define SQUAREDRAW_H

#include "canvas.h"
#include "event.h"
#include "tool.h"
#include <iostream>

using namespace std;

class SquareDraw : public Tool {
private:
    int initialRecX;
    int initialRecY;
    int finalRecX;
    int finalRecY;

public:
    SquareDraw(std::string str_name);
    ~SquareDraw();
    void drawAt(Canvas* canvas, EventHandler* evt);
    //void finalize();
};

#endif // SQUAREDRAW_H