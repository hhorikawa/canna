﻿// -*- coding:utf-8-with-signature -*-

/* Copyright (c) 2003 Canna Project. All rights reserved.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of the
 * author and contributors not be used in advertising or publicity
 * pertaining to distribution of the software without specific, written
 * prior permission.  The author and contributors no representations
 * about the suitability of this software for any purpose.  It is
 * provided "as is" without express or implied warranty.
 *
 * THE AUTHOR AND CONTRIBUTORS DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE AUTHOR AND CONTRIBUTORS BE LIABLE FOR
 * ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTUOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "config.h"
#include "canna/ccompat.h"
#include "RKindep/file.h"
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include "canna/net.h"
#include <stdio.h>
#include <assert.h>
#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
#endif

RCSID("$Id: file.c,v 1.3 2003/09/24 14:50:40 aida_s Exp $");

// lib/RKC/wconvert.c:try_connect() から呼び出される.
// @return If error, -1
int
RkiConnect( SOCKET fd, struct sockaddr* addrp, size_t len,
            const struct timeval* timeout)
{
    int flags;
    int res = -1, r;
    socklen_t optlen;
  struct timeval tval = *timeout;
  rki_fd_set wfds;

#ifndef _WIN32
    flags = fcntl(fd, F_GETFL, 0);
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK))
        return -1;
#else
    u_long iMode = 1; // non-blocking
    int iResult = ioctlsocket( fd, FIONBIO, &iMode);
    if ( iResult != NO_ERROR )
        return -1;
#endif

  if (!connect(fd, addrp, len)) {
    res = 0; /* succeeded at once */
    goto finish;
  }
  else if (errno != EINPROGRESS)
    goto finish;

  RKI_FD_ZERO(&wfds);
  RKI_FD_SET(fd, &wfds);
  r = select(fd + 1, NULL, &wfds, NULL, &tval);
    if (r <= 0 || !RKI_FD_ISSET(fd, &wfds))
        goto finish; /* timeout or error (FIXME: EINTR) */

    optlen = sizeof(int);
    if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (char *)&r, &optlen))
        goto finish; /* for nonstandard platforms */
  if (!r)
    res = 0;

finish:
#ifndef _WIN32
    fcntl(fd, F_SETFL, flags);
#endif
  return res;
}

/*
 * @return non NULL: Pointer to malloc()ed line buffer (you must free())
 *         NULL: Error or EOF.
 *   feof() != 0: no lines after this
 *   ferror() != 0: some error happened in stdio
 *   !feof() && !ferror(): out-of-memory (errno == ENOMEM)
 */
/* stdioのfeof()のセマンティクスはどこへ行っても共通なのだろうか？ */
char *
RkiGetLine(FILE* fp)
{
  char *buf, *tmp;
  size_t buflen;
  size_t pos;
  const char *readres;

    buflen = 32; /* for now */
    buf = (char*) malloc(buflen);
    if (!buf)
        return NULL;
  pos = 0;
  for (;;) {
    assert(pos < buflen);
    if (pos == buflen - 1) {
      buflen *= 2;
      tmp = (char*) realloc(buf, buflen);
      if (!tmp)
	goto err;
      buf = tmp;
    }
    readres = fgets(buf + pos, buflen - pos, fp);
    if (!readres) {
      if (pos == 0)
	goto err;
      clearerr(fp);
      break;
    }
    pos = strlen(buf); /* excluding '\0' */
    if (pos && buf[pos - 1] == '\n')
      break;
  }
  return buf;
err:
  free(buf);
  return NULL;
}

// @return If malloc() failed, NULL.
void *
RkiReadWholeFile(FILE* fp, size_t* retsize)
{
    size_t pos = 0;
    size_t buflen = 256;
    char *buf = (char*) malloc(buflen);
    if (!buf) /* needed even for empty file */
        return NULL;
  for (;;) {
    size_t nread;
    assert(pos < buflen); /* must not pos == buflen */
    nread = fread(buf + pos, 1, buflen - pos, fp);
    if (!nread) {
      if (feof(fp))
	break;
      goto fail;
    }
    pos += nread;
    assert(pos <= buflen);
    if (buflen - pos < 20) {
      char *tmp;
      buflen *= 2;
      tmp = (char*) realloc(buf, buflen);
      if (!tmp)
	goto fail;
      buf = tmp;
    }
  }
  if (retsize)
    *retsize = pos;
  return (void *)buf;
fail:
  free(buf);
  return NULL;
}

/* vim: set sw=2: */
