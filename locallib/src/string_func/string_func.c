#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "data_type.h"
#include "errno.h"
#include "alloc.h"
#include "memfunc.h"
#include "basefunc.h"
#include "struct_deal.h"
#include "crypto_func.h"
#include "memdb.h"
#include "message.h"

int find_substring(char * string, int maxstrlen,char * substring,int maxsublen)
{
	int ret;
	int i;
	int strlen,substrlen;
	if(maxstrlen==0)
		strlen=Strlen(string);
	else
		strlen=Strnlen(string,maxstrlen);
	if(strlen<0)
		return strlen;

	if(maxsublen==0)
		substrlen=Strlen(substring);
	else
		substrlen=Strnlen(substring,maxsublen);
	if(substrlen<0)
		return substrlen;

	if(strlen<substrlen)
		return 0;

	for(i=0;i<=strlen-substrlen;i++)
	{
		if(Memcmp(string+i,substring,substrlen)==0)
			return i+1;
	}
	return 0;
}

int __move_string_tail(char * string, int maxstrlen, int site,int new_site)
{
	int strlen;
	int i;

	if(maxstrlen==0)
		strlen=Strlen(string);
	else
		strlen=Strnlen(string,maxstrlen);
	if(site==new_site)
		return strlen;

	if(site<new_site)
	{
		if(strlen+new_site-site>maxstrlen)
			return -EINVAL;
		for(i=strlen;i>=new_site;i--)
			string[i-1]=string[i+new_site-site];
		if(strlen<maxstrlen)
			string[strlen]=0;
	}
	else
	{
		for(i=site;i<=strlen;i++)
			string[i+new_site-site]=string[i];
		if(strlen<maxstrlen)
			string[strlen]=0;
	}
	return strlen;
}

int replace_substring(char * string, int maxstrlen,char * substring,int maxsublen,char * replacestring,int maxreplacelen)
{
	int ret;
	int i;
	int strlen,substrlen,replacelen;
	if(maxstrlen==0)
		strlen=Strlen(string);
	else
		strlen=Strnlen(string,maxstrlen);
	if(strlen<0)
		return strlen;

	if(maxsublen==0)
		substrlen=Strlen(substring);
	else
		substrlen=Strnlen(substring,maxsublen);
	if(substrlen<0)
		return substrlen;

	if(maxreplacelen==0)
		replacelen=Strlen(replacestring);
	else
		replacelen=Strnlen(replacestring,maxreplacelen);
	if(replacelen<0)
		return replacelen;

	if(strlen<substrlen)
		return 0;

	for(i=0;i<=strlen-substrlen;i++)
	{
		if(Memcmp(string+i,substring,substrlen)==0)
		{
			strlen=__move_string_tail(string,maxstrlen,i+substrlen,i+replacelen);
			if(strlen>maxstrlen)
				return -EINVAL;
			Memcpy(string+i,replacestring,replacelen);
			i+=replacelen-1;
		}
	}
	return strlen;
} 

int upper_substring(char * string, int maxstrlen,int site, int len)
{
	int ret;
	int i;
	int strlen,upperlen;

	if(maxstrlen==0)
		strlen=Strlen(string);
	else
		strlen=Strnlen(string,maxstrlen);
	if(strlen<0)
		return strlen;
	if(site>=strlen)
		return 0;
	upperlen=strlen-site;
	if(len!=0)
		if(len<upperlen)
			upperlen=len;
	for(i=site;i<site+upperlen;i++)
	{
		if((string[i]>='a')&&(string[i]<='z'))
			string[i]+='A'-'a';
	}
	return upperlen;		
}
 
int getpara_from_substring(char * string, int maxstrlen,char front, char end,char * para)
{
	int ret;
	int i;
	int strlen,paralen;
	int para_start;

	if(maxstrlen==0)
		strlen=Strlen(string);
	else
		strlen=Strnlen(string,maxstrlen);
	if(strlen<0)
		return strlen;

	para_start=0;
	paralen=0;

	if(end==0)
		return 0;
	if(front!=0)
	{
		for(i=0;string[i]!=front;i++)
		{
			if(i>=strlen)
				return 0;
			if(string[i]==0)
				return 0;
		}
		para_start=i+1;
	}
	for(i=para_start;i<strlen;i++)
	{
		if(string[i]==end)
		{
			paralen=i-para_start;
			break;			
		}
	}	

	Memcpy(para,string+para_start,paralen);
	para[paralen]=0;
	
	return para_start+paralen;	
}
