/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUMBER = 258,
    STRING = 259,
    PRINT = 260,
    VAR = 261,
    BLTIN = 262,
    UNDEF = 263,
    WHILE = 264,
    IF = 265,
    ELSE = 266,
    FUNCTION = 267,
    PROCEDURE = 268,
    RETURN = 269,
    FUNC = 270,
    PROC = 271,
    READ = 272,
    ARG = 273,
    OR = 274,
    AND = 275,
    GT = 276,
    GE = 277,
    LT = 278,
    LE = 279,
    EQ = 280,
    NE = 281,
    UNARYMINUS = 282,
    NOT = 283
  };
#endif
/* Tokens.  */
#define NUMBER 258
#define STRING 259
#define PRINT 260
#define VAR 261
#define BLTIN 262
#define UNDEF 263
#define WHILE 264
#define IF 265
#define ELSE 266
#define FUNCTION 267
#define PROCEDURE 268
#define RETURN 269
#define FUNC 270
#define PROC 271
#define READ 272
#define ARG 273
#define OR 274
#define AND 275
#define GT 276
#define GE 277
#define LT 278
#define LE 279
#define EQ 280
#define NE 281
#define UNARYMINUS 282
#define NOT 283

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 21 "hoc.y" /* yacc.c:1909  */

	Symbol 	*sym;		/* symbol table pointer */
	Inst 	*inst;		/* machine instruction */
	int		narg;		/* number of arguments */

#line 116 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
