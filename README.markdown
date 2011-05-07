This is the result of playing around with generating analogues of the Sierpinski gasket, partially for a math course project. The method used to calculate the triangles is rather simple:

>If one takes Pascal's triangle with 2^n^ rows and colors the even numbers white, and the odd numbers black, 
the result is an approximation to the Sierpinski triangle. More precisely, the limit as n approaches infinity 
of this parity-colored 2^n^-row Pascal triangle is the Sierpinski triangle. 
 - [Wikipedia](http://en.wikipedia.org/wiki/Sierpinski_triangle#Properties)

Of course, this presents many opportunities for generalizations:

1) Even numbers are just those congruent to 0 modulo 2 - instead of coloring based on even/odd, color based on congruence to 0 modulo *n*. This gives a new parameter, the modulus, to expiriment with.

2) Instead of using a triangle of binomial coefficients, use a triangle of [trinomial](http://mathworld.wolfram.com/TrinomialTriangle.html) coefficients, or even one of quartic coefficients.



## Usage
*./executable filename.ppm \<multinomial type ∈ {2,3,4}\> \<modulus ∈ {0..255}\> \<image size eg. "500x500" with no spaces\>*

Images are saved as [ppm](http://en.wikipedia.org/wiki/Netpbm_format) files, and so really should be compressed before doing anything with them! PPM was targeted due to the shear simplicity of the format, and my natural laziness.
