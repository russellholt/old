/* Button Types */
#define Tog 1
#define But 0
#define Flip 2

/* number of buttons, general text, text edit items */
#define Nops 9
#define Ntext 14
#define Ntextedit 10

/* Button functions */
#define JorM 0
#define STALKS 1
#define SPIKES 2
#define SPLAT 3
#define SCALECOL 4
#define SAVE 5
#define LOAD 6
#define RESIZE 7
#define CLEAR 8

/* text edits */
#define teCENTERX 0
#define teCENTERY 1
#define teSTALK 2
#define teWIDTH 3
#define teCONSTr 4
#define teCONSTi 5
#define teITER 6
#define tePIXx 7
#define tePIXy 8
#define teCOLORS 9

/* button states */
#define On 1
#define Off 0

/* how to draw static text */
#define Black 1
#define Grey 0
#define Trans 1
#define Image 0

#define Boxit(which, win, w, h)	\
	XSetForeground(disp, mygc, (which)?myfore:myback);	\
	XDrawRectangle(disp, win, mygc, 0, 0, (w)-1, (h)-1);

/* stipple pattern */
#define pat_width  8
#define pat_height 8
static char pat_bits[] = { 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa};

typedef struct {
	Window win;
	int x, y, width, height;
	char *name, *name2;
	char type;
	char state;
	char len, slen;
} Buttons;

typedef struct {
	char *name,
		len,
		state,	/* 0 = grey, 1 = black */
		method;	/* 0 = DrawImageString, 1 = DrawString */
	int x, y;
} Screentext;

typedef struct {
	Window win;
	int x, y, width, height;
	char *string;
	char len, maxlen;
} TextEdit;

typedef struct {
	Window win;
	int nOps, nText, nTextedit;
	Buttons *ops;
	Screentext *txt;
	TextEdit *textedit;
} InputWindow;
