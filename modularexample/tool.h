#ifndef TOOL_H
#define TOOL_H

#include "canvas.h"
#include "event.h"
#include <string>

class Tool {
private:
public:
    std::string name;

    virtual ~Tool(){}; // forces implementation of the destrutor
    virtual void drawAt(Canvas* canvas, EventHandler* evt) = 0; //pure virtual method, cant instantiate class
    //virtual void finalize() = 0;
};

#endif //TOOL_H