/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
    INT_LITERAL = 258,
    DOUBLE_LITERAL = 259,
    IDENTIFIER = 260,
    DEFINE = 261,
    IF = 262,
    ELSE = 263,
    WHILE = 264,
    LP = 265,
    RP = 266,
    LC = 267,
    RC = 268,
    SEMICOLON = 269,
    COMMA = 270,
    ASSIGN = 271,
    EQ = 272,
    NE = 273,
    GT = 274,
    GE = 275,
    LT = 276,
    LE = 277,
    ADD = 278,
    SUB = 279,
    MUL = 280,
    DIV = 281,
    MOD = 282
  };
#endif
/* Tokens.  */
#define INT_LITERAL 258
#define DOUBLE_LITERAL 259
#define IDENTIFIER 260
#define DEFINE 261
#define IF 262
#define ELSE 263
#define WHILE 264
#define LP 265
#define RP 266
#define LC 267
#define RC 268
#define SEMICOLON 269
#define COMMA 270
#define ASSIGN 271
#define EQ 272
#define NE 273
#define GT 274
#define GE 275
#define LT 276
#define LE 277
#define ADD 278
#define SUB 279
#define MUL 280
#define DIV 281
#define MOD 282

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 6 "calc.y" /* yacc.c:1909  */

    char		*identifier;
    ParameterList	*parameter_list;
    Expression		*expression;

#line 114 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
