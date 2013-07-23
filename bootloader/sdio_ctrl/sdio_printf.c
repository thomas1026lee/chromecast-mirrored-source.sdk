/*
	Copyright 2001, 2002 Georges Menie (www.menie.org)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
	putchar is the only external dependency for this file,
	if you have a working putchar, just remove the following
	define. If the function should be called something else,
	replace outbyte(c) by your own function call.
*/



#ifdef EMMC_DEBUG
void dbg_printf(int logLevel,const char* szFormat, ...)
{
	register int *varg = (int *)(&szFormat);
	return lgpl_printf(varg) ;
}
#else
void dbg_printf(int logLevel,const char* szFormat, ...)
{
	return;
}
#endif
