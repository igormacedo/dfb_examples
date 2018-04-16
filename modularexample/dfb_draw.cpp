#include <string>
// #include <iostream>

// using namespace std;

#include "freeDraw.h"
#include "squareDraw.h"
#include "window.h"

// #define LOG() \
//     cout << __FILE__ << ": line " << __LINE__ << endl;

int main(int argc, char** argv)
{
    Window win;
    win.initiate(&argc, &argv);

    FreeDraw fd_tool(std::string("Free Draw"));
    win.addTool(&fd_tool);
    SquareDraw sd_tool(std::string("Draw Square"));
    win.addTool(&sd_tool);
    win.reloadToolMenu();

    win.runWindow();
    win.finalize();

    return 0;
}

// int main(int argc, char **argv){
//     LOG();
//     cout << __LINE__ << endl;
//     cout << __FILE__ << endl;
//     cout << __func__ << endl;
//     cout << __PRETTY_FUNCTION__ << endl;
// }