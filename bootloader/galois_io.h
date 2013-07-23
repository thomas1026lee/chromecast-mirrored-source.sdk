/*******************************************************************************
 * Copyright (C) Marvell International Ltd. and its affiliates
 *
 * This software file (the "File") is owned and distributed by Marvell
 * International Ltd. and/or its affiliates ("Marvell") under the following
 * alternative licensing terms.  Once you have made an election to distribute the
 * File under one of the following license alternatives, please (i) delete this
 * introductory statement regarding license alternatives, (ii) delete the two
 * license alternatives that you have not elected to use and (iii) preserve the
 * Marvell copyright notice above.
 *
 *******************************************************************************
 * Marvell Commercial License Option
 *
 * If you received this File from Marvell and you have entered into a commercial
 * license agreement (a "Commercial License") with Marvell, the File is licensed
 * to you under the terms of the applicable Commercial License.
 *
 ********************************************************************************
 * Marvell GPL License Option
 *
 * If you received this File from Marvell, you may opt to use, redistribute and/or
 * modify this File in accordance with the terms and conditions of the General
 * Public License Version 2, June 1991 (the "GPL License"), a copy of which is
 * available along with the File in the license.txt file or by writing to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 or
 * on the worldwide web at http://www.gnu.org/licenses/gpl.txt.
 *
 * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY
 * DISCLAIMED.  The GPL License provides additional details about this warranty
 * disclaimer.
 ********************************************************************************
 * Marvell BSD License Option
 *
 * If you received this File from Marvell, you may opt to use, redistribute and/or
 * modify this File under the following licensing terms.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *	Redistributions of source code must retain the above copyright notice,
 *	this list of conditions and the following disclaimer.
 *
 *	Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 *	Neither the name of Marvell nor the names of its contributors may be
 *	used to endorse or promote products derived from this software without
 *	specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************/

#ifndef	__GALOIS_IO_H__
#define	__GALOIS_IO_H__

#include "com_type.h"
#include "galois_common.h"

#define	__MEMIO_DIRECT


#define	CPU_PHY_MEM(x)					((MV_U32)(x))
#define	CPU_MEMIO_CACHED_ADDR(x)			((void*)(x))
#define	CPU_MEMIO_UNCACHED_ADDR(x)			((void*)(x))

/*!
 * CPU architecture dependent 32, 16, 8 bit read/write IO addresses
 */
#define	MV_MEMIO32_WRITE(addr, data)		((*((volatile unsigned int*)(addr))) = ((unsigned int)(data)))
#define	MV_MEMIO32_READ(addr)				((*((volatile unsigned int*)(addr))))
#define	MV_MEMIO16_WRITE(addr, data)		((*((volatile unsigned short*)(addr))) = ((unsigned short)(data)))
#define	MV_MEMIO16_READ(addr)				((*((volatile unsigned short*)(addr))))
#define	MV_MEMIO08_WRITE(addr, data)		((*((volatile unsigned char*)(addr))) = ((unsigned char)(data)))
#define	MV_MEMIO08_READ(addr)				((*((volatile unsigned char*)(addr))))


/*!
 * No Fast Swap implementation (in assembler) for ARM
 */
#define	MV_32BIT_LE_FAST(val)				MV_32BIT_LE(val)
#define	MV_16BIT_LE_FAST(val)				MV_16BIT_LE(val)
#define	MV_32BIT_BE_FAST(val)				MV_32BIT_BE(val)
#define	MV_16BIT_BE_FAST(val)				MV_16BIT_BE(val)

/*!
 * 32 and 16 bit read/write in big/little endian mode
 */

/*!
 * 16bit write in little endian mode
 */
#define	MV_MEMIO_LE16_WRITE(addr, data)		MV_MEMIO16_WRITE(addr, MV_16BIT_LE_FAST(data))

/*!
 * 16bit read in little endian mode
 */
#define	MV_MEMIO_LE16_READ(addr)			MV_16BIT_LE_FAST((MV_U16)(MV_MEMIO16_READ((MV_U32)(addr))))

/*!
 * 32bit write in little endian mode
 */
#define	MV_MEMIO_LE32_WRITE(addr, data)		MV_MEMIO32_WRITE(addr, MV_32BIT_LE_FAST(data))

/*!
 * 32bit read in little endian mode
 */
#define	MV_MEMIO_LE32_READ(addr)			MV_32BIT_LE_FAST((MV_U32)(MV_MEMIO32_READ((MV_U32)(addr))))

/*!
 * Generate 32bit mask
 */
#define	GA_REG_MASK(bits, l_shift)			((bits) ? (((bits) < 32) ? (((1uL << (bits)) - 1) << (l_shift)) : (0xFFFFFFFFuL << (l_shift))) : 0)

/*!
 * Galois's register address translate
 */

#	ifndef	INTER_REGS_BASE
#	define	INTER_REGS_BASE				0
#	endif

#	define	REG_ADDR(offset)			((MV_U32)(INTER_REGS_BASE | (offset)))

/*!
 * Galois controller register read/write macros
 *
 * offset -- address offset (32bits)
 * holder -- pointer to the variable that will be used to store the data being read in.
 * val -- variable contains the data that will be written out.
 * bitMask -- variable contains the data (32bits) that will be written out. 
 * clearMask -- variable contains the mask (32bits) that will be used to clear the corresponding bits. 
 *
 * GA_REG_WORD32_READ(offset, holder) -- Read a Double-Word (32bits) from 'offset' to 'holder'
 * GA_REG_WORD16_READ(offset, holder) -- Read a Word (16bits) from 'offset' to 'holder'
 * GA_REG_BYTE_READ(offset, holder) -- Read a Byte (8bits) from 'offset' to 'holder'
 *
 * GA_REG_WORD32_WRITE(offset, val) -- Write a Double-Word (32bits) to 'offset'
 * GA_REG_WORD16_WRITE(offset, val) -- Write a Word (16bits) to 'offset'
 * GA_REG_BYTE_WIRTE(offset, val) -- Write a Byte (8bits) to 'offset'
 *
 * GA_REG_WORD32_BIT_SET(offset, bitMask) -- Set bits to '1b' at 'offset', 'bitMask' should only be used to set '1b' for corresponding bits. 
 * GA_REG_WORD32_BITS_SET(offset, clearMask, val) -- Clear the bits to zero for the bits in clearMask are '1b' and write 'val' to 'offset'.
 * GA_REG_WORD32_BIT_CLEAR(offset, clearMask) -- Clear the bits to zero for the bits in bitMask are '1b'
 *
 */
#	define	GA_REG_WORD32_READ(offset, holder)	(*(holder) = MV_MEMIO_LE32_READ(REG_ADDR(offset)))
#	define	GA_REG_WORD16_READ(offset, holder)	(*(holder) = MV_MEMIO_LE16_READ(REG_ADDR(offset)))
#	define	GA_REG_BYTE_READ(offset, holder)	(*(holder) = MV_MEMIO08_READ(REG_ADDR(offset)))

#	define	GA_REG_WORD32_WRITE(offset, val)	(MV_MEMIO_LE32_WRITE(REG_ADDR(offset), (MV_U32)(val)))
#	define	GA_REG_WORD16_WRITE(offset, val)	(MV_MEMIO_LE16_WRITE(REG_ADDR(offset), (MV_U16)(val)))
#	define	GA_REG_BYTE_WRITE(offset, val)		(MV_MEMIO08_WRITE(REG_ADDR(offset), (MV_U8)(val)))

#	define	GA_REG_WORD32_BIT_SET(offset, bitMask)	(MV_MEMIO32_WRITE(REG_ADDR(offset),					\
							(MV_MEMIO_LE32_READ(REG_ADDR(offset)) | MV_32BIT_LE_FAST(bitMask))))
#	define	GA_REG_WORD32_BITS_SET(offset, clearMask, val)									\
							(MV_MEMIO32_WRITE(REG_ADDR(offset),					\
							((MV_MEMIO_LE32_READ(REG_ADDR(offset)) & MV_32BIT_LE_FAST(~(clearMask)))\
							| MV_32BIT_LE_FAST(val))))
#	define	GA_REG_WORD32_BIT_CLEAR(offset, clearMask)									\
							(MV_MEMIO32_WRITE(REG_ADDR(offset),					\
							(MV_MEMIO_LE32_READ(REG_ADDR(offset)) & MV_32BIT_LE_FAST(~(clearMask)))))


#endif	/* __GALOIS_IO_H__ */
