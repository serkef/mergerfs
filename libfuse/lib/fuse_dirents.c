#define _FILE_OFFSET_BITS 64

#include "fuse_dirents.h"

#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct fuse_dirent_s fuse_dirent_t;
struct fuse_dirent_s
{
  uint64_t ino;
  uint64_t off;
  uint32_t namelen;
  uint32_t type;
  char     name[];
};


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

  if(d_->buf_len < (size_ + d_->data_len))
    {
      p = realloc(d_->buf,(size_ + d_->buf_len));
      if(p == NULL)
        return -errno;

      d_->buf      = p;
      d_->buf_len += size_;
    }

  return 0;
}

static
fuse_dirent_t*
fuse_dirents_alloc(fuse_dirents_t *d_,
                   uint64_t        size_)
{
  int rv;
  char *p;
  fuse_dirent_t *d;

  rv = fuse_dirents_resize(d_,size_);
  if(rv)
    return NULL;

  p  = d_->buf;
  p += d_->data_len;
  d = (fuse_dirent_t*)p;

  return d;
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
  d->off     = ((char*)d - (char*)d_->buf);
  d->namelen = namelen;
  d->type    = dirent_->d_type;
  memcpy(d->name,dirent_->d_name,namelen);

  d_->data_len += size;

  return 0;
}

void
fuse_dirents_reset(fuse_dirents_t *d_)
{
  d_->data_len = 0;
}

void
fuse_dirents_init(fuse_dirents_t *d_)
{
  d_->buf      = NULL;
  d_->buf_len  = 0;
  d_->data_len = 0;
}

void
fuse_dirents_free(fuse_dirents_t *d_)
{
  d_->buf_len  = 0;
  d_->data_len = 0;
  free(d_->buf);
}

fuse_dirent_t*
fuse_dirents_next(fuse_dirents_t *dirents_,
                  fuse_dirent_t  *dirent_)
{
  return NULL;
}

void
fuse_dirents_fprintf(fuse_dirents_t *d_,
                     FILE           *f_)
{
  fuse_dirent_t *d;

  d = d_->buf;
  while(d < (d_->buf + d_->buf_len))
    {
      fprintf(stderr,"%.*s\n",d->namelen,d->name);
      d = ((char*)d + align_uint64_t(sizeof(fuse_dirent_t) + d->namelen));
    }
}
