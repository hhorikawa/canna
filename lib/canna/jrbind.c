/* Copyright 1992 NEC Corporation, Tokyo, Japan.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of NEC
 * Corporation not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission.  NEC Corporation makes no representations about the
 * suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * NEC CORPORATION DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN 
 * NO EVENT SHALL NEC CORPORATION BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF 
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR 
 * OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR 
 * PERFORMANCE OF THIS SOFTWARE. 
 */

#if !defined(lint) && !defined(__CODECENTER__)
static char rcs_id[] = "@(#) 102.1 $Id: jrbind.c,v 1.4 2003/09/17 08:50:53 aida_s Exp $";
#endif /* lint */

#include "canna.h"
#include <canna/mfdef.h>
#include <sys/types.h>

/*********************************************************************
 *                      wchar_t replace begin                        *
 *********************************************************************/
#ifdef wchar_t
# error "wchar_t is already defined"
#endif
#define wchar_t cannawc

#define ACTHASHTABLESIZE 64
#define KEYHASHTABLESIZE 16
   
/*

  jrKanjiString �� TTY �Υ������Ϥ������ꡢ���Υ����ˤ������ä�ɬ��
  �ʤ饫�ʴ����Ѵ���Ԥ������Υ������Ϥη�̤Ȥ���������ʸ����� 
  buffer_return ���֤���buffer_return �ϥ��ץꥱ�������¦���Ѱդ���Х�
  �ե��Ǥ��ꡢ���ץꥱ�������Ϥ��ΥХåե���Ĺ���� bytes_buffer ����
  ����

  kanji_status_return �ϳ��ꤷ�Ƥ��ʤ�����ʸ�����ɽ�����뤿��Υǡ���
  �Ǥ��ꡢ̤������ɤߤ��������ʤɤ��֤���롣kanji_status_return��
  ���Фˤϡ� echoStr, length, revPos, revLen �����ꤽ�줾�졢̤����
  ʸ����ؤΥݥ��󥿡�����Ĺ����̤����ʸ����Τ�������Ĵ������ʬ�ؤΥ�
  �ե��åȡ���Ĵ������ʬ��Ĺ�����֤���̤����ʸ������Ǽ�����ΰ�� 
  jrKanjiString �Ǽ�ưŪ���Ѱդ���롣

 */

extern int FirstTime;

extern BYTE *actFromHash();

exp(int)
wcKanjiString(context_id, ch, buffer_return, nbuffer, kanji_status_return)
const int context_id, ch, nbuffer;
wchar_t        *buffer_return;
wcKanjiStatus  *kanji_status_return;
{
  int res;

  *buffer_return = (wchar_t)ch;

  res = XwcLookupKanji2((unsigned int)0, (unsigned int)context_id,
			buffer_return, nbuffer,
			1/* byte */, 1/* functional char*/,
			kanji_status_return);
  return res;
}

/* jrKanjiControl -- ���ʴ����Ѵ��������Ԥ� */

exp(int)
wcKanjiControl(context, request, arg)
     const int context;
     const int request;
     char *arg;
{
  return XwcKanjiControl2((unsigned int)0, (unsigned int)context,
			  (unsigned int)request, (BYTE *)arg);
}

static uiContext
newUiContext(dpy, win)
unsigned int dpy, win;
{
  extern struct CannaConfig cannaconf;
  uiContext d;

  if ((d = (uiContext)malloc(sizeof(uiContextRec))) != (uiContext)0) {
    if (initRomeStruct(d, cannaconf.chikuji) == 0) {
      if (internContext(dpy, win, d)) {
	return d;
      }
      freeRomeStruct(d);
    }
    free((char *)d);
  }
  return (uiContext)0;
}

extern int kanjiControl pro((int, uiContext, caddr_t));

XwcLookupKanji2(dpy, win, buffer_return, nbuffer, nbytes, functionalChar,
		kanji_status_return)
unsigned int dpy, win;
int functionalChar, nbytes;
wchar_t *buffer_return;
int nbuffer;
wcKanjiStatus *kanji_status_return;
{
  uiContext d, keyToContext();
  int retval;

  /* ���� XLookupKanjiString ���ƤФ줿���ϼ���ν�����ʤɤν�����
     �Ԥ��롣 */

  if (FirstTime) {
    if (kanjiControl(KC_INITIALIZE, (uiContext)NULL, (char *)NULL) == -1) {
      return -1;
    }
    FirstTime = 0;
  }

  d = keyToContext(dpy, win);

  if (d == (uiContext)NULL) {
    /* ���Υ�����ɥ����饤�٥�Ȥ��褿�Τ��Ϥ�Ƥ��ä��ꤹ��櫓�� */
    d = newUiContext(dpy, win);
    if (d == (uiContext)NULL) {
      return NoMoreMemory();
    }
  }


  bzero(kanji_status_return, sizeof(wcKanjiStatus));
  
  d->ch = (unsigned)*buffer_return;
  d->buffer_return = buffer_return;
  d->n_buffer = nbuffer;
  d->kanji_status_return = kanji_status_return;

  debug_message("current_mode(0x%x)\n", d->current_mode,0,0);

  if ( nbytes || functionalChar ) { /* ����饯�������ɤ��Ȥ줿��� */
    int check;

    *buffer_return = key2wchar(d->ch, &check);
    if (!check) {
      return NothingChangedWithBeep(d);
    }

    d->nbytes = nbytes;

    retval = doFunc(d, 0);
#ifdef DEBUG
    checkModec(d);
#endif /* DEBUG */
    return(retval);
  }
  else { /* ����饯�������ɤ��Ȥ�ʤ��ä����ʥ��եȥ����ʤɡ�... */
    d->kanji_status_return->length = -1;
    return 0;
  }
}

uiContext keyToContext();

int
XwcKanjiControl2(display, window, request, arg)
unsigned int display, window, request;
BYTE *arg;
{
  if (request == KC_INITIALIZE || request == KC_FINALIZE ||
      request == KC_SETSERVERNAME || request == KC_SETINITFILENAME ||
      request == KC_SETVERBOSE || request == KC_KEYCONVCALLBACK ||
      request == KC_QUERYCONNECTION || request == KC_SETUSERINFO ||
      request == KC_QUERYCUSTOM) {
    return kanjiControl(request, (uiContext)NULL, (char *)arg);
  }
  else if (/* 0 <= request && (ɬ����) */ request < MAX_KC_REQUEST) {
    uiContext d;

    /* ���� wcKanjiString ���ƤФ줿���ϼ���ν�����ʤɤν�����
       �Ԥ��롣 */

    if (FirstTime) {
      if (kanjiControl(KC_INITIALIZE, (uiContext)NULL, (char *)NULL) == -1) {
	return -1;
      }
      FirstTime = 0;
    }

    d = keyToContext((unsigned int)display, (unsigned int)window);

    if (d == (uiContext)NULL) {
      d = newUiContext(display, window);
      if (d == (uiContext)NULL) {
	return NoMoreMemory();
      }
    }

    if (request == KC_CLOSEUICONTEXT) {
      rmContext(display, window);
    }
    return kanjiControl(request, d, (char *)arg);
  }
  else {
    return -1;
  }
}

struct map {
  KanjiMode tbl;
  BYTE key;
  KanjiMode mode;
  struct map *next;
} *mapFromHash();

/* cfuncdef

  pushCallback -- ������Хå��ν����ץå��夹�롣

  ������Хå��ν�����Ǽ�������� malloc ����ơ����줬 uiContext ��
  �ץå��夵��롣

  malloc ���줿��������ͤȤ����֤롣

 */

struct callback *
pushCallback(d, env, ev, ex, qu, au)
uiContext d;
mode_context env;
canna_callback_t ev, ex, qu, au;
{
  struct callback *newCB;

  newCB = (struct callback *)malloc(sizeof(struct callback));
  if (newCB) {
    newCB->func[0] = ev;
    newCB->func[1] = ex;
    newCB->func[2] = qu;
    newCB->func[3] = au;
    newCB->env = env;
    newCB->next = d->cb;
    d->cb = newCB;
  }
  return newCB;
}

void
popCallback(d)
uiContext d;
{
  struct callback *oldCB;

  oldCB = d->cb;
  d->cb = oldCB->next;
  free(oldCB);
}

#ifndef wchar_t
# error "wchar_t is already undefined"
#endif
#undef wchar_t
/*********************************************************************
 *                       wchar_t replace end                         *
 *********************************************************************/