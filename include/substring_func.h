#ifndef SUBSTRING_FUNC_H
#define SUBSTRING_FUNC_H

int find_substring(char * string, int maxstrlen,char * substring,int maxsublen);
int replace_substring(char * string, int maxstrlen,char * substring,int maxsublen,char * replacestring,int maxreplacelen);
int upper_substring(char * string,int maxstrlen,int site,int len);
int lower_substring(char * string,int maxstrlen,int site,int len);
int getpara_from_substring(char * string, int maxstrlen,char front, char end,char * para);
#endif
