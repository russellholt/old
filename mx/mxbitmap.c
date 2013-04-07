/************************************************************
 *
 *  Complex Plane Fractals - Mandelbrot and Julia Sets
 *  Version 4.4 - 11/20/92   by Russell Holt
 *
 *  New stuff: 2 different "spike" algorithms,
 *             weird equations, complex conjugates,
 *             fractional powers, the "Phoenix" iteration,
 *             stalks.
 *
 *  History
 *  10/12/92 - complex number structure,
 *  command line datafile, "splat" alg. selection
 *  continuous potential method
 *
 ************************************************************/

#include <stdio.h>
#include <math.h>
#include "complex.h"

#define loc(ox,x,y)  ((ox) * (y-1) + (x-1))
#define Norm_Alg(rx,ry) ((rx)*(rx) + (ry)*(ry))
#define Splat_Alg(rx,ry,cr,ci) ((rx)+(ry)-(cr)-(ci))
#define ZSplat(a,b) ((a.r)+(a.i)-(b.r)-(b.i))
#define too_small(a,b) (b = (a.r < small || a.i < small))

extern void readit();

int dist=100, maxiter=30, colscale=1, scr=1, ncolors=16, ox=75, oy=22,
	*display, area, splat, oy_range1, oy_range2, quiet, first, final,
	file_num, julia, pot, GS, spikes, rev_spk, stalks, phoenix,
	bitmap=1, bits=0, done, byte=0, i, j, perline=11;

double xcoor, ycoor, size, cr, ci, CPMsizer, MacTemp0, MacTemp1, MacTemp2,
		stalkconst;

char buffer[256], screen;

int main (argc, argv)
int argc, *argv[];
{
FILE *fp;
char filename[15];
char *b=" .:,;!/>)|&IH%*#@XYZ", c;
int index,color,k,x,y,in,e,write_info,pass;
double left,right,bottom,top,rx,ry,tx,ty,width,potential,
        cy,cx,qd,height,temp,xptemp,yptemp,scale=1,edist,small;
complex Z, X, Y, Zconst, Ztemp, Ztemp2, Ztemp3, Ztemp4;

small = -99999999999;

readit(argc, argv); screen = (ox == 640 || bitmap);
left = xcoor - (size/2); right=xcoor + (size/2);
bottom = ycoor - size*scale/2;
top = ycoor + size*scale/2;

if (!quiet)
  printf("\nleft: %f right: %f bottom: %f top: %f\n",left,right,bottom,top);

if (right < left) { temp=right; right=left; left=temp; }
if (top < bottom) { temp=top; top=bottom; bottom=temp; }

width = right-left; height = top-bottom;
tx = width/ox; ty = height/oy;
edist = ( ( (double) ncolors)/maxiter);

sprintf(filename, "ut%d", file_num);

fp = fopen (filename, "w");
printf("Calculating (%d-%d)...\n", oy_range1, oy_range2);

if (julia) {
	Zconst.r = cr;
	Zconst.i = ci;
	}

for (y=oy_range2-first; y>=oy_range1; y--) { if (scr) printf("\n");
        cy = bottom + y*ty;

        for(in=0, x=0; x<ox; x++) {
                cx = left + x*tx;

				if (julia) {
					Z.r=cx; Z.i=cy;
					}
				else {
					Zconst.r=cx; Zconst.i=cy;
					Z.r=0; Z.i=0;
					}

				Makez(0.5,Ztemp2);
				Makez(0, Y);
				Makez(0, X);

				pass = 0;
                for (k=0; k<maxiter; k++) {

					if (phoenix) {
							Zsq(Z, Ztemp);
							Zmul(Z, Ztemp2, Ztemp3);
							Zsub(Ztemp, Ztemp3, Z);
							Zadd(Z, Zconst, Z);

							Zsq(Z, Ztemp); Zmul(Ztemp2, Y, Ztemp3);
							Zsub(Ztemp, Ztemp3, X);
							Zadd(X, Zconst, X);

							Zeq(Y, Z);
							Zeq(Z, X);
						} /* phoenix */
					else {
							Zsq(Z, Ztemp);
							Zadd(Ztemp, Zconst, Z);
							}

					if (too_small(Z,e)) break;

					if (stalks && (fabs(Z.r) < stalkconst ||
						fabs(Z.i) < stalkconst))
						{
							pass = (pass != 0) ? pass : k;
						}

			qd = (splat) ? too_small(Z,in), ZSplat(Z,Zconst): Znorm(Z);

					if (spikes) qd = sqrt(qd);
                	if (qd > dist || in) break;
                }

		index = loc(ox, x, y-oy_range1);

		if (colscale) color = (int) (edist*k)%ncolors+1;
		else color = k%ncolors + 1;
		if (in) color = (color+ (ncolors >>1) )%ncolors + 1;
		if (spikes && k < 7)
			if ((fabs(Z.r) < dist) || (fabs(Z.i) < dist))
				color = ((color++) > ncolors) ? color%ncolors+1: color;
			else
				if (rev_spk)
					color = ((color+=2) > ncolors) ? color%ncolors+1: color;

		if (stalks && pass) {
			if (bitmap) color = 1;
			else
				color = (ncolors == 1)? 0 : (int) (pass%ncolors) + 1;
			}
			/* color = (int) (4*edist*pass)%ncolors+1; */
			/* color = ((color+=7) > ncolors) ? color%ncolors+1: color; */

			if (pot) {
				if (fabs(qd) > dist || in) {
					potential = 0.5*log10(qd) / pow(2.0, (double) k);
					color = ((int) (potential*CPMsizer))%ncolors + 1;
					if (in) color = (color+ (ncolors >>1) )%ncolors + 1;
					if (scr) printf("%c", b[color%16 + 1]);
					}
				else {
					potential = 0.0;
					color=0; if (scr) printf(" ");
					}
				display[index] = color;
				continue;
				}

		if ( (fabs(qd) > dist) || in || pass) {
			display[index] = color;
				if (scr)
					printf("%c", b[color%ncolors+1]);
			}
		else {
			display[index] = 0;
			if (scr)
				printf(" ");
			}
        } /* x */

} /* y */

if (scr) printf("\n");
printf("Writing (%d-%d) to file \"%s\"...", oy_range1, oy_range2,
			filename);

if (first && GS)
	fprintf(fp, "%x %x\n%x\n", ox, oy, ncolors); 
else
	if (first && bitmap) {
		fprintf(fp, "#define pic_width %d\n", ox);
		fprintf(fp, "#define pic_height %d\n", oy);
		fprintf(fp, "static char pic_bits[] = {\n");
	}

for (y=oy_range2-first; y>=oy_range1; y--)
	for(byte=bits=x=0; x<ox; x++) {
		index = loc(ox, x, y-oy_range1);

		if (bitmap) {

			byte >>= 1;
			byte |= (display[index] % 2)? 0x80:0;
			bits++;

			if (x==ox-1 && bits < 8) {	/* padding */
				byte >>= (8-bits);
				bits=8;
				}

			if (bits > 7) {

				if (++perline %12 == 0){
					fprintf(fp, "\n    " );
					perline=1;
					}

				done = (y==oy_range1 && x==ox-1);
				bits = 0; 
				fprintf(fp, "0x%x%s", byte, done ? "":", ");
				/* if (done && !final) fprintf(fp, ","); */
				byte=0;
				} /* if bits */
			} /* if bitmap */

		else
			fprintf(fp, "%c", display[index]);
	}

if (final) {
	if (bitmap)
		fprintf(fp, " };");
	fprintf(fp, "\n");
	}

fclose(fp);
printf("done.\n");

return 0;
}
