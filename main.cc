
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xinerama.h>
#include <X11/Xlib.h>

using namespace std;


#define INTERSECT(x,y,w,h,r)  (MAX(0, MIN((x)+(w),(r).x_org+(r).width)  - MAX((x),(r).x_org)) \
                             * MAX(0, MIN((y)+(h),(r).y_org+(r).height) - MAX((y),(r).y_org)))
#define MIN(a,b)              ((a) < (b) ? (a) : (b))
#define MAX(a,b)              ((a) > (b) ? (a) : (b))

int main(int argc, char** argv) {

    const char* display_name = NULL;

    Display *display = XOpenDisplay(display_name);

    if (display == NULL) {
        cerr << "Could not open display " << XDisplayName(display_name) << endl;
        return 1;
    }

    int screen = DefaultScreen(display);
    Window rootWindow = RootWindow(display, screen);

    XineramaScreenInfo *info;

    int n;

    if (!(info = XineramaQueryScreens(display, &n))) {
        return 1;
    }


    int di;
    Window dw, w;

    int x, y;
    unsigned int du;

    XGetInputFocus(display, &w, &di);
    XQueryPointer(display, rootWindow, &dw, &dw, &x, &y, &di, &di, &du);

    for (int i = 0; i < n; i++) {
        if (INTERSECT(x, y, 1, 1, info[i])) {
            cout << info[i].width << endl;
            return 0;
        }
    }

    XFree(info);

    return 0;
}

