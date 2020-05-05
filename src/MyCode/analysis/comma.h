#ifndef __GPIO_H
#define __GPIO_H

/*获得字符串中域的个数,以fmt为分隔符*/
int GetRegionNumByFmt( char *sString, char fmt );
/*获得第nCount域的内容,以fmt为分隔符，将获得的内容填入sStringOutput中*/
void GetCountRegionByFmt( char *sStringInput, char fmt, int nCount, char *sStringOutput );
/*获得字符串中的以fmt为分隔符的第nCount个域,返回域的总数*/
int GetRegionByFmt( char *sStringInput, char fmt, int nCount, char *sStringOutput );
/*获取下一个域的内容*/
/*sCurContent 当前域的内容, nCount 当前域的域号，sNextContent下一个域的内容, 返回下一个域的域号*/
/*nTotalRegion域的总数*/
int GetNextRegionByFmt( char *sStringInput, char fmt, int nCount, int nTotalRegion, char *sCurContent, char *sNextContent );


#endif