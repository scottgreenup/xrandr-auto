
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <X11/extensions/Xrandr.h>
#include <X11/Xlib.h>

using namespace std;

// X = X11, R=RandR, A=Auto
namespace xra {

class Screen {
public:
    Screen(Display* display, Window rootWindow, int screen)
    : m_display(display)
    , m_rootWindow(rootWindow)
    , m_screen(screen) {
        refreshSize();
    }

    void refreshSize() {
        Status result = XRRGetScreenSizeRange(
            m_display,
            m_rootWindow,
            &m_minWidth,
            &m_minHeight,
            &m_maxWidth,
            &m_maxHeight);

        if (result != 1) {
            throw runtime_error("Could not get range of screen size.");
        }

        // TODO what is the difference?
        //*screen_resources = XRRGetScreenResources(m_display, m_rootWindow);
        m_resources = XRRGetScreenResourcesCurrent(m_display, m_rootWindow);

        if (!m_resources) {
            throw runtime_error("Could not get screen resources.");
        }
    }

    int width() const {
        return DisplayWidth(m_display, m_screen);
    }

    int height() const {
        return DisplayHeight(m_display, m_screen);
    }

    friend std::ostream& operator<<(std::ostream&, const Screen&);

private:

    XRRScreenResources *m_resources;

    Display *m_display;
    Window m_rootWindow;

    int m_screen;
    int m_minWidth;
    int m_minHeight;
    int m_maxWidth;
    int m_maxHeight;
};

std::ostream& operator<<(std::ostream &ss, const Screen &k) {
  return ss
      << "Screen " << k.m_screen << ": "
      << "minimum " << k.m_minWidth << " x " << k.m_minHeight << ", "
      << "current " << k.width() << " x " << k.height() << ", "
      << "maximum " << k.m_maxWidth << " x " << k.m_maxHeight;
}

}

int main(int argc, char** argv) {

    const char* display_name = NULL;

    // Get a reference to the X server managing the display. If NULL is provided
    // as the display_name, defaults to environment variable 'DISPLAY'.
    Display *display = XOpenDisplay(display_name);

    if (display == NULL) {
        cerr << "Could not open display " << XDisplayName(display_name) << endl;
        return 1;
    }

    cerr << "Opened display " << XDisplayName(display_name) << endl;

    int screen = DefaultScreen(display);
    Window rootWindow = RootWindow(display, screen);

    // TODO validate extension and version of RandR

    int event_base;
    int error_base;
    int major_version;
    int minor_version;

    if (!XRRQueryExtension(display, &event_base, &error_base)
    ||  !XRRQueryVersion(display, &major_version, &minor_version)) {
        cerr << "RandR extension is missing." << endl;
    }

    cout << "RandR v" << major_version << "." << minor_version << endl;

    xra::Screen kScreen(display, rootWindow, screen);
    cout << kScreen << endl;

    cout << "Thank you. " << endl;


    return 0;
}

