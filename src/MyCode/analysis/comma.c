#include "comma.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define OUT_OF_REGION 0

/*����ַ�������ĸ���,��fmtΪ�ָ���*/
int GetRegionNumByFmt( char *sString, char fmt )
{
	int i , nNum = 0;
	/*���Կ�ͷ���߽�β�ķָ���*/
	for( i=1; i<strlen(sString)-1; i++ )
	{
		if(sString[i] == fmt ) nNum++;
	}
	if( nNum != 0 ) return ++nNum;
	return 0;
}

/*��õ�nCount�������,��fmtΪ�ָ���������õ���������sStringOutput��*/
void GetCountRegionByFmt( char *sStringInput, char fmt, int nCount, char *sStringOutput )
{
	int i , nNum = 0;
	char *sStart = NULL;
	char *sEnd   = NULL;

	/*��õ�nCount-1���ָ�����λ��,��ͷ�ͽ�β�ĺ���*/
	for( i=1; i<strlen(sStringInput)-1; i++ )
	{
		if(sStringInput[i] == fmt ) nNum++;

		if( nCount == 1 )
		{
			if( sStringInput[0] == fmt ) sStart = &(sStringInput[1]);
			else sStart = &(sStringInput[0]);
			break;
		}

		if( nNum == nCount-1) 
		{
			sStart = &(sStringInput[i]);
			sStart++;
			break;
		}
	}

	sEnd = strchr( sStart, fmt );
	
	if( sEnd == NULL )
	{
		strcpy( sStringOutput, sStart );
		return;
	}

	memcpy( sStringOutput, sStart, sEnd-sStart);
	
}

/*����ַ����е���fmtΪ�ָ����ĵ�nCount����,�����������*/
int GetRegionByFmt( char *sStringInput, char fmt, int nCount, char *sStringOutput )
{
	int nTotalRegion = 0;
	char *sStart = NULL;
	char *sEnd   = NULL;

	if( sStringInput == NULL ) return -1;

	/*����ַ�����������ܸ���*/
	nTotalRegion = GetRegionNumByFmt(sStringInput, fmt);
	printf("������Ϊ[%d]\n", nTotalRegion);

	/*���nCount����nTotalRegion����С��1���򷵻ؿմ�*/
	if( (nCount < 1) || (nCount > nTotalRegion) ) return nTotalRegion;
	
	/*��õ�nCount�������*/
	GetCountRegionByFmt( sStringInput, fmt, nCount, sStringOutput );

	return nTotalRegion;
	
}


/*��ȡ��һ���������*/
/*sCurContent ��ǰ�������, nCount ��ǰ�����ţ�sNextContent��һ���������, ������һ��������*/
/*nTotalRegion�������*/
int GetNextRegionByFmt( char *sStringInput, char fmt, int nCount, int nTotalRegion, char *sCurContent, char *sNextContent )
{
	char *sStart = NULL;
	char *sTemp  = NULL;
	char *sEnd   = NULL;

	if( sStringInput == NULL ) return -1;

	/*�����һ�����Count����nTotalRegion����С��1���򷵻ؿմ�*/
	if( (nCount+1 < 1) || (nCount+1 > nTotalRegion) ) return OUT_OF_REGION;


	/*�����ǰ����sStringInput����Ψһ�ģ���ôֱ��ȡ������һ����*/
	sStart = strstr(sStringInput, sCurContent);
	if( sStart == NULL ) return -1; /*��ǰ���ڴ���*/

	sStart = sStart+strlen(sCurContent);
	if( strstr(sStart, sCurContent) == NULL) /*��ǰ���ڴ�����Ψһ��*/
	{
		sStart++;
		sEnd   = strchr( sStart, fmt);
		if( sEnd == NULL ) strcpy( sNextContent, sStart);
		else memcpy( sNextContent, sStart, sEnd-sStart);
		
	}
	else /*�������GetRegionByFmt��õ�nCount+1�������*/
	{
		GetRegionByFmt( sStringInput, fmt, nCount+1, sNextContent);	
	}
	
	return nCount+1;
}
