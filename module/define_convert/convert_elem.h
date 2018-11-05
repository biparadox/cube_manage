/*************************************************
*       Hige security Linux Operating System Project
*
*	File description: 	Definition of data describe struct header file
*	File name:		struct_deal.h
*	date:    	2008-05-09
*	Author:    	Hu jun
*************************************************/
#ifndef  CONVERT_ELEM_H
#define  CONVERT_ELEM_H

int convert_record(int type,int subtype, char * record_define);
int convert_type(int start_type,int end_type, char * enum_name,char * record_define);
int convert_subtype(int type, char * enum_name,char * record_define);

#endif
