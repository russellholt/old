# roman

convert arbitrarily large arabic numbers (up to 100 digits, that is) to roman numerals. for instance

    $ roman 9879485734957349534583049583045 

    ((((((((((IX)DCCCLXXIX)CDLXXXV)DCCXXXIV)CMLVII)CCCXLIX)DXXXIV)DLXXXIII)XLIX)DLXXXIII)XLV

from the source comments:

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

