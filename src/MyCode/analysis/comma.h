#ifndef __GPIO_H
#define __GPIO_H

/*����ַ�������ĸ���,��fmtΪ�ָ���*/
int GetRegionNumByFmt( char *sString, char fmt );
/*��õ�nCount�������,��fmtΪ�ָ���������õ���������sStringOutput��*/
void GetCountRegionByFmt( char *sStringInput, char fmt, int nCount, char *sStringOutput );
/*����ַ����е���fmtΪ�ָ����ĵ�nCount����,�����������*/
int GetRegionByFmt( char *sStringInput, char fmt, int nCount, char *sStringOutput );
/*��ȡ��һ���������*/
/*sCurContent ��ǰ�������, nCount ��ǰ�����ţ�sNextContent��һ���������, ������һ��������*/
/*nTotalRegion�������*/
int GetNextRegionByFmt( char *sStringInput, char fmt, int nCount, int nTotalRegion, char *sCurContent, char *sNextContent );


#endif