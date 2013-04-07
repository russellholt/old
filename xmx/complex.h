/*-----------------------------------------
 *  complex functions and macros
 *  Russell Holt        10/11/92
 *----------------------------------------*/

int TEMP;

#define printz(a) (printf("(%3.2f, %3.2f)\n", a.r, a.i))
#define Makez(a,b,c) c.r= a; c.i= b
#define Zeq(a,c) a.r = c.r; a.i = c.i
#define Zadd(a,b,c) c.r= a.r+b.r; c.i= a.i+b.i
#define Zsub(a,b,c) c.r = a.r-b.r; c.i = a.i-b.i
#define Zmul(a,b,c) c.r= a.r*b.r - a.i*b.i; c.i= a.r*b.i + a.i*b.r
#define Znorm(a) (a.r*a.r + a.i*a.i)
#define Znorm2(a) sqrt(Znorm(a));
#define Zconj(a,c) c.r = a.r; c.i = -a.i
#define Zsq(a,c) c.r= a.r*a.r - a.i*a.i; c.i= 2*a.r*a.i
#define Zcube(a,c) Zsq(a,c); Zmul(a,c,c);
#define Zquad(a,c) Zsq(a,c); Zsq(c,c);

/*
      1           (a-ib)          a-ib
    ------  =  ------------ =  ---------
    (a+ib)     (a+ib)(a-ib)     a*a + b*b
*/

#define Zinv(a,c) TEMP = Znorm(a);	\
		if (TEMP!=0) {	\
			c.r = (double)a.r/(double)TEMP;	\
			c.i = (double)(-a.i)/(double)TEMP;	\
			}	\
		else	\
			c.r = c.i = 0

#define Zrealdiv(a,b,c) c.r = a.r/b; c.i = a.i/b;
#define Zrealmul(a,b,c) c.r = a.r*b; c.i = a.i*b;

#define Zsin(a,c) MacTemp1 = exp(a.i); MacTemp2 = exp(-a.i); \
			c.r = 0.5 * sin(a.r) * (MacTemp1 + MacTemp2); \
			c.i = 0.5 * cos(a.r) * (MacTemp1 - MacTemp2)

#define Zcos(a,c) MacTemp1 = exp(a.i); MacTemp2 = exp(-a.i); \
			c.r = 0.5 * cos(a.r) * (MacTemp1 + MacTemp2); \
			c.i = 0.5 * sin(a.r) * (MacTemp1 - MacTemp2)

#define Zpow(a,n,c) MacTemp0=(double) Znorm(a); \
			MacTemp1 = pow(MacTemp0, n/2.0); \
			MacTemp2 = (a.r != 0) ? n*atan(a.i / a.r) : 0; \
			c.r = MacTemp1 * cos(MacTemp2); \
			c.i = MacTemp1 * sin(MacTemp2)

typedef struct {
	double r,i;
	} complex;

complex add();
complex mul();
complex sq();
complex makez();
double norm();


double norm(a)
	complex a;
{
	return (a.r*a.r + a.i*a.i);
}


complex makez(a)
	double a;
{
	complex c;

	c.r = a;
	c.i = 0;
	return c;
}


complex add(a,b)
	complex a,b;
{
	complex c;

	c.r = a.r + b.r;
	c.i = a.i + b.i;
	return c;
}


complex mul(a,b)
	complex a,b;
{
	complex c;

	c.r = a.r*b.r - a.i*b.i;
	c.i = a.r*b.i + a.i*b.r;
	return c;
}


complex sq(a)
	complex a;
{
	complex c;

	c.r = a.r*a.r - a.i*a.i;
	c.i = 2*a.r*a.i;
	return c;
}
