/*****************************************************
 *                                                   *
 * Mandelbrot data functions                         *
 * Version 3.4   10/12/92  - command line stuff,     *
 *    external data file                             * 
 * By Russell Holt                                   *
 *****************************************************/

#include <stdio.h>

void readit();
void Usage();

extern int dist, maxiter, colscale, scr, ncolors, ox, oy, area;
extern int *display, splat, oy_range1, oy_range2, quiet, first, final;
extern int file_num, julia, pot, GS, spikes, stalks, phoenix;
extern char buffer[256];

extern double xcoor, ycoor, size, cr, ci, CPMsizer, stalkconst;


void readit(argc, argv)
int argc;
char *argv[];
{
FILE *data;
int input_res; /* get screen dimensions from keyboard ? */
int t1,t2,oytemp; /* temporary */

file_num=first=final=0;

if (argc > 4) {
	sscanf(argv[4], "%d", &file_num);
	first = (file_num == 1);
	final = (file_num == 99);
	}

if (argc < 2) Usage();

if ( (data = fopen (argv[1], "r")) == NULL) {
	printf("\nUnable to open file \"%s\".\n", argv[1]);
	Usage();
	exit(1);
	}

	fgets(buffer, 256, data);
	fscanf(data, "%d %d %d\n", &t1, &t2, &GS);

			oy = t2;
			ox = t1;

			if (argc > 2 ) {
				sscanf(argv[2], "%d", &oy_range1);

				if (argc > 3)
					sscanf(argv[3], "%d", &oy_range2);
				else oy_range2 = oy;
			}
			else {
				oy_range1 = 0;
				oy_range2 = oy;
				}

if (oy_range1 > oy_range2) {
		oytemp = oy_range2;
		oy_range2 = oy_range1;
		oy_range1 = oytemp;
		}

final = (final || argc < 5);
first = (first || argc < 5);
file_num = (file_num)? file_num:1;

area = ox*(oy_range2 - oy_range1 + 1);
display = (int *) malloc(area * sizeof(int));

	if (display == NULL) {
		fprintf(stderr, "Could not allocate enough memory.\n\n");
		exit(1);
	}

	fgets(buffer, 256, data);
	fscanf(data, "\n%lf %lf %lf\n", &xcoor, &ycoor, &size);
	fgets(buffer, 256, data);
	fscanf(data, "\n%d %lf\n", &dist, &CPMsizer);
	fgets(buffer, 256, data);
	fscanf(data, "\n%d\n", &maxiter);
	fgets(buffer, 256, data);
	fscanf(data, "\n%d %d\n", &ncolors, &colscale);
	fgets(buffer, 256, data);
	fscanf(data, "\n%d %d\n", &scr, &quiet);
	fgets(buffer, 256, data);
	fscanf(data, "\n%d %d %d\n", &splat, &spikes, &pot);
	fgets(buffer, 256, data);
	fscanf(data, "\n%d %lf %lf\n", &julia, &cr, &ci);
	fgets(buffer, 256, data);
	fscanf(data, "\n%d %lf \n", &stalks, &stalkconst);
	fgets(buffer, 256, data);
	fscanf(data, "\n%d\n", &phoenix);
	fclose(data);

	scr = (!quiet && scr);
	if (!quiet) {

		printf("Screen dimensions: %d by %d\n", ox, oy);
		printf("Vertical range:    %2d to %d\n\n",oy_range1, oy_range2);
		printf("x center: %lf\n", xcoor);
		printf("y center: %lf\n", ycoor);
		printf("width: %lf\n", size);
		printf("escape test distance: %d\n", dist);
		printf("max iterations: %d\n", maxiter);
		printf("number of colors: %d\n", ncolors);
		printf("scale colors: %s\n", (colscale)?"yes":"no");
		printf("screen output: %s\n", (scr)?"yes":"no");
		printf("algorithm: %s\n", (splat)?"splat":"normal");
		if (stalks) printf("draw stalks beneath %lf\n", stalkconst);
		if (phoenix) printf("using phoenix equations\n");
		printf("info in file: %s\n", GS?"yes":"no");
		}
	ncolors--;
}

void Usage()
{
	printf("Usage: mx datafile [line1] [line2]\n");
	exit(1);
}
