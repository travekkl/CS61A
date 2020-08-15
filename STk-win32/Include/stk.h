/*****************************************************************-*- C -*-****
 *
 * s t k . h
 *
 * Copyright � 1993-1999 Erick Gallesio - I3S-CNRS/ESSI <eg@unice.fr>
 * 
 *
 * Permission to use, copy, modify, distribute,and license this
 * software and its documentation for any purpose is hereby granted,
 * provided that existing copyright notices are retained in all
 * copies and that this notice is included verbatim in any
 * distributions.  No written agreement, license, or royalty fee is
 * required for any of the authorized uses.
 * This software is provided ``AS IS'' without express or implied
 * warranty.
 *
 *           Author: Erick Gallesio [eg@unice.fr]
 *    Creation date: 12-May-1993 10:34
 * Last file update:  4-Sep-1999 12:38 (eg)
 *
 ******************************************************************************/

#ifndef _STK_H
#define _STK_H

#include <stkvers.h>


#ifdef WIN32
#  define MACHINE "Ms-Win32"
#  include <windows.h>			/* for the panic procedure */
#endif

#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <setjmp.h>
#include <assert.h>
#include <signal.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#ifndef WIN32
#  include <memory.h>
#endif
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#include <stdlib.h>
#include <gmp.h>
  

#ifdef WIN32
#  define USE_HASH
#  define USE_SOCKET
#  define USE_REGEXP
#  define USE_PROCESS
  /* #define USE_POSIX */
#  define USE_HTML
#  define USE_PIXMAP
  /* #define USE_JPEG */
#  define USE_BASE64
#  define USE_LOCALE
#endif


/*
 * Headers <tcl*.h> are always included (even if not USE_TK) for the hash table
 * function prototypes, and for implementing the crazy Tcl_Obj type. 
 */
#ifdef STk_CODE
#   ifdef OSF1
#     define HAS_STDARG  /* Hacky */
#   endif
#endif
#include <tcl.h>
#include <tclInt.h>

/* ------------------------------------------------------------------------------ */

#define COMPACT_SMALL_CST			/* compact coding for small const */

#ifndef FALSE
#  define FALSE 0
#endif
#ifndef TRUE
#  define TRUE (!FALSE)
#endif


#define TKBUFFERN 		1024		/* max size of a token */
#define MAX_CHAR_CODE		255		/* Max code for a char */

#ifdef USE_TK
#  ifdef WIN32
#    define INITIAL_HEAP_SIZE 	40000		/* size of heap Win32 (in cells) */
#  else
#    define INITIAL_HEAP_SIZE 	25000		/* size of heap X11 (in cells) */
#  endif
#else
#  define INITIAL_HEAP_SIZE 	10000		/* size of heap snow (in cells) */
#endif

#ifdef _POSIX_PATH_MAX
#define MAX_PATH_LENGTH	 _POSIX_PATH_MAX
#else
#define MAX_PATH_LENGTH	 256
#endif

#define GC_VERBOSE	"*gc-verbose*"
#define ARGC		"*argc*"
#define ARGV		"*argv*"
#define PROG_NAME	"*program-name*"
#define DEBUG_MODE	"*debug*"
#define EVAL_HOOK	"*eval-hook*"
#define PRINT_BANNER	"*print-banner*"
#define LOAD_PATH	"*load-path*"
#define LOAD_SUFFIXES	"*load-suffixes*"
#define LOAD_VERBOSE	"*load-verbose*"
#define LAST_DEFINED    "*last-defined*"
#define STK_LIBRARY     "*stk-library*"

#define REPORT_ERROR	"report-error"

#ifdef USE_TK
#    include <tk.h>
#    define ROOT_WINDOW	"*root*"	/* Scheme name of main window */
#    define ERROR_INFO  "*error-info*"
#    define USE_THREAD  	/* Very very experimental */
  struct Tk_command {
    ClientData ptr;		/* pointer associated to the widget command */
    Tcl_CmdProc *fct;	   	/* Tk lib function associated to widget */
    Tcl_CmdDeleteProc *delproc; /* procedure to call when command is destroyed */
    ClientData deldata;	   	/* value to pass to delproc */
    short deleted;		/* 1 if command has already been deleted */
    short objproc;		/* 1 if command is an tcl_Obj procedure */
    char Id[1];		   	/* must be last field */
  };
#endif


struct obj {		/* most alignment constraining type first */
  union {struct {struct obj * car; struct obj * cdr;} 		cons;
	 struct {double data;}					flonum;
	 struct {char *pname; struct obj * vcell;} 		symbol;
	 struct {char *name; struct obj * (*f)(void);} 		subr0;
	 struct {char *name; struct obj * (*f)(void *,...);} 	subr;
	 struct {struct obj *env; struct obj *code;} 		closure;
	 struct {struct obj *code; struct obj* env;}		macro;
	 struct {long dim; char *data;} 			string;
	 struct {long dim; struct obj **data;} 			vector;
	 struct {struct port_descr *p; int ungetted_char;}	port;
	 struct {char *data;} 					keyword;
	 struct {MP_INT *data;}					bignum;
	 struct {short level, position; struct obj *symbol;}	localvar;
	 struct {struct obj *expr; int resultknown; }		promise;
	 struct {void *data; }					cont;
	 struct {struct obj *data;}				env;
	 struct {short id; char staticp; void *data; }		extension;
         struct {struct Tcl_HashTable *t; void *data; }		module;
	 struct {struct obj *data; }				tcl;
#ifdef USE_STKLOS
    	 struct {int type_flags; struct stklos_instance *data;} instance;
#endif
#ifdef USE_TK
	 /* Idea of l_data comes from Alexander Taranov <tay@jet.msk.edu> */
	 struct {struct Tk_command *data; struct obj *l_data;}	tk;
#endif
       } storage_as;

  unsigned char type;
  unsigned char gc_mark;
  short         cell_info;

};

typedef struct obj* SCM;
typedef struct obj* PRIMITIVE;


#define tc_nil    	0
#define tc_cons   	1
#define tc_flonum 	2
#define tc_integer	3
#define tc_bignum	4
#define tc_symbol 	5
#define tc_keyword	6
#define tc_subr_0 	7
#define tc_subr_1 	8
#define tc_subr_2 	9
#define tc_subr_3 	10
#define tc_subr_0_or_1  11
#define tc_subr_1_or_2  12
#define tc_subr_2_or_3	13
#define tc_lsubr  	14
#define tc_ssubr	15
#define tc_fsubr  	16
#define tc_syntax  	17
#define tc_closure 	18
#define tc_free_cell 	19
#define tc_char	        20
#define tc_string       21
#define tc_vector	22
#define tc_eof		23
#define tc_undefined	24
#define tc_iport       	25
#define tc_oport	26
#define tc_isport	27
#define tc_osport	28
#define tc_ivport	29
#define tc_ovport	30
#define tc_boolean	31
#define tc_macro	32
#define tc_localvar	33
#define tc_globalvar	34
#define tc_modulevar	35
#define tc_cont		36
#define tc_env		37
#define tc_address	38
#define tc_autoload	39
#define tc_Cpointer	40
#define tc_module	41
#define tc_frame	42
#define tc_values	43


#ifdef USE_STKLOS
#  define tc_instance	 45
#  define tc_next_method 46
#endif

#ifdef USE_TK
#  define tc_tkcommand	50
#endif
#define tc_tclobject	51	/* always defined, even if NO_TK */


#define tc_quote	61
#define tc_lambda	62
#define tc_if		63
#define tc_setq		64
#define tc_cond		65 
#define tc_and		66
#define tc_or		67
#define tc_let		68
#define tc_letstar	69
#define tc_letrec	70
#define tc_begin	71
#define tc_promise	72
#define tc_apply	73
#define tc_call_cc	74
#define tc_dynwind	75
#define tc_extend_env   76
#define tc_unbound	80
#define tc_start_extd	90	/* Number of first extended type */
#define tc_stop_extd	127	/* Number of last extended type */


#define CAR(x) 		((*x).storage_as.cons.car)
#define CDR(x) 		((*x).storage_as.cons.cdr)
#define PNAME(x) 	((*x).storage_as.symbol.pname)
#define KEYVAL(x)	((*x).storage_as.keyword.data)
#define VCELL(x) 	((*x).storage_as.symbol.vcell)
#define SUBR0(x) 	(*((*x).storage_as.subr0.f))
#define SUBRF(x) 	(*((*x).storage_as.subr.f))
#define FLONM(x) 	((*x).storage_as.flonum.data)
#define CHARS(x)	((*x).storage_as.string.data)
#define STRSIZE(x)	((*x).storage_as.string.dim)
#define VECT(x)		((*x).storage_as.vector.data)
#define VECTSIZE(x)	((*x).storage_as.vector.dim)
#define BIGNUM(x)	((*x).storage_as.bignum.data)
#define EXTDATA(x)	((*x).storage_as.extension.data)
#define EXTID(x)	((*x).storage_as.extension.id)
#define EXTSTATICP(x)	((*x).storage_as.extension.staticp)
#define TCLOBJDATA(x)   ((*x).storage_as.tcl.data)
#define CLOSENV(x)	((*x).storage_as.closure.env)
#define CLOSCODE(x)	((*x).storage_as.closure.code)
#define CLOSARITY(x)	((*x).cell_info)
#define CLOSPARAMS(x)	(CAR(CLOSCODE(x)))
#define CLOSBODY(x)	(CDR(CLOSCODE(x)))
#define CELLINFO(x)	((*x).cell_info)


#ifdef COMPACT_SMALL_CST
#  define MAKE_SMALL_CST(x,type)  (((long) (x) << 8) |((type) << 1) | 1)
#  define SMALL_CST_TYPE(x)	  (((long) (x) >> 1) & 0x7F)
#  define SMALL_CST_VALUE(x)	  ((long)  (x) >> 8)
#  define SMALL_CSTP(x)		  ((long)  (x) & 1)
#  define TYPE(x)		  (SMALL_CSTP(x) ? (int)SMALL_CST_TYPE(x):(x)->type)

#  define INTEGER(x)		  SMALL_CST_VALUE(x)
#  define SET_INTEGER(x, v)	  (x = (SCM) MAKE_SMALL_CST(v, tc_integer))
#  define CHAR(x)		  ((unsigned char) SMALL_CST_VALUE(x))
#  define SET_CHARACTER(x, v)	  (x = (SCM) MAKE_SMALL_CST(v, tc_char))
#else
#  define SMALL_CSTP(x)		  FALSE
#  define TYPE(x)		  ((x)->type)

#  define INTEGER(x)		  ((long) ((x)->storage_as.extension.data))
#  define SET_INTEGER(x, v)	  (INTEGER(x) = (v))
#  define CHAR(x)		  ((unsigned char) ((x)->storage_as.extension.data)
#  define SET_CHARACTER(x, v)	  (CHAR(x) = (v))
#endif

#define EQ(x,y) 	((x) == (y))
#define NEQ(x,y) 	((x) != (y))
#define NULLP(x) 	EQ(x,NIL)
#define NNULLP(x) 	NEQ(x,NIL)

#define TYPEP(x,y) 	(TYPE(x) == (y))
#define NTYPEP(x,y) 	(TYPE(x) != (y))

#define CONSP(x)   	 TYPEP(x,tc_cons)
#define CLOSUREP(x)	 TYPEP(x,tc_closure)
#define FLONUMP(x) 	 TYPEP(x,tc_flonum)
#define SYMBOLP(x) 	 TYPEP(x,tc_symbol)
#define KEYWORDP(x)	 TYPEP(x,tc_keyword)
#define STRINGP(x)	 TYPEP(x,tc_string)
#define EOFP(x)		 TYPEP(x, tc_eof)
#define BOOLEANP(x)	 TYPEP(x, tc_boolean)
#define VECTORP(x)	 TYPEP(x,tc_vector)
#define IPORTP(x)	 TYPEP(x,tc_iport)
#define OPORTP(x)	 TYPEP(x,tc_oport)
#define ISPORTP(x)	 TYPEP(x,tc_isport)
#define OSPORTP(x)	 TYPEP(x,tc_ovport)
#define IVPORTP(x)	 TYPEP(x,tc_ivport)
#define OVPORTP(x)	 TYPEP(x,tc_osport)
#define SPORTP(x)        (ISPORTP(x)||OSPORTP(x))
#define INTEGERP(x)	 TYPEP(x,tc_integer)
#define BIGNUMP(x)	 TYPEP(x,tc_bignum)
#define NUMBERP(x)	 (FLONUMP(x) || INTEGERP(x) || BIGNUMP(x))
#define EXACTP(x)	 (INTEGERP(x) || BIGNUMP(x))
#define INTP(x)		 (INTEGERP(x) || BIGNUMP(x))
#define CHARP(x)	 TYPEP(x,tc_char)
#define PROMISEP(x)	 TYPEP(x,tc_promise)
#define CONTINUATIONP(x) TYPEP(x,tc_cont)
#define ENVP(x)		 TYPEP(x,tc_env)
#define MACROP(x)	 TYPEP(x,tc_macro)
#define EXTENDEDP(x)	 (tc_start_extd <= TYPE(x))
#define CPOINTERP(x)	 TYPEP(x,tc_Cpointer)
#define MODULEP(x) 	 TYPEP(x,tc_module)
#define FRAMEP(x)	 TYPEP(x,tc_frame)
#define VALUESP(x)	 TYPEP(x,tc_values)

#define NCONSP(x)   	  NTYPEP(x,tc_cons)
#define NCLOSUREP(x)	  NTYPEP(x,tc_closure)
#define NFLONUMP(x) 	  NTYPEP(x,tc_flonum)
#define NSYMBOLP(x) 	  NTYPEP(x,tc_symbol)
#define NKEYWORDP(x)	  NTYPEP(x,tc_keyword)
#define NSTRINGP(x)	  NTYPEP(x,tc_string)
#define NEOFP(x)	  NTYPEP(x, tc_eof)
#define NBOOLEANP(x)	  NTYPEP(x, tc_boolean)
#define NVECTORP(x)	  NTYPEP(x,tc_vector)
#define NIPORTP(x)	  NTYPEP(x,tc_iport)
#define NOPORTP(x)	  NTYPEP(x,tc_oport)
#define NISPORTP(x)	  NTYPEP(x,tc_isport)
#define NOSPORTP(x)	  NTYPEP(x,tc_osport)
#define NSPORTP(x)	  (!SPORTP(x))
#define NINTEGERP(x)	  NTYPEP(x,tc_integer)
#define NBIGNUMP(x)	  NTYPEP(x,tc_bignum)
#define NNUMBERP(x)	  (NFLONUMP(x) && NINTEGERP(x) && NBIGNUMP(x))
#define NEXACTP(x)	  (NINTEGERP(x) && NBIGNUMP(x))
#define NINTP(x)	  (NINTEGERP(x) && NBIGNUMP(x))
#define NCHARP(x)	  NTYPEP(x,tc_char)
#define NPROMISEP(x)	  NTYPEP(x,tc_promise)
#define NCONTINUATIONP(x) NTYPEP(x,tc_cont)
#define NENVP(x)	  NTYPEP(x,tc_env)
#define NMACROP(x)	  NTYPEP(x,tc_macro)
#define NEXTENDEDP(x)	  (!EXTENDEDP(x))
#define NCPOINTERP(x)	  NTYPEP(x,tc_Cpointer)
#define NMODULEP(x) 	  NTYPEP(x,tc_module)
#define NFRAMEP(x)	  NTYPEP(x,tc_frame)
#define NVALUESP(x)	  NTYPEP(x,tc_values)

#ifdef USE_TK
#  define TKCOMMP(x)	  TYPEP(x,tc_tkcommand)
#  define TCLOBJP(x)	  TYPEP(x,tc_tclobject)

#  define NTKCOMMP(x)	  NTYPEP(x,tc_tkcommand)
#  define NTCLOBJP(x)	  NTYPEP(x,tc_tclobject)
#endif

#define ModifyCode()	NEQ(VCELL(STk_sym_debug), STk_truth)

#define SYNTAX_RETURN(x, need_eval) \
			  { *pform = (x); return (need_eval); }


#ifdef _DEBUG_MALLOC_INC
#define must_malloc(n)		malloc(n)
#define must_realloc(p, n)	realloc(p, n)
#endif


#define CELL_INFO_C_VAR 	01	/* Symbol is a C variable */
#define CELL_INFO_TRACED_VAR	02	/* Symbol is traced */
#define CELL_INFO_UNINTERNED	04	/* Symbol is not interned */



#define TRACED_VARP(var)	(((var)->cell_info) & CELL_INFO_TRACED_VAR)



/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*
  ------------------------------------------------------------------------------
  ----
  ----  A D D R E S S . C
  ----
  ------------------------------------------------------------------------------
*/
SCM 	  STk_address2object(char *buffer);

PRIMITIVE STk_address_of(SCM obj);
PRIMITIVE STk_addressp(SCM address);

/*
  ------------------------------------------------------------------------------
  ----
  ----  A R G V . C
  ----
  ------------------------------------------------------------------------------
*/

#if defined(WIN32) && defined(USE_DYNLOAD) && defined(MSC_VER)
#  ifdef EXPORT_DLL_GLOBALS
#    define extern __declspec( dllexport )
#  else
#    ifdef IMPORT_DLL_GLOBALS
#      define extern __declspec( dllimport )
#    else
#      define extern extern
#    endif
#  endif
#endif

#ifdef USE_TK
extern char *STk_arg_Xdisplay;
extern char *STk_arg_geometry;
extern char *STk_arg_name;
extern char *STk_arg_visual;
extern int   STk_arg_colormap;
extern int   STk_arg_sync;
extern int   STk_arg_no_tk;
extern int   STk_arg_console;
#endif
extern char *STk_arg_file;
extern char *STk_arg_load;
extern char *STk_arg_cells;
extern char *STk_arg_image;
extern int   STk_arg_interactive;

char** STk_process_argc_argv(int argc, char **argv);
void   STk_save_unix_args_and_environment(int argc, char **argv);
void   STk_restore_unix_args_and_environment(int *argc, char ***argv);
void   STk_initialize_scheme_args(char **argv);


/*
  ------------------------------------------------------------------------------
  ----
  ----  B O O L E A N . C
  ----
  ------------------------------------------------------------------------------
*/
PRIMITIVE STk_not(SCM x);
PRIMITIVE STk_booleanp(SCM x);
PRIMITIVE STk_eqv(SCM x, SCM y);
PRIMITIVE STk_eq(SCM x,SCM y);
PRIMITIVE STk_equal(SCM x, SCM y);


/*
  ------------------------------------------------------------------------------
  ----
  ----  C H A R . C
  ----
  ------------------------------------------------------------------------------
*/
unsigned char  STk_string2char(char *s);
unsigned char *STk_char2string(unsigned char c);
SCM   STk_makechar(unsigned char c);

PRIMITIVE STk_charp(SCM obj);

PRIMITIVE STk_chareq   (SCM c1, SCM c2);
PRIMITIVE STk_charless (SCM c1, SCM c2);
PRIMITIVE STk_chargt   (SCM c1, SCM c2);
PRIMITIVE STk_charlesse(SCM c1, SCM c2);
PRIMITIVE STk_chargte  (SCM c1, SCM c2);

PRIMITIVE STk_chareqi   (SCM c1, SCM c2);
PRIMITIVE STk_charlessi (SCM c1, SCM c2);
PRIMITIVE STk_chargti   (SCM c1, SCM c2);
PRIMITIVE STk_charlessei(SCM c1, SCM c2);
PRIMITIVE STk_chargtei  (SCM c1, SCM c2);

PRIMITIVE STk_char_alphap(SCM c);
PRIMITIVE STk_char_numericp(SCM c);
PRIMITIVE STk_char_whitep(SCM c);
PRIMITIVE STk_char_upperp(SCM c);
PRIMITIVE STk_char_lowerp(SCM c);
PRIMITIVE STk_char2integer(SCM c);
PRIMITIVE STk_integer2char(SCM i);
PRIMITIVE STk_char_upper(SCM c);
PRIMITIVE STk_char_lower(SCM c);

/*
  ------------------------------------------------------------------------------
  ----
  ----  C O N S O L E . C
  ----
  ------------------------------------------------------------------------------
*/
void STk_console_prompt(SCM env);
void STk_init_console(void);



/*
  ------------------------------------------------------------------------------
  ----
  ----  C O N T . C
  ----
  ------------------------------------------------------------------------------
*/
void 	  STk_mark_continuation(SCM cont);
void 	  STk_throw(SCM fct, SCM val);
SCM 	  STk_do_call_cc(SCM *x);
PRIMITIVE STk_continuationp(SCM obj);

void 	  STk_unwind_all(void);
PRIMITIVE STk_dynamic_wind(SCM thunk1, SCM thunk2, SCM thunk3);

/*
  ------------------------------------------------------------------------------
  ----
  ----  D U M P . C
  ----
  ------------------------------------------------------------------------------
*/
extern int STk_dumped_core;
void STk_restore_image(char *s);
PRIMITIVE STk_dump(SCM s);


/*
  ------------------------------------------------------------------------------
  ----
  ----  D Y N L O A D . C
  ----
  ------------------------------------------------------------------------------
*/
void STk_load_object_file(char *path);
PRIMITIVE STk_call_external(SCM l, int len);
PRIMITIVE STk_external_existsp(SCM entry_name, SCM library);
PRIMITIVE STk_cstring2string(SCM pointer);
PRIMITIVE STk_string2usymbol(SCM string);

/*
  ------------------------------------------------------------------------------
  ----
  ----  E N V . C
  ----
  ------------------------------------------------------------------------------
*/

SCM STk_makeframe(SCM formals, SCM actuals);
SCM STk_makeenv(SCM l, int create_if_null);
SCM *STk_value_in_env(SCM var, SCM env);
SCM *STk_varlookup(SCM x, SCM env, int err_if_unbound);
SCM STk_localvalue(SCM var, SCM env);

PRIMITIVE STk_symbol_boundp(SCM x, SCM env);
PRIMITIVE STk_the_environment(SCM args, SCM env, int len);
PRIMITIVE STk_parent_environment(SCM env);
PRIMITIVE STk_global_environment(void);
PRIMITIVE STk_environment2list(SCM env);
PRIMITIVE STk_environmentp(SCM obj);
PRIMITIVE STk_get_environment(SCM env);

/*
  ------------------------------------------------------------------------------
  ----
  ----  E R R O R . C
  ----
  ------------------------------------------------------------------------------
*/

#define EVAL_ERROR		((SCM) 1)

#define ERR_FATAL		0x01
#define ERR_OK			0x02
#define ERR_READ_FROM_STRING	0x04
#define ERR_IGNORED		0x08
#define ERR_TCL_BACKGROUND	0x10
#define ERR_IN_REPORT_ERROR	0x20
#define ERR_IN_LOAD		0x40


#define JMP_INIT		0
#define JMP_ERROR		1
#define JMP_THROW		2
#define JMP_RESTORE		3
#define JMP_INTERRUPT		4


struct error_handler {
  jmp_buf j;
  struct error_handler *prev;
  int context;
  SCM dynamic_handler;
};


#define PUSH_ERROR_HANDLER   					\
{								\
  struct error_handler _local_handler;				\
  int _k;							\
								\
  _local_handler.prev    	 = STk_err_handler;		\
  _local_handler.context 	 = STk_err_handler->context;	\
  _local_handler.dynamic_handler = NIL;				\
  STk_err_handler = &_local_handler;				\
  _k = setjmp(_local_handler.j);				\
  if (_k == 0) 							\
  /* no closing brace. It will be in POP_ERROR_HANDLER */		


#define WHEN_ERROR  		else
#define PROPAGATE_ERROR() {					\
  STk_err_handler = _local_handler.prev;			\
  longjmp(STk_err_handler->j, _k);				\
}
		       
#define POP_ERROR_HANDLER					\
  STk_err_handler = _local_handler.prev;			\
  /* close the brace opened by PUSH_ERROR_HANDLER */		\
}


extern struct error_handler *STk_err_handler;

void STk_err(char *message, SCM x);
void STk_procedure_error(char *procedure, char *message, SCM x);

#define Err 			STk_err
#define Serror(msg, who)	STk_procedure_error(proc_name, msg, who)


/*
  ------------------------------------------------------------------------------
  ----
  ----  E V A L . C
  ----
  ------------------------------------------------------------------------------
*/

/* The eval flag which tells eval that it has something to do before
 * evaluating the form
 */
extern int STk_eval_flag;

/* Eval stack. These are internals of the evaluator. Don't care*/
void STk_show_eval_stack(int depth, int uncode);
void STk_reset_eval_stack(void);


PRIMITIVE STk_user_get_eval_stack(void);

/* Eval hook management */
void STk_init_eval_hook(void);
void STk_reset_eval_hook(void);
PRIMITIVE STk_eval_hook(SCM x, SCM env, SCM hook);

/* Environment stack. These are internals of the evaluator. Don't care*/
PRIMITIVE STk_get_env_stack(void);

SCM STk_eval(SCM x,SCM env);
SCM STk_apply(SCM fct, SCM param);
SCM STk_apply0(SCM fct);
SCM STk_apply1(SCM fct, SCM param);
SCM STk_apply2(SCM fct, SCM param1, SCM param2);

PRIMITIVE STk_user_eval  (SCM expr, SCM env);
PRIMITIVE STk_eval_string(SCM str, SCM env);


#define EVAL(x)	  	  (STk_eval((x), env))
#define EVALCAR(x)	  (SYMBOLP(CAR(x)) ? *STk_varlookup((x),env,1):EVAL(CAR(x)))

#define Apply		  STk_apply
#define Apply0		  STk_apply0
#define Apply1		  STk_apply1
#define Apply2		  STk_apply2

#define add_frame(formals, actuals, env) Cons(STk_makeframe((formals), (actuals)),\
					      (env))

/*
  ------------------------------------------------------------------------------
  ----
  ----  E X T E N D  . C
  ----
  ------------------------------------------------------------------------------
*/
#define EXT_ISPROC 	01	/* procedure? should answer #t */
#define EXT_EVALPARAM 	02	/* evaluates parameter list when apply */

/* Easy access to variables from C */
#define STk_define_variable(var, value, module) \
  	STk_define_public_var((module), Intern(var), (value))
#define STk_lookup_variable(var, module) \
	*(STk_varlookup(Intern(var), (module), 0))
#define STk_define_scheme_variable(var, value) \
  	STk_define_variable((var), (value), STk_scheme_module)
#define STk_lookup_scheme_variable(var) \
	STk_lookup_variable((var), STk_scheme_module)

/* Old interface for global variables. Don't use it anuymore */
#define STk_set_symbol_value(var,value)  STk_define_variable((var), (value), NIL)
#define STk_get_symbol_value(var)	 STk_lookup_variable((var), NIL)

typedef struct {
  char *type_name;		/* The external name of this type */
  int  flags;			
  void (*gc_mark_fct)(SCM x);
  void (*gc_sweep_fct)(SCM x);
  SCM  (*apply_fct)(SCM x, SCM args, SCM env);
  void (*display_fct)(SCM x, SCM port, int mode);
  SCM  (*compare_fct)(SCM x, SCM y, int equalp);
  void *Reserved[7];		/* should be sufficient for a while */
} STk_extended_scheme_type;


int  STk_add_new_type(STk_extended_scheme_type *p);
void STk_add_new_primitive(char *fct_name, int fct_type, PRIMITIVE (*fct_ptr)());
SCM  STk_eval_C_string(char *s, SCM env);

#define ANONYMOUS_STAT_PTR_ID 0
#define ANONYMOUS_DYN_PTR_ID  1	

int STk_new_Cpointer_id(void (*display_func)(SCM x, SCM port, int mode));
SCM STk_make_Cpointer(int Cpointer_id, void *ptr, int staticp);

void STk_define_C_variable(char *var, SCM (*getter)(), void (*setter)());

/*
  ------------------------------------------------------------------------------
  ----
  ----  G C . C
  ----
  ------------------------------------------------------------------------------
*/
extern SCM  STk_freelist;
extern long STk_alloc_cells;
extern int  STk_gc_requested;

#define NEWCELL(_into,_type)    			\
 { 							\
   if (STk_gc_requested || NULLP(STk_freelist))		\
      STk_gc_for_newcell();				\
   _into 	      = STk_freelist;			\
   STk_freelist       = CDR(STk_freelist);		\
   STk_alloc_cells   += 1;				\
   _into->type        = _type;				\
 }

void STk_gc_for_newcell(void);
void STk_gc_protect(SCM *location);   /* protect against GC this cell */
void STk_gc_unprotect(SCM *location); /* un-protect against GC this cell */
void STk_gc_mark(SCM location);       /* mark (recursively) this location */

PRIMITIVE STk_gc_stats(void);				/* + */
PRIMITIVE STk_gc(void);					/* + */
PRIMITIVE STk_find_cells(SCM type);			/* + */
PRIMITIVE STk_expand_heap(SCM arg);			/* + */

/*
  ------------------------------------------------------------------------------
  ----
  ----  I O . C
  ----
  ------------------------------------------------------------------------------
*/

#define MAX_FPRINTF 1000       /* Max # of char produced by 1 call to STk_fprintf */

void   STk_StdinProc(void);
void   STk_fill_stdin_buffer(char *s);
int    STk_getc(SCM port);
int    STk_ungetc(int c, SCM port);
int    STk_putc(int c, SCM port);
int    STk_puts(char *s, SCM port);
int    STk_eof(SCM port);
int    STk_internal_flush(SCM port);
int    STk_internal_char_readyp(SCM port);
void   STk_close(SCM port);
void   STk_fprintf(SCM port, char *format, ...);
char * STk_line_bufferize_io(SCM port);
void   STk_init_io(void);

#define Getc(f)         (STk_getc(f))
#define Ungetc(c, f)    (STk_ungetc((c), (f)))
#define Putc(c, f)      (STk_putc((c),   (f)))
#define Puts(s, f)      (STk_puts((s),   (f)))
#define Eof(f)		(STk_eof(f))
#define Flush(f)        (STk_internal_flush(f))
#define Fprintf		STk_fprintf

/*
  ------------------------------------------------------------------------------
  ----
  ----  K E Y W O R D . C
  ----
  ------------------------------------------------------------------------------
*/
void STk_initialize_keyword_table(void);
void STk_free_keyword(SCM keyword);

SCM STk_makekey(char *token);
PRIMITIVE STk_make_keyword(SCM str);
PRIMITIVE STk_keywordp(SCM obj);
PRIMITIVE STk_keyword2string(SCM obj);
PRIMITIVE STk_get_keyword(SCM key, SCM l, SCM default_value);

/*
  ------------------------------------------------------------------------------
  ----
  ---- L I S T . C
  ----
  ------------------------------------------------------------------------------
*/
int STk_llength(SCM l);		  /* length of a list. -1 if not a proper list */
SCM STk_append2(SCM l1, SCM l2);  /* append with only two lists */
SCM STk_dappend2(SCM l1, SCM l2); /* the same one but destructive this time */

PRIMITIVE STk_pairp(SCM x);
PRIMITIVE STk_cons(SCM x, SCM y);
PRIMITIVE STk_car(SCM x);
PRIMITIVE STk_cdr(SCM x);
PRIMITIVE STk_setcar(SCM cell, SCM value);
PRIMITIVE STk_setcdr(SCM cell, SCM value);
PRIMITIVE STk_caar  (SCM l);
PRIMITIVE STk_cdar  (SCM l);
PRIMITIVE STk_cadr  (SCM l);
PRIMITIVE STk_cddr  (SCM l);
PRIMITIVE STk_caaar (SCM l);
PRIMITIVE STk_cdaar (SCM l);
PRIMITIVE STk_cadar (SCM l);
PRIMITIVE STk_cddar (SCM l);
PRIMITIVE STk_caadr (SCM l);
PRIMITIVE STk_cdadr (SCM l);
PRIMITIVE STk_caddr (SCM l);
PRIMITIVE STk_cdddr (SCM l);
PRIMITIVE STk_caaaar(SCM l);
PRIMITIVE STk_cdaaar(SCM l);
PRIMITIVE STk_cadaar(SCM l);
PRIMITIVE STk_cddaar(SCM l);
PRIMITIVE STk_caadar(SCM l);
PRIMITIVE STk_cdadar(SCM l);
PRIMITIVE STk_caddar(SCM l);
PRIMITIVE STk_cdddar(SCM l);
PRIMITIVE STk_caaadr(SCM l);
PRIMITIVE STk_cdaadr(SCM l);
PRIMITIVE STk_cadadr(SCM l);
PRIMITIVE STk_cddadr(SCM l);
PRIMITIVE STk_caaddr(SCM l);
PRIMITIVE STk_cdaddr(SCM l);
PRIMITIVE STk_cadddr(SCM l);
PRIMITIVE STk_cddddr(SCM l);
PRIMITIVE STk_nullp (SCM x);
PRIMITIVE STk_listp (SCM x);
PRIMITIVE STk_list  (SCM l, int len);
PRIMITIVE STk_list_length(SCM l);
PRIMITIVE STk_append(SCM l, int len);
PRIMITIVE STk_reverse(SCM l);
PRIMITIVE STk_list_tail(SCM list, SCM k);
PRIMITIVE STk_list_ref(SCM list, SCM k);
PRIMITIVE STk_memq  (SCM obj, SCM list);
PRIMITIVE STk_memv  (SCM obj, SCM list);
PRIMITIVE STk_member(SCM obj, SCM list);
PRIMITIVE STk_assq  (SCM obj, SCM alist);
PRIMITIVE STk_assv  (SCM obj, SCM alist);
PRIMITIVE STk_assoc (SCM obj, SCM alist);

PRIMITIVE STk_liststar(SCM l, int len);		/* + */
PRIMITIVE STk_copy_tree(SCM l);			/* + */
PRIMITIVE STk_last_pair(SCM l);			/* + */
PRIMITIVE STk_remq   (SCM obj, SCM list);	/* + */
PRIMITIVE STk_remv   (SCM obj, SCM list);	/* + */
PRIMITIVE STk_remove (SCM obj, SCM list);	/* + */
PRIMITIVE STk_dappend(SCM l, int len);		/* + */


#define Cons			 STk_cons
#define Reverse			 STk_reverse
#define LIST1(a)		 Cons((a), NIL)
#define LIST2(a,b) 		 Cons((a), LIST1(b))
#define LIST3(a,b,c)		 Cons((a), LIST2((b),(c)))
#define LIST4(a,b,c,d)		 Cons((a), LIST3((b),(c),(d)))
#define LIST5(a,b,c,d,e)	 Cons((a), LIST4((b),(c),(d),(e)))
#define LIST6(a,b,c,d,e,f)	 Cons((a), LIST5((b),(c),(d),(e),(f)))
#define LIST7(a,b,c,d,e,f,g)	 Cons((a), LIST6((b),(c),(d),(e),(f),(g)))
#define LIST8(a,b,c,d,e,f,g,h)	 Cons((a), LIST7((b),(c),(d),(e),(f),(g),(h)))

/*
  ------------------------------------------------------------------------------
  ----
  ---- M A C R O . C
  ----
  ------------------------------------------------------------------------------
*/
PRIMITIVE STk_macro(SCM args, SCM env, int len);		/* + */
PRIMITIVE STk_macro_expand(SCM form, SCM env, int len);		/* + */
PRIMITIVE STk_macro_expand_1(SCM form, SCM env, int len);	/* + */
PRIMITIVE STk_macro_body(SCM form);				/* + */
PRIMITIVE STk_macrop(SCM obj);					/* + */

PRIMITIVE STk_macro_R5(SCM args, SCM env, int len);		/* Undoc */


/*
  ------------------------------------------------------------------------------
  ----
  ---- M O D U L E  . C
  ----
  ------------------------------------------------------------------------------
*/

extern SCM STk_global_module;	/* Global module -- i.e STk pseudo-module */
extern SCM STk_scheme_module;	/* The Scheme module */
extern SCM STk_selected_module; /* Selected module */

SCM  STk_make_module(SCM name);
SCM  STk_make_parent_module(SCM module);
void STk_mark_module(SCM m);
void STk_free_module(SCM m);
void STk_mark_module_table(void);
void STk_define_public_var(SCM module, SCM var, SCM value);
SCM* STk_module_lookup(SCM module, SCM var, SCM context);
SCM  STk_modulevalue(SCM obj);
SCM  STk_module_env2list(SCM module);
void STk_select_stk_module(void);
void STk_initialize_stk_module(void);
void STk_init_modules(void);

PRIMITIVE STk_define_module(SCM l, SCM env, int len);
PRIMITIVE STk_modulep(SCM obj);
PRIMITIVE STk_with_module(SCM l, SCM env, int len);
PRIMITIVE STk_import(SCM l, SCM env, int len);
PRIMITIVE STk_export_symbol(SCM symbol, SCM module);
PRIMITIVE STk_export_all_symbols(void);

PRIMITIVE STk_select_module(SCM l, SCM env, int len);
PRIMITIVE STk_get_module(SCM env);
PRIMITIVE STk_current_module(SCM l, SCM env, int len);

PRIMITIVE STk_find_module(SCM name, SCM default_value);
PRIMITIVE STk_module_name(SCM module);
PRIMITIVE STk_module_imports(SCM module);
PRIMITIVE STk_module_exports(SCM module);
PRIMITIVE STk_module_env(SCM module);
PRIMITIVE STk_module_symbols(SCM module);
PRIMITIVE STk_get_selected_module(void);
PRIMITIVE STk_all_modules(void);


/*
  ------------------------------------------------------------------------------
  ----
  ---- N U M B E R  . C
  ----
  ------------------------------------------------------------------------------
*/
#ifdef COMPACT_SMALL_CST
#  define SMALLINT_MAX	(LONG_MAX>>8)
#else
#  define SMALLINT_MAX  LONG_MAX
#endif
#define SMALLINT_MIN	(-SMALLINT_MAX)

char   *STk_number2Cstr(SCM n, long base, char buffer[]);
SCM	STk_Cstr2number(char *str, long base);
SCM	STk_makenumber(double x);
SCM	STk_makeinteger(long x);
SCM	STk_makeunsigned(unsigned long x);
long	STk_integer_value(SCM x); /* Returns LONG_MIN if not representable as int */
long	STk_integer_value_no_overflow(SCM x); /* Returns LONG_MIN if not an int */
int	STk_equal_numbers(SCM number1, SCM number2); /* number1 = number2 */

long 	STk_integer2long(SCM x);
unsigned long STk_integer2ulong(SCM x);

#define STk_real2double(x)	FLONM(x)


PRIMITIVE STk_numberp(SCM x);
PRIMITIVE STk_integerp(SCM x);
    
PRIMITIVE STk_exactp(SCM x);
PRIMITIVE STk_inexactp(SCM x);

PRIMITIVE STk_numequal(SCM l, SCM env, int from_eval);
PRIMITIVE STk_lessp(SCM l, SCM env, int from_eval);
PRIMITIVE STk_greaterp(SCM l, SCM env, int from_eval);
PRIMITIVE STk_lessep(SCM l, SCM env, int from_eval);
PRIMITIVE STk_greaterep(SCM l, SCM env, int from_eval);

PRIMITIVE STk_zerop(SCM n);
PRIMITIVE STk_positivep(SCM n);
PRIMITIVE STk_negativep(SCM n);
PRIMITIVE STk_oddp(SCM n);
PRIMITIVE STk_evenp(SCM n);

PRIMITIVE STk_max(SCM l, SCM env, int from_eval);
PRIMITIVE STk_min(SCM l, SCM env, int from_eval);

PRIMITIVE STk_plus(SCM l, SCM env, int from_eval);
PRIMITIVE STk_difference(SCM l, SCM env, int from_eval);
PRIMITIVE STk_times(SCM l, SCM env, int from_eval);
PRIMITIVE STk_division(SCM l, SCM env, int from_eval);

PRIMITIVE STk_absolute(SCM x);
PRIMITIVE STk_quotient(SCM n1, SCM n2);
PRIMITIVE STk_remainder(SCM n1, SCM n2);
PRIMITIVE STk_modulo(SCM n1, SCM n2);

PRIMITIVE STk_gcd(SCM l, SCM env, int from_eval);
PRIMITIVE STk_lcm(SCM l, SCM env, int from_eval);

PRIMITIVE STk_floor(SCM x);
PRIMITIVE STk_ceiling(SCM x);
PRIMITIVE STk_truncate(SCM x);
PRIMITIVE STk_round(SCM x);

PRIMITIVE STk_exp(SCM z);
PRIMITIVE STk_log(SCM z);
PRIMITIVE STk_sin(SCM z);
PRIMITIVE STk_cos(SCM z);
PRIMITIVE STk_tan(SCM z);
PRIMITIVE STk_asin(SCM z);
PRIMITIVE STk_acos(SCM z);
PRIMITIVE STk_atan(SCM y, SCM x);
PRIMITIVE STk_sqrt(SCM z);
PRIMITIVE STk_expt(SCM z1, SCM z2);

PRIMITIVE STk_exact2inexact(SCM z);
PRIMITIVE STk_inexact2exact(SCM z);
PRIMITIVE STk_string2number(SCM str, SCM base);
PRIMITIVE STk_number2string(SCM n, SCM base);

PRIMITIVE  STk_bignump(SCM n);

/*
  ------------------------------------------------------------------------------
  ----
  ---- P O R T . C
  ----
  ------------------------------------------------------------------------------
*/

struct port_descr {		/* Slot order is important (see sport_descr) */
  FILE *file;
  int  flags;
  char *filename;
  SCM  read_event;
  SCM  write_event;
};

#define PORT_FILE(x)	((x)->storage_as.port.p->file)
#define PORT_UNGETC(x)  ((x)->storage_as.port.ungetted_char)
#define PORT_NAME(x)	((x)->storage_as.port.p->filename)
#define PORT_FLAGS(x)	((x)->storage_as.port.p->flags)
#define PORT_REVENT(x)	((x)->storage_as.port.p->read_event)
#define PORT_WEVENT(x)	((x)->storage_as.port.p->write_event)

#define PORT_CLOSED 	01
#define PIPE_PORT	02


#define OUTP(p) 	(OPORTP(p) || OSPORTP(p) || OVPORTP(p))
#define INP(p)  	(IPORTP(p) || ISPORTP(p) || IVPORTP(p))
#define F_READ  	01
#define F_WRITE 	02

/* external vars */
extern SCM STk_curr_iport, STk_curr_oport, STk_curr_eport, STk_eof_object;

void 	  STk_close_file_port(SCM port);
void 	  STk_freeport(SCM port);
void 	  STk_init_standard_ports(void);
SCM 	  STk_load_file(char *fname, int err_if_absent, SCM module);
SCM 	  STk_Cfile2port(char *name, FILE *f, int type, int flags);
SCM 	  STk_redirect_input(SCM port, SCM thunk);
SCM 	  STk_redirect_output(SCM port, SCM thunk);
SCM 	  STk_redirect_error(SCM port, SCM thunk);

PRIMITIVE STk_input_portp(SCM port);
PRIMITIVE STk_output_portp(SCM port);
PRIMITIVE STk_input_file_portp(SCM port);
PRIMITIVE STk_output_file_portp(SCM port);
PRIMITIVE STk_current_input_port(void);
PRIMITIVE STk_current_output_port(void);
PRIMITIVE STk_current_error_port(void);
PRIMITIVE STk_with_input_from_file(SCM string, SCM thunk);
PRIMITIVE STk_with_output_to_file(SCM string, SCM thunk);
PRIMITIVE STk_open_input_file(SCM filename);
PRIMITIVE STk_open_output_file(SCM filename);
PRIMITIVE STk_close_input_port(SCM port);
PRIMITIVE STk_close_output_port(SCM port);
PRIMITIVE STk_read(SCM port);
PRIMITIVE STk_read_char(SCM port);
PRIMITIVE STk_peek_char(SCM port);
PRIMITIVE STk_eof_objectp(SCM obj);
PRIMITIVE STk_char_readyp(SCM port);
PRIMITIVE STk_write(SCM expr, SCM port);
PRIMITIVE STk_display(SCM expr, SCM port);
PRIMITIVE STk_newline(SCM port);
PRIMITIVE STk_write_char(SCM c, SCM port);
PRIMITIVE STk_load(SCM filename, SCM module);


/* Non standard functions */
PRIMITIVE STk_with_error_to_file(SCM string, SCM thunk);
PRIMITIVE STk_with_input_from_port(SCM port, SCM thunk);
PRIMITIVE STk_with_output_to_port(SCM port, SCM thunk);
PRIMITIVE STk_with_error_to_port(SCM port, SCM thunk);
PRIMITIVE STk_format(SCM l, int len);
PRIMITIVE STk_error(SCM l, int len);
PRIMITIVE STk_try_load(SCM filename, SCM module);
PRIMITIVE STk_open_file(SCM filename, SCM mode);
PRIMITIVE STk_close_port(SCM port);
PRIMITIVE STk_port_closedp(SCM expr);
PRIMITIVE STk_read_line(SCM port);
PRIMITIVE STk_copy_port(SCM in, SCM out);
PRIMITIVE STk_flush(SCM porSTk_t);
PRIMITIVE STk_write_star(SCM expr, SCM port);


void      STk_do_autoload(SCM var, SCM autoload);
PRIMITIVE STk_autoload(SCM l, SCM env, int len);
PRIMITIVE STk_autoloadp(SCM symbol, SCM module);

PRIMITIVE STk_when_port_readable(SCM port, SCM closure);
PRIMITIVE STk_when_port_writable(SCM port, SCM closure);

PRIMITIVE STk_change_standard_ports(SCM in, SCM out, SCM err);	/* Undoc */
	
/*
  ------------------------------------------------------------------------------
  ----
  ---- P R I M I T I V E S . C
  ----
  ------------------------------------------------------------------------------
*/
void STk_init_primitives(void);

/*
  ------------------------------------------------------------------------------
  ----
  ---- P R I N T . C
  ----
  ------------------------------------------------------------------------------
*/
#define DSP_MODE	0x1
#define WRT_MODE	0x2
#define TK_MODE		0x4	/* Always defined even if no Tk support */
#define PANIC_MODE	0x8	/* Printing in "panic" mode (i.e. don't cons) */

SCM STk_print(SCM exp, SCM port, int mode);
SCM STk_print_star(SCM exp, SCM port);

/*
  ------------------------------------------------------------------------------
  ----
  ---- P R O C . C
  ----
  ------------------------------------------------------------------------------
*/

int       STk_is_thunk(SCM obj);
SCM 	  STk_makeclosure(SCM code, SCM env);

PRIMITIVE STk_procedurep(SCM obj);
PRIMITIVE STk_map(SCM l, int len);
PRIMITIVE STk_for_each(SCM l, int len);
PRIMITIVE STk_procedure_body(SCM proc);
PRIMITIVE STk_procedure_environment(SCM proc);
PRIMITIVE STk_procedure_arity(SCM proc);


/*
  ------------------------------------------------------------------------------
  ----
  ---- P R O M I S E . C
  ----
  ------------------------------------------------------------------------------
*/
PRIMITIVE STk_force(SCM promise);
PRIMITIVE STk_promisep(SCM promise);


/*
  ------------------------------------------------------------------------------
  ----
  ---- R E A D . C
  ----
  ------------------------------------------------------------------------------
*/
SCM STk_readf(SCM port, int case_significant);


/*
  ------------------------------------------------------------------------------
  ----
  ---- S I G N A L . C
  ----
  ------------------------------------------------------------------------------
*/
#define MAX_SYSTEM_SIG 		32		    /* True for all systems? */
#define SIGHADGC		MAX_SYSTEM_SIG	    /* End of a GC run */
#define MAX_SIGNAL		(MAX_SYSTEM_SIG+1)  

extern int STk_control_C;

void      STk_handle_signal(int sig);
void 	  STk_handle_sigint_signal(void);
PRIMITIVE STk_add_signal_handler(SCM sig, SCM proc);
PRIMITIVE STk_set_signal_handler(SCM sig, SCM proc);
PRIMITIVE STk_get_signal_handlers(SCM sig);
PRIMITIVE STk_send_signal(SCM sig);

void 	  STk_init_signal(void);
void 	  STk_mark_signal_table(void);

void STk_ignore_signals(void);	/* Block all signals */
void STk_allow_signals(void);  /* Restore signals as  before block_signals */
void STk_signal_GC(void);


/*
  ------------------------------------------------------------------------------
  ----
  ---- S L I B . C
  ----
  ------------------------------------------------------------------------------
*/
#ifndef _DEBUG_MALLOC_INC
void *STk_must_malloc(size_t size);
void *STk_must_realloc(void *ptr, size_t size);
#endif

double   STk_my_time(void);

SCM 	 STk_internal_eval_string(char *s, long err_code, SCM env);

PRIMITIVE STk_catch(SCM expr, SCM env, int len);
PRIMITIVE STk_quit_interpreter(SCM retcode);
PRIMITIVE STk_version(void);
PRIMITIVE STk_machine_type(void);
PRIMITIVE STk_library_location(void);
PRIMITIVE STk_random(SCM n);
PRIMITIVE STk_set_random_seed(SCM n);
PRIMITIVE STk_get_internal_info(void);
PRIMITIVE STk_time(SCM expr, SCM env, int len);
PRIMITIVE STk_global_set(SCM var, SCM value);
PRIMITIVE STk_uncode(SCM expr);
#ifdef SIGSEGV
PRIMITIVE STk_default_SIGSEGV_handler(void);
#endif
void STk_delete_Tcl_child_Interp(void);


#define must_malloc  STk_must_malloc
#define must_realloc STk_must_realloc


/*
  ------------------------------------------------------------------------------
  ----
  ---- S P O R T . C
  ----
  ------------------------------------------------------------------------------
*/

struct sport_descr {		/* Slot order is important (see port_descr) */
  FILE *file;
  int  flags;
};


SCM 	  STk_internal_open_input_string(char *s);
void 	  STk_free_string_port(SCM port);
SCM 	  STk_internal_read_from_string(SCM port, int *eof, int case_significant);
PRIMITIVE STk_open_input_string(SCM s);
PRIMITIVE STk_open_output_string(void);
PRIMITIVE STk_get_output_string(SCM port);
PRIMITIVE STk_input_string_portp(SCM port);
PRIMITIVE STk_output_string_portp(SCM port);
PRIMITIVE STk_with_input_from_string(SCM string, SCM thunk);
PRIMITIVE STk_with_output_to_string(SCM thunk);
PRIMITIVE STk_with_error_to_string(SCM thunk);
PRIMITIVE STk_read_from_string(SCM str);

/*
  ------------------------------------------------------------------------------
  ----
  ---- S T K L O S . C
  ----
  ------------------------------------------------------------------------------
*/
#ifdef USE_STKLOS
   PRIMITIVE STk_init_STklos(void);
#endif


/*
  ------------------------------------------------------------------------------
  ----
  ---- S T R I N G . C
  ----
  ------------------------------------------------------------------------------
*/
SCM       STk_makestrg(int len, char *init);
SCM 	  STk_embed_C_string(char *str);

PRIMITIVE STk_stringp(SCM obj);
PRIMITIVE STk_make_string(SCM len, SCM init_char);
PRIMITIVE STk_lstring(SCM l, int len);
PRIMITIVE STk_string_length(SCM str);

PRIMITIVE STk_string_ref(SCM str, SCM index);
PRIMITIVE STk_string_set(SCM str, SCM index, SCM value);

PRIMITIVE STk_streq   (SCM s1, SCM s2);
PRIMITIVE STk_strless (SCM s1, SCM s2);
PRIMITIVE STk_strgt   (SCM s1, SCM s2);
PRIMITIVE STk_strlesse(SCM s1, SCM s2);
PRIMITIVE STk_strgte  (SCM s1, SCM s2);

PRIMITIVE STk_streqi   (SCM s1, SCM s2);
PRIMITIVE STk_strlessi (SCM s1, SCM s2);
PRIMITIVE STk_strgti   (SCM s1, SCM s2);
PRIMITIVE STk_strlessei(SCM s1, SCM s2);
PRIMITIVE STk_strgtei  (SCM s1, SCM s2);

PRIMITIVE STk_substring(SCM string, SCM start, SCM end);
PRIMITIVE STk_string_append(SCM l, int len);
PRIMITIVE STk_string2list(SCM str);
PRIMITIVE STk_list2string(SCM l);
PRIMITIVE STk_string_copy(SCM str);
PRIMITIVE STk_string_fill(SCM str, SCM c);

PRIMITIVE STk_string_findp(SCM s1, SCM s2);			/* + */
PRIMITIVE STk_string_index(SCM s1, SCM s2);			/* + */
PRIMITIVE STk_string_lower(SCM s);				/* + */
PRIMITIVE STk_string_upper(SCM s);				/* + */
PRIMITIVE STk_split_string(SCM string, SCM delimiters);		/* + */

#define STk_makestring(s) STk_makestrg((int) strlen(s), (s))


/*
  ------------------------------------------------------------------------------
  ----
  ---- S Y M B O L . C
  ----
  ------------------------------------------------------------------------------
*/
void 	  STk_initialize_symbol_table(void);
void 	  STk_mark_symbol_table(void);
void 	  STk_free_symbol(SCM symbol);
SCM  	  STk_global_env2list(void);
SCM 	  STk_global_symbols(void);
SCM 	  STk_intern(char *name);

PRIMITIVE STk_symbolp(SCM x);
PRIMITIVE STk_symbol2string(SCM symbol);
PRIMITIVE STk_string2symbol(SCM string);

#define Intern	STk_intern

/*
  ------------------------------------------------------------------------------
  ----
  ---- S Y N T A X . C
  ----
  ------------------------------------------------------------------------------
*/
PRIMITIVE STk_syntax_quote     (SCM *pform, SCM env, int len);
PRIMITIVE STk_syntax_lambda    (SCM *pform, SCM env, int len);
PRIMITIVE STk_syntax_if        (SCM *pform, SCM env, int len);
PRIMITIVE STk_syntax_setq      (SCM *pform, SCM env, int len);
PRIMITIVE STk_syntax_cond      (SCM *pform, SCM env, int len);
PRIMITIVE STk_syntax_and       (SCM *pform, SCM env, int len);
PRIMITIVE STk_syntax_or        (SCM *pform, SCM env, int len);
PRIMITIVE STk_syntax_let       (SCM *pform, SCM env, int len);
PRIMITIVE STk_syntax_letstar   (SCM *pform, SCM env, int len);
PRIMITIVE STk_syntax_letrec    (SCM *pform, SCM env, int len);

PRIMITIVE STk_syntax_begin     (SCM *pform, SCM env, int len);
PRIMITIVE STk_syntax_delay     (SCM *pform, SCM env, int len);
PRIMITIVE STk_syntax_quasiquote(SCM *pform, SCM env, int len);

PRIMITIVE STk_syntax_define    (SCM *pform, SCM env, int len);

PRIMITIVE STk_while(SCM l, SCM env, int len);
PRIMITIVE STk_until(SCM l, SCM env, int len);
PRIMITIVE STk_syntax_extend_env(SCM *pform, SCM env, int len);

/*
  ------------------------------------------------------------------------------
  ----
  ---- T C L - L I B . C
  ----
  ------------------------------------------------------------------------------
*/
int STk_internal_Tcl_DeleteCommand(Tcl_Interp *interp, char *cmdName); 

/*
  ------------------------------------------------------------------------------
  ----
  ---- T O P L E V E L . C
  ----
  ------------------------------------------------------------------------------
*/
void STk_toplevel(int argc, char **argv);

/*
  ------------------------------------------------------------------------------
  ----
  ---- T R A C E . C
  ----
  ------------------------------------------------------------------------------
*/
void 	  STk_change_value	 (SCM var, SCM env);
void 	  STk_mark_tracevar_table(void);
PRIMITIVE STk_trace_var		 (SCM var, SCM code);
PRIMITIVE STk_untrace_var	 (SCM var);
void 	  STk_init_tracevar      (void);
/*
  ------------------------------------------------------------------------------
  ----
  ---- U N I X . C
  ----
  ------------------------------------------------------------------------------
*/

#ifndef WIN32
#  define ISDIRSEP(ch) 	 ((ch)=='/')
#  define ISABSOLUTE(cp) (ISDIRSEP(*cp))
#  define DIRSEP 	 '/'
#  define SDIRSEP  	 "/"
#  define PATHSEP	 ':'
#else
#  define ISDIRSEP(ch) 	 ((ch)=='\\' || (ch)=='/')
#  define ISABSOLUTE(cp) (ISDIRSEP(*cp) || \
			 ((strlen(cp)>=3) && isalpha(*cp) &&(cp[1]==':') && \
			 ISDIRSEP(cp[2])))
#  define DIRSEP 	 '\\'
#  define SDIRSEP  	 "\\"
#  define PATHSEP	 ';'
#endif


void 	  STk_whence(char *exec, char *path);
SCM 	  STk_internal_expand_file_name(char *s);
SCM 	  STk_resolve_link(char *path, int count);
int 	  STk_dirp(const char *path);

PRIMITIVE STk_expand_file_name(SCM s);
PRIMITIVE STk_canonical_path(SCM str);
PRIMITIVE STk_getcwd(void);
PRIMITIVE STk_chdir(SCM s);
PRIMITIVE STk_getpid(void);
PRIMITIVE STk_system(SCM com);
PRIMITIVE STk_getenv(SCM str);
PRIMITIVE STk_setenv(SCM var, SCM value);

PRIMITIVE STk_file_is_directoryp(SCM f);
PRIMITIVE STk_file_is_regularp(SCM f);
PRIMITIVE STk_file_is_readablep(SCM f);
PRIMITIVE STk_file_is_writablep(SCM f);
PRIMITIVE STk_file_is_executablep(SCM f);
PRIMITIVE STk_file_existp(SCM f);
PRIMITIVE STk_file_glob(SCM l, int len);

PRIMITIVE STk_remove_file(SCM filename);
PRIMITIVE STk_rename_file(SCM filename1, SCM filename2);
PRIMITIVE STk_temporary_file_name(void);


/*
  ------------------------------------------------------------------------------
  ----
  ---- V A L U E S . C
  ----
  ------------------------------------------------------------------------------
*/

PRIMITIVE STk_values(SCM l, int len);
PRIMITIVE STk_call_with_values(SCM producer, SCM consumer);

/*
  ------------------------------------------------------------------------------
  ----
  ---- V E C T O R . C
  ----
  ------------------------------------------------------------------------------
*/
SCM STk_makevect(int len, SCM init);

PRIMITIVE STk_vectorp(SCM obj);
PRIMITIVE STk_make_vector(SCM len, SCM init);
PRIMITIVE STk_vector(SCM l, int len);
PRIMITIVE STk_vector_length(SCM v);
PRIMITIVE STk_vector_ref(SCM v, SCM index);
PRIMITIVE STk_vector_set(SCM v, SCM index, SCM value);
PRIMITIVE STk_vector2list(SCM v);
PRIMITIVE STk_list2vector(SCM l);
PRIMITIVE STk_vector_fill(SCM v, SCM fill);

PRIMITIVE STk_vector_copy(SCM vect);
PRIMITIVE STk_vector_resize(SCM vect, SCM size);
PRIMITIVE STk_sort(SCM obj, SCM test);


/*
  ------------------------------------------------------------------------------
  ----
  ---- V P O R T . C
  ----
  ------------------------------------------------------------------------------
*/

struct vport_descr {		/* Slot order is important (see port_descr) */
  FILE *file;			/* structure is (for now) identical to str ports */
  int  flags;
};


void      STk_mark_virtual_port(SCM port);
void      STk_free_virtual_port(SCM port);
PRIMITIVE STk_open_input_virtual(SCM l, int len);
PRIMITIVE STk_open_output_virtual(SCM l, int len);
PRIMITIVE STk_input_virtual_portp(SCM port);
PRIMITIVE STk_output_virtual_portp(SCM port);

/*
  ------------------------------------------------------------------------------
  ----
  ---- U S E R I N I T . C
  ----
  ------------------------------------------------------------------------------
*/
void STk_user_init(void);
void STk_user_cleanup(void);

/*
  ------------------------------------------------------------------------------
  ----
  ---- G L O B A L   V A R I A B L E S 
  ----
  ------------------------------------------------------------------------------
*/
#if defined(WIN32) && defined(USE_DYNLOAD) && defined(MSC_VER)  
#  ifdef EXPORT_DLL_GLOBALS
#    define Extern __declspec( dllexport )
#    include <tkScale.h>
#  else
#    ifdef IMPORT_DLL_GLOBALS
#      define Extern __declspec( dllimport )
#    else
#      define Extern extern
#    endif
#  endif
#else 
   /* Unix */
#  ifdef STK_MAIN
#     define Extern
#  else
#     define Extern extern
#  endif
#endif


/* Remember if we are running the stk or snow interpreter */
Extern int STk_snow_is_running;

/* Program name (expanded) */
Extern char STk_Argv0[MAX_PATH_LENGTH];

/* Is it an intearctive interpreter? */
Extern int STk_interactivep;

/* Scheme booleans #t and #f */
Extern SCM STk_truth, STk_ntruth;

/* Scheme () and the undefined value */
Extern SCM STk_nil, STk_undefined, STk_unbound;

/* read buffer */
Extern char *STk_tkbuffer;
Extern int STk_line_counter;
Extern SCM STk_current_filename;

/* Special symbols */
Extern SCM STk_sym_lambda, STk_sym_quote,STk_sym_dot, STk_sym_imply, 
			  STk_sym_debug, STk_sym_else, STk_sym_quasiquote, 
           STk_sym_unquote, STk_sym_unquote_splicing, STk_sym_break;

/* Global environment */
Extern SCM STk_globenv;

/* Library location */
Extern char *STk_library_path;

/* Is the interpreter safe. Of course not!!! */
Extern int STk_is_safe;

/* The last variable defined with a DEFINE */
Extern SCM STk_last_defined;

/* Standard input/output/error ports */
Extern SCM STk_stdin, STk_stdout, STk_stderr;

#undef  Extern
#define Truth 	  STk_truth
#define Ntruth	  STk_ntruth
#define NIL	  STk_nil
#define UNBOUND   STk_unbound
#define UNDEFINED STk_undefined

#define Sym_lambda 	 STk_sym_lambda
#define Sym_quote 	 STk_sym_quote
#define Sym_dot 	 STk_sym_dot
#define Sym_imply  	 STk_sym_imply
#define Sym_debug 	 STk_sym_debug
#define Sym_else 	 STk_sym_else
#define Sym_quasiquote   STk_sym_quasiquote
#define Sym_unquote 	 STk_sym_unquote
#define Sym_unq_splicing STk_sym_unquote_splicing
#define Sym_break	 STk_sym_break
	
#ifdef USE_TK
/*
  ------------------------------------------------------------------------------
  ----
  ---- T K - G L U E . C
  ----
  ------------------------------------------------------------------------------
*/
#include "tk-glue.h"

/*
  ------------------------------------------------------------------------------
  ----
  ---- T K - U T I L . C
  ----
  ------------------------------------------------------------------------------
*/

PRIMITIVE STk_string2widget(SCM str);
PRIMITIVE STk_widget2string(SCM widget);
PRIMITIVE STk_tk_commandp(SCM obj);
PRIMITIVE STk_widget_name(SCM obj);
PRIMITIVE STk_get_widget_data(SCM widget);
PRIMITIVE STk_set_widget_data(SCM widget, SCM value);
PRIMITIVE STk_widget_environment(SCM widget);

/*
  ------------------------------------------------------------------------------
  ----
  ---- T K - M A I N . C
  ----
  ------------------------------------------------------------------------------
*/
extern Tcl_Interp *STk_main_interp;	/* Interpreter for this application. */
extern SCM STk_Tk_module;		/* The Tk module */
extern int Tk_initialized ;		/* 1 when Tk is fully initialized */

void Tk_main(int synchronize, char *name, char *fileName, char *Xdisplay,
	     char *geometry);


/*
  ------------------------------------------------------------------------------
  ----
  ---- BACKWARD COMPATIBILITY STUFF
  ----
  ---- Don't use these definitions
  ------------------------------------------------------------------------------
*/

#define err			STk_err	
#define STk_scheme_load(file)	STk_load(file, UNBOUND)
#define STk_loadfile(file,err)	STk_load_file(file, NIL, err)
#define CLOSURE_PARAMETERS	CLOSPARAMS
#endif /* USE_TK */

/*
  ------------------------------------------------------------------------------
  ----
  ---- MISC
  ----
  ------------------------------------------------------------------------------
*/

#define ENTER_PRIMITIVE(name)	static char* proc_name = name;
#define ENTER_SCM(name)		ENTER_PRIMITIVE(name)


#ifdef DEBUG_STK
void Debug(char *message, SCM obj);
#else
#define Debug(message, obj)
#endif


#ifdef __cplusplus
};
#endif

#if defined(WIN32) && defined(USE_DYNLOAD) && defined(MSC_VER)
#  undef extern
#endif 

#endif /* ifndef _STK_H */
