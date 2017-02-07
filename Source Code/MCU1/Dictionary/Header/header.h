/************************ header.h ******************/
/* 2015-11-05*/  /*  DaoHVN : Create new 			*/
/****************************************************/
#ifndef _HEADER_H
#define _HEADER_H

typedef	unsigned	char		uchar8 ;
typedef	signed		char		schar8 ;
typedef	unsigned	short	int	uint16 ;
typedef	signed		short	int	sint16 ;
typedef	unsigned	long	int	uint32 ;
typedef	signed		long	int	sint32 ;
typedef float                   float32;
typedef double                  float64;

// enum	bool	{ TRUE = (0==0) , FALSE = (!TRUE) };
// typedef		enum	bool	Bool ;

/***************** data length define ***************/
#define SCHAR8MIN		-128
#define SCHAR8MAX		127
#define UCHAR8MIN		0U
#define UCHAR8MAX		255U
#define SINT16MIN		-32768
#define SINT16MAX		32767
#define UINT16MIN		0U
#define UINT16MAX		65535U
#define UINT32MIN		0U
#define UINT32MAX		4294967295U
#define SINT32MIN		-2147483648
#define SINT32MAX		2147483647
/***************** BIT define *********************/

/* Bit-Pattern Definition */
#define		BIT0		(0x0001U)	/* 0000 0000 0000 0001 */
#define		BIT1		(0x0002U)	/* 0000 0000 0000 0010 */
#define		BIT2		(0x0004U)	/* 0000 0000 0000 0100 */
#define		BIT3		(0x0008U)	/* 0000 0000 0000 1000 */
#define		BIT4		(0x0010U)	/* 0000 0000 0001 0000 */
#define		BIT5		(0x0020U)	/* 0000 0000 0010 0000 */
#define		BIT6		(0x0040U)	/* 0000 0000 0100 0000 */
#define		BIT7		(0x0080U)	/* 0000 0000 1000 0000 */
#define		BIT8		(0x0100U)	/* 0000 0001 0000 0000 */
#define		BIT9		(0x0200U)	/* 0000 0010 0000 0000 */
#define		BIT10		(0x0400U)	/* 0000 0100 0000 0000 */
#define		BIT11		(0x0800U)	/* 0000 1000 0000 0000 */
#define		BIT12		(0x1000U)	/* 0001 0000 0000 0000 */
#define 	BIT13		(0x2000U)	/* 0010 0000 0000 0000 */
#define		BIT14		(0x4000U)	/* 0100 0000 0000 0000 */
#define		BIT15		(0x8000U)	/* 1000 0000 0000 0000 */
#define		BIT16		(0x00010000U)
#define		BIT17		(0x00020000U)
#define		BIT18		(0x00040000U)
#define		BIT19		(0x00080000U)
#define		BIT20		(0x00100000U)
#define		BIT21		(0x00200000U)
#define		BIT22		(0x00400000U)
#define		BIT23		(0x00800000U)
#define		BIT24		(0x01000000U)
#define		BIT25		(0x02000000U)
#define		BIT26		(0x04000000U)
#define		BIT27		(0x08000000U)
#define		BIT28		(0x10000000U)
#define		BIT29		(0x20000000U)
#define		BIT30		(0x40000000U)
#define		BIT31		(0x80000000U)

#define		NOTBIT0		(~BIT0)		/* 1111 1111 1111 1110 */
#define 	NOTBIT1		(~BIT1)		/* 1111 1111 1111 1101 */
#define 	NOTBIT2		(~BIT2)		/* 1111 1111 1111 1011 */
#define 	NOTBIT3		(~BIT3)		/* 1111 1111 1111 0111 */
#define 	NOTBIT4		(~BIT4)		/* 1111 1111 1110 1111 */
#define 	NOTBIT5		(~BIT5)		/* 1111 1111 1101 1111 */
#define 	NOTBIT6		(~BIT6)		/* 1111 1111 1011 1111 */
#define 	NOTBIT7		(~BIT7)		/* 1111 1111 0111 1111 */
#define 	NOTBIT8		(~BIT8)		/* 1111 1110 1111 1111 */
#define 	NOTBIT9		(~BIT9)		/* 1111 1101 1111 1111 */
#define 	NOTBIT10	(~BIT10)	/* 1111 1011 1111 1111 */
#define 	NOTBIT11	(~BIT11)	/* 1111 0111 1111 1111 */
#define 	NOTBIT12	(~BIT12)	/* 1110 1111 1111 1111 */
#define 	NOTBIT13	(~BIT13)	/* 1101 1111 1111 1111 */
#define 	NOTBIT14	(~BIT14)	/* 1011 1111 1111 1111 */
#define 	NOTBIT15	(~BIT15)	/* 0111 1111 1111 1111 */

/*****************************************************/
#define	CLR	0U
#define	SET	1U

/***************** set/clear bit define ***************/
#define SETBIT(BIT) ((BIT)=(1U))
#define CLRBIT(BIT) ((BIT)=(0U))

#define TSTBIT(BIT) ((BIT != 0U))

/* SETBIT/CLRBIT in byte position */
#define P_B_SETBIT(BYTE,pos) ((BYTE)|=(1U<<(pos)))
#define P_B_CLRBIT(BYTE,pos) ((BYTE)&=(~(1U<<(pos))))

#define P_B_TSTBIT(BYTE,pos) (((BYTE>>pos)&0x01U==1U))

/*	SETBIT  set BIT in BYTE	*/
#define	B_SETBIT(BYTE, BIT)	((BYTE)|=(BIT))
/*	CLRBIT	clear BIT in BYTE */
#define	B_CLRBIT(BYTE, BIT)	((BYTE)&=(~(BIT)))
/*	TSTBIT	return TRUE when BIT is set */
#define	B_TSTBIT(BYTE, BIT)	(((BYTE)&(BIT))!=0U)
/*	SETBIT/CLRBIT in word position*/
#define P_W_SETBIT(WORD,pos) ((pos <= 15U)?(WORD|=(uint16)((uint16)1U<<pos)):WORD)
#define P_W_CLRBIT(WORD,pos) ((pos <= 15U)?((WORD)&=(uint16)(~((uint16)((uint16)1U<<pos)))):WORD)

/*	SETBIT  set BIT in WORD	*/
#define	W_SETBIT(WORD, BIT)	((WORD)|=(BIT))
/*	CLRBIT	clear BIT in BYTE */
#define	W_CLRBIT(WORD, BIT)	((WORD)&=(~(BIT)))
/*	TSTBIT	return TRUE when BIT is set */
#define	W_TSTBIT(WORD, BIT)	(((WORD)&(BIT))!=0U)

/***************** set/clear timer define ***************/
#define START_TIMER(timer,time) ((timer)=(time))
#define CHECK_TIMER(timer) (((timer)==0U))


/***************** set/clear counter define ***************/
#define START_COUNTER(counter,times) ((counter)=(times))
#define CHECK_COUNTER(counter) 		(((counter)==0U))


/***************** min/max define ***************/
#define MIN(a,b) ((a>b)?b:a)
#define MAX(a,b) ((a>b)?a:b)

/****** Define limit variable from Min to Max ******/
#define MIN_MAX(a,min,max)		((a>max)?max:((a<min)?min:a))

/********** Define const global ************/
#define ON				1U
#define OFF				0U

#define HIGH			1U
#define LOW				0U

#define HIGH_BYTE		0xFFU
#define LOW_BYTE		0x00U

#define HIGH_WORD		0xFFFFU
#define LOW_WORD		0x0000U

#define	TRUE			1U
#define	FALSE			0U

#define	ENABLE			1U
#define	DISABLE			0U

/* OS */
#define IG_OFF 0U
#define IG_ON 1U
#define ACC 2U
#define GO_TO_SLEEP 0U
#define WAKE_UP 3U
#define REFUSE_TO_SLEEP 1U
#define READY_TO_SLEEP 2U

/* VCM CAN */
#define NULL				0U				/* Check NULL value */

#define MSGBOX_V_NUMBER		16U				/* Number MSGBOX in channel V of CPU */
#define MSGBOX_ITS_NUMBER	16U				/* Number MSGBOX in channel TIS of CPU */

#define LIM_MIN				0U				/* Min of signal Message */
#define LIM_MAX				3U				/* Max of signal Message */

#define MIN_SPEED			0U				/* Min of speed */
#define MAX_SPEED			2560U			/* Max of speed */

#define MIN_SPEEDDEC		0U				/* Min of deceleration */
#define MAX_SPEEDDEC		3200U			/* Max of deceleration */

#define MIN_FD				2U				/* Min of FDISTANCE */
#define MAX_FD				3200U			/* Max of FDISTANCE */

#define MIN_EMGDEC			0U				/* Min of EMGCYDEACC */
#define MAX_EMGDEC			3200U			/* Max of EMGCYDEACC */

#define MIN_DISTANCE		2U				/* Min distance FRDISTANCE,FLDISTANCE,RRDISTANCERLDISTANCE */
#define MAX_DISTANCE		160U				/* Max distance FRDISTANCE,FLDISTANCE,RRDISTANCERLDISTANCE */

#define MIN_SEQ				0U				/* Min of VCM_SEQ */
#define MAX_SEQ				7U				/* Max of VCM_SEQ */
#define PWSETBIT			15U
#define PWCLEARBIT 			15U

/***************** const define ************************/


#endif /* _HEADER_H */

/************* end file header.h ***************/
