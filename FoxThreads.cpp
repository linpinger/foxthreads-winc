#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

// ��СExe���
#pragma comment(lib,"msvcrt.lib")
#pragma comment(linker,"/ALIGN:4096" )

// ����ṹ�Ǵ��ݸ��̵߳Ĳ���
typedef struct {
	int index;
} FoxArg, * pFoxArg;


DWORD _stdcall iamThread(LPVOID lpParameter)
{
	FoxArg tArg = {0};
	int taskindex = 0;

	char * nowenv ;
	char nowEnvName[] = "FoxTask00" ;

	MoveMemory(&tArg, lpParameter, sizeof(tArg));
	taskindex = tArg.index ;

	sprintf(nowEnvName, "FoxTask%i", taskindex);
	nowenv = getenv(nowEnvName) ;

	printf("�߳�%d��ʼ: %s\n", taskindex, nowenv) ;

	// system(nowenv) ;

	STARTUPINFO si = {sizeof(si)};
	PROCESS_INFORMATION pi;
	CreateProcess(NULL, nowenv, NULL, NULL, false, 0, NULL, NULL, &si, &pi) ;
	WaitForSingleObject(pi.hProcess, INFINITE);

	printf("�߳�%d����: %s\n", taskindex, nowenv) ;

	return 0;
}

int main(int argc, char* argv[])
{
	HANDLE h_thread[55] ;
	FoxArg tArg[55] ;

	int taskCount = 0 ; // ������
	taskCount = atoi(getenv("FoxTaskCount")) ;
//	printf("������=<%d>\n", taskCount) ;
	

	for (int i=0; i<taskCount; i++) {
//		printf("aa:%d\n", i);
		tArg[i].index = i + 1 ;
		h_thread[i] = CreateThread(NULL, 0, iamThread, &(tArg[i]), 0, NULL);
	}

	for (i=0; i<=taskCount; i++) {
		WaitForSingleObject(h_thread[i],INFINITE);  
		CloseHandle(h_thread[i]);  
	}

//	system("pause");
	return 0 ;
}
