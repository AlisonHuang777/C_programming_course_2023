#include <stdio.h>
#include <math.h>

int main() {
    double x;
    printf("x = ");
    scanf("%lf", &x);
    printf(
        "\t ceil(x) = %.0lf\n" //prints the smallest integer greater or equal to x
        "\tfloor(x) = %.0lf\n" //prints the greatest integer smaller or equal to x
        "\t sqrt(x) = %lf\n"   //prints the square root of x
        "\t  log(x) = %lf\n"   //prints the natural logarithm (base e) of x
        "\tlog10(x) = %lf\n",  //prints the common logarithm (base 10) of x
        ceil(x), floor(x), sqrt(x), log(x), log10(x)
    );
}