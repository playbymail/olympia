#pragma once

// SYSV enables init_random? WTF?
#define SYSV

#define	TRUE	1
#define	FALSE	0

#define	abs(n)		((n) < 0 ? ((n) * -1) : (n))

#define	isalpha(c)	(((c)>='a' && (c)<='z') || ((c)>='A' && (c)<='Z'))
#define	isdigit(c)	((c) >= '0' && (c) <= '9')
#define	iswhite(c)	((c) == ' ' || (c) == '\t')

#define	tolower(c)	(((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))
#define	toupper(c)	(((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))

extern void *my_malloc(size_t size);
extern void *my_realloc(void *ptr, size_t size);
extern char *str_save(char *);

extern char *getlin(FILE *);
extern char *getlin_ew(FILE *);
extern int i_strncmp(char *s, char *t, int n);
extern int i_strcmp(char *s, char *t);
extern int rnd(int low, int high);

/*
 *  Assertion verifier
 */

// #ifdef __STDC__
// #define	assert(p)	if(! (p)) asfail(__FILE__, __LINE__, #p);
// #else
// #define	assert(p)	if(! (p)) asfail(__FILE__, __LINE__, "p");
// #endif


/*
 *  'ilist' reallocing array definitions
 *
 *  TODO: This code originally used int* and cast pointers to int, which
 *  worked on 32-bit systems where sizeof(int) == sizeof(void*). On 64-bit
 *  systems this truncates pointers, causing crashes. As a hack, we now use
 *  intptr_t to hold both integers and pointers without truncation. A proper
 *  fix would use separate typed lists or a tagged union.
 */

typedef intptr_t *ilist;

#define ilist_len(a)		(((intptr_t *)(a)) == NULL ? 0 : (int)((intptr_t *)(a))[-2])

extern void ilist_append(ilist *l, intptr_t n);
extern void ilist_prepend(ilist *l, intptr_t n);
extern void ilist_delete(ilist *l, int i);
extern void ilist_clear(ilist *l);
extern void ilist_reclaim(ilist *l);
extern int ilist_lookup(ilist l, intptr_t n);
extern void ilist_rem_value(ilist *l, intptr_t n);
extern ilist ilist_copy(ilist l);
void ilist_scramble(ilist l);

int read_file(char *name);
char *read_getlin();
char *read_getlin_ew();

void count_cities();
void count_continents();
void count_subloc_coverage();
void count_sublocs();
void count_tiles();
int create_a_city(int row, int col, char *name, int major);
void dir_assert();
void dump_continents();
void dump_gates();
void dump_roads();
void fix_terrain_land();
void init_random();
void make_gates();
void make_graveyards();
void make_islands();
void make_roads();
void map_init();
void open_fps();
void place_sublocations();
void print_map();
void print_sublocs();
int rc_to_region(int row, int col);
void read_map();
void set_province_clumps();
void set_regions();
void unnamed_province_clumps();
