/* lzo1b_r.ch -- literal run handling for the the LZO1B/LZO1C algorithm

   This file is part of the LZO-Professional data compression library.

   Copyright (C) 2006 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2005 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2004 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2003 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2002 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2001 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 2000 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1999 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1998 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1997 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1996 Markus Franz Xaver Johannes Oberhumer
   All Rights Reserved.

   CONFIDENTIAL & PROPRIETARY SOURCE CODE.

   ANY USAGE OF THIS FILE IS SUBJECT TO YOUR LICENSE AGREEMENT.

   Markus F.X.J. Oberhumer
   <markus@oberhumer.com>
   http://www.oberhumer.com/products/lzo-professional/
 */


/***********************************************************************
// store a literal run (internal)
************************************************************************/

LZO_PUBLIC(lzo_bytep )
STORE_RUN ( lzo_bytep const oo, const lzo_bytep const ii, lzo_uint r_len)
{
    register lzo_bytep op;
    register const lzo_bytep ip;
    register lzo_uint t;

    LZO_STATS(lzo_stats->literals += r_len);

    op = oo;
    ip = ii;
    assert(r_len > 0);

    /* code a long R0 run */
    if (r_len >= 512)
    {
        unsigned r_bits = 6;        /* 256 << 6 == 16384 */
        lzo_uint tt = 32768u;

        while (r_len >= (t = tt))
        {
            r_len -= t;
            *op++ = 0; *op++ = (R0FAST - R0MIN) + 7;
            MEMCPY8_DS(op, ip, t);
            LZO_STATS(lzo_stats->r0long_runs++);
        }
        tt >>= 1;
        do {
            if (r_len >= (t = tt))
            {
                r_len -= t;
                *op++ = 0; *op++ = LZO_BYTE((R0FAST - R0MIN) + r_bits);
                MEMCPY8_DS(op, ip, t);
                LZO_STATS(lzo_stats->r0long_runs++);
            }
            tt >>= 1;
        } while (--r_bits > 0);
    }
    assert(r_len < 512);

    while (r_len >= (t = R0FAST))
    {
        r_len -= t;
        *op++ = 0; *op++ = (R0FAST - R0MIN);
        MEMCPY8_DS(op, ip, t);
        LZO_STATS(lzo_stats->r0fast_runs++);
    }

    t = r_len;
    if (t >= R0MIN)
    {
        /* code a short R0 run */
        *op++ = 0; *op++ = LZO_BYTE(t - R0MIN);
        MEMCPY_DS(op, ip, t);
        LZO_STATS(lzo_stats->r0short_runs++);
    }
    else if (t > 0)
    {
        /* code a short literal run */
        LZO_STATS(lzo_stats->lit_runs++);
        LZO_STATS(lzo_stats->lit_run[t]++);
        *op++ = LZO_BYTE(t);
        MEMCPY_DS(op, ip, t);
    }

    return op;
}


/*
vi:ts=4:et
*/

