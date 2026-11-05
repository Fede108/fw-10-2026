#include "mathlib.h"

int32_t  math_factorial(int32_t n)
{
    int32_t fact = 1;
    
    for(int32_t i=1; i<=n; i++){
        fact = fact*i;
    }
    
    return fact;
}

int32_t  math_gcd(int32_t a, int32_t b)
{
    int32_t max, min;
    if(a<b){ 
        max = b; 
        min = a;     
    }
    else{ 
        max = a; 
        min = b;     
    }
    
    int32_t div = min;
    while(min % div ||  max % div)
        div--;

    return div;
}

uint32_t math_isqrt(uint32_t n)
{
    for(uint32_t i=1; i<n; i++)
    {
        if( i*i == n )
            return i;

        if( i*i > n)
            return i-1;
    }

    return n; 
}

int  math_is_prime(int32_t n)
{
    if(n <= 1) return 0;
    int div = n - 1;
    while( n % div)
        div--;
    
    return div == 1;
}
