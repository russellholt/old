#include <stdio.h>
#include <ctype.h>

main(argc,argv)
int argc;
char *argv[];
{
int a,c=0,ox,oy,area,ncolors,y;
FILE *fp;
char *b=".,:;!/>)|&IH%*#@XYZ", *pict;

if (argc > 1)
	fp = fopen(argv[1], "r");
else
	fp = fopen("ut1", "r");

fscanf(fp, "%x %x\n%x\n", &ox, &oy, &ncolors);
printf("\n%d columns.\n\n",ox);

area = ox*oy + 1;
pict = (char *) malloc (area * sizeof(char));

for (y=0;y<=area;y++) {

	c= fgetc(fp);
	if (c == 0) printf(" ");
	else printf("%c", b[c%16]);

		if ((y+1)%ox == 0) printf("\n");
	}
fclose(fp);
}
