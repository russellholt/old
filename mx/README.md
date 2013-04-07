# mx

a command line fractal generator, 1992

my first attempt a writing a multiprocessing fractal generator.

input file <code>mxd</code> specifies options. the various <code>go</code> scripts divide the work
into pieces and then stitch the output together in a neutral file format. My thought (as a college
sophmore) was that even though my timeslice was limited, that if I forked a lot of processes I could
get a bigger share of the pie, especially on a the smp Sequent Symmetry computers (running DYNIX)
Though I never had dedicated access so I couldn't really test it).

There are several other files with code snippets from other algorithms.

like this

    main(){int x,y,k;char*b=" .,;/$#@";float r,i,Z,z,t,X,Y;for(y=30;puts(" "),Y=y*0.1-1.5,y--;)
    {for(x=0;X=x*0.04-2,Z=z=0,x++-75;){for(r=X,i=Y,k=0;t=Z*Z-z*z+r,z=2*Z*z+i,Z=t,k<'p';k++)
    if(Z*Z+z*z>4)break;putchar(b[k%8]);}}}
