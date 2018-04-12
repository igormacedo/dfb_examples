#ifndef FREEDRAW_H
#define FREEDRAW_H

#include "tool.h"
#include "canvas.h"
#include "event.h"

class FreeDraw: public Tool {
   private:
    public:
        std::string name;

        ~FreeDraw();
        void drawAt(Canvas *canvas, EventHandler *evt);
        //void finalize(); 
};


#endif // FREEDRAW_H