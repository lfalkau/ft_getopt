#include <string.h>
#include <ft_getopt.h>
#include <stdio.h>
#include <unistd.h>

/*
	isopt checks if @s is a command line option, and of which kind
	Returns the option kind
*/
static int getargtype(const char *s) {
	if (strlen(s) < 2 || s[0] != '-')
		return no_opt;
	if (s[1] != '-')
		return short_opt;
	if (strlen(s) > 2)
		return long_opt;
	return break_opt;
}

/*
	findopt iterates through an option_t array and tries to match the next option character/string with.
	Returns a pointer to the matched option, or NULL if no match is found
	
	An error message is printed if no match is found.
	TODO: Add an interface to dissble error message e.g. environment variable
*/
static const option_t *findopt(char **av, const int av_type, char *nextchar, int optindex, const option_t *opts) {
	for (const option_t *o = opts; o->longname != NULL || o->shortname != '\0'; o += 1) {
		if (av_type == short_opt && ((nextchar == NULL && av[optindex][1] == o->shortname) || (nextchar != NULL && *nextchar == o->shortname))) {
			return o;
		}
		if (av_type == long_opt && strcmp(&av[optindex][2], o->longname) == 0) {
			return o;
		}
	}
	return NULL;
}

/*
	updoptptrs updates optindex and nextchar so that the next option can be proceeded.
	optindex stores the index of the next argument to be proceeded in @av
	nextchar stores a pointer to the next character to be proceeded (useful for short options combinations)

	For more details, see the official getopt documentation:
	https://www.man7.org/linux/man-pages/man3/getopt.3.html
*/
static void updoptptrs(char **av, const int av_type, int *oip, char **ncp) {
	if (av_type == short_opt) {
		*ncp = *ncp ? *ncp + 1 : &av[*oip][2];
		if (**ncp == '\0')
			*ncp = NULL;
	}
	if (*ncp == NULL) {
		*oip += 1;
	}
}

/*
	getoptarg return a pointer to the option argument, which is either the nextchar pointer
	if set, or the next av argument, pointed by optindex. It also update the nextchar and
	optindex values
	Returns the argument pointer, or NULL if no argument is present
*/
char *getoptarg(int ac, char **av, int *oip, char **ncp) {
	char *optarg = NULL;

	if (*oip < ac) {
		optarg = *ncp ? *ncp : av[*oip];
		*oip += 1;
		*ncp = NULL;
	}
	return optarg;
}

int ft_getopt(int ac, char **av, const char *optstring, char **optarg) {
	return -1;
}

int ft_getopt_long(int ac, char **av, const option_t *opts, char **optarg) {
	static int optindex = 1;
	static char	*nextchar = NULL;
	const option_t	*opt = NULL;
	int av_type;

	if (optindex >= ac)
		return -1;

	av_type = getargtype(av[optindex]);
	if (av_type == no_opt || av_type == break_opt) {
		// TODO: reorganize arguments it no_opt, to accept post arg options
		return -1;
	}

	opt = findopt(av, av_type, nextchar, optindex, opts);
	if (opt == NULL) {
		if (av_type == short_opt)
			printf("%s: Invalid short option -- '%c'\n", av[0], av[optindex][1]);
		else
			printf("%s: Invalid long option -- '%s'\n", av[0], &av[optindex][2]);
	}
	updoptptrs(av, av_type, &optindex, &nextchar);
	
	if (opt && opt->has_arg) {
		*optarg = getoptarg(ac, av, &optindex, &nextchar);
		if (*optarg == NULL) {
			if (av_type == short_opt)
				printf("%s: Option requires an argument -- '%c'\n", av[0], av[optindex - 1][1]);
			else
				printf("%s: Option requires an argument -- '%s'\n", av[0], &av[optindex - 1][2]);
		}
	}
	return opt ? opt->shortname : '?';
}

int main(int ac, char **av) {
	option_t opts[] = {
		{"verbose",		'v',	no_argument},
		{"help",		'h',	no_argument},
		{"count",		'c',	required_argument},
		{"timestamp",	'D',	no_argument},
		{"flood",		'f',	no_argument},
		{"interval",	'i',	required_argument},
		{"quiet",		'q',	no_argument},
		{NULL,			'\0',	no_argument},
	};
	int opt;
	char *optarg;

	while ((opt = ft_getopt_long(ac, av, opts, &optarg)) != -1) {
		switch (opt) {
		case 'v':
			printf("-v\n");
			break;
		case 'h':
			printf("-h\n");
			break;
		case 'c':
			printf("-c: %s\n", optarg);
			break;
		case 'D':
			printf("-D\n");
			break;
		case 'f':
			printf("-f\n");
			break;
		case 'i':
			printf("-i: %s\n", optarg);
			break;
		case 'q':
			printf("-q\n");
			break;
		case '?':
			printf("Usage: ...\n");
			break;
		}
		// sleep(1);
	}

}
