/* vax630_defs.h: MicroVAX II model-specific definitions file

   Copyright (c) 2009-2012, Matt Burke
   This module incorporates code from SimH, Copyright (c) 1998-2008, Robert M Supnik

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
   THE AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
   IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

   Except as contained in this notice, the name(s) of the author(s) shall not be
   used in advertising or otherwise to promote the sale, use or other dealings
   in this Software without prior written authorization from the author(s).

   08-Nov-12    MB      First version

   This file covers the KA630 ("Mayflower") Qbus system.

   System memory map

        0000 0000 - 00FF FFFF           main memory
        0100 0000 - 1FFF FFFF           reserved

        2000 0000 - 2000 1FFF           Qbus I/O page
        2004 0000 - 2004 FFFF           ROM space, halt protected
        2005 0000 - 2005 FFFF           ROM space, halt unprotected
        2008 0000 - 2008 000F           Local register space
        2008 8000 - 2008 FFFF           Qbus mapping registers
        200B 8000 - 200B 80FF           Watch chip registers
        3000 0000 - 303F FFFF           Qbus memory space
        3400 0000 - 3FFF FFFF           reserved
*/

#ifdef FULL_VAX                                         /* subset VAX */
#undef FULL_VAX
#endif

#ifndef _VAX_630_DEFS_H_
#define _VAX_630_DEFS_H_ 1

/* Microcode constructs */

#define VAX620_SID      (16 << 24)                      /* system ID */
#define VAX630_SID      (8 << 24)                       /* system ID */
#define CON_HLTPIN      0x0200                          /* external CPU halt */
#define CON_PWRUP       0x0300                          /* powerup code */
#define CON_HLTINS      0x0600                          /* HALT instruction */
#define CON_DBLMCK      0x0500                          /* Machine check in machine check */
#define CON_BADPSL      0x4000                          /* invalid PSL flag */
#define CON_MAPON       0x8000                          /* mapping on flag */
#define MCHK_TBM_P0     0x05                            /* PPTE in P0 */
#define MCHK_TBM_P1     0x06                            /* PPTE in P1 */
#define MCHK_M0_P0      0x07                            /* PPTE in P0 */
#define MCHK_M0_P1      0x08                            /* PPTE in P1 */
#define MCHK_INTIPL     0x09                            /* invalid ireq */
#define MCHK_READ       0x80                            /* read check */
#define MCHK_WRITE      0x82                            /* write check */

/* Machine specific IPRs */

#define MT_TBDR         36                              /* Translation Buffer Disable */
#define MT_CADR         37                              /* Cache Disable Register */
#define MT_MCESR        38                              /* Machine Check Error Summary */
#define MT_CAER         39                              /* Cache Error Register */
#define MT_CONISP       41                              /* Console Saved ISP */
#define MT_CONPC        42                              /* Console Saved PC */
#define MT_CONPSL       43                              /* Console Saved PSL */
#define MT_SBIFS        48                              /* SBI fault status */
#define MT_SBIS         49                              /* SBI silo */
#define MT_SBISC        50                              /* SBI silo comparator */
#define MT_SBIMT        51                              /* SBI maint */
#define MT_SBIER        52                              /* SBI error */
#define MT_SBITA        53                              /* SBI timeout addr */
#define MT_SBIQC        54                              /* SBI timeout clear */
#define MT_IORESET      55                              /* I/O Bus Reset */
#define MT_TBDATA       59                              /* Translation Buffer Data */
#define MT_MBRK         60                              /* microbreak */

/* CPU */

#define CPU_MODEL_MODIFIERS \
                        { MTAB_XTD|MTAB_VDV, 0, "MODEL", NULL, \
                          NULL, &cpu_show_model },
/* Memory */

#define MAXMEMWIDTH     24                              /* max mem, std KA655 */
#define MAXMEMSIZE      (1 << MAXMEMWIDTH)              /* max mem size */
#define MAXMEMWIDTH_X   24                              /* max mem, KA655X */
#define MAXMEMSIZE_X    (1 << MAXMEMWIDTH_X)
#define INITMEMSIZE     (1 << 24)                       /* initial memory size */
#define MEMSIZE         (cpu_unit.capac)
#define ADDR_IS_MEM(x)  (((uint32) (x)) < MEMSIZE)
#define MEM_MODIFIERS   { UNIT_MSIZE, (1u << 20), NULL, "1M", &cpu_set_size }, \
                        { UNIT_MSIZE, (1u << 21), NULL, "2M", &cpu_set_size }, \
                        { UNIT_MSIZE, (1u << 22), NULL, "4M", &cpu_set_size }, \
                        { UNIT_MSIZE, (1u << 23), NULL, "8M", &cpu_set_size }, \
                        { UNIT_MSIZE, (1u << 23) + (1u << 22), NULL, "12M", &cpu_set_size }, \
                        { UNIT_MSIZE, (1u << 24), NULL, "16M", &cpu_set_size }

/* Qbus I/O page */

#define IOPAGEAWIDTH    13                              /* IO addr width */
#define IOPAGESIZE      (1u << IOPAGEAWIDTH)            /* IO page length */
#define IOPAGEMASK      (IOPAGESIZE - 1)                /* IO addr mask */
#define IOPAGEBASE      0x20000000                      /* IO page base */
#define ADDR_IS_IO(x)   ((((uint32) (x)) >= IOPAGEBASE) && \
                        (((uint32) (x)) < (IOPAGEBASE + IOPAGESIZE)))

/* Read only memory - appears twice */

#define ROMAWIDTH       16                              /* ROM addr width */
#define ROMSIZE         (1u << ROMAWIDTH)               /* ROM length */
#define ROMAMASK        (ROMSIZE - 1)                   /* ROM addr mask */
#define ROMBASE         0x20040000                      /* ROM base */
#define ADDR_IS_ROM(x)  ((((uint32) (x)) >= ROMBASE) && \
                        (((uint32) (x)) < (ROMBASE + ROMSIZE + ROMSIZE)))

/* KA630 board registers */

#define KAAWIDTH        4                               /* REG addr width */
#define KASIZE          (1u << KAAWIDTH)                /* REG length */
#define KABASE          0x20080000                      /* REG addr base */

/* Qbus map registers */

#define QBMAPAWIDTH     15                              /* map addr width */
#define QBMAPSIZE       (1u << QBMAPAWIDTH)             /* map length */
#define QBMAPAMASK      (QBMAPSIZE - 1)                 /* map addr mask */
#define QBMAPBASE       0x20088000                      /* map addr base */

/* Non-volatile RAM - 128 Bytes long */

#define NVRAWIDTH       7                               /* NVR addr width */
#define NVRSIZE         (1u << NVRAWIDTH)               /* NVR length */
#define NVRAMASK        (NVRSIZE - 1)                   /* NVR addr mask */
#define NVRBASE         0x200B8000                      /* NVR base */
#define ADDR_IS_NVR(x)  ((((uint32) (x)) >= NVRBASE) && \
                        (((uint32) (x)) < (NVRBASE + NVRSIZE)))

/* Qbus memory space */

#define QBMAWIDTH       22                              /* Qmem addr width */
#define QBMSIZE         (1u << QBMAWIDTH)               /* Qmem length */
#define QBMAMASK        (QBMSIZE - 1)                   /* Qmem addr mask */
#define QBMBASE         0x30000000                      /* Qmem base */
#define ADDR_IS_QBM(x)  ((((uint32) (x)) >= QBMBASE) && \
                        (((uint32) (x)) < (QBMBASE + QBMSIZE)))

/* Other address spaces */

#define ADDR_IS_CDG(x)  (0)

/* Machine specific reserved operand tests (all NOPs) */

#define ML_PA_TEST(r)
#define ML_LR_TEST(r)
#define ML_SBR_TEST(r)
#define ML_PXBR_TEST(r)
#define LP_AST_TEST(r)
#define LP_MBZ84_TEST(r)
#define LP_MBZ92_TEST(r)

/* Qbus I/O modes */

#define READ            0                               /* PDP-11 compatibility */
#define WRITE           (L_WORD)
#define WRITEB          (L_BYTE)

/* Common CSI flags */

#define CSR_V_GO        0                               /* go */
#define CSR_V_IE        6                               /* interrupt enable */
#define CSR_V_DONE      7                               /* done */
#define CSR_V_BUSY      11                              /* busy */
#define CSR_V_ERR       15                              /* error */
#define CSR_GO          (1u << CSR_V_GO)
#define CSR_IE          (1u << CSR_V_IE)
#define CSR_DONE        (1u << CSR_V_DONE)
#define CSR_BUSY        (1u << CSR_V_BUSY)
#define CSR_ERR         (1u << CSR_V_ERR)

/* Timers */

#define TMR_CLK         0                               /* 100Hz clock */

/* I/O system definitions */

#define DZ_MUXES        4                               /* max # of DZV muxes */
#define DZ_LINES        4                               /* lines per DZV mux */
#define VH_MUXES        4                               /* max # of DHQ muxes */
#define DLX_LINES       16                              /* max # of KL11/DL11's */
#define DCX_LINES       16                              /* max # of DC11's */
#define MT_MAXFR        (1 << 16)                       /* magtape max rec */
#define AUTO_LNT        34                              /* autoconfig ranks */

#define DEV_V_UBUS      (DEV_V_UF + 0)                  /* Unibus */
#define DEV_V_QBUS      (DEV_V_UF + 1)                  /* Qbus */
#define DEV_V_Q18       (DEV_V_UF + 2)                  /* Qbus, mem <= 256KB */
#define DEV_V_FLTA      (DEV_V_UF + 3)                  /* flt addr */
#define DEV_UBUS        (1u << DEV_V_UBUS)
#define DEV_QBUS        (1u << DEV_V_QBUS)
#define DEV_Q18         (1u << DEV_V_Q18)
#define DEV_FLTA        (1u << DEV_V_FLTA)

#define UNIBUS          FALSE                           /* 22b only */

#define DEV_RDX         16                              /* default device radix */

/* Device information block */

#define VEC_DEVMAX      4                               /* max device vec */

typedef struct {
    uint32              ba;                             /* base addr */
    uint32              lnt;                            /* length */
    t_stat              (*rd)(int32 *dat, int32 ad, int32 md);
    t_stat              (*wr)(int32 dat, int32 ad, int32 md);
    int32               vnum;                           /* vectors: number */
    int32               vloc;                           /* locator */
    int32               vec;                            /* value */
    int32               (*ack[VEC_DEVMAX])(void);       /* ack routine */
    } DIB;

/* I/O page layout - RQB,RQC,RQD float based on number of DZ's */

#define IOBA_DZ         (IOPAGEBASE + 000100)           /* DZ11 */
#define IOLN_DZ         010
#define IOBA_RQB        (IOPAGEBASE + 000334 +  (020 * (DZ_MUXES / 2)))
#define IOLN_RQB        004
#define IOBA_RQC        (IOPAGEBASE + IOBA_RQB + IOLN_RQB)
#define IOLN_RQC        004
#define IOBA_RQD        (IOPAGEBASE + IOBA_RQC + IOLN_RQC)
#define IOLN_RQD        004
#define IOBA_VH         (IOPAGEBASE + 000440)           /* DHQ11 */
#define IOLN_VH         020
#define IOBA_RQ         (IOPAGEBASE + 012150)           /* RQDX3 */
#define IOLN_RQ         004
#define IOBA_TS         (IOPAGEBASE + 012520)           /* TS11 */
#define IOLN_TS         004
#define IOBA_RL         (IOPAGEBASE + 014400)           /* RL11 */
#define IOLN_RL         012
#define IOBA_XQ         (IOPAGEBASE + 014440)           /* DEQNA/DELQA */
#define IOLN_XQ         020
#define IOBA_XQB        (IOPAGEBASE + 014460)           /* 2nd DEQNA/DELQA */
#define IOLN_XQB        020
#define IOBA_TQ         (IOPAGEBASE + 014500)           /* TMSCP */
#define IOLN_TQ         004
#define IOBA_XU         (IOPAGEBASE + 014510)           /* DEUNA/DELUA */
#define IOLN_XU         010
#define IOBA_RP         (IOPAGEBASE + 016700)           /* RP/RM */
#define IOLN_RP         054
#define IOBA_CR         (IOPAGEBASE + 017160)           /* CD/CR/CM */
#define IOLN_CR         010
#define IOBA_RX         (IOPAGEBASE + 017170)           /* RXV11 */
#define IOLN_RX         004
#define IOBA_RY         (IOPAGEBASE + 017170)           /* RXV21 */
#define IOLN_RY         004
#define IOBA_QVSS       (IOPAGEBASE + 017200)           /* QVSS */
#define IOLN_QVSS       0100
#define IOBA_QDSS       (IOPAGEBASE + 017400)           /* QDSS */
#define IOLN_QDSS       002
#define IOBA_DBL        (IOPAGEBASE + 017500)           /* doorbell */
#define IOLN_DBL        002
#define IOBA_LPT        (IOPAGEBASE + 017514)           /* LP11 */
#define IOLN_LPT        004
#define IOBA_PTR        (IOPAGEBASE + 017550)           /* PC11 reader */
#define IOLN_PTR        004
#define IOBA_PTP        (IOPAGEBASE + 017554)           /* PC11 punch */
#define IOLN_PTP        004

/* The KA620/KA630 maintains 4 separate hardware IPL levels, IPL 17 to IPL 14;
   however, DEC Qbus controllers all interrupt on IPL 14
   Within each IPL, priority is right to left
*/

/* IPL 17 */

/* IPL 16 */

#define INT_V_CLK       0                               /* clock */

/* IPL 15 */

/* IPL 14 - devices through RY are IPL 15 on Unibus systems */

#define INT_V_RQ        0                               /* RQDX3 */
#define INT_V_RL        1                               /* RLV12/RL02 */
#define INT_V_DZRX      2                               /* DZ11 */
#define INT_V_DZTX      3
#define INT_V_TS        4                               /* TS11/TSV05 */
#define INT_V_TQ        5                               /* TMSCP */
#define INT_V_XQ        6                               /* DEQNA/DELQA */
#define INT_V_RY        7                               /* RXV21 */

#define INT_V_TTI       8                               /* console */
#define INT_V_TTO       9
#define INT_V_PTR       10                              /* PC11 */
#define INT_V_PTP       11
#define INT_V_LPT       12                              /* LP11 */
#define INT_V_CSI       13                              /* SSC cons UART */
#define INT_V_CSO       14
#define INT_V_TMR0      15                              /* SSC timers */
#define INT_V_TMR1      16
#define INT_V_VHRX      17                              /* DHQ11 */
#define INT_V_VHTX      18 
#define INT_V_QDSS      19                              /* QDSS */
#define INT_V_CR        20
#define INT_V_QVSS      21                              /* QVSS */

#define INT_CLK         (1u << INT_V_CLK)
#define INT_RQ          (1u << INT_V_RQ)
#define INT_RL          (1u << INT_V_RL)
#define INT_DZRX        (1u << INT_V_DZRX)
#define INT_DZTX        (1u << INT_V_DZTX)
#define INT_TS          (1u << INT_V_TS)
#define INT_TQ          (1u << INT_V_TQ)
#define INT_XQ          (1u << INT_V_XQ)
#define INT_RY          (1u << INT_V_RY)
#define INT_TTI         (1u << INT_V_TTI)
#define INT_TTO         (1u << INT_V_TTO)
#define INT_PTR         (1u << INT_V_PTR)
#define INT_PTP         (1u << INT_V_PTP)
#define INT_LPT         (1u << INT_V_LPT)
#define INT_CSI         (1u << INT_V_CSI)
#define INT_CSO         (1u << INT_V_CSO)
#define INT_TMR0        (1u << INT_V_TMR0)
#define INT_TMR1        (1u << INT_V_TMR1)
#define INT_VHRX        (1u << INT_V_VHRX)
#define INT_VHTX        (1u << INT_V_VHTX)
#define INT_QDSS        (1u << INT_V_QDSS)
#define INT_CR          (1u << INT_V_CR)
#define INT_QVSS        (1u << INT_V_QVSS)

#define IPL_CLK         (0x16 - IPL_HMIN)                       /* relative IPL */
#define IPL_RQ          (0x14 - IPL_HMIN)
#define IPL_RL          (0x14 - IPL_HMIN)
#define IPL_DZRX        (0x14 - IPL_HMIN)
#define IPL_DZTX        (0x14 - IPL_HMIN)
#define IPL_TS          (0x14 - IPL_HMIN)
#define IPL_TQ          (0x14 - IPL_HMIN)
#define IPL_XQ          (0x14 - IPL_HMIN)
#define IPL_RY          (0x14 - IPL_HMIN)
#define IPL_TTI         (0x14 - IPL_HMIN)
#define IPL_TTO         (0x14 - IPL_HMIN)
#define IPL_PTR         (0x14 - IPL_HMIN)
#define IPL_PTP         (0x14 - IPL_HMIN)
#define IPL_LPT         (0x14 - IPL_HMIN)
#define IPL_CSI         (0x14 - IPL_HMIN)
#define IPL_CSO         (0x14 - IPL_HMIN)
#define IPL_TMR0        (0x14 - IPL_HMIN)
#define IPL_TMR1        (0x14 - IPL_HMIN)
#define IPL_VHRX        (0x14 - IPL_HMIN)
#define IPL_VHTX        (0x14 - IPL_HMIN)
#define IPL_QDSS        (0x14 - IPL_HMIN)
#define IPL_CR          (0x14 - IPL_HMIN)
#define IPL_QVSS        (0x14 - IPL_HMIN)

#define IPL_HMAX        0x17                            /* highest hwre level */
#define IPL_HMIN        0x14                            /* lowest hwre level */
#define IPL_HLVL        (IPL_HMAX - IPL_HMIN + 1)       /* # hardware levels */
#define IPL_SMAX        0xF                             /* highest swre level */

/* Device vectors */

#define VEC_QBUS        1                               /* Qbus system */
#define VEC_Q           0x200                           /* Qbus vector offset */
#define VEC_PTR         (VEC_Q + 0070)
#define VEC_PTP         (VEC_Q + 0074)
#define VEC_XQ          (VEC_Q + 0120)
#define VEC_XU          (VEC_Q + 0120)
#define VEC_RQ          (VEC_Q + 0154)
#define VEC_RL          (VEC_Q + 0160)
#define VEC_LPT         (VEC_Q + 0200)
#define VEC_TS          (VEC_Q + 0224)
#define VEC_CR          (VEC_Q + 0230)
#define VEC_TQ          (VEC_Q + 0260)
#define VEC_RX          (VEC_Q + 0264)
#define VEC_RY          (VEC_Q + 0264)
#define VEC_DZRX        (VEC_Q + 0300)
#define VEC_DZTX        (VEC_Q + 0304)
#define VEC_VHRX        (VEC_Q + 0310)   
#define VEC_VHTX        (VEC_Q + 0314)

/* Interrupt macros */

#define IVCL(dv)        ((IPL_##dv * 32) + INT_V_##dv)
#define IREQ(dv)        int_req[IPL_##dv]
#define SET_INT(dv)     int_req[IPL_##dv] = int_req[IPL_##dv] | (INT_##dv)
#define CLR_INT(dv)     int_req[IPL_##dv] = int_req[IPL_##dv] & ~(INT_##dv)
#define IORETURN(f,v)   ((f)? (v): SCPE_OK)             /* cond error return */

/* Logging */

#define LOG_CPU_I       0x1                             /* intexc */
#define LOG_CPU_R       0x2                             /* REI */
#define LOG_CPU_P       0x4                             /* context */

/* Function prototypes for virtual memory interface */

int32 Read (uint32 va, int32 lnt, int32 acc);
void Write (uint32 va, int32 val, int32 lnt, int32 acc);

/* Function prototypes for physical memory interface (inlined) */

SIM_INLINE int32 ReadB (uint32 pa);
SIM_INLINE int32 ReadW (uint32 pa);
SIM_INLINE int32 ReadL (uint32 pa);
SIM_INLINE int32 ReadLP (uint32 pa);
SIM_INLINE void WriteB (uint32 pa, int32 val);
SIM_INLINE void WriteW (uint32 pa, int32 val);
SIM_INLINE void WriteL (uint32 pa, int32 val);
void WriteLP (uint32 pa, int32 val);

/* Function prototypes for I/O */

int32 Map_ReadB (uint32 ba, int32 bc, uint8 *buf);
int32 Map_ReadW (uint32 ba, int32 bc, uint16 *buf);
int32 Map_WriteB (uint32 ba, int32 bc, uint8 *buf);
int32 Map_WriteW (uint32 ba, int32 bc, uint16 *buf);

int32 clk_cosched (int32 wait);

t_stat cpu_show_model (FILE *st, UNIT *uptr, int32 val, void *desc);

#include "pdp11_io_lib.h"

#endif
