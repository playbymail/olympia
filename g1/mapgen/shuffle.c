
#include	<stdio.h>
#include	"z.h"


char *names[10000];
int top = 0;

main() {
	char *line;
	char *tmp;
	int one, two;
	int i;

	while (line = getlin(stdin))
	{
		names[top++] = str_save(line);
	}

	for (i = 1; i <= 10000; i++)
	{
		one = rnd(0, top-1);
		two = rnd(0, top-1);
		tmp = names[one];
		names[one] = names[two];
		names[two] = tmp;
	}

	for (i = 0; i < top; i++)
		printf("%s\n", names[i]);

	fprintf(stderr, "top = %d\n", top);
}
