

#include <stdio.h>

main() {
	char prev[1024];
	char buf[1024];
	char fnam[1024];
	char *p;
	FILE *fp = NULL;
	FILE *indexfp;
	int num;
	int count=0;

	indexfp = fopen("index.html", "w");

	while (fgets(buf, 1024, stdin) != NULL)
	{
		for (p = buf; *p && *p != '\n'; p++)
			;
		*p = '\0';

		if (strncmp(buf, "---", 3) == 0) {
			if (fp) {
				fprintf(fp, "</pre>\n");
				fclose(fp);
			}

			for (p = prev; *p && *p != '['; p++)
				;
			if (*p != '[') {
				fprintf(stderr, "error\n");
				exit(1);
			}
			p++;
			num = atoi(p);
			sprintf(fnam, "%d.html", num);
			fp = fopen(fnam, "w");
			fprintf(fp, "<pre>\n");

			if (num % 10 == 0)
			{
				fprintf(indexfp, "%3d-%4d|</ul><p>\n", num, count++);
				fprintf(indexfp, "%3d-%4d|\n", num, count++);
				fprintf(indexfp,
					"%3d-%4d|<li><a href=\"%d.html\">%s</a>\n",
					num, count++, num, prev);
				fprintf(indexfp, "%3d-%4d|<ul>\n", num, count++);
			}
			else
			{
				fprintf(indexfp,
					"%3d-%4d|\t<dt><a href=\"%d.html\">%s</a>\n",
					num, count++, num, prev);
			}
		}

		if (fp)
			fprintf(fp, "%s\n", prev);

		strcpy(prev, buf);
	}

	fclose(indexfp);
}

