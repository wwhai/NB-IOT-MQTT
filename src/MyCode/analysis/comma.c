#include "comma.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define OUT_OF_REGION 0

/*获得字符串中域的个数,以fmt为分隔符*/
int GetRegionNumByFmt( char *sString, char fmt )
{
	int i , nNum = 0;
	/*忽略开头和者结尾的分隔符*/
	for( i=1; i<strlen(sString)-1; i++ )
	{
		if(sString[i] == fmt ) nNum++;
	}
	if( nNum != 0 ) return ++nNum;
	return 0;
}

/*获得第nCount域的内容,以fmt为分隔符，将获得的内容填入sStringOutput中*/
void GetCountRegionByFmt( char *sStringInput, char fmt, int nCount, char *sStringOutput )
{
	int i , nNum = 0;
	char *sStart = NULL;
	char *sEnd   = NULL;

	/*获得第nCount-1个分隔符的位置,开头和结尾的忽略*/
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

/*获得字符串中的以fmt为分隔符的第nCount个域,返回域的总数*/
int GetRegionByFmt( char *sStringInput, char fmt, int nCount, char *sStringOutput )
{
	int nTotalRegion = 0;
	char *sStart = NULL;
	char *sEnd   = NULL;

	if( sStringInput == NULL ) return -1;

	/*获得字符串中子域的总个数*/
	nTotalRegion = GetRegionNumByFmt(sStringInput, fmt);
	printf("域总数为[%d]\n", nTotalRegion);

	/*如果nCount大于nTotalRegion或者小于1，则返回空串*/
	if( (nCount < 1) || (nCount > nTotalRegion) ) return nTotalRegion;
	
	/*获得第nCount域的内容*/
	GetCountRegionByFmt( sStringInput, fmt, nCount, sStringOutput );

	return nTotalRegion;
	
}


/*获取下一个域的内容*/
/*sCurContent 当前域的内容, nCount 当前域的域号，sNextContent下一个域的内容, 返回下一个域的域号*/
/*nTotalRegion域的总数*/
int GetNextRegionByFmt( char *sStringInput, char fmt, int nCount, int nTotalRegion, char *sCurContent, char *sNextContent )
{
	char *sStart = NULL;
	char *sTemp  = NULL;
	char *sEnd   = NULL;

	if( sStringInput == NULL ) return -1;

	/*如果下一个域的Count大于nTotalRegion或者小于1，则返回空串*/
	if( (nCount+1 < 1) || (nCount+1 > nTotalRegion) ) return OUT_OF_REGION;


	/*如果当前域在sStringInput中是唯一的，那么直接取他的下一个域*/
	sStart = strstr(sStringInput, sCurContent);
	if( sStart == NULL ) return -1; /*当前域不在串中*/

	sStart = sStart+strlen(sCurContent);
	if( strstr(sStart, sCurContent) == NULL) /*当前域在串中是唯一的*/
	{
		sStart++;
		sEnd   = strchr( sStart, fmt);
		if( sEnd == NULL ) strcpy( sNextContent, sStart);
		else memcpy( sNextContent, sStart, sEnd-sStart);
		
	}
	else /*否则调用GetRegionByFmt获得第nCount+1域的内容*/
	{
		GetRegionByFmt( sStringInput, fmt, nCount+1, sNextContent);	
	}
	
	return nCount+1;
}
