#include <string.h>
//printable characters
  static char prtchr[]={32,33,34,35,36,37,38,39,40,41, \
		 42,43,44,45,46,47,48,49,50,51, \
		 52,53,54,55,56,57,58,59,60,61, \
		 62,63,64,65,66,67,68,69,70,71, \
		 72,73,74,75,76,77,78,79,80,81, \
		 82,83,84,85,86,87,88,89,90,91, \
		 92,93,94,95,96,97,98,99,100,101, \
		 101,103,104,105,106,107,108,109,110, \
		 111,112,113,114,115,116,117,118,119, \
		 120,121,122,123,124,125,126};//char a='x'is equivalent to char a=120, but char a='120' is nothing but a fault!

void bwt(char *str,int len,int *result)
{
  /* another form of Burrow Wheller Transform */
  int prtchrNum=strlen(prtchr);
  int i=0,j=0,x=0;
  for(i=0;i<prtchrNum;i++){
    char ch=prtchr[i];
    for(j=0;j<len;j++){
      if(str[j] == ch)
	result[x++]=(j==(len-1)?0:(j+1));//j-1 is ok
    }
  }
}
void sortstring(char *str,int len,int *result)
{
  int prtchrNum=strlen(prtchr);
  int i=0,j=0,x=0;
  for(i=0;i<prtchrNum;i++){
    char ch=prtchr[i];
    for(j=0;j<len;j++){
      if(str[j] == ch)
	result[x++]=j;
    }
  }
}

