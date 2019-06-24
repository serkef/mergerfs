#pragma once

#include "fuse_entry.h"
#include "fuse_attr.h"
#include "fuse_dirent.h"

typedef struct fuse_direntplus_s fuse_direntplus_t;
struct fuse_direntplus_s
{
  fuse_entry_t  entry;
  fuse_attr_t   attr;
  fuse_dirent_t dirent;
};
