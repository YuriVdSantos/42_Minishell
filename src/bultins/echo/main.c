#include "echo.h"
#include <stdio.h>

int	main(void)
{
	char	*test1[] = {"echo", "hello", "world", NULL};
	char	*test2[] = {"echo", "-n", "no", "newline", NULL};
	char	*test3[] = {"echo", NULL};

	printf("Test 1 (normal):\n");
	ft_echo(test1);
	printf("\nTest 2 (-n option):\n");
	ft_echo(test2);
	printf(" (should be no newline after this)\n");
	printf("Test 3 (empty):\n");
	ft_echo(test3);

	return (0);
}