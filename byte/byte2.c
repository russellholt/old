/*
 * A byte editor for files
 *
 * cursor movement almost perfect.
 * Does not edit or save yet.
 *
 * by Russell Holt June 29-30, 1993
 */
#include <stdio.h>
#include <curses.h>
#include <ctype.h>
#include <strings.h>

#define BlockSize 1024
#define PageLines 20
#define HalfPage 10
#define Left 0
#define Right 1

#define StatusLine \
	move(indexy, indexx); printw("%05X (%d)", index, index); \
	move(filey, filex); printw("%s", filename); \
	move(lengthy, lengthx); printw("%05X (%d)", nchars, nchars)

extern char *malloc();
extern char *calloc();
extern char *realloc();

int OnePage();
int ReadFile();
void DrawCurs();

unsigned *file;
char *filename="test";
int nchars=0, Size, pagetop;
short maincy, maincx, indexy, indexx, filey, filex, lengthy, lengthx,
	cursy=0, cursx=7, Side;
WINDOW *page1;

main(argc, argv)
int argc;
char **argv;
{
int c, i, index;
char buf[80];
short Done=0;
FILE *fp, *test;

	if (argc > 1) {
		free(filename);
		filename = (char *) malloc (sizeof(char) * (strlen(argv[1]) + 1));
		filename = strcpy(filename, argv[1]);
		}

	initscr();
	scrollok(stdscr, 1);
	page1 = newwin(23, 79, 0, 0);
	clear();

	Size = ReadFile(fp, filename);

	cbreak();
	noecho();
	index = 0;	/* top of the file */
	wmove(page1, 20, 0);
	wprintw(page1, "------------------------------------------");
	wprintw(page1, "-----------------------------------");

	wmove(page1, 21,0);
	wprintw(page1, "Byte: "); getyx(page1, indexy, indexx);
	wmove(page1, 21, 25);
	wprintw(page1, "Length: "); getyx(page1, lengthy, lengthx);
	wmove(page1, 21, 55);
	wprintw(page1, "File: "); getyx(page1, filey, filex);
	wmove(page1, 22, 0);
	wstandout(page1);
	wprintw(page1, "Command:");
	wstandend(page1);
	wprintw(page1, " ");	getyx(page1, maincy, maincx);
	overwrite(page1, stdscr);
	StatusLine;
	pagetop = OnePage(index);
	refresh();

	Side = Left;
	cursy = 0;
	cursx = 7;
	while (!Done) {
		standout();
		DrawCurs(Side, index, cursy, cursx);
		standend();
		move(indexy, indexx);
		printw("%05X (%d)", index, index);
		printw("   ");
		move(maincy, maincx);
		refresh();
		c = getch();
		Done = 0;
		move(maincy+1, 0);
		clrtoeol();
		move(maincy, maincx);
		clrtoeol();
		DrawCurs(Side, index, cursy, cursx);
		switch(c) {
			case '?':
				move(maincy+1, 0);
				printw("Kk)up Jj)down h)left l)right q)uit ^l)oad");
				printw("  others: g,G,y,r,R,i,w,W,0");
				break;
			case 'k':	/* up */
				printw("up");
				index -= 16;
				if (index < 0) {
					index = 0;
					cursx=7; cursy=0;
					}
				else {
					cursy--;
					if (cursy < 0) {
						pagetop = OnePage(pagetop-16);
						cursy = 0;
						}
					}
				standout();
				DrawCurs(Side, index, cursy, cursx);
				standend();
				break;

			case 'j':	/* down */
				printw("down");
				index += 16;
				if (index > nchars) {
					index-= 16;
					}
				else {
					cursy++;
					if (cursy >= PageLines) {
						pagetop = OnePage(pagetop+16);
						cursy--;
						}
					}
				standout();
				DrawCurs(Side, index, cursy, cursx);
				standend();
				break;

			case 'h':	/* left */
				printw("left");
				index--;
				if (index < 0)
					index = 0;

				cursx-=3;

				if (cursx < 7) {
					cursx = 53;
					cursy--;
					if (cursy < 0) {
						pagetop = OnePage(index);
						cursy = 0;
						if (index == 0)
							cursx = 7;
						}
					}
				else
					if (cursx == 29)
						cursx--;		/* skip middle blank space */

				standout();
				DrawCurs(Left, index, cursy, cursx);
				standend();
				break;

			case 'l':	/* right */
				printw("right");
				index++;
				if (index > nchars) {
					index = nchars;
					}
				else {
					cursx+= 3;
					if (cursx > 53) {
						cursx = 7;
						cursy++;
						if (cursy > PageLines) {
							pagetop = OnePage(index);
							cursy = PageLines;
							}
						}
					else
						if (cursx == 31)
							cursx++;
					}
				standout();
				DrawCurs(Left, index, cursy, cursx);
				standend();
				break;

			case 'K':	/* half page up */
				printw("half page up");
				index -= (16 * HalfPage);
				if (index < 0)
					index = 0;
				pagetop = OnePage(index);
				standout();
				DrawCurs(Left, index, cursy, cursx);
				standend();
				break;

			case 'J':	/* half page down */
				printw("half page down");
				index += (16 * HalfPage);
				move(maincy+1, maincx);
				if (index >= nchars)
					index = nchars - (16 * PageLines);
				pagetop = OnePage(index);
				standout();
				DrawCurs(Left, index, cursy, cursx);
				standend();
				break;

			case 'g':	/* goto byte index */
				printw("goto byte index (not yet)");
				break;

			case '0':	/* beginning of file */
				printw("beginning of file");
				index = 0;
				pagetop = OnePage(index);
				cursx=7; cursy=0;
				standout();
				DrawCurs(Side, index, cursy, cursx);
				standend();
				break;

			case 'G':	/* goto file end */
				printw("end of file");
				index = nchars - PageLines * 16 + 1;
				if (index < 0)
					index = 0;
				pagetop = OnePage(index);
				cursx=7; cursy=0;
				standout();
				DrawCurs(Side, index, cursy, cursx);
				standend();
				break;

			case 'i':
				printw("insert (not yet)");
				break;

			case 'r':
				printw("replace 1 byte (not yet)");
				break;
			case 'R':
				printw("Replace some bytes (not yet)");
				break;
			case 'y':
				printw("copy some bytes (not yet)");
				break;
			case 'w':
				printw("Redraw");
				pagetop = OnePage(pagetop);
				break;
			case 'W':
				printw("index to top");
				pagetop = OnePage(index);
				cursx=7; cursy=0;
				standout();
				DrawCurs(Side, index, cursy, cursx);
				standend();
				break;

			case 12:
				standout();
				printw("Load:");
				standend();
				printw(" ");
				refresh();
				echo();
				scanw("%s", buf);
				noecho();
				test = fopen(buf, "r");
				if (test == NULL) {
					move(maincy+1, 0);
					standout();
					printw("Unable to open file '%s'.", buf);
					standend();
					}
				else {
					fp = test;
					Size = ReadFile(fp, buf);
					free(filename);
					filename = (char *) malloc(sizeof(char) * 
						(strlen(buf) + 1));
					filename = (char *) strcpy(filename, buf);
					overwrite(page1, stdscr);
					StatusLine;
					index = 0;
					pagetop = OnePage(index);
					}
				move(maincy, maincx); clrtoeol();
				break;
			case 'q':	/* quit */
				printw("quit");
				Done=1;
				break;
			default: printw("default: %02X (%d)", c, c);

			} /* switch */

		refresh();

		}	/* while !Done */

	nocbreak();
	echo();
	endwin();
}


int OnePage(index)
int index;
{
int i, ch, chleft, line, to;
unsigned c;

	to = index + PageLines * 16;
	if (to > nchars) {
		move(maincy+1, maincx);
		printw("to > nchars");
		to = index + PageLines * 16;
		if (to > Size)
			to = Size;
		}
	line=0; ch=60; chleft=7;
	move(0,0);

	for (i=index; i<to; i++) {
		c = file[i];

		move(line, chleft);
		printw("%02X", c);
		move(line, ch);

		if (isprint(c))
			printw("%c", c);
		else
			printw(".");

		ch++;
		chleft+=3;

		if (ch == 68)
			chleft++;

		if (ch == 76) {
			move(line, 0);
			printw("%05X:", index + line * 16);
			move(line, 59);
			printw("|");
			move(line, 76);
			printw("|");
			line++;
			ch=60;
			chleft=7;
			}
		}
return index;
} /* OnePage */


int ReadFile(fp, filename)
FILE *fp;
char *filename;
{
unsigned half, ptr, size, c;

	free(file);

	fp = fopen(filename, "r");

	if (fp == NULL) {
		fprintf(stderr, "Unable to open file 'test'.\n");
		exit(1);
		}

	file = (unsigned *) calloc((unsigned) BlockSize, (unsigned) sizeof(char));

	if (file == NULL) {
		fprintf(stderr, "Unable to malloc for file.\n");
		exit(1);
		}

	nchars = 0; ptr=0; size=BlockSize;
	half = (unsigned) (BlockSize / 2);

	while ((c=getc(fp)) != EOF) {

		if (ptr == half) {
			ptr=0;
			refresh();
			file = (unsigned *) realloc((char *) file, sizeof(unsigned) *
					(nchars + BlockSize));
			size = nchars + BlockSize;
			if (file == NULL) {
				fprintf(stderr, "Null!\n");
				exit (1);
				}
			}

		file[nchars++] = (unsigned) c;
		ptr++;

		}
	fclose(fp);

	return size;
}


void DrawCurs(side, index, cursy, cursx)
short side;
int index;
short cursy, cursx;
{

	move(cursy, cursx);
	printw("%02X", file[index]);
	move(cursy, 60 + ((index-pagetop) % 16));
	
	if (isprint(file[index]))
		printw("%c", file[index]);
	else printw(".");

	move(maincy, maincx);
}
