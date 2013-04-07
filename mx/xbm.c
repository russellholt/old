#include <stdio.h>

main()
{
int i, j, ox=10, oy=8;
int bits, byte, end=0, perline=11;
char a;

	scanf("%d %d", &ox, &oy);

	printf("#define pic_width %d\n", ox);
	printf("#define pic_height %d\n", oy);
	printf("static char pic_bits[] = {");

	for (i=0; i<oy; i++) {
		for (byte=bits=0,j=0; j<ox; j++) {
			scanf(" %c", &a);
			byte >>= 1;
			byte |= (a % 2)? 0x80:0;
			bits++;

			if (j==ox-1 && bits < 8) {	/* padding */
				byte >>= (8-bits);
				bits=8;
				}

			if (bits > 7) {

				if (++perline %12 == 0){
					printf("\n    " );
					perline=1;
					}

				end = (i==oy-1 && j==ox-1);
				bits = 0; 
				printf("0x%x%s", byte, end ? " };\n":", ");
				byte=0;
				}
		scanf(" \n");
			}
		}
}
