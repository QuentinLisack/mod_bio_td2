#ifndef _MEM_H_
#define _MEM_H_

#include <stdlib.h>

/* allocate memory, or print errMess and die if malloc fails */
void *mallocOrDie(size_t size, char *errMess) ;
void reallocOrDie(void *ptr, size_t size, char *errMess);

#endif
