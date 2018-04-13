#ifndef FREEDRAW_H
#define FREEDRAW_H

#include "canvas.h"
#include "event.h"
#include "tool.h"

class FreeDraw : public Tool {
private:
public:
    FreeDraw(std::string str_name);
    ~FreeDraw();
    void drawAt(Canvas* canvas, EventHandler* evt);
    //void finalize();
};

#endif // FREEDRAW_H