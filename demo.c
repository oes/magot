/* This file is part of magot.
 *
 * Magot is free software. You can redistribute it and/or modify it
 * under the terms of the Do What The Fuck You Want To Public License,
 * Version 2, as published by Sam Hocevar. See http://www.wtfpl.net/
 * for more details.
 *
 * Magot is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <magot.h>
#include <stdio.h>
#include <string.h>

#define EMPTY(str) (str == NULL || *str == '\0')

void print_usage(int optc, magot_t **optv, magot_style_t style) {
  puts("Usage: demo [OPTIONS]");
  puts("SYNOPSIS");
  puts("  demo --foo foobar -z");
  puts("  demo -f foobar -qz --lorem-ipsum");
  puts("OPTIONS");
  magot_print_help(stdout, optc, optv, style);
}

char *opt_value_str(magot_t *opt) {
  if (opt->flag) {
    return magot_isset(opt) ? "true" : "false";
  } else {
    return magot_isset(opt) ? opt->value : "N/A";
  }
}

int main(int argc, char **argv) {
  // the magot_t struct describes an option
  magot_t foo, bar, baz, quux, lorem;
  magot_t *opts[] = {
    // In case it's not obvious, the first two strings are
    // long name and short name, the boolean specifies whether
    // the opt is required and the last string is the help text.
    magot_opt(&foo, "foo", "f", true,
	      "the foo option. 'tis required."),
    magot_opt(&bar, "bar", "b", false,
	      "the bar option. 'tis optional."),
    // flags obviously can't be mandatory so there's
    // no required parameter
    magot_flag(&baz, "baz", "z", "a useless flag"),

    // null or empty string is all the same to magot
    magot_flag(&lorem, "lorem-ipsum", NULL, ""),

    // you don't have to give long and short names
    // but at least one of them must not be empty
    magot_flag(&quux, "", "q", "the quux flag"),

    // When using GNU style there is no difference
    // between long and short names and you could use
    // a longer string for the short name than for the long name
    // with no problems.
    // If you use POSIX style however, you should stick to
    // multiple characters for the long name otherwise
    // magot accepts the long name with a short prefix.
    // You can use more than one character for the short name
    // but such an opt won't be able to participate in
    // flag clusters and it's probably confusing.
  };
  // magot needs to know the number of opts
  int optc = sizeof(opts) / sizeof(opts[0]);

  // customize the argument name used in the help output
  foo.arg_name = "file";

  magot_parser_t parser;
  magot_parser(&parser, argc, argv);

  // By default magot starts to parse at the second arg
  // i.e. it ignores the first argument.
  // you can control this by changing the value of
  // parser.offset which defaults to 1.

  // If you want to collect remaining arguments allocate
  // an array that's at least parser.argc - parser.offset big.
  // If you don't assign this array then no remaining
  // arguments will be collected and magot will fail
  // if it encounters them.
  char *remaining[parser.argc - parser.offset];
  parser.remaining = remaining;

  // the opt style defaults to MAGOT_STYLE_POSIX
  // parser.style = MAGOT_STYLE_GNU

  if (argc == 1) {
    // print usage if we have no args
    print_usage(optc, opts, parser.style);
    return 0;
  }

  magot_err_t err;
  memset(&err, 0, sizeof(magot_err_t));
  bool success = magot_parse(optc, opts, &parser, &err);
  if (!success) {
    printf("%s: %s\n", magot_errstr(&err), err.arg);
    return 1;
  }

  // dump our results
  for (int i = 0; i < optc; ++i) {
    magot_t *opt = opts[i];
    printf("%s: <%s>\n",
	   EMPTY(opt->name) ? opt->short_name : opt->name,
	   opt_value_str(opt));
  }
  if (parser.rem_count > 0) {
    fputs("remaining args: ", stdout);
    for (int i = 0; i < parser.rem_count; ++i) {
      fprintf(stdout, "'%s'", parser.remaining[i]);
      if (i + 1 < parser.rem_count) {
	fputs(", ", stdout);
      }
    }
    puts("");
  }
  return 0;
}
