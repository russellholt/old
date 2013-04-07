#define loc(ox,x,y)  ((ox) * (y-1) + (x-1))
#define Norm_Alg(rx,ry) ((rx)*(rx) + (ry)*(ry))
#define Splat_Alg(rx,ry,cr,ci) ((rx)+(ry)-(cr)-(ci))
#define ZSplat(a,b) ((a.r)+(a.i)-(b.r)-(b.i))
#define too_small(a,b) (b = (a.r < small || a.i < small))

#define NORMALM     Zsq(Z, Ztemp);  \
                    Zadd(Ztemp, Zconst, Z)

#define PHOENIX(Z,X,Y)	\
	Zsq(Z, Ztemp);	\
	Zrealmul(Z, 0.5, Ztemp3);	\
	Zsub(Ztemp, Ztemp3, Z);	\
	Zadd(Z, Zconst, Z);	\
\
	Zsq(Z, Ztemp); Zmul(Ztemp2, Y, Ztemp3);	\
	Zsub(Ztemp, Ztemp3, X);	\
	Zadd(X, Zconst, X);	\
\
	Zeq(Y, Z);	\
	Zeq(Z, X)

/* other equations will follow! */
