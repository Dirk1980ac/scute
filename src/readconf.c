/* readconf.c - Read configuration file
 * Copyright (C) 2020 g10 Code GmbH
 *
 * This file is part of Scute.
 *
 * Scute is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * Scute is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see <https://gnu.org/licenses/>.
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GPGRT_ENABLE_ARGPARSE_MACROS 1
#include <gpg-error.h>

#include "options.h"

static const char *
my_strusage (int level)
{
  const char *p;

  switch (level)
    {
    case 9:  p = "LGPL-2.1-or-later"; break;
    case 11: p = "scute"; break;
    default: p = NULL;
    }
  return p;
}


/* Read the global configuration file.  This functon needs to be
 * called early.  */
void
_scute_read_conf (void)
{
  enum { oNull = 500, oUser, oDebug, oLogfile };
  gpgrt_opt_t opts[] =
    {
     ARGPARSE_s_s(oUser, "user", NULL ),
     ARGPARSE_s_s(oDebug, "debug", NULL),
     ARGPARSE_s_s(oLogfile, "log-file", NULL),
     ARGPARSE_end()
    };
  int dummy_argc = 0;
  char **dummy_argv = NULL;
  gpgrt_argparse_t pargs = { &dummy_argc, &dummy_argv, ARGPARSE_FLAG_SYS };

  gpgrt_set_strusage (my_strusage);
  gpgrt_set_confdir (GPGRT_CONFDIR_SYS, "/etc/gnupg");

  while (gpgrt_argparser  (&pargs, opts, "scute.conf"))
    {
      switch (pargs.r_opt)
        {
        case oUser:
          free (_scute_opt.user);
          _scute_opt.user = strdup (pargs.r.ret_str);
          break;
        case oDebug: _scute_opt.debug_flags = 1; break;
        case oLogfile: break;
        case ARGPARSE_CONFFILE: break;
        default : pargs.err = ARGPARSE_PRINT_WARNING; break;
	}
    }

  gpgrt_argparse (NULL, &pargs, NULL);

}
