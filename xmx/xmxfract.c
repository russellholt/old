#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>
#include <stdio.h>
#include "complex.h"
#include "xmxfract.h"
#include "fract.h"

#define Check(s) fprintf(stderr, "%s\n", s)

void Fractalize(Fract *);
int Loops();
void CheckEvents();

extern Display *disp;
extern GC mygc;
extern unsigned long myfore, myback;
extern Window drawing;
extern char drawn;

int k;	/* iterations */

char	inside=0, Black=1, White=0, EveryOther = 1, colscale,
		julia, stalks, spikes, splat, bitmap, count=0;

unsigned long color;

int		x, y, in, pass, maxiter, PixWidth=0, PixHeight=0,
		ncolors, InColor;

double	left, right, bottom, top, rx, ry, tx, ty, width, cr, ci,
        cy, cx, qd, height, temp, xptemp, yptemp, edist, small,
		stalkconst, dist = 4.0, CenterX, CenterY;

complex Z, X, Y, Zconst, Ztemp, Ztemp2, Ztemp3, Ztemp4, Zold;

Drawable drawer;
Pixmap theImage;
XEvent thevent;

void Fractalize (Fract *T)
{
int res;

/*
 * For speed, set vars from struct
 */

	InColor = myback;
	Black = myfore;
	White = myback;

	julia  = T->Set;		/* 0 = Mandelbrot, 1 = Julia */
	stalks = T->stalks;
	spikes = T->spikes;
	splat = T->splat;
	colscale = T->colortype;
	maxiter = T->Iterations;
	ncolors = T->NColors;	/* # colors */
	bitmap = (ncolors == 2);	/* special case */
	cr = T->ConstR;	/* const : real part */
	ci = T->ConstI;
	CenterX = T->CenterX;
	CenterY = T->CenterY;
	width = T->Width;
	stalkconst = T->BeneathStalks;
	PixWidth = T->PixWidth;
	PixHeight = T->PixHeight;

	drawer = T->D;

	left = CenterX - (width/2); right=CenterX + (width/2);
	bottom = CenterY - width/2;
	top = CenterY + width/2;

	height = top-bottom;
	tx = width/PixWidth; ty = height/PixHeight;
	edist = ( ( (double) ncolors)/maxiter);


	if (julia) {
		Zconst.r = cr;
		Zconst.i = ci;
		}

	if (!drawn) {
		theImage = XCreatePixmap(disp, drawer, PixWidth, PixHeight, 
			DefaultDepth(disp, DefaultScreen(disp)));
		XSetForeground(disp, mygc, myback);
		XFillRectangle(disp, theImage, mygc,
			0, 0, PixWidth, PixHeight);
		}

	res = Loops();
	drawn = 1;

}


int Loops ()
{
XPoint *points, *wpoints;
int npoints=0, nwpoints=0;

	points = (XPoint *) malloc (sizeof(XPoint) * PixWidth * 20);
	wpoints = (XPoint *) malloc (sizeof(XPoint) * PixWidth * 20);

	for (y=0; y<PixHeight; y++) {
        cy = bottom + y*ty;

        for(in=0, x=0; x<PixWidth; x++)
		{
			cx = left + x*tx;

			if (julia) {
				Z.r=cx; Z.i=cy;
				}
			else {
				Zconst.r=cx;
				Zconst.i=cy;
				Z.r=0; Z.i=0;
				}

			for (pass=k=0; k<maxiter; k++) {

				NORMALM;

				if (stalks && (fabs(Z.r) < stalkconst ||
					fabs(Z.i) < stalkconst))
					{
						if (!pass) pass = k;
					}

				qd = Znorm(Z);
				inside = (qd < dist);
				if (!inside)
					break;
				}

		if (colscale)
			color = (int) (edist*k);
		else
			color = k;

		if (inside) color = InColor;

		if (spikes) {
			if (EveryOther && k % 2 == 0) {
				if ((fabs(Z.r) < dist*2) || (fabs(Z.i) < dist*2))
					color = bitmap ? Black : ncolors-color;
				}
			else
				if ((fabs(Z.r) < dist*2) || (fabs(Z.i) < dist*2))
					color = bitmap ? Black : ncolors-color;
			}

		if (stalks && pass) {
				color = !InColor;
			}

		if (color%2 == myfore)
		{
			points[npoints].x = x;
			points[npoints++].y = y;
		}
		else
		{
			wpoints[nwpoints].x = x;
			wpoints[nwpoints++].y = y;
		}

		if (0 != XCheckMaskEvent(disp, ButtonPressMask, &thevent))
		{
			if (thevent.xbutton.window == drawing)
				return 0;	/* quit */
		}

	}	/* for x */

		if (++count == 20 || y >= PixHeight-1)
		{
			count = 0;
/*
			XSetForeground(disp, mygc, myback);
			XFillRectangle(disp, drawer, mygc, 0, y-19, PixWidth, 20);
*/
			XSetForeground(disp, mygc, myfore);
			XDrawPoints(disp, theImage, mygc, points, npoints,
				CoordModeOrigin);
			XSetForeground(disp, mygc, myback);
			XDrawPoints(disp, theImage, mygc, wpoints, nwpoints,
				CoordModeOrigin);
			npoints = nwpoints = 0;
			XClearArea(disp, drawer, 0, 0, 1, 1, True);
/*
			XCopyArea(disp, theImage, drawer, mygc, 0, 0, PixWidth, y,
				0, 0);
*/
			CheckEvents();
		}
	}
	free(points);
	free(wpoints);
	return 1; 	/* finished */
}


void CheckEvents()
{
	/*
	if (0 != XCheckMaskEvent(disp, ExposureMask, &thevent))
		if (thevent.xexpose.window == drawing)
			XCopyArea(disp, theImage, drawing, mygc, 0, 0,
				PixWidth, PixHeight, 0, 0);
	*/

}
