#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>

typedef struct fuse_dirents_s fuse_dirents_t;
struct fuse_dirents_s
{
  char     *buf;
  uint64_t  buf_len;
  uint64_t  data_len;
};

int  fuse_dirents_init(fuse_dirents_t *d);
void fuse_dirents_free(fuse_dirents_t *d);
int  fuse_dirents_add(fuse_dirents_t *d,
                      struct dirent  *de);
void fuse_dirents_reset(fuse_dirents_t *d);

#ifdef __cplusplus
}
#endif
