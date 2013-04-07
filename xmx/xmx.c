#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <ctype.h>
#include "Mbug.xbm"
#include "xmxquit.xbm"
#include "arbutton.xbm"
#include "ardown.xbm"
#include "xmx.h"
#include "fract.h"

extern char *malloc();
extern char *realloc();
extern void Fractalize();

void FindPic();
void NewString();
void invert();
void Setup();
void Add();
void AddFlip();
void AddT();
void AddTextEdit();
void DrawT();
void DoIt();
void DoAbout();
void DoMoreOptions();
void DoPointer();

extern Pixmap theImage;
extern int PixWidth, PixHeight;


char string1[] = "Drawing goes here", Title[] = "XmX",
	squit[] = "Quit", RunString[] = "Do It", AboutDoneString[] = " Done ",
	credits[]="Russell Holt, July 1993";
char buf[32], DrawMapped=0;

Buttons Run, About, AboutDone;
TextEdit *Active=NULL;

InputWindow mywindow;
Window drawing, quit, AboutDialog, SaveDialog, morestalk, MoreOptions;
Display *disp;
GC mygc;
unsigned long myfore, myback;
Pixmap pat, Mbug, QuitPic, ArrowButton, ArrowDown;
int MoreMapped = 0;
Fract T;
char drawn=0;

main(argc,argv)
int argc;
char *argv[];
{
	XEvent myevent;
	XKeyEvent keyevent;
	XMappingEvent mapevent;
	KeySym mykey;
	XSizeHints myhint, draw_hint, Abhint, MOhint;
	XWMHints mywmhint, draw_wmhint, Abwmhint, MOwmhint;
	XWindowAttributes draw_att, mess_att;
	int myscreen;
	int i, j, ret;
	int lastx, lasty, button, but;
	char text[10], *title="XmX";
	char down_on_quit=0, done=0;
	Font titlef, normf, butf;


	disp = XOpenDisplay(":0");
	myscreen = DefaultScreen(disp);

	myback= WhitePixel(disp,myscreen);
	myfore= BlackPixel(disp,myscreen);

/*
 *   window manager hints
 */

	myhint.x = 200;
	myhint.y = 400;
	myhint.width = myhint.min_width = myhint.min_height = 300;
	myhint.height = myhint.max_width = myhint.max_height = 300;
	myhint.flags = PPosition | PSize | PMinSize | PMaxSize;

	mywmhint.flags = InputHint;
	mywmhint.input = False;

	draw_hint.x = 800;
	draw_hint.y = 400;
	draw_hint.width = draw_hint.max_width = 300;
	draw_hint.height = draw_hint.max_height = 300;
	draw_hint.flags = PPosition | PSize | PMaxSize;

	draw_wmhint.flags = InputHint;
	draw_wmhint.input = False;

	Abhint.x = 200;
	Abhint.y = 400;
	Abhint.width = Abhint.min_width = Abhint.max_width = 200;
	Abhint.height = Abhint.min_height = Abhint.max_height = 100;
	Abhint.flags = PPosition | PSize | PMinSize | PMaxSize;

	Abwmhint.flags = InputHint;
	Abwmhint.input = False;

	MOhint.x = 200;
	MOhint.y = 400;
	MOhint.width = MOhint.min_width = MOhint.max_width = 300;
	MOhint.height = MOhint.min_height = MOhint.max_height = 200;
	MOhint.flags = PPosition | PSize | PMinSize | PMaxSize;

	MOwmhint.flags = InputHint;
	MOwmhint.input = False;

	mywindow.win =
		XCreateSimpleWindow(disp, DefaultRootWindow(disp),
			myhint.x, myhint.y, myhint.width, myhint.height, 5,
			myfore, myback);
	XSetStandardProperties(disp, mywindow.win, " XmX ", " XmX ", None,
		argv, argc, &myhint);
	XSetWMHints(disp, mywindow.win, &mywmhint);
	XSelectInput(disp, mywindow.win, ExposureMask | KeyPressMask);


/* About.. button */

	About.name = Title;
	About.len = strlen(Title);
	About.x = 1; About.y = 1; About.width = 78; About.height = 38;
	About.win = XCreateSimpleWindow(disp, mywindow.win, 1, 1, 78, 38, 2,
									myfore, myback);
	XSelectInput(disp, About.win, ButtonPressMask | ButtonReleaseMask |
		ExposureMask);

/* Do It button */
	Run.name = RunString;
	Run.len = strlen(RunString);
	Run.x=217; Run.y=1; Run.width=78; Run.height=38;
	Run.win = XCreateSimpleWindow(disp, mywindow.win, 217, 1, 78, 38, 2,
									myfore, myback);
	XSelectInput(disp, Run.win, ButtonPressMask | ButtonReleaseMask |
		ExposureMask);

/*
 *	About dialog box stuff
 */
	AboutDialog =
		XCreateSimpleWindow(disp, DefaultRootWindow(disp),
			Abhint.x, Abhint.y, Abhint.width, Abhint.height, 1,
			myfore, myback);
	XSetStandardProperties(disp, AboutDialog, " About XmX", " About ", None,
		argv, argc, &Abhint);
	XSetWMHints(disp, AboutDialog, &Abwmhint);
	XSelectInput(disp, AboutDialog, ButtonPressMask | ButtonReleaseMask |
		ExposureMask);

	AboutDone.x = 60; AboutDone.y = 70;
	AboutDone.width=55; AboutDone.height = 20;
	AboutDone.name = AboutDoneString;
	AboutDone.len = strlen(AboutDoneString);
	AboutDone.win =
		XCreateSimpleWindow(disp, AboutDialog, AboutDone.x,
			AboutDone.y, AboutDone.width, AboutDone.height,
			1, myfore, myback);
	XSelectInput(disp, AboutDone.win, ExposureMask | ButtonPressMask |
		ButtonReleaseMask);

/*- - - - - - - - - - - - - - - 
 *  More options window
 */

	MoreOptions =
		XCreateSimpleWindow(disp, DefaultRootWindow(disp),
			MOhint.x, MOhint.y, MOhint.width, MOhint.height, 1,
			myfore, myback);
	XSetStandardProperties(disp, MoreOptions, "XmX Probe Arcana",
		"XmX Arcana", None, argv, argc, &MOhint);
	XSetWMHints(disp, MoreOptions, &MOwmhint);
	XSelectInput(disp, MoreOptions, ButtonPressMask | ButtonReleaseMask |
		ExposureMask);

	/* more options: EveryOther, InsideOnly, spike ranges */

/* - - - - - - - - - - - - -
 * Drawing window
 */

	drawing =
		XCreateSimpleWindow(disp, DefaultRootWindow(disp),
			draw_hint.x, draw_hint.y, draw_hint.width, draw_hint.height,
			5, myfore, myback);
	XSetStandardProperties(disp, drawing, "Drawing", "XmX", None,
		argv, argc, &draw_hint);
	XSetWMHints(disp, drawing, &draw_wmhint);
	XSelectInput(disp, drawing, ButtonPressMask | KeyPressMask |
		ExposureMask | ButtonReleaseMask);

	PixWidth = draw_hint.width;
	PixHeight = draw_hint.height;




/* Create button/toggle/flip data structures */
	Setup();

/* - - - - - - - - - - - - -
 *	Create Buttons / Toggles / Flip windows
 */
	for (i=0; i<mywindow.nOps; i++) {
		mywindow.ops[i].win =
			XCreateSimpleWindow(disp, mywindow.win, mywindow.ops[i].x,
				mywindow.ops[i].y, mywindow.ops[i].width,
				mywindow.ops[i].height, (mywindow.ops[i].type==Tog) ? 0 : 1,
				myfore, myback);
		XSelectInput(disp, mywindow.ops[i].win, ButtonPressMask |
			ExposureMask | ButtonReleaseMask);
		}


/* - - - - - - - - - - - - -
 *	Create Text Edits
 */
	for (i=0; i<Ntextedit; i++) {
		mywindow.textedit[i].win = XCreateSimpleWindow(disp, mywindow.win,
			mywindow.textedit[i].x, mywindow.textedit[i].y,
			mywindow.textedit[i].width,
			mywindow.textedit[i].height, 1, myfore, myback);
		XSelectInput(disp, mywindow.textedit[i].win, ExposureMask |
			ButtonPressMask | ButtonReleaseMask | KeyPressMask);
		}


	quit = XCreateSimpleWindow(disp, mywindow.win, 300-xmxquit_width,
				300-xmxquit_height, 32, 32, 0, myfore, myback);
	XSelectInput(disp, quit, ButtonPressMask | ExposureMask |
		ButtonReleaseMask);

	morestalk = XCreateSimpleWindow(disp, mywindow.win, 265, 144,
		arbutton_width, arbutton_height, 0, myfore, myback);
	XSelectInput(disp, morestalk, ButtonPressMask | ExposureMask |
		ButtonReleaseMask);


	mygc = XCreateGC(disp, mywindow.win, 0, 0);
	XSetBackground(disp, mygc, myback);
	XSetForeground(disp, mygc, myfore);

/* - - - - - - - - - - - - -
 *	Fonts
 */
	titlef = XLoadFont(disp,
		"-adobe-new century schoolbook-bold-r-*-*-*-240-*-*-*-*-*-*");
	normf = XLoadFont(disp, "-adobe-times-bold-r-*-*-*-140-*-*-*-*-*-*");
	butf = XLoadFont(disp, "-adobe-times-medium-r-*-*-*-140-*-*-*-*-*-*");
	
	XSetFont(disp, mygc, normf);


/* - - - - - - - - - - - - -
 *	Bitmaps, icons, etc.
 */
	pat = XCreateBitmapFromData(disp, mywindow.win, pat_bits,
								pat_width, pat_height);
	XSetStipple(disp, mygc, pat);

	Mbug = XCreateBitmapFromData(disp, mywindow.win, Mbug_bits,
								Mbug_width, Mbug_height);
	QuitPic = XCreateBitmapFromData(disp, quit, xmxquit_bits,
								xmxquit_width, xmxquit_height);

	ArrowButton = XCreateBitmapFromData(disp, mywindow.win, arbutton_bits,
								arbutton_width, arbutton_height);

	ArrowDown = XCreateBitmapFromData(disp, mywindow.win, ardown_bits,
								ardown_width, ardown_height);

	XMapRaised(disp, mywindow.win);
	XMapSubwindows(disp, mywindow.win);

	/*
	theImage = XCreatePixmap(disp, drawing, PixWidth, PixHeight, 
			DefaultDepth(disp, myscreen ));
	*/

	

/* ======================================================================
 *
 *	Event loop
 *
 */

	down_on_quit= 0;

	FindPic();

	while (!done)
	{
		XNextEvent(disp, &myevent);
		switch(myevent.type)
		{
			case Expose:

				if (myevent.xexpose.count == 0) {

					XSetFont(disp, mygc, butf);
					XSetForeground(disp, mygc, myfore);

				/* expose the picture */
					if (myevent.xexpose.window == drawing) {
						if (drawn) {
							/* XCopyArea(disp, theImage, drawing, mygc, 0, 0, PixWidth, PixHeight, 0, 0); */
							}
							printf("expose drawing\n");
						}

					if (myevent.xexpose.window == quit) {
						/*
						XCopyArea(disp, QuitPic, quit, mygc,
							0, 0, xmxquit_width, xmxquit_height, 0, 0);
						*/
						printf("expose quit\n");
					}

					if (myevent.xexpose.window == morestalk) {
						/*
						XCopyArea(disp, ArrowButton, morestalk, mygc,
							0, 0, arbutton_width, arbutton_height, 0, 0);
						*/
						printf("expose morestalk\n");
						}

					if (myevent.xexpose.window == MoreOptions) {
						XDrawString(myevent.xexpose.display,
							MoreOptions, mygc, 50, 50, title, strlen(title));
						printf("expose moreoptions\n");
						}

					for(but=0; but < mywindow.nOps; but++) {

						if (myevent.xexpose.window == mywindow.ops[but].win)
						{
							if (mywindow.ops[but].type == Flip)
							{
								XSetFillStyle(disp, mygc, FillSolid);
								XDrawImageString(myevent.xexpose.display,
									mywindow.ops[but].win, mygc, 5, 14,
									mywindow.ops[but].state ?
										mywindow.ops[but].name2 :
										mywindow.ops[but].name,
									mywindow.ops[but].state ?
										mywindow.ops[but].slen :
										mywindow.ops[but].len);
								XDrawLine(myevent.xexpose.display,
									mywindow.win, mygc, mywindow.ops[but].x+1,
									mywindow.ops[but].y +
									mywindow.ops[but].height + 2,
									mywindow.ops[but].x + 2 +
									mywindow.ops[but].width,
									mywindow.ops[but].y +
									mywindow.ops[but].height+2);
								XDrawLine(myevent.xexpose.display,
									mywindow.win, mygc,
									mywindow.ops[but].x + 2 +
									mywindow.ops[but].width,
									mywindow.ops[but].y +
									mywindow.ops[but].height+2,
									mywindow.ops[but].x + 2+
									mywindow.ops[but].width,
									mywindow.ops[but].y + 1);
							}
							else	/* not a flipper */
							{
								if (mywindow.ops[but].type == Tog) /* toggle */
								{
									XSetFillStyle(disp, mygc, FillSolid);
									DrawT(&mywindow, but);
									XDrawString(myevent.xexpose.display,
										mywindow.ops[but].win, mygc, 10, 14,
										mywindow.ops[but].name,
										mywindow.ops[but].len);
								}
								else	/* push button */
								{
									XSetFillStyle(disp, mygc,
										mywindow.ops[but].state ?
											FillSolid : FillStippled);
									XDrawString(myevent.xexpose.display,
										mywindow.ops[but].win, mygc, 4, 14,
										mywindow.ops[but].name,
										mywindow.ops[but].len);
									}


							}	/* else: if type == Flip */

							}

						}	/* for (search through buttons) */

					XSetFillStyle(disp, mygc, FillSolid);
					for (i=0; i<Ntextedit; i++) {
					if (myevent.xexpose.window == mywindow.textedit[i].win) {

						if (mywindow.textedit[i].string != NULL) {
							XSetFont(disp, mygc, butf);
							XSetForeground(disp, mygc, myfore);
							XDrawImageString(myevent.xexpose.display,
								mywindow.textedit[i].win, mygc, 5, 14,
								mywindow.textedit[i].string,
								mywindow.textedit[i].len);

							if (&(mywindow.textedit[i]) == Active) {
								XSetFillStyle(disp, mygc, FillStippled);
								Boxit(On, Active->win, Active->width,
									Active->height);
								XSetFillStyle(disp, mygc, FillSolid);
								}

							}	/* string != NULL */
						}	/* exposed window */

						}	/* for */

					if (myevent.xexpose.window == About.win) {
						XSetFont(disp, mygc, titlef);
						XSetFillStyle(disp, mygc, FillStippled);
						XDrawString(myevent.xexpose.display,
							About.win, mygc, 2, 30, About.name,
							About.len);
						XSetFillStyle(disp, mygc, FillSolid);
						XDrawString(myevent.xexpose.display,
							About.win, mygc, 5, 27, About.name,
							About.len);
						}

					if (myevent.xexpose.window == AboutDialog) {
						XSetFont(disp, mygc, titlef);
						XSetFillStyle(disp, mygc, FillStippled);
						XDrawString(myevent.xexpose.display,
							AboutDialog, mygc, 2, 30, About.name,
							About.len);
						XSetFillStyle(disp, mygc, FillSolid);
						XDrawString(myevent.xexpose.display,
							AboutDialog, mygc, 5, 27, About.name,
							About.len);
						XSetFont(disp, mygc, normf);
						XDrawString(myevent.xexpose.display,
							AboutDialog, mygc, 10, 45, credits,
							strlen(credits));
						}

					if (myevent.xexpose.window == AboutDone.win) {
						XSetFont(disp, mygc, normf);
						XDrawString(myevent.xexpose.display,
							AboutDone.win, mygc, 2, 15, AboutDone.name,
							AboutDone.len);
						}

					if (myevent.xexpose.window == Run.win) {
						XSetFont(disp, mygc, titlef);
						XSetFillStyle(disp, mygc, FillStippled);
						XDrawString(myevent.xexpose.display,
							Run.win, mygc, 7, 30, Run.name,
							Run.len);
						XSetFillStyle(disp, mygc, FillSolid);
						XDrawString(myevent.xexpose.display,
							Run.win, mygc, 10, 27, Run.name,
							Run.len);
						}

					if (myevent.xexpose.window == mywindow.win) {

							XSetFillStyle(disp, mygc, FillStippled);
							XDrawLine(myevent.xbutton.display, mywindow.win,
								mygc, 10, 135, 7, 135);
							XDrawLine(myevent.xbutton.display, mywindow.win,
								mygc, 5, 135, 5, 190);
							XDrawLine(myevent.xbutton.display, mywindow.win,
								mygc, 5, 190, 295, 190);
							XDrawLine(myevent.xbutton.display, mywindow.win,
								mygc, 295, 190, 295, 135);
							XDrawLine(myevent.xbutton.display, mywindow.win,
								mygc, 295, 135, 55, 135);

						/* XDrawRectangle(myevent.xbutton.display, mywindow.win,
							 	mygc, 7, 137, 45, 20); */
							XSetFillStyle(disp, mygc, FillSolid);

						/* XCopyArea(disp, Mbug, mywindow.win, mygc,
								0, 0, Mbug_width, Mbug_height, 240, 5); */

							XSetFont(disp, mygc, normf);
							for (i=0; i<Ntext; i++) {
								XSetFillStyle(disp, mygc,
									mywindow.txt[i].state ?
									FillSolid : FillStippled);
								if (mywindow.txt[i].method = Trans)
									XDrawString(myevent.xexpose.display,
										mywindow.win, mygc,
										mywindow.txt[i].x,
										mywindow.txt[i].y,
										mywindow.txt[i].name,
										mywindow.txt[i].len);
								else
									XDrawImageString(myevent.xexpose.display,
										mywindow.win, mygc,
										mywindow.txt[i].x,
										mywindow.txt[i].y,
										mywindow.txt[i].name,
										mywindow.txt[i].len);
								}

							break;
						}

					XSetFillStyle(disp, mygc, FillSolid);
					}	/* expose */


				break;

			case MappingNotify:
				XRefreshKeyboardMapping(&mapevent);
				break;

			case ButtonPress:
					if (myevent.xbutton.window == quit &&
						myevent.xbutton.button == 1) {
						/* Boxit(On, quit, 40, 20); */
						XSetForeground(disp, mygc, myfore);
						XDrawLine(disp, quit, mygc, 2, 30, 30, 2);
						XDrawLine(disp, quit, mygc, 30, 2, 30, 30);
						XDrawLine(disp, quit, mygc, 30, 30, 2, 30);
						down_on_quit = 1;
						}
					else {
					down_on_quit = 0;

					if ((myevent.xbutton.button == 1) &&
						(myevent.xbutton.window == About.win ||
						myevent.xbutton.window == Run.win)) {
						Boxit(On, myevent.xbutton.window, 78, 38);
						break;
						}

					if (myevent.xbutton.window == drawing) {
						DoPointer(myevent.xbutton.button);
						}

					if ((myevent.xbutton.button == 1) &&
						(myevent.xbutton.window == morestalk)) {
						/*
						XCopyArea(disp, ArrowDown, morestalk, mygc,
							0, 0, ardown_width, ardown_height, 0, 0);
						*/
						}

					for(i=0; i<Ntextedit; i++)
						if (myevent.xbutton.window ==
								mywindow.textedit[i].win &&
								myevent.xbutton.button == 1) {
							if (Active != &(mywindow.textedit[i])) {
								XSetFillStyle(disp, mygc, FillStippled);
								Boxit(On, mywindow.textedit[i].win,
									mywindow.textedit[i].width,
									mywindow.textedit[i].height);
								XSetFillStyle(disp, mygc, FillSolid);
								Boxit(Off, Active->win, Active->width,
									Active->height);
								Active = &(mywindow.textedit[i]);
								} /* if active != new text edit box */
							break;
							}
					}

						for(but=0; but< mywindow.nOps; but++) {
							if (myevent.xbutton.window == mywindow.ops[but].win
								&& myevent.xbutton.button == 1) {
								if (mywindow.ops[but].type == Flip || 
										mywindow.ops[but].type == Tog ||
										mywindow.ops[but].state)
									invert(&mywindow, but, 1);
								break;
								}
								
							}

						if (myevent.xbutton.window == AboutDone.win) {
							AboutDone.state = 1;
							Boxit(On, AboutDone.win, AboutDone.width,
								AboutDone.height);
							}

					break;

			case ButtonRelease:
					if (myevent.xbutton.button == 1)
						if (down_on_quit && myevent.xbutton.window == quit) {
							done = 1;
							/* Boxit(Off, quit, 40, 20); */
							XSetForeground(disp, mygc, myback);
							XDrawLine(disp, quit, mygc, 2, 30, 30, 2);
							XDrawLine(disp, quit, mygc, 30, 2, 30, 30);
							XDrawLine(disp, quit, mygc, 30, 30, 2, 30);
							}
						else {
						if (myevent.xbutton.window == Run.win) {
							Boxit(Off, Run.win, 78, 38);
							DoIt();
							}

						if (myevent.xbutton.window == About.win) {
							Boxit(Off, About.win, 78, 38);
							DoAbout();
							}
						if (myevent.xbutton.window == morestalk) {
							/*
							XCopyArea(disp, ArrowButton, morestalk, mygc,
								0, 0, arbutton_width, arbutton_height, 0, 0);
							*/
							DoMoreOptions();
							}

						if (myevent.xbutton.window == AboutDone.win) {
							if (AboutDone.state == 1) {
								Boxit(Off, AboutDone.win, AboutDone.width,
									AboutDone.height);
								XUnmapWindow(disp, AboutDialog);
								}
							}

						else
						for(but=0; but < mywindow.nOps; but++) {

							if (myevent.xbutton.window ==
									mywindow.ops[but].win) {
								if (mywindow.ops[but].type ==
										But && !mywindow.ops[but].state)
									break;
								invert(&mywindow, but, 1);

								if (mywindow.ops[but].type != But)
									mywindow.ops[but].state =
										!mywindow.ops[but].state;

								invert(&mywindow, but, 0);

								if (but == CLEAR) {
									if (drawn) {
										XClearArea(disp, drawing, 0, 0,
											PixWidth, PixHeight, 0);
										XSetForeground(disp, mygc, myback);
										XFillRectangle(disp, theImage, mygc,
											0, 0, PixWidth, PixHeight);
										}
									}

				/* generate expose events to flip-able buttons */
								if (mywindow.ops[but].type == Flip) {
									XClearArea(disp, mywindow.ops[but].win,
										0, 0, mywindow.ops[but].width,
										mywindow.ops[but].height,
										True);	/* true means expose */
									}
								break;
								}
							}	/* for */
						}
					break;

			case MotionNotify:
					break;

			case KeyPress:
				i = XLookupString((XKeyEvent *) &myevent, text, 10, &mykey, 0);
				/* if ((i==1) && (text[0]=='q'))
					done = 1; */
				if (i==1) {
					if (text[0] == 0x08 || text[0] == 0x7f) {
						/* backspace or delete */
						if (Active->len > 0) {
							Active->string[Active->len - 1] = '\0';
							(Active->len)--;
							XClearArea(disp, Active->win, 1, 1,
								Active->width-2, Active->height-2, True);
							}
						}
					else
						if (text[0] == 21) {
							Active->len = 0;
							free(Active->string);
							Active->string = (char *) malloc(sizeof(char));
							Active->string[0] = '\0';
							XClearArea(disp, Active->win, 1, 1,
								Active->width-2, Active->height-2, True);
						}
					else
						if (isprint(text[0]) && Active->len < Active->maxlen) {
							(Active->len)++;
							Active->string = (char *) realloc(Active->string,
								((Active->len) * sizeof(char) + 1));
							sprintf(buf, "%s%c", Active->string, text[0]);
							(void *) strcpy(Active->string, buf);
							XClearArea(disp, Active->win, 1, 1, 2, 2, True);
					} /* else : not backspace or del */
					}	/* one char came from lookupstring */
				break;
		}
	}

/*- - - - - - - - - - - -
 *  Done
 */

	XFreeGC(disp,mygc);
	XDestroyWindow(disp,drawing);
	XDestroyWindow(disp,mywindow.win);
	if (drawn)
		XFreePixmap(disp, theImage);
	XFreePixmap(disp, pat);
	XFreePixmap(disp, Mbug);
	XFreePixmap(disp, QuitPic);
	XFreePixmap(disp, ArrowButton);
	XFreePixmap(disp, ArrowDown);
	XCloseDisplay(disp);

	exit(0);
}


void DrawT(Iwin, but)
InputWindow *Iwin;
int but;
{

	if (Iwin->ops[but].state) {
		XSetForeground(disp, mygc, myfore);
		XFillRectangle(disp, Iwin->ops[but].win, mygc, 1, 5, 6, 6);
		}
	else {
		XSetForeground(disp, mygc, myback);
		XFillRectangle(disp, Iwin->ops[but].win, mygc, 1, 5, 6, 6);

		XSetForeground(disp, mygc, myfore);
		XDrawRectangle(disp, Iwin->ops[but].win, mygc, 1, 5, 6, 6);
		}
}



void invert(Iwin, but, which)
InputWindow *Iwin;
int but, which;
{

	if (Iwin->ops[but].type == But || Iwin->ops[but].type == Flip) {
		Boxit(which, Iwin->ops[but].win, Iwin->ops[but].width,
			Iwin->ops[but].height);
		}
	else
		DrawT(Iwin, but);
}


void Setup()
{
	int i;

	mywindow.nOps = Nops;
	mywindow.nText = Ntext;
	mywindow.nTextedit = Ntextedit;

	mywindow.ops = (Buttons *) malloc(sizeof(Buttons) * mywindow.nOps);
	mywindow.txt = (Screentext *) malloc(sizeof(Screentext) * mywindow.nText);
	mywindow.textedit = (TextEdit *) malloc(sizeof(TextEdit) *
		mywindow.nTextedit);

	AddT(&mywindow, 0, "Set", 90, 25, Black, Trans);
	AddFlip(JorM, "Mandelbrot", "Julia", 120, 10, 80, 18);

	AddT(&mywindow, 1, "Center:  X:", 5, 65, Black, Trans);
	AddTextEdit(teCENTERX, "-0.5", 78, 50, 90, 18, 12);
	AddT(&mywindow, 2, "Y:", 178, 65, Black, Trans);
	AddTextEdit(teCENTERY, "0.0", 195, 50, 90, 18, 12);

	AddT(&mywindow, 3, "Width:", 5, 90, Black, Trans);
	AddTextEdit(teWIDTH, "3", 60, 75, 80, 18, 10);

	AddT(&mywindow, 4, "Iterations:", 155, 90, Black, Trans);
	AddTextEdit(teITER, "50", 235, 75, 50, 18, 5);

	AddT(&mywindow, 5, "Const:", 5, 115, Black, Trans);
	AddT(&mywindow, 6, "r:", 60, 115, Black, Trans);
	AddTextEdit(teCONSTr, "0.566666", 78, 100, 90, 18, 12);
	AddT(&mywindow, 7, "i:", 178, 115, Black, Trans);
	AddTextEdit(teCONSTi, "0.0", 195, 100, 90, 18, 12);

	AddT(&mywindow, 8, "Probe", 15, 140, Black, Image);
	Add(STALKS, "Stalks", Tog, Off, 93, 142, 45, 18);
	AddT(&mywindow, 9, "beneath", 144, 157, Black, Trans);
	AddTextEdit(teSTALK, "0.003", 200, 142, 60, 18, 8);

	Add(SPIKES, "Spikes", Tog, Off, 15, 142, 60, 18);
	Add(SCALECOL, "Scaled Colors", Tog, Off, 15, 165, 100, 18);
	AddT(&mywindow, 10, "of", 115, 178, Black, Image);
	AddTextEdit(teCOLORS, "32", 140, 165, 40, 18, 3);
	Add(SPLAT, "Splat", Tog, Off, 225, 165, 40, 18);

	AddT(&mywindow, 11, "Pixmap  Width", 5, 220, Black, Image);
	AddTextEdit(tePIXx, "300", 105, 205, 40, 18, 4);
	AddT(&mywindow, 12, "Height", 155, 220, Black, Trans);
	AddTextEdit(tePIXy, "300", 205, 205, 40, 18, 4);
	Add(RESIZE, "Apply", But, On, 255, 205, 40, 18);

	Add(SAVE, "Save", But, On, 10, 270, 40, 18);
	Add(LOAD, "Load", But, On, 60, 270, 40, 18);
	Add(CLEAR, "Clear", But, On, 150, 270, 45, 18);

	Active = &(mywindow.textedit[0]);
}


void Add(i, name, type, state, x, y, w, h)
int i, x, y, w, h;
char *name;
char type, state;
{


	mywindow.ops[i].type = type;
	mywindow.ops[i].len = strlen(name);
	mywindow.ops[i].name = (char *) malloc(sizeof(char) *
		(mywindow.ops[i].len + 1));
	(void *) strcpy(mywindow.ops[i].name, name);
	mywindow.ops[i].state = state;
	mywindow.ops[i].x = x; mywindow.ops[i].y = y;
	mywindow.ops[i].width = w;
	mywindow.ops[i].height = h;

}


/* two state buttons: change their name */
void AddFlip(i, name1, name2, x, y, w, h)
int i, x, y, w, h;
char *name1, *name2;
{

	Add(i, name1, Flip, 0, x, y, w, h);

	mywindow.ops[i].slen = strlen(name2);
	mywindow.ops[i].name2 = (char *) malloc(sizeof(char) *
		(mywindow.ops[i].slen + 1));
	(void *) strcpy(mywindow.ops[i].name2, name2);
}


void AddT(Iwin, i, name, x, y, state, method)
InputWindow *Iwin;
int i;
char *name;
int x, y;
char state, method;
{

	Iwin->txt[i].len = strlen(name);
	Iwin->txt[i].name = (char *) malloc(sizeof(char) * (Iwin->txt[i].len + 1));
	(void *) strcpy(Iwin->txt[i].name, name);
	Iwin->txt[i].x = x; Iwin->txt[i].y = y;
	Iwin->txt[i].state = state; Iwin->txt[i].method = method;
}


void AddTextEdit(i, name, x, y, w, h, maxlen)
int i, x, y, w, h, maxlen;
{

	mywindow.textedit[i].x = x;
	mywindow.textedit[i].y = y;
	mywindow.textedit[i].width = w;
	mywindow.textedit[i].height = h;
	mywindow.textedit[i].maxlen = maxlen;
	mywindow.textedit[i].len = strlen((char *) name);
	mywindow.textedit[i].string = (char *) malloc(sizeof (char) *
						(mywindow.textedit[i].len + 2));
	sprintf(mywindow.textedit[i].string, "%s", (char *) name);
	/* (void *) strcpy(mywindow.textedit[i].string, (char *) name); */
}



void FindPic()
{

	T.Equations = 0;
	T.Set = mywindow.ops[JorM].state;
	T.stalks = mywindow.ops[STALKS].state;
	T.spikes = mywindow.ops[SPIKES].state;
	T.splat = mywindow.ops[SPLAT].state;
	T.colortype = mywindow.ops[SCALECOL].state;
	sscanf(mywindow.textedit[teCENTERX].string, "%lf", &(T.CenterX));
	sscanf(mywindow.textedit[teCENTERY].string, "%lf", &(T.CenterY));
	sscanf(mywindow.textedit[teWIDTH].string, "%lf", &(T.Width));
	sscanf(mywindow.textedit[teCONSTr].string, "%lf", &(T.ConstR));
	sscanf(mywindow.textedit[teCONSTi].string, "%lf", &(T.ConstI));
	sscanf(mywindow.textedit[teITER].string, "%d", &(T.Iterations));
	sscanf(mywindow.textedit[teCOLORS].string, "%d", &(T.NColors));
	sscanf(mywindow.textedit[teSTALK].string, "%lf", &(T.BeneathStalks));
	sscanf(mywindow.textedit[tePIXx].string, "%d", &(T.PixWidth));
	sscanf(mywindow.textedit[tePIXy].string, "%d", &(T.PixHeight));

	T.D = drawing;	/* drawable */
}

void DoIt()
{
	printf("DoIt()\n");
	FindPic();

	if (!DrawMapped) {
		DrawMapped = 1;
		XMapRaised(disp, drawing);
		}

	fprintf(stderr, "Set: %s\n", (T.Set)?"Julia":"Mandelbrot");
	fprintf(stderr, "Center: (%lf, %lf) width: %lf\n", T.CenterX,
		T.CenterY, T.Width);
	fprintf(stderr, "const: (%lf, %lf) %d iterations  %d colors\n",
		T.ConstR, T.ConstI, T.Iterations, T.NColors);
	fprintf(stderr, "stalks: %s beneath %lf\n", (T.stalks)?"yes":"no",
		T.BeneathStalks);
	fprintf(stderr, "%s%s%s", (T.spikes)?"spikes ":"", (T.splat)?"splat ":"",
		(T.colortype)?"scaled colors":"");
	fprintf(stderr, "  Pixmap (w x h): (%d x %d)\n", T.PixWidth, T.PixHeight);

	XCopyArea(disp, Mbug, mywindow.win, mygc,
		0, 0, Mbug_width, Mbug_height, 205, 260);

	Fractalize(&T);

	XClearArea(disp, mywindow.win, 205, 260,
		Mbug_width, Mbug_height, 0);
}


void DoAbout()
{

	XMapRaised(disp, AboutDialog);
	XMapSubwindows(disp, AboutDialog);

}


void DoMoreOptions()
{

	if (MoreMapped)
		XUnmapWindow(disp, MoreOptions);
	else {
		XMapRaised(disp, MoreOptions);
	/* XMapSubwindows(disp, MoreOptions); */
		}

	MoreMapped = !MoreMapped;
}


void NewString(int which, char *src)
{

	free(mywindow.textedit[which].string);

	mywindow.textedit[which].len = strlen(src);
	mywindow.textedit[which].string =
		(char *) malloc (sizeof(char) * (mywindow.textedit[which].len + 1));

	(void *) strcpy(mywindow.textedit[which].string, src);
}


void DoPointer(int button)
{
int ret, rootX, rootY, winX, winY, PW, PH;
double cx, cy, width, newx, newy;
unsigned int masks;
Window childReturn, rootReturn;
char bufx[30], bufy[30];

	ret = XQueryPointer(disp, drawing, &rootReturn,
		&childReturn, &rootX, &rootY, &winX, &winY,
		&masks);


	newx = (T.CenterX - T.Width/2) + (double) winX *
		((double)T.Width / T.PixWidth);
	newy = (T.CenterY - T.Width/2) + (double) winY *
		((double)T.Width / T.PixHeight);

	sprintf(bufx, "%12.8lf", newx);
	sprintf(bufy, "%12.8lf", newy);


	switch (button)
	{
	case 1:
		NewString(teCENTERX, bufx);
		NewString(teCENTERY, bufy);
		XClearArea(disp, mywindow.textedit[teCENTERX].win, 0, 0,
			mywindow.textedit[teCENTERX].width,
			mywindow.textedit[teCENTERX].height, True);
		XClearArea(disp, mywindow.textedit[teCENTERY].win, 0, 0,
			mywindow.textedit[teCENTERY].width,
			mywindow.textedit[teCENTERY].height, True);
		break;
	case 2:
		NewString(teCONSTr, bufx);
		NewString(teCONSTi, bufy);
		XClearArea(disp, mywindow.textedit[teCONSTr].win, 0, 0,
			mywindow.textedit[teCONSTr].width,
			mywindow.textedit[teCONSTr].height, True);
		XClearArea(disp, mywindow.textedit[teCONSTi].win, 0, 0,
			mywindow.textedit[teCONSTi].width,
			mywindow.textedit[teCONSTi].height, True);
		break;
	case 3:
		break;
	default: ;
	}

}
