#define _FILE_OFFSET_BITS 64

#include "fuse_attr.h"
#include "fuse_dirent.h"
#include "fuse_direntplus.h"
#include "fuse_dirents.h"
#include "fuse_entry.h"
#include "stat_utils.h"

#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SIZE 4096

static
uint64_t
align_uint64_t(uint64_t v_)
{
  return ((v_ + sizeof(uint64_t) - 1) & ~(sizeof(uint64_t) - 1));
}

static
int
fuse_dirents_resize(fuse_dirents_t *d_,
                    uint64_t        size_)
{
  void *p;

  if((d_->data_len + size_) >= d_->buf_len)
    {
      p = realloc(d_->buf,(d_->buf_len * 2));
      if(p == NULL)
        return -errno;

      d_->buf      = p;
      d_->buf_len *= 2;
    }

  return 0;
}

static
fuse_dirent_t*
fuse_dirents_alloc(fuse_dirents_t *d_,
                   uint64_t        size_)
{
  int rv;
  fuse_dirent_t *d;

  rv = fuse_dirents_resize(d_,size_);
  if(rv)
    return NULL;

  d = (fuse_dirent_t*)&d_->buf[d_->data_len];

  d_->data_len += size_;

  return d;
}

static
void
fuse_dirents_fill_attr(fuse_attr_t       *attr_,
                       const struct stat *st_)
{
  attr_->ino       = st_->st_ino;
  attr_->size      = st_->st_size;
  attr_->blocks    = st_->st_blocks;
  attr_->atime     = st_->st_atime;
  attr_->mtime     = st_->st_mtime;
  attr_->ctime     = st_->st_ctime;
  attr_->atimensec = ST_ATIM_NSEC(st_);
  attr_->mtimensec = ST_MTIM_NSEC(st_);
  attr_->ctimensec = ST_CTIM_NSEC(st_);
  attr_->mode      = st_->st_mode;
  attr_->nlink     = st_->st_nlink;
  attr_->uid       = st_->st_uid;
  attr_->gid       = st_->st_gid;
  attr_->rdev      = st_->st_rdev;
  attr_->blksize   = st_->st_blksize;
}

int
fuse_dirents_add(fuse_dirents_t *d_,
                 struct dirent  *dirent_)
{
  uint64_t size;
  uint64_t namelen;
  fuse_dirent_t *d;

  namelen = _D_EXACT_NAMLEN(dirent_);
  size = align_uint64_t(sizeof(fuse_dirent_t) + namelen);

  d = fuse_dirents_alloc(d_,size);
  if(d == NULL)
    return -ENOMEM;

  d->ino     = dirent_->d_ino;
  d->off     = d_->data_len;
  d->namelen = namelen;
  d->type    = dirent_->d_type;
  memcpy(d->name,dirent_->d_name,namelen);

  return 0;
}

int
fuse_dirents_add_plus(fuse_dirents_t      *d_,
                      const struct dirent *dirent_,
                      const fuse_entry_t  *entry_,
                      const struct stat   *st_)
{
  uint64_t size;
  uint64_t namelen;
  fuse_direntplus_t *d;



  return 0;
}

void
fuse_dirents_reset(fuse_dirents_t *d_)
{
  d_->data_len = 0;
}

int
fuse_dirents_init(fuse_dirents_t *d_)
{
  void *buf;

  buf = malloc(DEFAULT_SIZE);
  if(buf == NULL)
    return -ENOMEM;

  d_->buf      = buf;
  d_->buf_len  = DEFAULT_SIZE;
  d_->data_len = 0;

  return 0;
}

void
fuse_dirents_free(fuse_dirents_t *d_)
{
  d_->buf_len  = 0;
  d_->data_len = 0;
  free(d_->buf);
}
