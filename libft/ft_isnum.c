#include "libft.h"
#include <ctype.h>

int ft_isnum(char *str)
{
    int has_digits_before_decimal = 0;
    int has_digits_after_decimal = 0;

    while (isspace(*str))
        str++;
    if (*str == '+' || *str == '-')
        str++;
    while (ft_isdigit(*str))
    {
        has_digits_before_decimal = 1;
        str++;
	}
    if (*str == '.')
    {
        str++;
        while (ft_isdigit(*str))
        {
            has_digits_after_decimal = 1;
            str++;
        }
    }
    if (*str != '\0')
        return 0;
    return has_digits_before_decimal || has_digits_after_decimal;
}