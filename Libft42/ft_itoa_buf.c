#include "libft.h"

int ft_itoa_buf(int n, char *buf)
{
    int len = ft_numlen(n);
    int i = len;
    
    buf[i--] = '\0';
    if (n == 0)
        buf[0] = '0';
    else
    {
        while (n != 0)
        {
            buf[i--] = '0' + ft_abs(n % 10);
            n /= 10;
        }
        if (n < 0)
            buf[0] = '-';
    }
    return len;
}

static int ft_numlen(int n)
{
    int len = (n <= 0) ? 1 : 0;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    return len;
}