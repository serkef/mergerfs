#pragma once

#include <stdint.h>

typedef struct fuse_dirent_s fuse_dirent_t;
struct fuse_dirent_s
{
  uint64_t ino;
  uint64_t off;
  uint32_t namelen;
  uint32_t type;
  char     name[];
};
