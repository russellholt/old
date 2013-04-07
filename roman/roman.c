/**********************************************************************
  roman

  Arabic to Roman Numeral Converter   v2.1
  Converts arabic numbers from 1 to about 10^100 (for now)

  Usage: roman <number>

  if <number> is given on the command line, roman prints its conversion
  to stdout and quits. If <number> is not given, roman enters
  interactive mode. Enter a number to convert. Type something illegal or
  0 to quit
  
  CHANGE HISTORY (as if it matters)

  2001 	added command line feature - Finally! After 9 years! Yeehaa!
  1996	generalized roman()
  1992	first version - convert only up to 3999

  Russell Holt <holtrf@mac.com>
 **********************************************************************/

#define maxArabicDigits 100
#define maxRomanBuf 300
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
void roman(char *, char *);

int main(argc, argv)
int argc; char *argv[];
{
int x;
char arabs[maxArabicDigits], buf[maxRomanBuf];

	 /* get number from command line, if present */
	if (argc > 1) {
		sscanf(	argv[1], "%s", &arabs);
		roman(arabs, buf);		
		printf("%s\n", buf);
		exit(0);
	}

	for(;;) {
		printf("\nArabic number (%d digits max, 0 to quit): ", maxArabicDigits);

		scanf("%s", &arabs);

		if (arabs[0] == '0' || arabs[0] == 0 || !isdigit(arabs[0]))
		{
			printf("0\n");
			break;
		}

		roman(arabs, buf);		
		printf("%s\n", buf);
	}
}

/**********************************************************************
  roman()
  
  Arabic bignum to Roman numeral converter (v2.0)
  Converts arabic numbers from 1 to about 10^40 (for now)
  
  Number to convert is given as the string 'in'. Conversion is stored
  in 'out'. Does not allocate- you must give a buffer big enough.
 
  I = 1       3999 is the max that can be represented with these symbols
  V = 5       and the standard Roman numeral rules, which are grouped
  X = 10      in threes and either add to or subtract from the greater
  L = 50      symbol to the left or right. Traditionally, to represent
  C = 100     numbers greater than 3999 one would write a bar above a
  D = 500     grouping to mean "multiply by 1000" , such as
  M = 1000                     __
                               IV I
              which is IV x M + I  = 4 x 1000 + 1 = 4001. This program
  does this, except using parenthesis instead of a bar: (IV)I = 4001.
  Also, the Romans probably didn't intend this, but roman() will
  apply this rule recursively as deeply as necessary: eg,

             4001 = (IV)I
          4001002 = ((IV)I)II
       4001002003 = (((IV)I)II)III
        123456789 = ((CXXIII)CDLVI)DCCLXXXIX
        
    pi x 10^20= ((((((MMMCXLI)DXXVI)DXXXV)DCCCXCVII)CMXXXII)CCCLXXXIV)DCXXVI

  If you can think of a use for this, email me!!

  Russell Holt  <holtrf@mac.com>  http://www.holtorama.com/russell/

 **********************************************************************/
void roman(char *in, char *out) {
static char *x="*IVXXLCCDMMDC";
static int a[10][5]={
		{0, 0, 0, 0, 0},	/* 0 */
		{0, 1, 0, 0, 0},	/* 1 */
		{0, 1, 1, 0, 0},	/* 2 */
		{0, 1, 1, 1, 0},	/* 3 */
		{0, 1, 2, 0, 0},	/* 4 */
		{0, 2, 0, 0, 0},	/* 5 */
		{0, 2, 1, 0, 0},	/* 6 */
		{0, 2, 1, 1, 0},	/* 7 */
		{0, 2, 1, 1, 1},	/* 8 */
		{0, 1, 3, 0, 0} 	/* 9 */
	};

int i,j,y, len = strlen(in);
char *p;

	/* split up long input & recurse */
	if (len >= 4)
	{
		int iTest;
		char sub[maxArabicDigits], subout[maxRomanBuf];	/* way upper bounds */
		sscanf(in+len-4,	/* 3rd from last digit */
			"%d", &iTest);
		if (iTest > 3999 || len > 4)
		{
			strncpy(sub, in, len-3);
			sub[len-3] = '\0';
			roman(sub, subout);
			sprintf(out, "(%s)\0",subout);
			out += strlen(subout) + 2;	/* 2 for parens */
			in += len-3;	/* reset for last 3 digits */
			while(*in && *in == '0') in++;	/* skip leading zeros */
		}
	}

	len = strlen(in);
	if (len == 0)
		return;

	/* convert numbers less than 4000 */
	for(p=in, i=len; i>0; i--, p++)
		for(j=1;j<=4;j++)
			if (y = a[*p - '0'][j])	/* = is not a bug ;-) */
				*out++ = x[3*(i-1) + y];
			*out='\0';
}

