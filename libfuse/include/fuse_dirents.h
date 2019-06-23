#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "fuse_entry.h"

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct fuse_dirents_s fuse_dirents_t;
struct fuse_dirents_s
{
  char     *buf;
  uint64_t  buf_len;
  uint64_t  data_len;
};

int  fuse_dirents_init(fuse_dirents_t *d);
void fuse_dirents_free(fuse_dirents_t *d);
void fuse_dirents_reset(fuse_dirents_t *d);

int  fuse_dirents_add(fuse_dirents_t *d,
                      struct dirent  *de);
int  fuse_dirents_add_plus(fuse_dirents_t      *d,
                           const struct dirent *de,
                           const fuse_entry_t  *entry,
                           const struct stat   *st);


#ifdef __cplusplus
}
#endif
