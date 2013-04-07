/********************************************/
/* Hex Dump |  by Russell Holt   2/18/92    */
/*___V0.5___|                               */
/*  Prints hex / ascii translations         */
/*  Usage: dump [file]                      */
/*------------------------------------------*/
/* -Without file specification, dump takes  */
/*    input from stdin                      */
/* -Non-printable chars appear as "."       */
/********************************************/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void printextline();
void clearline();

int addr;

int main(argc, argv)
int argc; char *argv[];
{
int c,d=0,a[17],b,done=0;
FILE *fp;

	for(b = 0; b<17; b++)
		a[b] = 0;

	if (argc > 1) {
		if ( (fp = fopen(argv[1], "r")) == NULL) {
			printf("Cannot open file '%s'.", argv[1]);
			exit(1); }
		printf("File: %s\n",argv[1]); }
	else fp=stdin;

	addr=0;
	/* print first addr */
	printf("%05x: ", addr);

	while ((c=getc(fp)) != EOF) {
		addr++;
		printf("%02x ", c); if (d==7) printf(" ");
		a[d]=c;
		if ((c=getc(fp)) == EOF) done=1;
		ungetc(c,fp);
		if ( (++d == 16) || done ) {
			printextline(a,d);
			for (b=0; b<16; b++) a[b] = 0;
			printf("|\n"); d=0;
			if (!done)
				printf("%05x: ", (addr&0xffff));
		} /* if */
	} /* while */
	printf("\n");
	return 0;
	
} /* main */

void printextline(int *a, int d)
{
int b;

	if (!a)
		return;

	for (b=0; b < 3*(16-d); b++)
		printf(" ");

	if (d<8)
		printf(" ");	/* middle extra space */

	printf(" |");

	for (b=0; b<16; b++)
	{
		if (isprint(a[b]))
			printf("%c",(char) a[b]);
		else
			printf(".");
	}
}
