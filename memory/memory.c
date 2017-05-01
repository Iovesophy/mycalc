/************************************************************
 * Copyright (C) K.Maebashi 1997/07/02
 ************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MEM.h"

static void default_error_handler(char *filename, int line, char *msg);
static MEM_ErrorHandler st_current_error_handler = default_error_handler;
static MEM_FailMode	st_current_fail_mode = MEM_FAIL_AND_EXIT;

static void
default_error_handler(char *filename, int line, char *msg)
{
    fprintf(stderr,"MEM:%s failed in %s at %d\n", msg, filename, line);
}

static void
error_handler(char *filename, int line, char *msg)
{
    st_current_error_handler(filename, line, msg);

    if (st_current_fail_mode == MEM_FAIL_AND_EXIT) {
	exit(1);
    }
}

void*
MEM_malloc_func(char *filename, int line, size_t size)
{
    void	*ptr;

    if ((ptr = malloc(size)) == NULL) {
	error_handler(filename, line, "malloc");
    }

    return(ptr);
}

void*
MEM_realloc_func(char *filename, int line, void *ptr,size_t size)
{
    if (ptr == NULL) {
	if ((ptr = malloc(size)) == NULL) {
	    error_handler(filename, line, "realloc(malloc)");
	}
    } else {
	if ((ptr = realloc(ptr,size)) == NULL) {
	    error_handler(filename, line, "realloc");
	}
    }
    return(ptr);
}

char *
MEM_strdup_func(char *filename, int line, char *str)
{
    char	*ptr;

    if ((ptr = malloc(strlen(str) + 1)) == NULL) {
	error_handler(filename, line, "strdup");
    }
    strcpy(ptr, str);

    return(ptr);
}

void
MEM_free(void *ptr)
{
    if (ptr != NULL) {
	free(ptr);
    }
}

void
MEM_set_error_handler(MEM_ErrorHandler handler)
{
    st_current_error_handler = handler;
}

void
MEM_set_fail_mode(MEM_FailMode mode)
{
    st_current_fail_mode = mode;
}

