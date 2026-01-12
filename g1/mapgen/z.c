
#include	<stdio.h>
#include	"z.h"


void *
my_malloc(unsigned size)
{
	char *p;
	extern char *malloc();

	p = malloc(size);

	if (p == NULL) {
		fprintf(stderr, "my_malloc: out of memory (can't malloc "
				"%d bytes)\n", size);
		exit(1);
	}

	bzero(p, size);

	return p;
}


void *
my_realloc(void *ptr, unsigned size)
{
	char *p;
	extern char *realloc();
	extern char *malloc();

	if (ptr == NULL)
		p = malloc(size);
	else
		p = realloc(ptr, size);

	if (p == NULL) {
		fprintf(stderr, "my_realloc: out of memory (can't realloc "
				"%d bytes)\n", size);
		exit(1);
	}

	return p;
}


char *
str_save(char *s)
{
	char *p;

	p = my_malloc(strlen(s) + 1);
	strcpy(p, s);

	return p;
}


void
asfail(file, line, cond)
char *file;
int line;
char *cond;
{
	fprintf(stderr, "assertion failure: %s (%d): %s\n",
						file, line, cond);
	abort();
	exit(1);
}


void
lcase(s)
char *s;
{

	while (*s)
	{
		*s = tolower(*s);
		s++;
	}
}


/*
 *  Line reader with no size limits
 *  strips newline off end of line
 */

#define	GETLIN_ALLOC	256

char *
getlin(FILE *fp)
{
	static char *buf = NULL;
	static unsigned int size = 0;
	int len;
	int c;

	len = 0;

	while ((c = fgetc(fp)) != EOF)
	{
		if (len + 1 >= size)
		{
			size += GETLIN_ALLOC;
			buf = my_realloc(buf, size);
		}

		if (c == '\n')
		{
			buf[len] = '\0';
			return buf;
		}

		buf[len++] = (char) c;
	}

	if (len == 0)
		return NULL;

	buf[len] = '\0';

	return buf;
}


/*
 *  Get line, remove leading and trailing whitespace
 */

char *
getlin_ew(FILE *fp)
{
	char *line;
	char *p;

	line = getlin(fp);

	if (line)
	{
		while (*line && iswhite(*line))
			line++;			/* eat leading whitespace */

		for (p = line; *p; p++)
			if (*p < 32 || *p == '\t')	/* remove ctrl chars */
				*p = ' ';
		p--;
		while (p >= line && iswhite(*p))
		{				/* eat trailing whitespace */
			*p = '\0';
			p--;
		}
	}

	return line;
}


#define	COPY_LEN	1024

void
copy_fp(a, b)
FILE *a;
FILE *b;
{
	char buf[COPY_LEN];

	while (fgets(buf, COPY_LEN, a) != NULL)
		fputs(buf, b);
}


int
i_strcmp(char *s, char *t)
{
	char a, b;

	do {
		a = tolower(*s);
		b = tolower(*t);
		s++;
		t++;
		if (a != b)
			return a - b;
	} while (a);

	return 0;
}


int
i_strncmp(char *s, char *t, int n)
{
	char a, b;

	do {
		a = tolower(*s);
		b = tolower(*t);
		if (a != b)
			return a - b;
		s++;
		t++;
		n--;
	} while (a && n > 0);

	return 0;
}


#ifdef SYSV

unsigned short seed[3];

void
init_random()
{
	long l;

	if (seed[0] == 0 && seed[1] == 0 && seed[2] == 0)
	{
		l = time(NULL);
		seed[0] = l & 0xFFFF;
		seed[1] = getpid();
		seed[2] = l >> 16;
	}
}


int
rnd(int low, int high)
{
	extern double erand48();

	return (int) (erand48(seed) * (high - low + 1) + low);
}

#else	/* ifdef SYSV */

init_random() {
	long l;

	srandom(l);
}


rnd(low, high)
int low;
int high;
{

	return random() % (high - low + 1) + low;
}
#endif	/* ifdef SYSV */


void
test_random()
{
	int i;

	if (isatty(1))
	    for (i = 0; i < 10; i++)
		printf("%3d  %3d  %3d  %3d  %3d  %3d  %3d  %3d  %3d  %3d\n",
			rnd(1, 10), rnd(1, 10), rnd(1, 10), rnd(1, 10),
			rnd(1, 10), rnd(1, 10), rnd(1, 10), rnd(1, 10),
			rnd(1, 10), rnd(1, 10));
	else
	    for (i = 0; i < 100; i++)
		printf("%d\n", rnd(1, 10));

	for (i = -10; i >= -16; i--)
		printf("rnd(%d, %d) == %d\n", -3, i, rnd(-3, i));
}



#define		ILIST_ALLOC	6	/* doubles with each realloc */


/*
 *  Reallocing array handler
 *
 *  Length is stored in ilist[0], maximum in ilist[1].
 *  The user-visible ilist is shifted to &ilist[2], so
 *  that iterations can proceed from index 0.
 */

void
ilist_append(ilist *l, int n)
{
	int *base;

	if (*l == NULL)
	{
		base = my_malloc(sizeof(**l) * ILIST_ALLOC);
		base[1] = ILIST_ALLOC;

		*l = &base[2];
	}
	else
	{
		base = (*l)-2;
		assert(&base[2] == *l);

		if (base[0] + 2 >= base[1])
		{
			base[1] *= 2;
			base = my_realloc(base, base[1] * sizeof(*base));
			*l = &base[2];
		}
	}

	base[ base[0] + 2] = n;
	base[0]++;
}


void
ilist_prepend(ilist *l, int n)
{
	int *base;
	int i;

	if (*l == NULL)
	{
		base = my_malloc(sizeof(**l) * ILIST_ALLOC);
		base[1] = ILIST_ALLOC;

		*l = &base[2];
	}
	else
	{
		base = (*l)-2;
		assert(&base[2] == *l);

		if (base[0] + 2 >= base[1])
		{
			base[1] *= 2;
			base = my_realloc(base, base[1] * sizeof(*base));
			*l = &base[2];
		}
	}

	base[0]++;
	for (i = base[0]+1; i > 2; i--)
		base[i] = base[i-1];
	base[2] = n;
}


void
ilist_delete(ilist *l, int i)
{
	int *base;
	int j;

	assert(i >= 0 && i < ilist_len(*l));		/* bounds check */
	base = (*l)-2;

	for (j = i+2; j <= base[0]; j++)
		base[j] = base[j+1];

	base[0]--;
}


void
ilist_clear(ilist *l)
{
	int *base;

	if (*l != NULL)
	{
		base = (*l)-2;
		base[0] = 0;
	}
}


void
ilist_reclaim(ilist *l)
{
	int *base;

	if (*l != NULL)
	{
		base = (*l)-2;
		free(base);
	}
	*l = NULL;
}


int
ilist_lookup(ilist l, int n)
{
	int i;

	if (l == NULL)
		return -1;

	for (i = 0; i < ilist_len(l); i++)
		if (l[i] == n)
			return i;

	return -1;
}


void
ilist_rem_value(ilist *l, int n)
{
	int i;
	int ret = FALSE;

	for (i = 0; i < ilist_len(*l); i++)
		if ((*l)[i] == n)
		{
			ilist_delete(l, i);
			i--;
		}
}


ilist
ilist_copy(ilist l)
{
	int *base;
	int *copy_base;

	if (l == NULL)
		return NULL;

	base = l-2;
	assert(&base[2] == l);

	copy_base = my_malloc(base[1] * sizeof(*base));
	bcopy(base, copy_base, (base[0] + 2) * sizeof(*base));

	return &copy_base[2];
}


void
ilist_scramble(ilist l)
{
        int i;
        int tmp;
        int one, two;
        int len;

        len = ilist_len(l);

        for (i = 0; i < len * 2; i++)
        {
                one = rnd(0, len-1);
                two = rnd(0, len-1);

                tmp = l[one];
                l[one] = l[two];
                l[two] = tmp;
        }
}


ilist_test()
{
	int i;
	ilist x;
	ilist y;

	setbuf(stdout, NULL);
	bzero(&x, sizeof(x));

	printf("len = %d\n", ilist_len(x));

	for (i = 0; i < 100; i++)
		ilist_append(&x, i);

	assert(x[ilist_len(x)-1] == 99);

	printf("len = %d\n", ilist_len(x));
	for (i = 0; i < ilist_len(x); i++)
		printf("%d ", x[i]);
	printf("\n");

	for (i = 900; i < 1000; i++)
	{
		ilist_prepend(&x, i);
		if (x[ilist_len(x)-1] != 99)
			fprintf(stderr, "fail: i = %d\n", i);
	}

	printf("len = %d\n", ilist_len(x));
	for (i = 0; i < ilist_len(x); i++)
		printf("%d ", x[i]);
	printf("\n");

	ilist_delete(&x, 100);

	printf("len = %d\n", ilist_len(x));
	for (i = 0; i < ilist_len(x); i++)
		printf("%d ", x[i]);
	printf("\n");

	printf("len before = %d\n", ilist_len(x));
	ilist_append(&x, 15);
	printf("len after = %d\n", ilist_len(x));
	printf("x[0] = %d\n", x[0]);

	printf("ilist_lookup(998) == %d\n", ilist_lookup(x, 998));

	y = ilist_copy(x);
	assert(ilist_len(x) == ilist_len(y));
	for (i = 0; i < ilist_len(x); i++)
	{
		assert(&x[i] != &y[i]);
		if (x[i] != y[i])
		{
			fprintf(stderr, "[%d] different\n", i);
			assert(FALSE);
		}
	}

	printf("ilist_lookup(998) == %d\n", ilist_lookup(x, 998));

	ilist_clear(&x);
	assert(ilist_len(x) == 0);
}


#if 0
static char *read_buffer = NULL;
static int read_size = 0;
static int read_len = 0;
static int read_pos = 0;

#define	READ_CHUNK	4096


int
read_file(char *name)
{
	int fd;
	int i;
	int n;

	fd = open(name, 0);
	if (fd < 0)
	{
		fprintf(stderr, "can't read %s: ", name);
		perror("");
		return FALSE;
	}

	read_len = 0;

	do {
	
		if (read_size - read_len < READ_CHUNK)
		{
			read_buffer = my_realloc(read_buffer,
						read_size + READ_CHUNK);
			read_size += READ_CHUNK;
		}

		n = read(fd, &read_buffer[read_len], READ_CHUNK);

		if (n < 0)
		{
			fprintf(stderr, "error reading %s: ", name);
			perror("");
			close(fd);
			return FALSE;
		}

		read_len += n;

	} while (n > 0);

	close(fd);
	read_pos = 0;

	for (i = 0; i < read_len; i++)
		if (read_buffer[i] == '\n')
			read_buffer[i] = '\0';

	return TRUE;
}


char *
read_getlin()
{
	char *p;

	if (read_pos >= read_len)
		return NULL;

	p = &read_buffer[read_pos];

	while (read_pos < read_len && read_buffer[read_pos] != '\0')
		read_pos++;
	read_pos++;

	return p;
}


char *
read_getlin_ew()
{
	char *line;
	char *p;

	line = read_getlin();

	if (line)
	{
		while (*line && iswhite(*line))
			line++;			/* eat leading whitespace */

		for (p = line; *p; p++)
			if (*p < 32 || *p == '\t')	/* remove ctrl chars */
				*p = ' ';
		p--;
		while (p >= line && iswhite(*p))
		{				/* eat trailing whitespace */
			*p = '\0';
			p--;
		}
	}

	return line;
}
#endif
