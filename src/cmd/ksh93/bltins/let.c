/***********************************************************************
 *                                                                      *
 *               This software is part of the ast package               *
 *          Copyright (c) 1982-2012 AT&T Intellectual Property          *
 *                      and is licensed under the                       *
 *                 Eclipse Public License, Version 1.0                  *
 *                    by AT&T Intellectual Property                     *
 *                                                                      *
 *                A copy of the License is available at                 *
 *          http://www.eclipse.org/org/documents/epl-v10.html           *
 *         (with md5 checksum b35adb5213ca9657e911e9befb180842)         *
 *                                                                      *
 *              Information and Software Systems Research               *
 *                            AT&T Research                             *
 *                           Florham Park NJ                            *
 *                                                                      *
 *                    David Korn <dgkorn@gmail.com>                     *
 *                                                                      *
 ***********************************************************************/
//
// let expr...
//
//   David Korn
//   AT&T Labs
//
#include "config_ast.h"  // IWYU pragma: keep

#include <getopt.h>
#include <stdlib.h>

#include "builtins.h"
#include "defs.h"
#include "shcmd.h"

static const char *short_options = "+:";
static const struct option long_options[] = {
    {"help", no_argument, NULL, 1},  // all builtins support --help
    {NULL, 0, NULL, 0}};

int b_let(int argc, char *argv[], Shbltin_t *context) {
    Shell_t *shp = context->shp;
    char *cmd = argv[0];
    int opt;

    optind = opterr = 0;
    while ((opt = getopt_long_only(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (opt) {
            case 1: {
                builtin_print_help(shp, cmd);
                return 0;
            }
            case ':': {
                builtin_missing_argument(shp, cmd, argv[optind - 1]);
                return 2;
            }
            case '?': {
                builtin_unknown_option(shp, cmd, argv[optind - 1]);
                return 2;
            }
            default: { abort(); }
        }
    }
    argv += optind;

    if (!*argv) {
        builtin_usage_error(shp, cmd, "you must provide at least one expression");
        return 2;
    }

    int rv = 0;
    while (*argv) {
        rv = !sh_arith(shp, *argv);
        argv++;
    }
    return rv;
}
