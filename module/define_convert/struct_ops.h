/*************************************************
*       Hige security Linux Operating System Project
*
*	File description: 	Definition of data describe struct header file
*	File name:		struct_deal.h
*	date:    	2008-05-09
*	Author:    	Hu jun
*************************************************/
#ifndef  STRUCT_OPS_H
#define  STRUCT_OPS_H

struct elem_template
{
	struct struct_elem_attr * elem_desc;
	int offset;    // the offset of this elem compare to the father
	int size;      // this elem's size in the struct
	int tempsize;  // an value just used one time,
	void * ref;    // a pointer to the ref value	
	void * def;    // a pointer point to this elem's define value	
	int flag;      // used to select some elem in this struct
	int index;     // count the elem in an array  
	int limit;     // count the elem's size
	struct elem_template * father; // this elem's father elem
}ELEM_NODE;

typedef struct struct_template_node
{
	void * parent;   // this struct's parent node ,NULL when this node is root node
	int offset;      // this struct's offset
	int size;        // this struct's size
	int elem_no;     // count which elem is the current elem it deal with
	int flag;        // used to select some elem in this struct
	struct struct_elem_attr * struct_desc;   // the describe array of this struct
	struct elem_template * elem_list;        // this struct's elem node list 
	int temp_var;   // a temp value in this struct node
	
}STRUCT_NODE;

struct struct_recordtype
{
	int type;
	int subtype;
	BYTE uuid[DIGEST_SIZE];
	int flag_no;
	struct flag_index * index;
}__attribute__((packed)) ;
#endif
