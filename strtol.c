#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define INVALID_SYMBOL_VALUE 37

// Converts a symbol to a corresponding intiger value. Returns INVALID_SYMBOL_VALUE for invalid symbols.
static int symbol_to_int(const char symbol)
{
	if (symbol >= '0' && symbol <= '9')
		return symbol - '0';
	if (symbol >= 'a' && symbol <= 'z')
		return symbol - 'a' + 10;
	if (symbol >= 'A' && symbol <= 'Z')
		return symbol - 'A' + 10;
	return INVALID_SYMBOL_VALUE;
}

// Helper function for the my_strtol function.
// Handles the sign, prefixes, base detection and truncates invalid leading and trailing symbols.
static const char *preprocess(const char *nptr, int *base, int *sign, size_t *size)
{
	for (; isspace(*nptr); nptr++)
		;

	if (*nptr == '-')
	{
		*sign = -1;
		nptr++;
	}
	else if (*nptr == '+')
	{
		nptr++;
	}
	*size = strlen(nptr);

	if (nptr[0] == '0')
	{
		if ((*base == 0 || *base == 16) && *size > 2 && (nptr[1] == 'x' || nptr[1] == 'X') && symbol_to_int(nptr[2]) < 16)
		{
			nptr += 2;
			*base = 16;
		}
		else if (*base == 0)
		{
			*base = 8;
		}
	}
	else if (*base == 0)
	{
		*base = 10;
	}
	for (*size = 0; symbol_to_int(nptr[*size]) < *base; (*size)++)
		;
	return nptr;
}

long strtol(const char *nptr, char **endptr, int base)
{
	if (nptr == NULL)
	{
		return 0;
	}
	if (endptr != NULL)
		*endptr = (char *)nptr;
	if ((base != 0 && base < 2) || base > 36)
	{
		errno = EINVAL;
		return 0;
	}
	int sign = 1;
	size_t size = 0;
	const char *begin = preprocess(nptr, &base, &sign, &size);
	if (endptr != NULL && size > 0)
	{
		*endptr = (char *)(begin + size);
	}

	long int value = 0;
	int symbol;
	for (symbol = 0; symbol < size; symbol++)
	{
		int symbol_value = symbol_to_int(begin[symbol]);
		if (sign == 1 && value > (LONG_MAX - symbol_value) / base)
		{
			errno = ERANGE;
			return LONG_MAX;
		}
		if (sign == -1 && value < (LONG_MIN + symbol_value) / base)
		{
			errno = ERANGE;
			return LONG_MIN;
		}
		value = value * base + sign * symbol_value;
	}

	return value;
}