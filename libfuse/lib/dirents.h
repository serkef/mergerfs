#pragma once

#include <dirent.h>
#include <stdint.h>

typedef struct fuse_dirents_s fuse_dirents_t;
struct fuse_dirents_s
{
  void *buf;
  uint64_t buf_len;
  uint64_t data_len;
};
