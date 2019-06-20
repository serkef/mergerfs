#pragma once

#include "DG_dynarr.h"

#include <dirent.h>
#include <stdint.h>

DA_TYPEDEF(uint64_t,uint64_array_t);

typedef struct fuse_dirents_s fuse_dirents_t;
struct fuse_dirents_s
{
  void     *buf;
  uint64_t  buf_len;
  uint64_t  data_len;
  uint64_array_t idxs;
};

void fuse_dirents_init(fuse_dirents_t *d);
void fuse_dirents_free(fuse_dirents_t *d);
int  fuse_dirents_add(fuse_dirents_t *d,
                      struct dirent  *de);
void fuse_dirents_reset(fuse_dirents_t *d);
