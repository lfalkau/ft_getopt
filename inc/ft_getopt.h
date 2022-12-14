// SPDX-FileCopyrightText: 2022 lfalkau
// SPDX-License-Identifier: Apache-2.0

#ifndef FT_GETOPT_H
#define FT_GETOPT_H

#define no_argument 0
#define required_argument 1

#define no_opt 0
#define short_opt 1
#define long_opt 2
#define break_opt 3

typedef struct option {
	const char	*longname;
	int			shortname;
	int			has_arg;
} option_t;

int ft_getopt_long(int ac, char **av, const option_t *opts, char **optarg);

#endif
