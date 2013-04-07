typedef struct {
	char	Set,			/* 0 = Mandelbrot, 1 = Julia */
			stalks,
			spikes,
			splat,
			colortype,		/* 0 = modulus, 1 = scaled */
			Equations;		/* 0 = usual, 1 = phoenix, ... */

	int		Iterations,
			NColors,
			PixWidth, PixHeight,	/* size of the pixmap */
			divisions;		/* number of processes for fork() */

	double	CenterX, CenterY,
			Width,
			ConstR, ConstI,
			BeneathStalks;
	Drawable D;	/* Where it lives */
} Fract;
