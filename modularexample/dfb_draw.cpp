#include "window.h"

int main(int argc, char **argv){

    Window win;
    win.initiate(&argc,&argv);
    win.runWindow();
    win.finalize();

    return 0;
}