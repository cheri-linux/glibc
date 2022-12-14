# Copyright (C) 1991-2018 Free Software Foundation, Inc.
# This file is part of the GNU C Library.

# The GNU C Library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# The GNU C Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with the GNU C Library; if not, see
# <http://www.gnu.org/licenses/>.

# errno.texi contains lines like:
# @deftypevr Macro int ENOSYS
# @errno{ENOSYS, 78, Function not implemented}
# Descriptive paragraph...
# @end deftypevr

BEGIN {

    # Here we list the E* names that might be duplicate names for the
    # same integer value on some systems.  This causes the code below
    # to generate ``#if defined (ALIAS) && ALIAS != ORIGINAL'' in the code,
    # so the output does not presume that these are in fact aliases.
    # We list here all the known potential cases on any system,
    # so that the C source we produce will do the right thing based
    # on the actual #define'd values it's compiled with.
    alias["EWOULDBLOCK"]= "EAGAIN";
    alias["EDEADLOCK"]	= "EDEADLK";
    alias["ENOTSUP"]	= "EOPNOTSUPP";

    print "/* This file is generated from errno.texi by errlist.awk.  */"
    print "";
    print "#include <errno.h>";
    print "#include <libintl.h>";
    print "";
    print "#ifndef ERR_REMAP";
    print "# define ERR_REMAP(n) n";
    print "#endif";
    print "";

    print "#if !defined EMIT_ERR_MAX && !defined ERRLIST_NO_COMPAT";
    print "# include <errlist-compat.h>";
    print "#endif";
    print "#ifdef ERR_MAX";
    print "# define ERRLIST_SIZE ERR_MAX + 1";
    print "#else"
    print "# define ERR_MAX 0";
    print "# define ERRLIST_SIZE";
    print "#endif";

    print "const char *const _sys_errlist_internal[ERRLIST_SIZE] =";
    print "  {";
    print "    [0] = N_(\"Success\"),"
  }

/^@errno\{/ \
  {
    etext = $3;
    for (i = 4; i <= NF; ++i)
      etext = etext " " $i;
    etext = substr(etext, 1, length(etext)-1)
    e = substr($1, 8, length($1)-8)
    errno = substr($2, 1, length($2)-1) + 0
    if (alias[e])
      printf "#if defined (%s) && %s != %s\n", e, e, alias[e];
    else
      printf "#ifdef %s\n", e;
    errnoh = 4;
    desc="";
    next;
  }
errnoh == 4 && $1 == "@end" && $2 == "deftypevr" \
  {
    printf "/*%s */\n", desc;
    printf "    [ERR_REMAP (%s)] = N_(\"%s\"),\n", e, etext;
    printf "# if %s > ERR_MAX\n", e;
    print  "# undef ERR_MAX";
    printf "# define ERR_MAX %s\n", e;
    print  "# endif";
    print "#endif";
    errnoh = 0;
    next;
  }
errnoh == 4 \
  {
    # This magic tag in C comments gets them copied into libc.pot.
    desc = desc "\nTRANS" ($0 != "" ? " " : "") $0; next
  }
END {
  print "  };";
  print "";
  print "#define NERR \\";
  print "  (sizeof _sys_errlist_internal / sizeof _sys_errlist_internal [0])";
  print "const int _sys_nerr_internal = NERR;"
  print "";
  print "#if IS_IN (libc) && !defined ERRLIST_NO_COMPAT";
  print "# include <errlist-compat.c>";
  print "#endif";
  print "";
  print "#ifdef EMIT_ERR_MAX";
  print "void dummy (void)"
  print "{ asm volatile (\".ascii \\\" @@@ %0 @@@ \\\"\" : : \"i\" (ERR_REMAP (ERR_MAX))); }"
  print "#endif";
}
