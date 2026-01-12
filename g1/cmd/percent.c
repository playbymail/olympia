
#include	<stdio.h>

#define		MAX	1000

double ar[MAX];
int top = 0;

main() {
	char buf[256];
	int i, j;
	double a;

	while (fgets(buf, 256, stdin) != NULL)
	{
		float r;

		sscanf(buf, "%f", &r);

		ar[top++] = r;
	}

	for (j = 2; j <= top; j++)
	{
		a = 1.0 - ar[0] / 100.0;

		printf("%2.3f%%", ar[0]);

		for (i = 1; i < j; i++)
		{
			printf(" + %2.3f%%", ar[i]);
			a *= 1.0 - ar[i] / 100.0;
		}

		printf(" = %2.3f%%\n", (1.0 - a) * 100.0);
	}
}

