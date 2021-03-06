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

#ifndef MAGOT_H
#define MAGOT_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum {
  MAGOT_ERR_MISSING_REQUIRED,
  MAGOT_ERR_MISSING_ARG,
  MAGOT_ERR_UNKNOWN_OPT
} magot_errtype_t;

typedef enum {
  MAGOT_STYLE_POSIX,
  MAGOT_STYLE_GNU
} magot_style_t;

typedef struct {
  magot_style_t style;
  int argc;
  char **argv;
  int offset;
  int rem_count;
  char **remaining;
  magot_errtype_t err_type;
  char *err_arg;
  bool mixed;
} magot_parser_t;

typedef struct {
  char *name;
  char *short_name;
  bool flag;
  bool required;
  char *help;
  char *arg_name;
  char *value;
} magot_t;

void magot_parser(magot_parser_t *parser,
                  int argc,
                  char **argv);

magot_t *magot_init(magot_t *opt,
                    char *name,
                    char *short_name,
                    bool flag,
                    bool required,
                    char *help);

magot_t *magot_opt(magot_t *opt,
                   char *name,
                   char *short_name,
                   bool required,
                   char *help);

magot_t *magot_flag(magot_t *opt,
                    char *name,
                    char *short_name,
                    char *help);

bool magot_parse(int optc, magot_t **optv,
                 magot_parser_t *parser);

bool magot_isset(magot_t *opt);

void magot_print_help(FILE *f,
                      int optc,
                      magot_t **optv,
                      magot_style_t style);

void magot_print_error(FILE *f, magot_parser_t *parser);

void magot_unset(magot_t *opt);

bool magot_is_flag(magot_t *opt);

void magot_set_arg_name(magot_t *opt, char *name);

char *magot_long_name(magot_t *opt);

char *magot_short_name(magot_t *opt);

char *magot_name(magot_t *opt);

char *magot_value(magot_t *opt);

void magot_set_style(magot_parser_t *p, magot_style_t style);

magot_style_t magot_get_style(magot_parser_t *p);

void magot_set_remaining(magot_parser_t *p, char **rem);

int magot_remaining_size(magot_parser_t *p);

void magot_set_offset(magot_parser_t *p, int offset);

char *magot_err_arg(magot_parser_t *p);

magot_errtype_t magot_err_type(magot_parser_t *p);

int magot_args_size(magot_parser_t *p);

void magot_allow_mixed(magot_parser_t *p, bool mixed);

#endif /* MAGOT_H */
