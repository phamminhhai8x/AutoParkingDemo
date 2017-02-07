#ifndef FLAG_H_
#define FLAG_H_
/************************************************************************************************/
#include "..\Header\header.h"
/************************************************************************************************/
/*		 OS flag 																				*/
struct _job_rsv
{
	uchar8	JOB10ms			: 1 ;	/* 1 *//* flag to allow 10ms I/O JOB run							*/
	uchar8	JOB100ms		: 1 ;	/* 2 *//* flag to allow 100ms I/O JOB run							*/
	uchar8	JOBApp			: 1 ;	/* 3 *//* flag to allow JOB App run									*/
	uchar8	JOBRunning		: 1 ;	/* 4 *//* detect I/O JOB is Running 								*/
} ;	/*	 JOB Reserve Flag 																				*/

extern struct _job_rsv JOBRsv;
extern uchar8 d[5];
extern signed char mode;

#define	fUC8_JOB10ms		JOBRsv.JOB10ms		/* 1 *//* flag to allow 10ms I/O JOB run							*/
#define	fUC8_JOB100ms		JOBRsv.JOB100ms		/* 2 *//* flag to allow 100ms I/O JOB run							*/
#define	fUC8_JOBApp			JOBRsv.JOBApp		/* 3 *//* flag to allow JOB App run									*/
#define	fUC8_JOBRunning		JOBRsv.JOBRunning	/* 4 *//* detect I/O JOB is Running 								*/

///************************************************************************************************/
///*	 Define Global Flag Here 																	*/
//struct _flag_list{
//	uchar8 CAN_V_HardInitNG					: 1;	/* 1 *//* CAN Init NG Channel V 								*/
//};
//extern struct _flag_list flag_list;
///************************************************************************************************/
///*		Re-Define Global Flag Here 																*/
///************************************************************************************************/
//
//#define fUC8_CAN_V_HardInitNG				flag_list.CAN_V_HardInitNG				/* 1 *//* CAN Init NG Channel V 								*/
//
///***** Define Global Flag Need Backup Here *****/
//struct _flag_list_bk_z{
//	uchar8 BrakeLamp		: 1;			/* 2 *//* The flag control Brlamp signal in brake operation	*/
//
//};
//extern struct _flag_list_bk_z flag_list_bk, flag_list_bk_z;
//
///**** Redefine Global Flag Need Backup ****/
//#define fUC8_BrakeLamp			flag_list_bk.BrakeLamp		/* 2 *//* The flag control Brlamp signal in brake operation			*/
//#define fUC8_BrakeLamp_z		flag_list_bk_z.BrakeLamp	/* 2 *//* The last flag control Brlamp signal in brake operation	*/
//

#endif /* flag.h*/
/************************************************************************************************/

