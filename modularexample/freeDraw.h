#ifndef FREEDRAW_H
#define FREEDRAW_H

#include "tool.h"

class FreeDraw: public Tool {
   public:
      int Draw() {
         return 10; 
      }
};


#endif // FREEDRAW_H