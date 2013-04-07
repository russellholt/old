					Zsq(Z, Ztemp);
					Zmul(Z, Ztemp2, Ztemp3);
					Zsub(Ztemp, Ztemp3, Z);
					Zadd(Z, Zconst, Z);		/* Z = Z^2 - .5Z + c */

					Zsq(Z, Ztemp); Zmul(Ztemp2, Y, Ztemp3);
					Zsub(Ztemp, Ztemp3, X);
					Zadd(X, Zconst, X);		/* X = Z^2 - .5Y + c */

					Zeq(Y, Z);  /* Y = Z */
					Zeq(Z, X);  /* Z = X */
