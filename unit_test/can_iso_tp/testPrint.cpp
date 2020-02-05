#include <stdio.h>  
#include <stdarg.h>  
#include <string.h>  

#include "testPrint.h"


bool print_enable = false;

void testPrintf(const char *cmd, ...)
{
	if (print_enable)
	{
		va_list args;       //����һ��va_list���͵ı������������浥������  
		va_start(args, cmd); //ʹargsָ��ɱ�����ĵ�һ������  
		vprintf(cmd, args);  //������vprintf�ȴ�V��  
		va_end(args);       //�����ɱ�����Ļ�ȡ
		int n = strlen(cmd);
		if(n>=1 && cmd[n-1] != '\n')
			printf("\n");
	}
}