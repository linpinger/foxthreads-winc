#��дС���ߡ����㡡���߳�ִ�г���

**����:** ���߳�ִ�г���

**����:** ������֮��(linpinger)

**����:** <mailto:linpinger@gmail.com>

**Ե��:** AHK �ڲ��з����е�Կ���H���ֲ���ô���ã������뵽����Cд�����߳�ִ������İ취

## ԭ�����÷�:

- ���û�������FoxTaskCount��ʾ����������
- ���û�������FoxTask1��ʾ��һ������Ҫִ�е������������
- Ȼ��������Cд�ĳ���
- ��������ȡ������������FoxTaskCount���߳�ִ������ÿ�������ȡ���Ա�ŵ�FoxTaskN�е��ַ���������CreateProcessִ�У��ȴ�ִ�����
- һ��ʼû��CreateProcess��ʹ�õ���system�������ÿ���̶߳��������ӽ��̣�һ��cmd.exe��һ���������

### linuxƽ̨bash���ư취(û���Թ�)

- ��ִ̨��forѭ���е�����
- wait����ȴ����к�̨����ִ�����

### windowsƽ̨���ư취(ò�Ʋ��ȴ�)

- start xxx.bat
- Ҫ�ȴ���Щ������ִ�����ò�ƺܵ��ۣ������ѵ��ķ�����VBS��WMI�ȵȣ�������
- bat�������Ľ��̶��˵㣬ÿ��bat����һ��cmd.exe

*****

## ��������

### AHK ����

```
	EnvSet, FoxTaskCount, 2
	EnvSet, FoxTask1, wget -O c:\m1.html "http://www.autohotkey.com"
	EnvSet, FoxTask2, wget -O c:\m2.html "http://www.ahkscript.org"
	runwait, FoxThreads.exe
```

### BAT ����

```
set FoxTaskCount=2
set FoxTask1=wget -O c:\m1.html "http://www.autohotkey.com"
set FoxTask2=wget -O c:\m2.html "http://www.ahkscript.org"
FoxThreads.exe
```

*****

## C����Դ��(VC6����)

```
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

```
