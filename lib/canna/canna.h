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

/*
 * @(#) 102.1 $Id: canna.h,v 1.11 2003/09/25 07:24:54 aida_s Exp $
 */

#ifndef _CANNA_H_
#define _CANNA_H_

#include "config.h"
#include "canna/ccompat.h"
#include <stdio.h>

#ifndef pro
  #define pro(x) x
#endif

#define CANNA_NEW_WCHAR_AWARE
#include <canna/RK.h>
#include <canna/jrkanji.h>

#define POINTERINT canna_intptr_t
#define exp(x) x

#if 0
#define USE_MALLOC_FOR_BIG_ARRAY
#endif

#define	WCHARSIZE	(sizeof(cannawc))

#define MBstowcs CANNA_mbstowcs
#define WCstombs CANNA_wcstombs

#include "canna/sglobal.h"

#define XLookupKanji2			 IROHA_G300_XLookupKanji2
#define XKanjiControl2			 IROHA_G301_XKanjiControl2
#define XwcLookupKanji2			 IROHA_G425_XwcLookupKanji2
#define XwcKanjiControl2		 IROHA_G426_XwcKanjiControl2
#define FirstTime			 CANNA_G271_FirstTime

#define STROKE_LIMIT 500 /* ���ȥ�������³���ڤ� */

typedef unsigned char BYTE;


/*
 * CANNALIBDIR  -- �����ƥ�Υ������ޥ����ե��������޻������Ѵ�
 *                 �ơ��֥뤬�֤����ǥ��쥯�ȥꡣ
 */

#ifndef CANNALIBDIR
#define CANNALIBDIR "/usr/lib/canna"
#endif

/* flags �ξ��� */
#define CANNA_KANJIMODE_TABLE_SHARED	01
#define CANNA_KANJIMODE_EMPTY_MODE	02

/* func ���軰���� */
#define KEY_CALL  0
#define KEY_CHECK 1
#define KEY_SET   2

extern BYTE default_kmap[];

/* menuitem -- ��˥塼ɽ���ι��ܤ��������ơ��֥� */

typedef struct _menuitem {
  int flag; /* ���򸫤� */
  union {
    struct _menustruct *menu_next; /* ��˥塼�ؤΥݥ��� */
    int fnum;    /* ��ǽ�ֹ� */
    char *misc;  /* ����¾(lisp �Υ���ܥ�ʤ�) */
  } u;
} menuitem;

#define MENU_SUSPEND 0 /* �ޤ���ޤäƤ��ʤ�(lisp �Υ���ܥ�) */
#define MENU_MENU    1 /* ��˥塼 */
#define MENU_FUNC    2 /* ��ǽ�ֹ� */

/* menustruct -- ��˥塼�򤷤��빽¤�� */

typedef struct _menustruct {
  int     nentries; /* ��˥塼�ι��ܤο� */
  cannawc **titles; /* ��˥塼�θ��Ф��ꥹ�� */
  cannawc *titledata; /* ��Υꥹ�Ȥμ���ʸ���� */
  menuitem *body;   /* ��˥塼�����(����) */
  int     modeid;   /* ��˥塼�Υ⡼���ֹ� */
  struct _menustruct *prev; /* ������Υ�˥塼�ؤΥݥ��� */
} menustruct;

typedef struct _menuinfo {
  menustruct *mstruct; /* �ɤΥ�˥塼�� */
  int        curnum;   /* ���ʤ������򤵤줿�ֹ�Ϥ���Ǥ��� */
  struct _menuinfo *next;
} menuinfo;

/* defselection ��������줿����ط��ΰ�����ȤäƤ�����¤�� */

typedef struct {
  cannawc	**kigo_data;	/* ����ɽ���γ����Ǥ����� */
  cannawc	*kigo_str;	/* ����ɽ���������Ǥ���������� */
  int		kigo_size;	/* ���Ǥο� */
  int		kigo_mode;	/* ���ΤȤ��Υ⡼�� */
} kigoIchiran;

typedef struct _selectinfo {
  kigoIchiran	*ichiran;	/* �ɤΰ����� */
  int		curnum;		/* �������򤵤줿�ֹ� */
  struct _selectinfo *next;
} selectinfo;

/* deldicinfo -- ñ�����κݤ�ɬ�פʼ���ξ���򤤤�Ƥ�����¤�� */

#define INDPHLENGTH 16 /* ��Ω��ǰ���Ĺ���ʻ��Ĺ�� */

typedef struct _deldicinfo {
  cannawc *name;
  cannawc hcode[INDPHLENGTH];
} deldicinfo;

/*
 * glineinfo -- �������ɽ���Τ��������������Ǽ���Ƥ�������ι�¤�Ρ�
 * ���줾��Υ��Фϰʲ��ΰ�̣����ġ�
 *
 * glkosu -- ���ιԤˤ������ο�
 * glhead -- ���ιԤ���Ƭ���䤬��kouhoinfo�β����ܤ�(0���������)
 * gllen  -- ���ιԤ�ɽ�����뤿���ʸ�����Ĺ��
 * gldata -- ���ιԤ�ɽ�����뤿���ʸ����ؤΥݥ���
 */

typedef struct {
  int glkosu;
  int glhead;
  int gllen;
  cannawc *gldata;
} glineinfo;

/*
 * kouhoinfo -- ��������Τ��������������Ǽ���Ƥ�������ι�¤��
 * ���줾��Υ��Фϰʲ��ΰ�̣����ġ�
 *
 * khretsu -- ���θ��䤬�����
 * khpoint -- ���θ���ιԤΤʤ��Ǥΰ���
 * khdata -- ���θ����ʸ����ؤΥݥ���
 */

typedef struct {
  int khretsu;
  int khpoint;
  cannawc *khdata;
} kouhoinfo;

#define ROMEBUFSIZE 	1024
#define	BANGOSIZE	2	/* �������γƸ�����ֹ�Υ����� */
#define	BANGOMAX   	9	/* ���������κ������� */

#define	KIGOBANGOMAX   	16	/* ���������κ������� */
#define GOBISUU		9

#define	ON		1
#define	OFF		0

#define	NG		-1

#define NO_CALLBACK     (canna_callback_t)0
#define NCALLBACK	4

#define	JISHU_HIRA	0
#define JISHU_ZEN_KATA	1
#define JISHU_HAN_KATA	2
#define JISHU_ZEN_ALPHA	3
#define JISHU_HAN_ALPHA	4
#define MAX_JISHU	5

#define  SENTOU        0x01
#define  HENKANSUMI    0x02
#define  SUPKEY        0x04
#define  GAIRAIGO      0x08
#define  STAYROMAJI    0x10

/* ñ����Ͽ���ʻ� */
#define MEISHI       0
#define KOYUMEISHI   1
#define DOSHI        2
#define KEIYOSHI     3
#define KEIYODOSHI   4
#define FUKUSHI      5
#define TANKANJI     6
#define SUSHI        7
#define RENTAISHI    8
#define SETSUZOKUSHI 9
#define SAHENMEISHI 10
#define MEISHIN     11
#define JINMEI      12
#define KOYUMEISHIN 13
#define GODAN       14
#define RAGYODOSHI  15
#define RAGYOGODAN  16
#define KAMISHIMO   17
#define KEIYOSHIY   18
#define KEIYOSHIN   19
#define KEIYODOSHIY 20
#define KEIYODOSHIN 21
#define FUKUSHIY    22
#define FUKUSHIN    23

/* identifier for each context structures */
#define CORE_CONTEXT       ((BYTE)0)
#define YOMI_CONTEXT       ((BYTE)1)
#define ICHIRAN_CONTEXT    ((BYTE)2)
#define FORICHIRAN_CONTEXT ((BYTE)3)
#define MOUNT_CONTEXT      ((BYTE)4)
#define TOUROKU_CONTEXT    ((BYTE)5)
#define TAN_CONTEXT	   ((BYTE)6)

typedef struct _coreContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode; /* �������Υ⡼�� */
  struct _coreContextRec *next;
} coreContextRec, *coreContext;

typedef coreContext mode_context;

typedef struct  _yomiContextRec {
  /* core �����Ʊ������ */
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  struct _kanjiMode *curMode;
  struct _tanContextRec	 *left, *right;

  /* ���޻������Ѵ��ط� */
  struct RkRxDic *romdic;	/* ���޻������Ѵ��ơ��֥� */
  cannawc   romaji_buffer[ROMEBUFSIZE];
  /* ���޻��Хåե��� rStartp, rEndp �Σ��ĤΥ���ǥå����ˤ�äƴ�������
   * �롣rStartp �ϥ��ʤ��Ѵ��Ǥ��ʤ��ä����޻��κǽ��ʸ���ؤΥ���ǥ�
   * �����Ǥ��ꡢrEndp �Ͽ����˥��޻������Ϥ�����ˡ���Ǽ���٤�
   * romaji_buffer ��Υ���ǥå����Ǥ��롣���������Ϥ������޻��ϡ�
   * romaji_buffer + rEndp �����˳�Ǽ���졢���Υ��޻��򥫥ʤ��Ѵ���
   * ����ϡ�romaji_buffer + rStartp ���� rEndp - rStartp �Х��Ȥ�ʸ����
   * �оݤȤʤ롣 */
  int		  rEndp, rStartp, rCurs; /* ���޻��Хåե��Υݥ��� */
  cannawc         kana_buffer[ROMEBUFSIZE];
  BYTE            rAttr[ROMEBUFSIZE], kAttr[ROMEBUFSIZE];
  int		  kEndp; /* ���ʥХåե��κǸ�򲡤���ݥ��� */
  int             kRStartp, kCurs;

  /* ����¾�Υ��ץ���� */
  BYTE            myMinorMode;  /* yomiContext ��ͭ�Υޥ��ʥ⡼�� */
  struct _kanjiMode *myEmptyMode;		/* empty �⡼�ɤϤɤ줫 */
  long		  generalFlags;		/* see below */
  long		  savedFlags;		/* ��Υե饰�ΰ����Υ����� */
  BYTE		  savedMinorMode;	/* �ޥ��ʥ⡼�ɤΥ����� */
  BYTE		  allowedChars;		/* see jrkanji.h */
  BYTE		  henkanInhibition;	/* see below */
  int             cursup;		/* ���ʤ����ɤλ��˻Ȥ� */
#define SUSPCHARBIAS 100
  int             n_susp_chars;

/* from henkanContext */
  /* ���ʴ����Ѵ��ط� */
  int            context;
  int		 kouhoCount;	/* ���� henkanNext ��Ϣ³���Ʋ����줿�� */
  cannawc        echo_buffer[ROMEBUFSIZE];
  cannawc        **allkouho; /* RkGetKanjiList��������ʸ���������ˤ���
				�ȤäƤ����Ȥ��� */
  int            curbun;     /* ������ʸ�� */
  int		 curIkouho;  /* �����ȸ��� */
  int            nbunsetsu;  /* ʸ��ο� */

/* ifdef MEASURE_TIME */
  long		 proctime;   /* ��������(�Ѵ��Ƿ�¬����) */
  long		 rktime;     /* ��������(RK�ˤ��������) */
/* endif MEASURE_TIME */
/* end of from henkanContext */

/* �༡����ƥ����Ȥ��� */
  int		 ye, ys, status;
/* �༡����ƥ����Ȥ���(�����ޤ�) */
  int		 cStartp, cRStartp; /* �༡���ɤߤȤ��ƻĤäƤ�����ʬ */

/* ���拾��ƥ����Ȥ��� */
  BYTE           inhibition;
  BYTE           jishu_kc, jishu_case;
  int            jishu_kEndp, jishu_rEndp;
  short          rmark;
/* ���拾��ƥ����Ȥ���(�����ޤ�) */

/* adjustContext ���� */
  int kanjilen, bunlen;           /* ������ʬ��ʸ���Ĺ�� */
/* adjustContext ����(�����ޤ�) */
  struct _kanjiMode *tanMode; /* ñ����ΤȤ��Υ⡼�� */
  int tanMinorMode;     /*        ��            */

  /* ������ѿ� */
  int		  last_rule;		/* ����Υ����Ѵ��˻Ȥ�줿�롼�� */
  cannawc	  *retbuf, *retbufp;
  int		  retbufsize;
  short           pmark, cmark; /* �������Υޡ����Ⱥ��Υޡ��� */
  BYTE            englishtype;  /* �Ѹ쥿����(�ʲ��򸫤�) */
} yomiContextRec, *yomiContext;

/* for generalFlags */
#define CANNA_YOMI_MODE_SAVED		0x01L /* savedFlags �ˤ����Ȥ��ʤ� */

#define CANNA_YOMI_BREAK_ROMAN		0x01L
#define CANNA_YOMI_CHIKUJI_MODE		0x02L
#define CANNA_YOMI_CHGMODE_INHIBITTED	0x04L
#define CANNA_YOMI_END_IF_KAKUTEI	0x08L
#define CANNA_YOMI_DELETE_DONT_QUIT	0x10L

#define CANNA_YOMI_IGNORE_USERSYMBOLS	0x20L
#define CANNA_YOMI_IGNORE_HENKANKEY	0x40L

#define CANNA_YOMI_BASE_CHIKUJI		0x80L /* �����༡ */

/* for generalFlags also used in savedFlags */

/* �ʲ��� ATTRFUNCS �˥ޥ��������ӥåȤ� defmode ��°���Ȥ��ƻȤ��� */
#define CANNA_YOMI_KAKUTEI		0x0100L
#define CANNA_YOMI_HENKAN		0x0200L
#define CANNA_YOMI_ZENKAKU		0x0400L
#define CANNA_YOMI_HANKAKU		0x0800L /* �ºݤ�Ⱦ�� */
#define CANNA_YOMI_HIRAGANA		0x1000L
#define CANNA_YOMI_KATAKANA		0x2000L
#define CANNA_YOMI_ROMAJI		0x4000L
#define CANNA_YOMI_JISHUFUNCS		0x7c00L
#define CANNA_YOMI_ATTRFUNCS		0x7f00L

#define CANNA_YOMI_BASE_HANKAKU		0x8000L /* ����Ⱦ�� */

/* kind of allowed input keys */
#define CANNA_YOMI_INHIBIT_NONE		0
#define CANNA_YOMI_INHIBIT_HENKAN	1
#define CANNA_YOMI_INHIBIT_JISHU	2
#define CANNA_YOMI_INHIBIT_ASHEX	4
#define CANNA_YOMI_INHIBIT_ASBUSHU	8
#define CANNA_YOMI_INHIBIT_ALL		15

/* ��������Τ���Υե饰 */
#define NUMBERING 			1
#define CHARINSERT			2

#define CANNA_JISHU_UPPER		1
#define CANNA_JISHU_LOWER		2
#define CANNA_JISHU_CAPITALIZE		3
#define CANNA_JISHU_MAX_CASE		4

/* englishtype */
#define CANNA_ENG_KANA			0 /* �̾����뤳�� */
#define CANNA_ENG_ENG1			1
#define CANNA_ENG_ENG2			2 /* ξü�˶������äƤ��� */
#define CANNA_ENG_NO			3

/* yc->status �Υե饰(�༡��) */

#define	CHIKUJI_ON_BUNSETSU		0x0001 /* ʸ���ˤ��� */
#define	CHIKUJI_OVERWRAP		0x0002 /* ʸ�ᤫ���ɤ߾��֡� */
#define	CHIKUJI_NULL_STATUS	        0 /* ��Τ�ä��� */

/* yc ��Ȥ��⡼�ɤζ���(ͥ���) */

#define adjustp(yc) (0< (yc)->bunlen)
#define jishup(yc) (0 < (yc)->jishu_kEndp)
#define chikujip(yc) ((yc)->generalFlags & CANNA_YOMI_CHIKUJI_MODE)
#define henkanp(yc) (0 < (yc)->nbunsetsu)

#define chikujiyomiremain(yc) ((yc)->cStartp < (yc)->kEndp)

typedef struct _ichiranContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  int            svIkouho;   /* �����ȸ�������ȤäƤ���(����ɽ����) */
  int            *curIkouho; /* �����ȸ��� */
  int            nIkouho;    /* ����ο�(����ɽ����) */
  int		 tooSmall;   /* �������������Ƹ���������Ф��ʤ���ե饰 */
  int            curIchar;   /* ̤����ʸ���󤢤��ñ����Ͽ��ñ�����Ϥ�
    							��Ƭʸ���ΰ��� */
  BYTE           inhibit;
  BYTE           flags;	     /* ���򸫤Ƥ� */
  cannawc        **allkouho; /* RkGetKanjiList��������ʸ���������ˤ���
				�ȤäƤ����Ȥ��� */
  cannawc        *glinebufp; /* ��������Τ����Ԥ�ɽ�����뤿���ʸ��
				��ؤΥݥ��� */
  kouhoinfo      *kouhoifp;  /* ��������ط��ξ�����Ǽ���Ƥ�����¤��
				�ؤΥݥ��� */
  glineinfo      *glineifp;  /* ��������ط��ξ�����Ǽ���Ƥ�����¤��
				�ؤΥݥ��� */
} ichiranContextRec, *ichiranContext;

/* �ե饰�ΰ�̣ */
#define ICHIRAN_ALLOW_CALLBACK 1 /* ������Хå��򤷤Ƥ��ɤ� */
#define ICHIRAN_STAY_LONG    0x02 /* ���֤�ȴ���� */
#define ICHIRAN_NEXT_EXIT    0x04 /* ���� quit ��ȴ���� */


typedef struct _foirchiranContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  int            curIkouho;  /* �����ȸ��� */
  cannawc        **allkouho; /* RkGetKanjiList��������ʸ���������ˤ���
				�ȤäƤ����Ȥ��� */
  menustruct     *table;  /* ʸ����ȴؿ��Υơ��֥� */
  int            *prevcurp;  /* ���Υ����ȸ��� */
} forichiranContextRec, *forichiranContext;

typedef struct _mountContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  BYTE            *mountOldStatus; /* �ޥ���Ȥ���Ƥ��뤫���ʤ��� */
  BYTE            *mountNewStatus; /* �ޥ���Ȥ���Ƥ��뤫���ʤ��� */
  char            **mountList;   /* �ޥ���Ȳ�ǽ�ʼ���ΰ��� */
  int            curIkouho;     /* �����ȸ��� */
} mountContextRec, *mountContext;

typedef struct _tourokuContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  cannawc        genbuf[ROMEBUFSIZE];
  cannawc        qbuf[ROMEBUFSIZE];
  cannawc        tango_buffer[ROMEBUFSIZE];
  int            tango_len;  /* ñ����Ͽ��ñ���ʸ�����Ĺ�� */
  cannawc        yomi_buffer[ROMEBUFSIZE];
  int            yomi_len;   /* ñ����Ͽ���ɤߤ�ʸ�����Ĺ�� */
  int            curHinshi;  /* �ʻ������ */
  int            workDic;    /* ����Ѥμ��� */
  deldicinfo     *workDic2;  /* ñ������ǽ�ʼ��� */
  int            nworkDic2;  /* ñ������ǽ�ʼ���ο� */
  deldicinfo     *workDic3;  /* ñ�������뼭�� */
  int            nworkDic3;  /* ñ�������뼭��ο� */
  struct dicname *newDic;    /* �ɲä��뼭�� */
  cannawc        hcode[INDPHLENGTH];  /* ñ����Ͽ���ʻ� */
  int            katsuyou;   /* ñ����Ͽ��ư��γ��ѷ� */
  cannawc        **udic;     /* ñ����Ͽ�Ǥ��뼭�� (����̾) */
  int            nudic;      /* ñ����Ͽ�Ǥ��뼭��ο� */
  int            delContext; /* ñ�����ǣ��Ĥμ����ޥ���Ȥ��� */
} tourokuContextRec, *tourokuContext;

typedef struct _tanContextRec {
  BYTE id;
  BYTE majorMode, minorMode;
  struct _kanjiMode *prevMode;	/* �������Υ⡼�� */
  mode_context    next;

  struct _kanjiMode *curMode;
  struct _tanContextRec	 *left, *right;

  struct RkRxDic *romdic;	/* ���޻������Ѵ��ơ��֥� */
  BYTE            myMinorMode;  /* yomiContext ��ͭ�Υޥ��ʥ⡼�� */
  struct _kanjiMode *myEmptyMode;		/* empty �⡼�ɤϤɤ줫 */
  long generalFlags, savedFlags; /* yomiContext �Υ��ԡ� */
  BYTE		  savedMinorMode;	/* �ޥ��ʥ⡼�ɤΥ����� */
  BYTE		  allowedChars;		/* see jrkanji.h */
  BYTE		  henkanInhibition;	/* see below */

  cannawc *kanji, *yomi, *roma;
  BYTE *kAttr, *rAttr;
} tanContextRec, *tanContext;

struct moreTodo {
  BYTE          todo; /* ��äȤ���Ρ��򼨤� */
  BYTE          fnum; /* �ؿ��ֹ档���ʤ鼡��ʸ���Ǽ�����뤳�Ȥ򤹤� */
  int		ch;   /* ʸ�� */
};

/* �⡼��̾���Ǽ����ǡ����η���� */

struct ModeNameRecs {
  int           alloc;
  cannawc       *name;
};

/* �������ֹ�Υ��ѥ졼�����Υǥե���Ȥ���� */

#define DEFAULTINDEXSEPARATOR     '.'

/*
   wcKanjiAttribute for internal use
 */

typedef struct {
  wcKanjiAttribute u;
  int len;
  char *sp, *ep;
} wcKanjiAttributeInternal;

/*

  uiContext �ϥ��޻������Ѵ������ʴ����Ѵ��˻Ȥ��빽¤�ΤǤ��롣
  XLookupKanjiString �ʤɤˤ���Ѵ��ϡ�������ɥ���ʬΥ���줿ʣ������
  �ϥݡ��Ȥ��б����Ƥ���Τǡ�������Υ��޻��ξ���䡢���ʴ����Ѵ�
  ���ͻҤʤɤ򤽤줾��Υ�����ɥ����ʬΥ�����ݻ����Ƥ����ʤ���Ф�
  ��ʤ������ι�¤�ΤϤ��Τ���˻Ȥ��빽¤�ΤǤ��롣

  ��¤�ΤΥ��Ф��ɤΤ褦�ʤ�Τ����뤫�ϡ�����򻲾Ȥ��뤳��

 */

typedef struct _uiContext {

  /* XLookupKanjiString�Υѥ�᥿ */
  cannawc        *buffer_return;
  int            n_buffer;
  wcKanjiStatus    *kanji_status_return;

  /* XLookupKanjiString������ͤǤ���ʸ�����Ĺ�� */
  int		 nbytes;

  /* ����饯�� */
  int ch;

  /* ���ߥ����Х�ǡ��� */
  int		 contextCache;	 /* �Ѵ�����ƥ����ȥ���å��� */
  struct _kanjiMode *current_mode;
  BYTE		 majorMode, minorMode;	 /* ľ���Τ�� */

  short		 curkigo;	 /* �����ȵ���(��������) */
  char           currussia;	 /* �����ȵ���(����ʸ��) */
  char           curgreek;	 /* �����ȵ���(���ꥷ��ʸ��) */
  char           curkeisen;	 /* �����ȵ���(����) */
  short          curbushu;       /* ����������̾ */
  int            ncolumns;	 /* ��ԤΥ���������������λ����Ѥ����� */
  cannawc        genbuf[ROMEBUFSIZE];	/* ���ѥХåե� */
  short          strokecounter;  /* �������ȥ����Υ�����
				    ���޻��⡼�ɤǥ��ꥢ����� */
  wcKanjiAttributeInternal *attr;

  /* �ꥹ�ȥ�����Хå���Ϣ */
  char           *client_data;   /* ���ץꥱ��������ѥǡ��� */
  int            (*list_func) pro((char *, int, cannawc **, int, int *));
  jrEUCListCallbackStruct elistcb; /* EUC�ξ��μ���(��wc�����) */
                 /* �ꥹ�ȥ�����Хå��ؿ� */
  /* ����¾ */
  char		 flags;		 /* ���򸫤Ƥ� */
  char		 status;	 /* �ɤΤ褦�ʾ��֤��֤ä��Τ��򼨤���
				    ���Υ⡼�ɤȤ��ơ�
				     ��������
				     ��������λ
				     ����������
				     ������¾
				    �����롣(���򸫤�) */

  /* ������Хå��������� */
  struct callback *cb;

  /* ��äȤ��뤳�Ȥ������Ȥ�����¤�� */
  struct moreTodo more;

  /* �����åȥ������� */
  menustruct *prevMenu;

  /* �ƥ�˥塼�����Ф줿�ֹ��Ͽ���Ƥ�����¤�ΤؤΥݥ��� */
  menuinfo *minfo;

  /* �ư��������Ф줿�ֹ��Ͽ���Ƥ�����¤�ΤؤΥݥ��� */
  selectinfo *selinfo;

  /* ���֥���ƥ����ȤؤΥ�� */
  mode_context   modec;		/* ���������ˤĤʤ�ͽ�� */
} uiContextRec, *uiContext;

/* uiContext �� flags �ΥӥåȤΰ�̣ */
#define PLEASE_CLEAR_GLINE	1	/* GLine ��ä��Ƥ� */
#define PCG_RECOGNIZED		2	/* GLine �򼡤Ͼä��ޤ��� */
#define MULTI_SEQUENCE_EXECUTED	4	/* ���ä��ޥ���������󥹤��Ԥ�줿 */

#define EVERYTIME_CALLBACK	0
#define EXIT_CALLBACK		1
#define QUIT_CALLBACK		2
#define AUX_CALLBACK		3

/*
 * ���ʴ����Ѵ��Τ�����͡��ʥ����ޥåץơ��֥�
 * �����ޥåץơ��֥�Ͻ����ؿ��ؤΥݥ��󥿤�����ȤʤäƤ��롣
 */

struct funccfunc {
  BYTE funcid;
  int (*cfunc) pro((struct _uiContext *));
};

typedef struct _kanjiMode {
  int (*func) pro((struct _uiContext *, struct _kanjiMode *, int, int, int));
  BYTE *keytbl;
  int flags;			/* ���򸫤� */
  struct funccfunc *ftbl;
} *KanjiMode, KanjiModeRec;

struct callback {
  int (*func[NCALLBACK]) pro((struct _uiContext *, int, mode_context));
  mode_context    env;
  struct callback *next;
};

/* ���޻������Ѵ��ơ��֥� */

extern struct RkRxDic *romajidic;
extern struct RkRxDic *RkwOpenRoma pro((char *));

/*
 * �����̾��������Ƥ����ѿ�
 */

struct dicname {
  struct dicname *next;
  char *name;
  int dictype;
  unsigned long dicflag;
};

/* dictype �ˤϰʲ��Τ����줫������ */
#define DIC_PLAIN 0     /* �̾������ */
#define DIC_USER  1     /* ñ����Ͽ�Ѽ��� */
#define DIC_BUSHU 2     /* �����Ѵ��Ѽ��� */
#define DIC_GRAMMAR 3   /* ʸˡ���� */
#define DIC_RENGO 4     /* Ϣ��ؽ����� */
#define DIC_KATAKANA 5  /* �������ʳؽ����� */
#define DIC_HIRAGANA 6  /* �Ҥ餬�ʳؽ����� */

/* dicflag �ˤϰʲ��Τ����줫������ */
#define DIC_NOT_MOUNTED  0
#define DIC_MOUNTED      1
#define DIC_MOUNT_FAILED 2

extern struct dicname *kanjidicnames;

/*
 * ���顼�Υ�å�����������Ƥ����ѿ�
 */

extern char *jrKanjiError;

/*
 * �ǥХ�ʸ��ɽ�����뤫�ɤ����Υե饰
 */

extern int iroha_debug;

/*
 * �����������󥹤�ȯ������褦�ʥ���
 */

#define IrohaFunctionKey(key) \
  ((0x80 <= (int)(unsigned char)(key) &&  \
    (int)(unsigned char)(key) <= 0x8b) || \
   (0x90 <= (int)(unsigned char)(key) &&  \
    (int)(unsigned char)(key) <= 0x9b) || \
   (0xe0 <= (int)(unsigned char)(key) &&  \
    (int)(unsigned char)(key) <= 0xff) )

/* selectOne �ǥ�����Хå���ȼ�����ɤ�����ɽ���ޥ��� */

#define WITHOUT_LIST_CALLBACK 0
#define WITH_LIST_CALLBACK    1

/*
 * Rk �ؿ���ȥ졼�����뤿���̾���ν񤭴�����
 */

#ifdef DEBUG
#include "traceRK.h"
#endif /* DEBUG */

/*
 * �ǥХ���å����������ѤΥޥ���
 */

#ifdef DEBUG
#define debug_message(fmt, x, y, z)	dbg_msg(fmt, x, y, z)
#else /* !DEBUG */
#define debug_message(fmt, x, y, z)
#endif /* !DEBUG */

/*
 * malloc �ΥǥХ�
 */

#ifdef DEBUG_ALLOC
extern char *debug_malloc pro((int));
extern int fail_malloc;
#define malloc(n) debug_malloc(n)
#endif /* DEBUG_MALLOC */

/*
 * �������⡼�ɤ�������빽¤��
 */

typedef struct {
  char           *romaji_table; /* ���޻������Ѵ��ơ��֥�̾(EUC) */
  struct RkRxDic *romdic;	 /* ���޻�����¤�� */
  int             romdic_owner;  /* ���޻������ʬ��Open������ */
  long            flags;	 /* flags for yomiContext->generalFlags */
  KanjiMode       emode;	 /* current_mode �����빽¤�� */
} newmode;

/* ���޻������Ѵ�����­���륭����ʸ�����Ѵ��ơ��֥� */

typedef struct {
  cannawc	key;		/* ���� */
  cannawc       xkey;
  int		groupid;	/* ���롼��id */
  int           ncand;          /* ����ο� */
  cannawc       **cand;         /* ��������� */
  cannawc	*fullword;	/* ������ (����1@����2@...����n@@) */
#ifdef WIN_CANLISP
  int		fullwordsize;	/* sizeof fullword by cannawc unit */
#endif
} keySupplement;

#define MAX_KEY_SUP 64

#define HEX_USUAL     0
#define HEX_IMMEDIATE 1

#define ModeInfoStyleIsString		0
#define ModeInfoStyleIsNumeric		1
#define ModeInfoStyleIsExtendedNumeric	2
#define ModeInfoStyleIsBaseNumeric      3
#define MaxModeInfoStyle                ModeInfoStyleIsBaseNumeric

#define killmenu(d) ((d)->prevMenu = (menustruct *)0)
#define	defineEnd(d) killmenu(d)
#define	deleteEnd(d) killmenu(d)

/* defmode��defselection��defmenu �Ѥι�¤�� */

typedef struct _extra_func {
  int  		fnum;		/* �ؿ��ֹ� */
  int		keyword;	/* �������⡼�ɤ�������줿������� */
  cannawc	*display_name;	/* �⡼��ɽ��̾ */
  union {
    newmode 	*modeptr;	/* defmode ���б����빽¤�� */
    kigoIchiran	*kigoptr;	/* defselection ���б����빽¤�� */
    menustruct	*menuptr;	/* defmenu ���б����빽¤�� */
  } u;
#ifdef BINARY_CUSTOM
  int           mid;
  char          *symname;
#endif
  struct _extra_func *next;
} extraFunc;

#define EXTRA_FUNC_DEFMODE	1
#define EXTRA_FUNC_DEFSELECTION	2
#define EXTRA_FUNC_DEFMENU	3

#define tanbunMode(d, tan) /* tanContext ��Ϣ�⡼�ɤؤΰܹ� */ \
  { extern KanjiModeRec tankouho_mode; (d)->current_mode = &tankouho_mode; \
    (d)->modec = (mode_context)(tan); currentModeInfo(d); }

#define freeForIchiranContext(fc) free((char *)fc)
#define freeIchiranContext(ic) free((char *)ic)
#define freeYomiContext(yc) free((char *)yc)
#define freeCoreContext(cc) free((char *)cc)

#ifndef DICHOME
#define DICHOME "/usr/lib/canna/dic"
#endif

#define DEFAULT_CANNA_SERVER_NAME "cannaserver"

#ifndef	_UTIL_FUNCTIONS_DEF_

#define	_UTIL_FUNCTIONS_DEF_

/* ����ʤΥС�������Ĵ�٤� */
#define canna_version(majv, minv) ((majv) * 1024 + (minv))

/* �褯���ڥ�ߥ�����Τǥ���ѥ�����ˤҤä�����褦������� */
extern int RkwGoto pro((char *, int));

/* storing customize configuration to the following structure. */
struct CannaConfig { /* �ʲ��Υ����Ȥϥ��������ʤɤ˵��Ҥ���Ȥ��ʤɤ�
			�Ѥ�����á�! ����Ƭ�ˤĤ��Ƥ���Τϥ��å���ȿž
			���Ƥ��뤳�Ȥ�ɽ�� */
  int CannaVersion;  /* (�ߴ���) ����ʤΥС������ */
  int kouho_threshold; /* �Ѵ������򲿲��ǤĤȰ������Ф뤫 */
  int strokelimit;  /* (�ߴ���) �����ȥ�������ե��٥åȤ����������Ǥ� */
  int indexSeparator; /* (�ߴ���) �������Υ���ǥå����Υ��ѥ졼�� */
  BYTE ReverseWidely; /* ȿž�ΰ�򹭤��Ȥ�       */
  BYTE chikuji;       /* �༡��ư�Ѵ�             */
  BYTE Gakushu;       /* �ؽ����뤫�ɤ���         */
  BYTE CursorWrap;    /* ��ü�Ǳ��Ǻ�ü�عԤ�     */
  BYTE SelectDirect;  /* ������������ǰ�����ȴ���� */
  BYTE HexkeySelect;  /* (�ߴ���) 16�ʿ����Ǥ��������� */
  BYTE BunsetsuKugiri; /* �Ѵ���ʸ��֤˶��������  */
  BYTE ChBasedMove;   /* !���޻������Ѵ�ñ�̤Υ��������ư   */
  BYTE ReverseWord;   /* (�ߴ���) �����Ǹ��ȿž���� */
  BYTE QuitIchiranIfEnd; /* ���������ǰ������Ĥ��� */
  BYTE kakuteiIfEndOfBunsetsu; /* ʸ�������Ǳ���ư�ǳ��ꤹ�� */
  BYTE stayAfterValidate; /* !����������弡��ʸ��ذ�ư */
  BYTE BreakIntoRoman;    /* BS�����ǥ��޻����᤹ */
  BYTE grammaticalQuestion; /* (�ߴ���) ñ����Ͽ��ʸˡŪ����򤹤� */
  BYTE forceKana;           /* Isn't this used? */
  BYTE kCount;        /* (�ߴ���) ���䤬�����ܤ���ɽ������ */
  BYTE LearnNumericalType;  /* Isn't this used? */
  BYTE BackspaceBehavesAsQuit; /* �༡��ư�Ѵ��� BS ���������Τ��ɤߤ��᤹ */
  BYTE iListCB;       /* (�ߴ���) �ꥹ�ȥ�����Хå���ػߤ��� */
  BYTE keepCursorPosition;  /* !�Ѵ�����BS�Ǹ�������������֤������˰�ư */
  BYTE abandonIllegalPhono; /* ���޻������Ѵ��˻Ȥ��ʤ�������ΤƤ� */
  BYTE hexCharacterDefiningStyle; /* Isn't this used? */
  BYTE kojin;         /* �Ŀͳؽ� */
  BYTE indexHankaku;  /* (�ߴ���) �������Υ���ǥå�����Ⱦ�Ѥˤ��� */
  BYTE allowNextInput; /* �������ɽ�������������Ϥ���ǽ�ˤ��� */
  BYTE doKatakanaGakushu; /* Isn't this used? */
  BYTE doHiraganaGakushu; /* Isn't this used? */
  BYTE ChikujiContinue; /* �༡��ư�Ѵ����������ϤǴ��Ѵ���ʬ����ꤷ�ʤ� */
  BYTE RenbunContinue;  /* Ϣʸ���Ѵ����������ϤǴ��Ѵ���ʬ����ꤷ�ʤ� */
  BYTE MojishuContinue; /* �����Ѵ����������ϤǴ��Ѵ���ʬ����ꤷ�ʤ� */
  BYTE chikujiRealBackspace; /* �༡��ư�Ѵ���BS��ɬ����ʸ���õ�� */
  BYTE ignore_case;   /* ��ʸ����ʸ������̤��ʤ� */
  BYTE romaji_yuusen; /* ���޻������Ѵ���ͥ�褹�� */
  BYTE auto_sync;     /* ���Ū�˼������᤹ */
  BYTE quickly_escape; /* (�ߴ���) ����ɽ�����������¨�¤˰�����ȴ���� */
  BYTE InhibitHankakuKana; /* Ⱦ�ѥ������ʤζػ� */
  BYTE code_input;    /* ������(0: jis, 1: sjis, 2: ����) */
  BYTE DelayConnect;  /* ��������ˤ����˥����Ф���³���ʤ� */
};

#define CANNA_CODE_JIS   0
#define CANNA_CODE_SJIS  1
#define CANNA_CODE_KUTEN 2
#define CANNA_MAX_CODE   3

extern struct CannaConfig cannaconf;
extern void InitCannaConfig pro((struct CannaConfig *));

typedef int (* canna_callback_t) pro((uiContext, int, mode_context));

extern void makeGLineMessage pro((uiContext, cannawc *, int));
extern void makeGLineMessageFromStrings pro((uiContext, char *));
extern newmode *findExtraKanjiMode pro((int));
extern int setWStrings pro((cannawc **, char **, int));

extern size_t WStrlen pro((const cannawc* s));

extern cannawc* WStrcat pro((cannawc* dest, const cannawc* src));

extern cannawc* WStrcpy pro((cannawc* dest, const cannawc* src));

extern cannawc* WStrncpy pro((cannawc* dest, const cannawc* src, int n));

extern int WStrncmp pro((const cannawc* s1, const cannawc* s2, int n));

extern cannawc *WString pro((char *));
#ifndef NO_EXTEND_MENU
extern int prevMenuIfExist pro((uiContext));
extern int showmenu pro((uiContext, menustruct *));
#endif
extern yomiContext
newYomiContext pro((cannawc *, int, int, int, int, int));

yomiContext GetKanjiString pro((uiContext, cannawc *, int, int, int, int, int,
		      canna_callback_t, canna_callback_t, canna_callback_t));
extern void restoreFlags pro((yomiContext));
extern void kPos2rPos pro((yomiContext, int, int, int *, int *));
extern void makeKanjiStatusReturn pro((uiContext, yomiContext));
extern cannawc key2wchar pro((int, int *));
extern struct bukRec *internContext
  pro((unsigned int, unsigned int, uiContext));
extern void freeRomeStruct pro((uiContext));
extern void rmContext pro((unsigned int, unsigned int));
extern struct callback *pushCallback
  pro((uiContext, mode_context,
       canna_callback_t, canna_callback_t,
       canna_callback_t, canna_callback_t));
extern void popCallback pro((uiContext));
extern void makeYomiReturnStruct pro((uiContext));
extern void moveToChikujiTanMode pro((uiContext));
extern void moveToChikujiYomiMode pro((uiContext));
extern void makeGLineMessageFromString pro((uiContext, char *));
extern void addWarningMesg pro((char *));
extern int prepareHenkanMode pro((uiContext));
extern void makeAllContextToBeClosed pro((int));
extern void CannaBeep pro((void));
#ifndef NO_EXTEND_MENU
extern void freeAllMenuInfo pro((menuinfo *));
extern void freeMenu pro((menustruct *));
#endif
extern void restoreDefaultKeymaps pro((void));
#ifndef NO_EXTEND_MENU
extern void finExtMenu pro((void));
#endif
extern void freeIchiranBuf pro((ichiranContext));
extern exp(char *) RkwGetServerName pro((void));
extern void popForIchiranMode pro((uiContext));
extern void clisp_main pro((void));
extern void clisp_fin pro((void));
extern void popYomiMode pro((uiContext));
extern void freeTanContext pro((tanContext));
extern void enterJishuMode pro((uiContext, yomiContext));
extern void leaveJishuMode pro((uiContext, yomiContext));
extern void finishTanKakutei pro((uiContext));
extern void removeKana pro((uiContext, yomiContext, int, int));
extern void clearHenkanContext pro((yomiContext));
extern void doMuhenkan pro((uiContext, yomiContext));
extern void removeCurrentBunsetsu pro((uiContext, tanContext));
extern int uiUtilIchiranTooSmall pro((uiContext, int, mode_context));
extern int dicTourokuHinshiDelivery pro((uiContext));
extern int uuTTangoQuitCatch pro((uiContext, int, mode_context));
extern void freeAndPopTouroku pro((uiContext));
extern void popMountMode pro((uiContext));
extern void freeDic pro((tourokuContext));

/* for VC++ 4.0, by kon */
extern int doFunc pro((uiContext, int));
extern int _doFunc pro((uiContext, int));
extern int _afterDoFunc pro((uiContext, int));
extern int alphaMode pro((uiContext));
extern void jrKanjiPipeError pro((void));
extern int NoMoreMemory pro((void));
extern int WCinit pro((void));
extern int WStringOpen pro((void));
extern void WStringClose pro((void));
extern int WStrcmp pro((cannawc *, cannawc *));
extern int WSfree pro((cannawc *));
extern void initModeNames pro((void));
extern int initKeyTables pro((void));
#ifdef BINARY_CUSTOM
extern int binparse pro((void));
#else
extern void parse pro((void));
#endif
extern int initIchiran pro((void));
extern int RomkanaInit pro((void));
extern void RomkanaFin pro((void));
extern int KanjiInit pro((void));
extern int KanjiFin pro((void));
extern void resetModeNames pro((void));
extern int escapeToBasicStat pro((uiContext, int));
extern void EmptyBaseModeInfo pro((uiContext, yomiContext));
extern void GlineClear pro((uiContext));
extern void currentModeInfo pro((uiContext));
extern int queryMode pro((uiContext, cannawc *));
extern int RkSetServerName pro((char *));
extern int parse_string pro((char *));
extern void RomajiStoreYomi pro((uiContext, cannawc *, cannawc *));
extern int WWhatGPlain pro((cannawc));
extern int WIsG0 pro((cannawc)), WIsG1 pro((cannawc));
extern int WIsG2 pro((cannawc)), WIsG3 pro((cannawc));
extern int XwcKanjiControl2
  pro((unsigned int, unsigned int, unsigned int, BYTE *));
extern int XKanjiControl2
  pro((unsigned int, unsigned int, unsigned int, BYTE *));
extern int XwcLookupKanji2 pro((unsigned int, unsigned int, cannawc *, int,
                                int, int, wcKanjiStatus *));
extern int NothingChangedWithBeep pro((uiContext));
extern int searchfunc pro((uiContext, KanjiMode, int, int, int));
extern int initRomeStruct pro((uiContext, int));
/* extern int kanjiControl pro((int, uiContext, caddr_t)); */
extern int getBaseMode pro((yomiContext));
extern int RkwMapPhonogram
  pro((struct RkRxDic *, cannawc *, int, const cannawc *, int, cannawc, int,
       int *, int *, int *, int *));
extern int RkMapPhonogram
  pro((struct RkRxDic *, unsigned char *, int, unsigned char *, int,
       unsigned, int,
       int *, int *, int *, int *));
extern int RkMapRoma
  pro((struct RkRxDic *, unsigned char *, int, unsigned char *, int,
       int, int *));
extern int RkCvtRoma
  pro((struct RkRxDic *, unsigned char *, int, unsigned char *, int,
       unsigned));
extern int RkwCvtRoma
  pro((struct RkRxDic *, cannawc *, int, const cannawc *, int, int));
extern int exitJishu pro((uiContext));
extern int RomajiFlushYomi pro((uiContext, cannawc *, int));
extern void generalReplace
  pro((cannawc *, BYTE *, int *, int *, int *, int, cannawc *, int, int));
extern int ChikujiSubstYomi pro((uiContext));
extern int TanMuhenkan pro((uiContext));
extern size_t CANNA_mbstowcs pro((cannawc* dest, const char* src, size_t n));
extern size_t CANNA_wcstombs pro((char* dest, const cannawc* src, size_t n));
extern int makeRkError pro((uiContext, char *));
extern void moveStrings pro((cannawc *, BYTE *, int, int, int));
extern int TanBackwardBunsetsu pro((uiContext));
extern int TbBackward pro((uiContext));
extern int NothingChanged pro((uiContext));
extern int TbEndOfLine pro((uiContext));
extern int TanForwardBunsetsu pro((uiContext));
extern int TbForward pro((uiContext));
extern int TbBeginningOfLine pro((uiContext));
extern int tanMuhenkan pro((uiContext, int));
extern int extractTanString pro((tanContext, cannawc *, cannawc *));
extern int extractTanYomi pro((tanContext, cannawc *, cannawc *));
extern int extractTanRomaji pro((tanContext, cannawc *, cannawc *));
extern int leaveAdjustMode pro((uiContext, yomiContext));
extern cannawc *WStraddbcpy pro((cannawc *, cannawc *, int));
extern int YomiExit pro((uiContext, int));
extern void clearYomiContext pro((yomiContext));
extern int abandonContext pro((uiContext, yomiContext));
extern void RomajiClearYomi pro((uiContext));

// Convert Shift-JIS to EUC-JP.
extern int RkCvtEuc pro((unsigned char *, int, const unsigned char *, int));

extern int RkCvtNone pro((unsigned char *, int, const unsigned char *, int));
extern int selectOne
  pro((uiContext, cannawc **, int *, int, int, unsigned, int, int,
       canna_callback_t, canna_callback_t, canna_callback_t,canna_callback_t));
extern void makeGlineStatus pro((uiContext));
extern int YomiMark pro((uiContext));
extern int doHenkan pro((uiContext, int, cannawc *));
extern int tanKouhoIchiran pro((uiContext, int));
extern int EmptyBaseKana pro((uiContext));
extern int EmptyBaseKakutei pro((uiContext));
extern int EmptyBaseHenkan pro((uiContext));
extern int WToupper pro((cannawc));
extern int WTolower pro((cannawc));
extern int TanKakutei pro((uiContext));
extern int Yomisearchfunc pro((uiContext, KanjiMode, int, int, int));

// cannawc -> EUC-JP
extern int CNvW2E pro((const cannawc* src, int srclen, unsigned char* dest, size_t));
extern void initKigoTable pro((void));
extern int NothingForGLine pro((uiContext));
extern int NothingForGLineWithBeep pro((uiContext));
extern void echostrClear pro((uiContext));
extern void freeGetIchiranList pro((cannawc **));
extern int TanKouhoIchiran pro((uiContext));
extern int GLineNGReturn pro((uiContext));
extern int GLineNGReturnFI pro((uiContext));
extern int appendTan2Yomi pro((tanContext, yomiContext));
extern int confirmContext pro((uiContext, yomiContext));
extern int ChikujiTanDeletePrevious pro((uiContext));
extern int YomiKakutei pro((uiContext));
extern int YomiInsert pro((uiContext));
extern int cutOffLeftSide pro((uiContext, yomiContext, int));
extern int enterAdjustMode pro((uiContext, yomiContext));
extern int TanNextKouho pro((uiContext));
extern int TanPreviousKouho pro((uiContext));
extern int TanBeginningOfBunsetsu pro((uiContext));
extern int TanEndOfBunsetsu pro((uiContext));
extern int TanDeletePrevious pro((uiContext));
extern int TanBubunMuhenkan pro((uiContext));
extern int xString pro((cannawc *, int, cannawc *, cannawc *));
extern int KanaDeletePrevious pro((uiContext));
extern int findSup pro((cannawc));
extern int selectKeysup pro((uiContext, yomiContext, int));
extern int containUnconvertedKey pro((yomiContext));
extern int CYsearchfunc pro((uiContext, KanjiMode, int, int, int));
extern int TanBubunKakutei pro((uiContext));
extern int TanZenkaku pro((uiContext));
extern int TanHankaku pro((uiContext));
extern int TanHiragana pro((uiContext));
extern int TanKatakana pro((uiContext));
extern int TanRomaji pro((uiContext));
extern int TanUpper pro((uiContext));
extern int TanCapitalize pro((uiContext));
extern int TanPrintBunpou pro((uiContext));
extern int TanNop pro((uiContext));
extern int YomiQuotedInsert pro((uiContext));
extern int AlphaMode pro((uiContext));
extern int HenkanNyuryokuMode pro((uiContext));
extern int KigoIchiran pro((uiContext));
extern int UiUtilMode pro((uiContext));
extern int EmptyBaseHira pro((uiContext));
extern int EmptyBaseKata pro((uiContext));
extern int EmptyBaseEisu pro((uiContext));
extern int EmptyBaseZen pro((uiContext));
extern int EmptyBaseHan pro((uiContext));
extern int YomiBaseHiraKataToggle pro((uiContext));
extern int YomiBaseZenHanToggle pro((uiContext));
extern int YomiBaseKanaEisuToggle pro((uiContext));
extern int YomiBaseKakuteiHenkanToggle pro((uiContext));
extern int YomiBaseRotateForw pro((uiContext));
extern int YomiBaseRotateBack pro((uiContext));
extern int DoFuncSequence pro((uiContext));
extern int YomiModeBackup pro((uiContext));
extern int UseOtherKeymap pro((uiContext));
extern int JapaneseMode pro((uiContext));
extern void ichiranFin pro((uiContext));
extern int IchiranForwardKouho pro((uiContext));
extern int IchiranBackwardKouho pro((uiContext));
extern int IchiranNextKouhoretsu pro((uiContext));
extern int IchiranPreviousKouhoretsu pro((uiContext));
extern int IchiranBeginningOfKouho pro((uiContext));
extern int IchiranEndOfKouho pro((uiContext));
extern int IchiranQuit pro((uiContext));
extern int IchiranNop pro((uiContext));
extern int RkwCvtNone pro((cannawc *, int, const cannawc *, int));
extern int RkwSetAppName pro((int, char *));
extern void close_engine pro((void));
extern char *KanjiInitError pro((void));
extern void prepare_autodic pro((void));
extern int doKakutei
 pro((uiContext, tanContext, tanContext, cannawc *, cannawc *, yomiContext *));
extern int EUCListCallback pro((char *, int, cannawc **, int, int *));
#if SUPPORT_OLD_WCHAR
extern int owcListCallback pro((char *, int, cannawc **, int, int *));
#endif

#endif /* _UTIL_FUNCTIONS_DEF_ */


#endif /* !_CANNA_H_ */
