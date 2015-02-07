/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/* yylex - scanner front-end for flex */

/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Vern Paxson.
 * 
 * The United States Government has rights in this work pursuant
 * to contract no. DE-AC03-76SF00098 between the United States
 * Department of Energy and the University of California.
 *
 * Redistribution and use in source and binary forms with or without
 * modification are permitted provided that: (1) source distributions retain
 * this entire copyright notice and comment, and (2) distributions including
 * binaries display the following acknowledgement:  ``This product includes
 * software developed by the University of California, Berkeley and its
 * contributors'' in the documentation or other materials provided with the
 * distribution and in all advertising materials mentioning features or use
 * of this software.  Neither the name of the University nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* $Header: /home/daffy/u0/vern/flex/RCS/yylex.c,v 2.13 95/03/04 16:10:41 vern Exp $ */

#include <ctype.h>
#include "flexdef.h"
#include "parse.h"


/* yylex - scan for a regular expression token */

int yylex()
	{
	int toktype;
	static int beglin = false;
	extern char *yytext;

	if ( eofseen )
		toktype = EOF;
	else
		toktype = flexscan();

	if ( toktype == EOF || toktype == 0 )
		{
		eofseen = 1;

		if ( sectnum == 1 )
			{
			synerr( _( "premature EOF" ) );
			sectnum = 2;
			toktype = SECTEND;
			}

		else
			toktype = 0;
		}

	if ( trace )
		{
		if ( beglin )
			{
			fprintf( stderr, "%d\t", num_rules + 1 );
			beglin = 0;
			}

		switch ( toktype )
			{
			case '<':
			case '>':
			case '^':
			case '$':
			case '"':
			case '[':
			case ']':
			case '{':
			case '}':
			case '|':
			case '(':
			case ')':
			case '-':
			case '/':
			case '\\':
			case '?':
			case '.':
			case '*':
			case '+':
			case ',':
				(void) putc( toktype, stderr );
				break;

			case '\n':
				(void) putc( '\n', stderr );

				if ( sectnum == 2 )
				beglin = 1;

				break;

			case SCDECL:
				fputs( "%s", stderr );
				break;

			case XSCDECL:
				fputs( "%x", stderr );
				break;

			case SECTEND:
				fputs( "%%\n", stderr );

				/* We set beglin to be true so we'll start
				 * writing out numbers as we echo rules.
				 * flexscan() has already assigned sectnum.
				 */
				if ( sectnum == 2 )
					beglin = 1;

				break;

			case NAME:
				fprintf( stderr, "'%s'", nmstr );
				break;

			case CHAR:
				switch ( yylval )
					{
					case '<':
					case '>':
					case '^':
					case '$':
					case '"':
					case '[':
					case ']':
					case '{':
					case '}':
					case '|':
					case '(':
					case ')':
					case '-':
					case '/':
					case '\\':
					case '?':
					case '.':
					case '*':
					case '+':
					case ',':
						fprintf( stderr, "\\%c",
							yylval );
						break;

					default:
						if ( ! isascii( yylval ) ||
						     ! isprint( yylval ) )
							fprintf( stderr,
								"\\%.3o",
							(unsigned int) yylval );
						else
							(void) putc( yylval,
								stderr );
					break;
					}

				break;

			case NUMBER:
				fprintf( stderr, "%d", yylval );
				break;

			case PREVCCL:
				fprintf( stderr, "[%d]", yylval );
				break;

			case EOF_OP:
				fprintf( stderr, "<<EOF>>" );
				break;

			case OPTION_OP:
				fprintf( stderr, "%s ", yytext );
				break;

			case OPT_OUTFILE:
			case OPT_PREFIX:
			case CCE_ALNUM:
			case CCE_ALPHA:
			case CCE_BLANK:
			case CCE_CNTRL:
			case CCE_DIGIT:
			case CCE_GRAPH:
			case CCE_LOWER:
			case CCE_PRINT:
			case CCE_PUNCT:
			case CCE_SPACE:
			case CCE_UPPER:
			case CCE_XDIGIT:
				fprintf( stderr, "%s", yytext );
				break;

			case 0:
				fprintf( stderr, _( "End Marker\n" ) );
				break;

			default:
				fprintf( stderr,
				_( "*Something Weird* - tok: %d val: %d\n" ),
					toktype, yylval );
				break;
			}
		}

	return toktype;
	}
