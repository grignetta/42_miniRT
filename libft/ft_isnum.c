#include "libft.h"

int	ft_isnum(char *str)
{
	int	has_digit;

	has_digit = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
		str++;
	while (ft_isdigit(*str))
	{
		has_digit = 1;
		str++;
	}
	if (*str == '.')
	{
		str++;
		while (ft_isdigit(*str))
		{
			has_digit = 1;
			str++;
		}
	}
	while (ft_isspace(*str))
		str++;
	return (*str == '\0' && has_digit);
}
