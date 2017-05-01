/************************************************************
 * Copyright (C) K.Maebashi 1997/07/02
 ************************************************************/
#ifndef	PUBLIC_MEM_H
#define	PUBLIC_MEM_H

#include <stdlib.h>

typedef enum {
    MEM_FAIL_AND_EXIT,
    MEM_FAIL_AND_RETURN
} MEM_FailMode;
typedef void (*MEM_ErrorHandler)(char *, int, char *);
typedef struct MEM_Storage_tag *MEM_Storage;

/*
 * Don't use mem_*_func function.
 * There are private functions of MEM module.
 */
void *MEM_malloc_func(char *filename, int line, size_t size);
void *MEM_realloc_func(char *filename, int line, void *ptr, size_t size);
char *MEM_strdup_func(char *filename, int line, char *str);
MEM_Storage MEM_open_storage_func(char *filename, int line, int page_size);
void *MEM_storage_malloc_func(char *filename, int line,
			      MEM_Storage storage, size_t size);

#define	MEM_malloc(size)\
  (MEM_malloc_func(__FILE__, __LINE__, size))
#define MEM_realloc(ptr, size)\
  (MEM_realloc_func(__FILE__, __LINE__, ptr, size))
#define MEM_strdup(str)\
  (MEM_strdup_func(__FILE__, __LINE__, str))
#define MEM_open_storage(page_size)\
  (MEM_open_storage_func(__FILE__, __LINE__, page_size))
#define MEM_storage_malloc(storage, size)\
  (MEM_storage_malloc_func(__FILE__, __LINE__, storage, size))

void	MEM_free(void *ptr);
void	MEM_set_error_handler(MEM_ErrorHandler handler);
void	MEM_set_fail_mode(MEM_FailMode mode);
void	MEM_dispose_storage(MEM_Storage storage);
#endif	/* PUBLIC_MEM_H */
