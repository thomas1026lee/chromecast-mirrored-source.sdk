/*
 * (C) Copyright Marvell Semiconductors,Inc 2006 All rightes reserved
 */

#include "util.h"
#include "ctypes.h"
#include "string.h"
#include "debug.h"
#include "apb_perf_base.h"


/*
 * heap start point
 */
static void *	malloc_base;

void *	UtilMemSet(void *s, int c, int n)
{

	unsigned char *	char_ptr = s;
	unsigned int *	int_ptr = s;
	int		remainer;
	
	if((remainer = ((int)s & 3)) != 0) {
		while (remainer-- > 0) {
			*char_ptr++ = (unsigned char)c;
			n--;
		}
		int_ptr = (unsigned int *)char_ptr;
		remainer = n & 3;
	}

	n >>= 2;
	while (n-- > 0) {
		*int_ptr++ = (unsigned int)c;
	}
	
	if (remainer != 0) {
		unsigned int	temp = *int_ptr;
		unsigned char *	temp_p = (unsigned char *)&temp;
		while (remainer-- > 0) {
			*temp_p++ = (unsigned char)c;
		}
		*int_ptr = temp;
	}
	return(s);
}

void *	UtilMemCpy(void *s1, const void *s2, int n)
{

	unsigned int *		dst = s1;
	const unsigned int *	src = s2;
	int			remainer = n & 3;

	if (((unsigned int)s1 & 3) || ((unsigned int)s2 & 3)) {
		unsigned char *		dst_char = s1;
		const unsigned char *	src_char = s2;

		while (n-- > 0) {
			*dst_char++ = *src_char++;
		}
		return s1;
	}

	n >>= 2;
	while (n-- > 0) {
		*dst++ = *src++;
	}
	
	if (remainer != 0) {
		unsigned int		temp = *dst;
		unsigned char *		temp_p = (unsigned char *)&temp;
		const unsigned char *	src_c = (const unsigned char *)src;
		while (remainer-- > 0) {
			*temp_p++ = (unsigned char)*src_c++;
		}
		*dst = temp;
	}

	return(s1);
}

int	UtilMemCmp(const void *s1, const void *s2, int n)
{

	int			i = 0;
	const unsigned char *	ss1 = s1;
	const unsigned char *	ss2 = s2;
	
	for ( ; i < n; i++) {
		if (ss1[i] > ss2[i]) {
			return(i + 1);
		}
		else if (ss1[i] < ss2[i]) {
			return(-i - 1);
		}
	}

	return 0;
}

void *	UtilMemAllocZ(int size)
{
	if (size > 0) {
		unsigned long malloc_align = (((unsigned long) malloc_base + 31) >> 5) << 5;
		void *	ptr = (void *) malloc_align;
	
		UtilMemSet(ptr, 0, size);
		malloc_base = (void *) malloc_align + size;
		return(ptr);
	}
	return(0);
}

void *	UtilMemAlloc(int size)
{
	if (size > 0) {
		unsigned long malloc_align = (((unsigned long) malloc_base + 31) >> 5) << 5;
		void *	ptr = (void *) malloc_align;
	
		if( (malloc_align + size) > __user_read_heap_top() )
			return 0;
		malloc_base =  malloc_align + size;
		return(ptr);
	}
	return(0);
}

void	UtilMemReset(void)
{
		malloc_base = __user_read_heap_bottom();
}


#define DBG_MEM	                (0x3000000)
#define DBG_PORT_BASE			(SM_APB_UART0_BASE)
#define PUT_CHAR(ch)  BFM_HOST_Bus_Write32((DBG_PORT_BASE+0x00), (UNSG32)ch)
#define	put_char	PUT_CHAR

#define MAX_HISBUF_ENTRY 16
static char dbg_buf[256];
static char his_buf[MAX_HISBUF_ENTRY][80];      //  for Historical commands support
static SIGN32 his_mode=0;
SIGN32 his_next_idx=0;
SIGN32 his_cur_idx=0;
SIGN32 his_cmd_cnt=0;
SIGN32 pre_his_cmd_length;

static void *   malloc_base;

unsigned char GET_CHAR()
{
	UNSG32 data32;
	unsigned char data8;
	BFM_HOST_Bus_Read32((DBG_PORT_BASE+0x00), &data32);

	data8=data32&0xFF;
	return data8;

}

////////////////////////////////////////////////////////////
// dbg port (uart) driver
SIGN32 dbg_port_tx_ready()
{
	UNSG32 status=0;
	do
	{
		BFM_HOST_Bus_Read32((DBG_PORT_BASE+0x7c),&status);
	} while ((status&0x03)!=0x02);
	// add extra protection below
	do
	{
		BFM_HOST_Bus_Read32((DBG_PORT_BASE+0x80),&status);
	} while (status>2);  //delay control
	return 1;
}

/////////////////////////////////////////////////////////////////////////
//
//  showDbgPrompt() - Used by the debugger.  Displays the appropriate
//                debugger prompt.
//
/////////////////////////////////////////////////////////////////////////
void showDbgPrompt(char* prompt)
{
    lgpl_printf("\n\n%s", prompt);
}


SIGN32 dbg_port_rx_ready()
{
	UNSG32 read;
	BFM_HOST_Bus_Read32((DBG_PORT_BASE+0x7c),&read);      // status
	if(read&0x08)
	{
		BFM_HOST_Bus_Read32((DBG_PORT_BASE+0x84),&read);  // RFL
		if (read>0)
		return 1;
	}
	return 0;
}

void dbg_port_print( char* str)
{
	char ch;
	SIGN32 i;
	SIGN32 len=strlen(str);
	dbg_port_tx_ready();
	for (i=0;i<len;i++)
	{
		ch=str[i];
		put_char(ch);
		if (ch=='\n')
		{
			ch='\r';
			put_char(ch);
		}
		//extra delay
		i=i;
		len=len;
		ch=str[i];
	}
}


//////////////////////////////////////////////////////
// read command via UART (RS232)
#define CR_CHAR		13
#define LF_CHAR		10
#define BS_CHAR		8
#define UP_CHAR		'.'
#define DIR1_CHAR	0x1B
#define DIR2_CHAR	'['
#define UP3_CHAR	'A'
#define DOWN3_CHAR	'B'

int getline(char *pCmd)
{
   unsigned char ch;
   SIGN32 i, j;
   SIGN32 comment = 0;
   unsigned char arrow = 0;
   int	input_cmd_line_length = 0 ; 


   for (i = 0; i < CMD_LINE_BUFFER_SIZE; i++)
    	pCmd[i] = 0;

	i = 0;
	input_cmd_line_length = 0 ; 
	while (1)
	{
		while (dbg_port_rx_ready() == 0);
		ch = GET_CHAR();
		arrow += 2;
		input_cmd_line_length++ ; 
		if(input_cmd_line_length>CMD_LINE_BUFFER_SIZE){
			lgpl_printf("cmd line overflow, please input the command line again\n") ;
			return 1 ; 
		}

		switch (ch)
		{
			case CR_CHAR:
			case LF_CHAR:
			case '\0':
				pCmd[i] = '\0';
				comment = 0;
#if 0
				if (his_mode == 0 && strlen(pCmd))
				{	// only store new command that is not null
					memset(his_buf[his_cur_idx], 0, 80);       // clear buf
					memcpy(his_buf[his_cur_idx], pCmd, strlen(pCmd));
					if (his_cur_idx < MAX_HISBUF_ENTRY - 1)
					{
						his_next_idx = his_cur_idx;
						his_cur_idx++;
					}
					else 
					{
						his_next_idx = MAX_HISBUF_ENTRY - 1;
						his_cur_idx = 0;
					}
					his_cmd_cnt = (his_cmd_cnt < MAX_HISBUF_ENTRY) ? (his_cmd_cnt + 1) : MAX_HISBUF_ENTRY;
				}
				his_mode = 0;
#endif
				return 0 ;

#if 0
			case ';':
			case '/':
				comment = 1;
				break;

			case BS_CHAR:
				i--;
				i--;
				PUT_CHAR(ch);
				PUT_CHAR(' ');
				PUT_CHAR(ch);
				his_mode = 0;		// modify historical command
				arrow = 0;
				break;
#endif
			default:
				pCmd[i] = ch ; 
//				PUT_CHAR(ch);
				PUT_CHAR('*');
#if 0
				if (comment == 0)
				{				
					pCmd[i] = ch;
					if (ch == DIR1_CHAR)
						arrow = 1;
					else if (ch == DIR2_CHAR && arrow == 3)
						arrow = arrow;
					else if (ch == UP3_CHAR && arrow == 5)
					{
						arrow = 0;
						PUT_CHAR(DIR1_CHAR);
						PUT_CHAR(DIR2_CHAR);
						PUT_CHAR(DOWN3_CHAR);
						i = previous_cmd(pCmd, i, 0);
					}
					else if (ch == DOWN3_CHAR && arrow == 5)
					{
						arrow = 0;
						i = next_cmd(pCmd, i);
					}
					else
					{
						his_mode = 0; // modify history command
						arrow = 0;
					}
				}
#endif
				break;
		}
		i++;
	}
}

int previous_cmd(char *pCmd, unsigned int cur_cmd_length, unsigned int loop)
{
	int i, j;
	int erase;

	if (his_cmd_cnt == 0)
		return (cur_cmd_length - 3);

	if (his_mode == 0)
	{
		his_mode = 1;
	}
	else 
	{
		if (loop == 0)
		{
			if (his_next_idx == his_cur_idx + 1)
				return (cur_cmd_length - 3);

			if ((his_next_idx == 0) && (his_cur_idx == his_cmd_cnt))
				return (cur_cmd_length - 3);	
		}

		if (his_next_idx == 0)
			his_next_idx = his_cmd_cnt - 1;
		else
			his_next_idx--;
	}

	// need to clear line, move cursor, and print prompt again
	erase = strlen(PROMPT) + (pre_his_cmd_length > cur_cmd_length ? pre_his_cmd_length : cur_cmd_length);
	put_char('\r');
	for (j = 0; j <= erase ; j++)
	{
		put_char(' ');
	}
	put_char('\r');   		// back to beginning of line
	dbg_port_print(PROMPT);	// show prompt

	dbg_port_print(his_buf[his_next_idx]);

	memcpy(pCmd, his_buf[his_next_idx], strlen(his_buf[his_next_idx]));
	i = strlen(his_buf[his_next_idx]) - 1;
	pre_his_cmd_length = i;		// record the his command length

	return i;
}

int next_cmd(char *pCmd, unsigned int cur_cmd_length)
{
	int i, j;
	int erase;

	his_mode = 2;
	if (his_cur_idx == his_next_idx + 1)
		return (cur_cmd_length - 3);

	if ((his_cur_idx == 0) && (his_next_idx == his_cmd_cnt))
		return (cur_cmd_length - 3);
	
	if (his_next_idx == his_cmd_cnt - 1)
		his_next_idx = 0;
	else
		his_next_idx++;

	// need to clear line, move cursor, and print prompt again
	erase = strlen(PROMPT) + (pre_his_cmd_length > cur_cmd_length ? pre_his_cmd_length : cur_cmd_length);
	put_char('\r');
	for (j = 0; j <= erase; j++)
	{
		put_char(' ');
	}
	put_char('\r');   		// back to beginning of line
	dbg_port_print(PROMPT);	// show prompt      				

	dbg_port_print(his_buf[his_next_idx]);

	memcpy(pCmd, his_buf[his_next_idx], strlen(his_buf[his_next_idx]));
	i = strlen(his_buf[his_next_idx]) - 1;
	pre_his_cmd_length = i;		// record the his command length

	return i;
}

/////////////////////////////////////////////////////////////////////////
//
//  skipSpace() - Used by the debugger.  Skips non-text characters at
//                the beginning of a string
//
/////////////////////////////////////////////////////////////////////////
void skipSpace ( char **inStr )
{
    char *s = *inStr;

    while (( *s == ' ' ) || ( *s == '\t' ) || ( *s == '\r' ) || ( *s == '\n' ))
        s++;

    *inStr = s;
}


/////////////////////////////////////////////////////////////////////////
//
//  getTokenLength() - Used by the debugger.  Gets the length of the
//                     next token in a string
//
/////////////////////////////////////////////////////////////////////////
SIGN32 getTokenLength (char *s )
{
    SIGN32     index;
    char    x;

    for ( index = 0; s[ index] != 0; index++ )
    {
        x = s[ index];
        if (( x == ' ' ) || ( x == '\t' ) || ( x == '\r' ) || ( x == '\n' ))
            break;
    }

    return( index );
}

/////////////////////////////////////////////////////////////////////////
//
//  getIntToken() - Used by the debugger Gets the next token from a string
//
/////////////////////////////////////////////////////////////////////////
char*  getIntToken ( char *inStr, UNSG32 *outVal )
{
    SIGN32  iSum = 0;
    char  c;
    char  *thisStr = inStr;
	SIGN32  bPositive = 1;
    // If first character is ';', we have a comment, so
    // get out of here.
    if ( *thisStr == ';' )
        return thisStr;

    // Process decimal characters
    // a negative number?
    if ( *thisStr == '-' )
    {
        bPositive =0;
        thisStr ++;
    }

    while ( *thisStr )
    {
        c = *thisStr;

        // Check for valid digits.
        if ( !( c >= '0' && c <= '9' ) )
            break;

        // ASCII to decimal conversion
        iSum = iSum * 10 + ( c - '0' );

        thisStr ++;
    }

    // make it negative
    if ( !bPositive )
        iSum = -1 * iSum;

    // Return updated pointer and decoded value.
    *outVal = iSum;
    return thisStr;
}

/////////////////////////////////////////////////////////////////////////
//
//  getSignedIntToken() - Used by the debugger Gets the next token from a string
//
/////////////////////////////////////////////////////////////////////////
char*  getSignedIntToken ( char *inStr, SIGN32 *outVal )
{
    SIGN32  iSum = 0;
    char  c;
    char  *thisStr = inStr;
	SIGN32  bPositive = 1;
    // If first character is ';', we have a comment, so
    // get out of here.
    if ( *thisStr == ';' )
        return thisStr;

    // Process decimal characters
    // a negative number?
    if ( *thisStr == '-' )
    {
        bPositive =0;
        thisStr ++;
    }

    while ( *thisStr )
    {
        c = *thisStr;

        // Check for valid digits.
        if ( !( c >= '0' && c <= '9' ) )
            break;

        // ASCII to decimal conversion
        iSum = iSum * 10 + ( c - '0' );

        thisStr ++;
    }

    // make it negative
    if ( !bPositive )
        iSum = -1 * iSum;

    // Return updated pointer and decoded value.
    *outVal = iSum;
    return thisStr;
}

/////////////////////////////////////////////////////////////////////////
//
//  getHexToken() - Used by the debugger Gets the next token from a string
//
/////////////////////////////////////////////////////////////////////////
char* getHexToken ( char *inStr, UNSG32 *outVal )
{
    UNSG32   thisVal, tVal;
    char    x;
    char    *thisStr = inStr;

    thisVal = 0;

        // If first character is ';', we have a comment, so
        // get out of here.

    if ( *thisStr == ';' )
        return( thisStr );

        // Process hex characters.

    while ( *thisStr )
    {
        // Do uppercase conversion if necessary.

        x = *thisStr;
        if (( x >= 'a') && ( x <= 'f'))
            x &= ~0x20;

        // Check for valid digits.

        if ( !((( x >= '0') && (x <= '9')) || (( x >= 'A') && ( x <= 'F'))))
            break;

        // Hex ASCII to binary conversion.

        tVal = x - '0';
        if ( tVal > 9 )
            tVal -= 7;

        thisVal = (thisVal * 16) + tVal;

        thisStr++;
    }

        // Return updated pointer and decoded value.

    *outVal = thisVal;
    return( thisStr );
}

/////////////////////////////////////////////////////////////////////////
//
//  getToken() - Used by the debugger Gets the next token from a string
//
/////////////////////////////////////////////////////////////////////////
char* getToken ( char *inStr, char **outVal )
{
    char    *thisStr = inStr;

        // If first character is ';', we have a comment, so
        // get out of here.

    if ( *thisStr == ';' )
        return( thisStr );

    while ( *thisStr != ' ' && *thisStr != '\t' && *thisStr != '\0' )
    {
        thisStr++;
    }

    if ( *thisStr != '\0' )
    {
        *thisStr = '\0';
        thisStr ++;
    }

    *outVal = inStr;
    return( thisStr );
}

// Qingwei Huang added for google's toolchain
int raise(int i)
{
    return 0;
}
