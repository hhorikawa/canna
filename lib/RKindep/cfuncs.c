/* The Authors:
 *   Copyright (c) 2002-2004 Canna Project.
 *   Copyright (c) 1990-1997 NEC Corporation, Tokyo, Japan.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of the
 * authors and contributors not be used in advertising or publicity
 * pertaining to distribution of the software without specific, written
 * prior permission.  The authors and contributors make no representations
 * about the suitability of this software for any purpose.  It is
 * provided "as is" without express or implied warranty.
 *
 * THE AUTHORS AND CONTRIBUTORS DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE AUTHORS AND CONTRIBUTORS BE LIABLE FOR
 * ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTUOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "config.h"
#include "canna/ccompat.h"
#include "RKindep/ecfuncs.h"

RCSID("$Id: cfuncs.c,v 1.2.2.1 2003/12/27 17:15:24 aida_s Exp $");


#ifndef HAVE_STRLCPY
size_t
RkiAltStrlcpy(char* dst, const char* src, size_t size)
{
  const char *sp = src;
  char *dp = dst;
  char *dstend;
  if (!size) /* should not happen */
    goto last;
  dstend = dst + size - 1;
  for (; dp < dstend && *sp; ++dp, ++sp)
    *dp = *sp;
  *dp = '\0';
last:
  for (; *sp; ++sp)
    ;
  return sp - src;
}

size_t
RkiAltStrlcat(char* dst, const char* src, size_t size)
{
  const char *sp = src;
  char *dp = dst;
  char *dstend;

  dstend = dst + size; /* first dstend */
  for (; dp < dstend; ++dp)
    if (!*dp)
      goto next;
  /* dp == dstend */
  goto last; /* should not happen */
next:
  --dstend; /* second dstend */
  for (; dp < dstend && *src; ++dp, ++src)
    *dp = *src;
  *dp = '\0';
last:
  for (; *sp; ++sp)
    ;
  return (dp - dst) + (sp - src);
}
#endif /* !HAVE_STRLCPY */

/* vim: set sw=2: */
