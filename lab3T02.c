#include <stdio.h>

int main(void)
{
    double x;
    printf("Enter x: ");
    scanf("%lf", &x); 
    
    double e = 1.0;
    double term = 1.0;

    printf("term    value\n");
    printf(" 1 %15.7f\n", e);
    int n = 1;

    while ((term > 0.0000001)||(term < -0.0000001)) {
        term *= x / n;
        e += term;
        printf("%2d %15.7f\n", n+1, e);
        n ++;
    }

    return 0;
}

//EOF
