#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

char hello[] = {"Hello, World."};
char hi[] = {"Hi!"};

main(argc, argv)
int argc;
char **argv;
{
Display *mydisplay;
Window mywindow;
GC mygc;
XEvent myevent;
KeySym mykey;
XSizeHints myhint;
int myscreen;
unsigned long myforeground, mybackground;
int i;
char text[10];
int done;

		mydisplay = XOpenDisplay("");
		myscreen = DefaultScreen(mydisplay);

		mybackground = WhitePixel(mydisplay, myscreen);
		myforeground = BlackPixel(mydisplay, myscreen);

		myhint.x = 200; myhint.y = 300;
		myhint.width=350; myhint.height = 250;
		myhint.flags = PPosition | PSize;

		mywindow = XCreateSimpleWindow(mydisplay, DefaultRootWindow(mydisplay),
			myhint.x, myhint.y, myhint.width, myhint.height, 5, myforeground,
			mybackground,);
		XSetStandardProperties(mydisplay, mywindow, hello, hello, None,
			argv, argc, &myhint);

		mygc = XCreateGC(mydisplay, mywindow, 0, 0);
		XSetBackground(mydisplay, mygc, mybackground);
		XSetForeground(mydisplay, mygc, myforeground);

		XSelectInput(mydisplay, mywindow,
			ButtonPressMask | KeyPressMask | ExposureMask);

		XMapRaised(mydisplay, mywindow);

		done=0;
		while( done==0) {
			XNextEvent(mydisplay, &myevent);
			switch(myevent.type) {
				case Expose:
					if (myevent.xexpose.count == 0)
					XDrawImageString(
						myevent.xexpose.display, myevent.xexpose.window, mygc,
						50, 50, hello, strlen(hello));
					break;
						
				case MappingNotify:
					XRefreshKeyboardMapping (&myevent);
					break;

				case ButtonPress:
					XDrawImageString(
						myevent.xbutton.display, myevent.xbutton.window, mygc,
						myevent.xbutton.x, myevent.xbutton.y,
						hi, strlen(hi));
					break;

				case KeyPress:
					i = XLookupString(&myevent, text, 10, &mykey, 0);
					if (i == 1 && text[0] = 'q') done = 1;
					break;
				}
			}

		XFreeGC(mydisplay, mygc);
		XDestroyWindow(mydisplay, mywindow);
		XCloseDisplay(mydisplay);
		exit(0);
}
