/*
 *  What to do about idles?
 *  Set a datestamp each time they run a real turn?
 *  pick some arbitrary period like three weeks, and call them
 *  idle over that?
 */


#include	<stdio.h>
#include	<sys/types.h>
#include	<dirent.h>
#include	<unistd.h>
#include	<time.h>
#include	<errno.h>
#include	<assert.h>


#define		ACCT_DIR	"/u/oly/act"
#define		OLD_ACCT_DIR	"/u/oly/old-act"
#define		MAX_GAMES	100
#define		LEN		256
#define		TRUE		1
#define		FALSE		0

#define		min(a,b)	((a) < (b) ? (a) : (b))


struct account {
	char *player;
	double balance;			/* player balance */
	double free;			/* free turn balance */
	char *games[MAX_GAMES];		/* list of games we're in */

	double orig_balance;
	double orig_free;
	int games_changed;
};


/*
 *  malloc safety checks:
 *
 *	Space for two extra ints is is allocated beyond what the client
 *	asks for.  The size of the malloc'd region is stored at the
 *	beginning, and a magic number is placed at the end.  realloc's
 *	and free's check the integrity of these markers.  This protects
 *	against overruns, makes sure that non-malloc'd memory isn't freed,
 *	and that memory isn't freed twice.
 */

void *
my_malloc(unsigned size)
{
	char *p;
	extern char *malloc();

	size += sizeof(int);

	p = malloc(size + sizeof(int));

	if (p == NULL)
	{
		fprintf(stderr, "my_malloc: out of memory (can't malloc "
				"%d bytes)\n", size);
		exit(1);
	}

	bzero(p, size);

	*((int *) p) = size;
	*((int *) (p + size)) = 0xABCF;

	return p + sizeof(int);
}

void
my_free(void *ptr)
{
	char *p = ptr;

	p -= sizeof(int);

	assert(*((int *) (p + *(int *) p)) == 0xABCF);
	*((int *) (p + *(int *) p)) = 0;
	*((int *) p) = 0;

	free(p);
}

char *
str_save(char *s)
{
	char *p;

	p = my_malloc(strlen(s) + 1);
	strcpy(p, s);

	return p;
}

struct account *
new_account()
{
	struct account *p;

	p = my_malloc(sizeof(*p));

	p->balance = 0.0;
	p->free = 0.0;
	p->orig_balance = -1;
	p->orig_free = -1;
	p->player = NULL;
	p->games[0] = NULL;
	p->games_changed = TRUE;

	return p;
}

void
free_account(struct account *acct)
{
	int count;

	if (acct->player)
		my_free(acct->player);

	for (count = 0; acct->games[count]; count++)
		my_free(acct->games[count]);

	my_free(acct);
}

void
load_account(char *player, struct account *acct)
{
	FILE *fp;
	char fnam[LEN];
	char buf[LEN];

	if (player == NULL || *player == '\0')
	{
		fprintf(stderr, "acct: must specify -p player\n");
		exit(1);
	}

	acct->player = str_save(player);

/*
 *  Load acct->balance
 */

	sprintf(fnam, "%s/%s/bal", ACCT_DIR, player);

	fp = fopen(fnam, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "can't read %s:", fnam);
		perror("");
		exit(1);
	}
	else
	{
		fscanf(fp, "%lf", &(acct->balance));
		fclose(fp);
	}

/*
 *  Load acct->free
 */

	sprintf(fnam, "%s/%s/free", ACCT_DIR, player);

	fp = fopen(fnam, "r");
	if (fp == NULL)
	{
		acct->free = 0.0;
	}
	else
	{
		fscanf(fp, "%lf", &(acct->free));
		fclose(fp);
	}

/*
 *  Load acct->games
 */

	sprintf(fnam, "%s/%s/games", ACCT_DIR, player);

	fp = fopen(fnam, "r");
	if (fp == NULL)
	{
		acct->games[0] = NULL;
	}
	else
	{
		int count = 0;

		while (fgets(buf, LEN, fp) != NULL)
		{
			char *p;

			for (p = buf; *p && *p != '\n'; p++)
				;
			*p = '\0';

			if (*buf == '\0')
				continue;	/* skip blank lines */

			acct->games[count++] = str_save(buf);
			if (count+1 >= MAX_GAMES)
			{
				fprintf(stderr, "account %s: too many lines in games file\n", player);
				break;
			}
		}
		acct->games[count] = NULL;

		fclose(fp);
	}

	acct->orig_balance = acct->balance;
	acct->orig_free = acct->free;
	acct->games_changed = FALSE;
}

void
save_account(struct account *acct)
{
	FILE *fp;
	char fnam[LEN];

/*
 *  Save acct->balance
 */

	if (acct->balance != acct->orig_balance)
	{
		sprintf(fnam, "%s/%s/bal", ACCT_DIR, acct->player);

		fp = fopen(fnam, "w");
		if (fp == NULL)
		{
			fprintf(stderr, "can't write %s:", fnam);
			perror("");
			exit(1);
		}

		fprintf(fp, "%.2lf\n", acct->balance);
		fclose(fp);
	}

/*
 *  Save acct->free
 */

	if (acct->free != acct->orig_free)
	{
		sprintf(fnam, "%s/%s/free", ACCT_DIR, acct->player);

		if (acct->free <= 0)
			unlink(fnam);
		else
		{
			fp = fopen(fnam, "w");
			if (fp == NULL)
			{
				fprintf(stderr, "can't write %s:", fnam);
				perror("");
				exit(1);
			}

			fprintf(fp, "%.2lf\n", acct->free);
			fclose(fp);
		}
	}

/*
 *  Save acct->games
 */

	if (acct->games_changed)
	{
		sprintf(fnam, "%s/%s/games", ACCT_DIR, acct->player);

		if (acct->games[0] == NULL)
			unlink(fnam);
		else
		{
			int count;

			fp = fopen(fnam, "w");
			if (fp == NULL)
			{
				fprintf(stderr, "can't write %s:", fnam);
				perror("");
				exit(1);
			}

			for (count = 0; acct->games[count]; count++)
			{
				fprintf(fp, "%s\n", acct->games[count]);
			}

			fclose(fp);
		}
	}
}

void
add_game(struct account *acct, char *game)
{
	int i;

	for (i = 0; acct->games[i]; i++)
	{
		if (strcmp(acct->games[i], game) == 0)
			return;
	}

	if (i+1 >= MAX_GAMES)
	{
		fprintf(stderr, "account %s has too many games\n", acct->player);
		return;
	}

	acct->games[i] = str_save(game);
	acct->games[i+1] = NULL;
	acct->games_changed = TRUE;
}

void
remove_game(struct account *acct, char *game)
{
	int i;

	for (i = 0; acct->games[i]; i++)
	{
		if (strcmp(acct->games[i], game) == 0)
			break;
	}

	if (acct->games[i] == NULL)
		return;

	while (acct->games[i+1])
	{
		acct->games[i] = acct->games[i+1];
		i++;
	}

	acct->games[i] = NULL;
	acct->games_changed = TRUE;
}

char *
get_email_from_file(char *pl, char *file)
{
	static char ret[LEN];
	char buf[LEN];
	FILE *fp;
	char *p;

	*ret = '\0';

	fp = fopen(file, "r");
	if (fp == NULL)
		return ret;

	while (fgets(buf, LEN, fp) != NULL)
	{
		for (p = buf; *p && *p != '\n'; p++)
			;
		*p = '\0';

		for (p = buf; *p && *p != '|'; p++)
			;
		if (*p == '|')
			*p++ = 0;

		if (strcmp(buf, pl) != 0)
			continue;

		strcpy(ret, p);
		break;
	}

	fclose(fp);

	return ret;
}

char *
get_email(char *pl)
{
	static char ret[LEN];
	static char fnam[LEN];

	strcpy(ret, get_email_from_file(pl, "/g1/factions"));

	if (*ret == '\0')
		strcpy(ret, get_email_from_file(pl, "/g2/factions"));

	if (*ret == '\0')
		strcpy(ret, get_email_from_file(pl, "/arena/factions"));

	if (*ret == '\0')
	{
		FILE *fp;
		char *p;

		sprintf(fnam, "%s/%s/email", ACCT_DIR, pl);

		fp = fopen(fnam, "r");
		if (fp)
		{
			if (fgets(ret, LEN, fp) != NULL)
			{
				for (p = ret; *p && *p != '\n'; p++)
					;
				*p = '\0';
			}

			fclose(fp);
		}
	}

	return ret;
}

char *
get_name(char *pl)
{
	static char ret[LEN];
	char fnam[LEN];
	FILE *fp;
	char *p;

	*ret = '\0';

	sprintf(fnam, "%s/%s/name", ACCT_DIR, pl);

	fp = fopen(fnam, "r");
	if (fp == NULL)
		return ret;

	if (fgets(ret, LEN, fp) != NULL)
	{
		for (p = ret; *p && *p != '\n'; p++)
			;
		*p = '\0';
	}

	fclose(fp);

	return ret;
}

char *
get_name_email(char *pl)
{
	static char ret[LEN];

	sprintf(ret, "%s <%s>", get_name(pl), get_email(pl));
	return ret;
}

char *months[] = {
	"jan", "feb", "mar", "apr", "may", "jun",
	"jul", "aug", "sep", "oct", "nov", "dec"
};

char *
get_date() {
	struct tm *t;
	long l;
	static char buf[LEN];

	time(&l);
	t = localtime(&l);

	sprintf(buf, "%2d-%s-%2d", t->tm_mday, months[t->tm_mon], t->tm_year);

	return buf;
}

void
log(struct account *acct, double val, char *comment)
{
	FILE *fp;
	char fnam[LEN];

	sprintf(fnam, "%s/%s/log", ACCT_DIR, acct->player);

	fp = fopen(fnam, "a+");
	if (fp == NULL)
	{
		fprintf(stderr, "can't write %s:", fnam);
		perror("");
		exit(1);
	}

	fprintf(fp, "%s\t%-25s\t%6.2lf\n", get_date(), comment, val);
	fclose(fp);
}

char *
alloc_account()
{
	char fnam[LEN];
	char new[LEN];
	FILE *fp;
	FILE *newfp;
	char buf[LEN];
	static char player[LEN];
	char *p;
	struct account *acct;

	sprintf(fnam, "%s/.alloc", ACCT_DIR);
	sprintf(new, "%s/.new_alloc", ACCT_DIR);

	fp = fopen(fnam, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "can't read %s:", fnam);
		perror("");
		exit(1);
	}

	if (fgets(player, LEN, fp) == NULL)
	{
		fprintf(stderr, "file %s empty\n", fnam);
		exit(1);
	}

	newfp = fopen(new, "w");
	if (newfp == NULL)
	{
		fprintf(stderr, "can't write %s:", new);
		perror("");
		exit(1);
	}

	while (fgets(buf, LEN, fp) != NULL)
	{
		fputs(buf, newfp);
	}

	fclose(newfp);
	fclose(fp);

	unlink(fnam);
	if (rename(new, fnam) < 0)
	{
		fprintf(stderr, "rename(%s, %s) failed:", new, fnam);
		perror("");
		exit(1);
	}

	for (p = player; *p && *p != '\n'; p++)
		;
	*p = '\0';

	sprintf(fnam, "%s/%s", ACCT_DIR, player);

	if (mkdir(fnam, 0700) < 0)
	{
		fprintf(stderr, "can't mkdir %s:", fnam);
		perror("");
		exit(1);
	}

	acct = new_account();
	acct->player = str_save(player);
	save_account(acct);
	log(acct, 0.0, "new account");
	free_account(acct);

	return player;
}

void
turn(struct account *acct, double fee, char *comment)
{
	double paid = 0.0;
	double free_paid = 0.0;

	if (acct->balance > 0)
	{
		paid = min(acct->balance, fee);
		acct->balance -= paid;
		fee -= paid;
	}

	if (fee > 0 && acct->free > 0)
	{
		free_paid = min(acct->free, fee);
		acct->free -= free_paid;
		fee -= free_paid;
	}

	if (fee > 0)
	{
		acct->balance -= fee;
		paid += fee;
	}

	log(acct, (-paid), comment);
}

int
check_turn(struct account *acct, double fee)
{

	if (acct->balance + acct->free >= fee)
		return TRUE;
	return FALSE;
}

void
trans(struct account *acct, double amount, char *comment)
{
	acct->balance += amount;
	log(acct, amount, comment);
}

void
show(struct account *acct, int n)
{
	char cmd[LEN];
	double bal;
	int free;

	printf("Account: %s\n\n", acct->player);
	fflush(stdout);

	sprintf(cmd, "tail -%d %s/%s/log", n, ACCT_DIR, acct->player);
	system(cmd);

	fflush(stdout);

	printf("\nBalance:        $%.2lf\n", acct->balance);
	if (acct->free > 0)
		printf("Free balance:   $%.2lf\n", acct->free);

	fflush(stdout);
}

void
list_accounts(double max_balance, int cc_only, char *game, int free_flag)
{
	DIR *d;
	struct dirent *e;
	char fnam[LEN];
	double bal;
	int free;
	struct account *acct;

	d = opendir(ACCT_DIR);

	if (d == NULL)
	{
		fprintf(stderr, "can't open %s: ", ACCT_DIR);
		perror("");
		exit(1);
	}

	while ((e = readdir(d)) != NULL)
	{
		if (*(e->d_name) == '.')
			continue;

		acct = new_account();
		load_account(e->d_name, acct);

		if (acct->games[0] == NULL) {	/* not in any games */
			free_account(acct);
			continue;
		}

		if (free_flag)
		{
			if (acct->free <= 0)
			{
				free_account(acct);
				continue;
			}
		}
		else if (acct->balance + acct->free > max_balance)
		{
			free_account(acct);
			continue;
		}

		if (cc_only)
		{
			sprintf(fnam, "%s/%s/cc.pgp", ACCT_DIR, e->d_name);

			if (access(fnam, R_OK) < 0)
			{
				free_account(acct);
				continue;
			}
		}

		if (game && *game)
		{
			int i;

			for (i = 0; acct->games[i]; i++)
				if (strcmp(game, acct->games[i]) == 0)
					break;

			if (acct->games[i] == NULL) {	/* not found */
				free_account(acct);
				continue;
			}
		}

		printf("%s  %6.2lf    %6.2lf    %s\n",
					acct->player,
					acct->balance,
					acct->free,
					get_name_email(acct->player));

		free_account(acct);
	}

	closedir(d);
}

void
delete_account(char *pl)
{
	char one[LEN], two[LEN];

	mkdir(OLD_ACCT_DIR, 0700);

	sprintf(one, "%s/%s", ACCT_DIR, pl);
	sprintf(two, "%s/%s", OLD_ACCT_DIR, pl);

	if (rename(one, two) < 0)
	{
		fprintf(stderr, "rename(%s, %s) failed:", one, two);
		perror("");
		exit(1);
	}
}

void
list_deleted_accounts(int reclaim)
{
	DIR *d;
	struct dirent *e;
	char fnam[LEN];
	double bal;
	int free;
	char *idle_s;
	struct account *acct;

	d = opendir(ACCT_DIR);

	if (d == NULL)
	{
		fprintf(stderr, "can't open %s: ", ACCT_DIR);
		perror("");
		exit(1);
	}

	while ((e = readdir(d)) != NULL)
	{
		if (*(e->d_name) == '.')
			continue;

		acct = new_account();
		load_account(e->d_name, acct);

		if (acct->games[0] != NULL) {
			free_account(acct);
			continue;
		}

		if (reclaim)
			delete_account(acct->player);
		else
			printf("%s  %6.2lf    %6.2lf    %s\n",
					acct->player,
					acct->balance,
					acct->free,
					get_name_email(acct->player));

		free_account(acct);
	}

	closedir(d);
}

/*

	acct -g game -n [-p player]			add a new player, info on stdin
		invokes add-$game script?

	acct -g game -p player -t x -y comment		charge turn costing x
	acct -p player -l x -y comment			credit x to player account
	acct -g game -p player -d			player dropped from game
	acct -p player -s n				account summary
	acct -p player -T x				can a turn costing x be run?
	have a global free flag

 */


double tflag_arg = 0.0;
double Tflag_arg = 0.0;
double lflag_arg = 0.0;
double Lflag_arg = 0.0;
char player[LEN];
char game[LEN];
char comment[LEN];
int nflag = 0;
int sflag = 0;
int sflag_arg = 0;
int tflag = 0;
int Tflag = 0;
int lflag = 0;
int cflag = 0;
int Lflag = 0;
int dflag = 0;
int Dflag = 0;
int Rflag = 0;
int Fflag = 0;


usage()
{
    fprintf(stderr, "usage: acct [options]\n");
    fprintf(stderr, "   -n    add a new player (give -g)\n");
    fprintf(stderr, "   -l n  log credit of n (give p and y)\n");
    fprintf(stderr, "   -t x  charge a turn costing x (give p and y)\n");
    fprintf(stderr, "   -T x  can a turn costing X be run?\n");
    fprintf(stderr, "   -p n  player account to credit/debit to\n");
    fprintf(stderr, "   -y n  comment for credit/debit\n");
    fprintf(stderr, "   -g x  what game (g1, g2, arena, ...)\n");
    fprintf(stderr, "   -L x  show all player accounts below x\n");
    fprintf(stderr, "   -c       .. that pay by credit card\n");
    fprintf(stderr, "   -D    List accounts with no games\n");
    fprintf(stderr, "   -F    List accounts with free balances\n");
    fprintf(stderr, "   -R    Move deleted accounts to OLD_ACCT_DIR\n");
    fprintf(stderr, "   -d    Remove player from -g game\n");
    exit(1);
}

main(argc, argv)
int argc;
char **argv;
{
	extern int optind, opterr;
	extern char *optarg;
	int errflag = 0;
	int c;
	struct account *acct;

	switch (getuid())
	{
	case 201:	/* skrenta */
	case 311:	/* srt */
	case 312:	/* arena */
		break;

	default:
		fprintf(stderr, "not authorized\n");
		exit(1);
	}

	{
		FILE *fp;
		char buf[LEN];
		long l;
		int i;
		char *p;

		sprintf(buf, "%s/.acct_log", ACCT_DIR);
		fp = fopen(buf, "a+");
		if (fp == NULL)
		{
			fprintf(stderr, "can't append %s", buf);
			perror("");
			exit(1);
		}

		time(&l);
		sprintf(buf, "%s uid=%d acct", ctime(&l), getuid());

		for (p = buf; *p && *p != '\n'; p++)
			;
		if (*p)
			*p = ' ';

		for (i = 1; i < argc; i++)
		{
			strcat(buf, " ");
			strcat(buf, argv[i]);
		}

		fprintf(fp, "%s\n", buf);
		fclose(fp);
	}

	acct = new_account();

	strcpy(player, "");
	strcpy(game, "");
	strcpy(comment, "n/a");

	while ((c = getopt(argc, argv, "FRDcdL:nl:p:g:y:s:t:T:?")) != EOF)
	{
		switch (c)
		{
		case 'd':		/* remove player from -g game */
			dflag++;
			break;

		case 'F':		/* list free accounts */
			Fflag++;
			break;

		case 'D':		/* list accounts with no games */
			Dflag++;
			break;

		case 'R':		/* move deleted accounts to OLD_ACCT_DIR */
			Rflag++;
			break;

		case 'p':		/* -p player */
			strcpy(player, optarg);
			break;

		case 'g':		/* -g game */
			strcpy(game, optarg);
			break;

		case 'y':		/* -y comment */
			strcpy(comment, optarg);
			break;

		case 't':		/* charge a turn costing X */
			tflag++;
			sscanf(optarg, "%lf", &tflag_arg);
			break;

		case 'T':		/* can we afford a turn that costs X? */
			Tflag++;
			sscanf(optarg, "%lf", &Tflag_arg);
			break;

		case 'n':		/* new player (info on stdin) */
			nflag++;
			break;

		case 'l':		/* log credit to player account (-y comment) */
			lflag++;
			sscanf(optarg, "%lf", &lflag_arg);
			break;

		case 'L':		/* show accounts below x */
			Lflag++;
			sscanf(optarg, "%lf", &Lflag_arg);
			break;

		case 'c':		/* (with -L) only show credit card accounts */
			cflag++;
			break;

		case 's':		/* show account and last n transactions */
			sflag++;
			sflag_arg = atoi(optarg);
			break;

		case '?':
		default:
			errflag++;
			break;
		}
	}

	if (errflag)
		usage();	/* doesn't return */

	if (nflag)
	{
		if (!*game)
		{
			fprintf(stderr, "acct: -n requires -g\n");
			exit(1);
		}

		if (!*player)	/* allocate new account id */
		{
			strcpy(player, alloc_account());
		}

		load_account(player, acct);
		add_game(acct, game);
		save_account(acct);
		printf("%s\n", player);
	}
	else if (lflag)
	{
		load_account(player, acct);
		trans(acct, lflag_arg, comment);
		save_account(acct);
	}
	else if (Tflag)
	{
		load_account(player, acct);
		if (check_turn(acct, Tflag_arg))
			exit(0);
		exit(1);
	}
	else if (tflag)
	{
		load_account(player, acct);
		turn(acct, tflag_arg, comment);
		save_account(acct);
	}
	else if (sflag)
	{
		load_account(player, acct);
		show(acct, sflag_arg);
	}
	else if (Lflag)
	{
		list_accounts(Lflag_arg, cflag, game, FALSE);
	}
	else if (Fflag)
	{
		list_accounts(0.0, cflag, game, TRUE);
	}
	else if (dflag)
	{
		if (!*game)
		{
			fprintf(stderr, "acct: -d requires -g\n");
			exit(1);
		}

		if (!*player)
		{
			fprintf(stderr, "acct: -d requires -g\n");
			exit(1);
		}

		load_account(player, acct);
		remove_game(acct, game);
		save_account(acct);
	}
	else if (Dflag)
	{
		list_deleted_accounts(FALSE);
	}
	else if (Rflag)
	{
		list_deleted_accounts(TRUE);
	}
	else
		usage();

	exit(0);
}

