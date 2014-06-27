#自写小工具　方便　多线程执行程序

**功能:** 多线程执行程序

**作者:** 爱尔兰之狐(linpinger)

**邮箱:** <mailto:linpinger@gmail.com>

**缘起:** AHK 在并行方面有点吃亏，H版又不怎么会用，于是想到了用C写个多线程执行命令的办法

## 原理与用法:

- 设置环境变量FoxTaskCount表示并行任务数
- 设置环境变量FoxTask1表示第一个任务要执行的命令，依次类推
- 然后运行用C写的程序
- 本程序会读取环境变量，开FoxTaskCount个线程执行任务，每个任务获取各自编号的FoxTaskN中的字符串，并用CreateProcess执行，等待执行完毕
- 一开始没用CreateProcess，使用的是system命令，这样每个线程都有两个子进程，一个cmd.exe，一个命令进程

### linux平台bash类似办法(没测试过)

- 后台执行for循环中的命令
- wait命令等待所有后台命令执行完毕

### windows平台类似办法(貌似不等待)

- start xxx.bat
- 要等待这些批处理执行完毕貌似很蛋疼，网上搜到的方法是VBS或WMI等等，不方便
- bat方法开的进程多了点，每个bat都多一个cmd.exe

*****

## 调用例子

### AHK 例子

```
	EnvSet, FoxTaskCount, 2
	EnvSet, FoxTask1, wget -O c:\m1.html "http://www.autohotkey.com"
	EnvSet, FoxTask2, wget -O c:\m2.html "http://www.ahkscript.org"
	runwait, FoxThreads.exe
```

### BAT 例子

```
set FoxTaskCount=2
set FoxTask1=wget -O c:\m1.html "http://www.autohotkey.com"
set FoxTask2=wget -O c:\m2.html "http://www.ahkscript.org"
FoxThreads.exe
```

*****

## C语言源码(VC6编译)

```
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

// 减小Exe体积
#pragma comment(lib,"msvcrt.lib")
#pragma comment(linker,"/ALIGN:4096" )

// 这个结构是传递给线程的参数
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

	printf("线程%d开始: %s\n", taskindex, nowenv) ;

	// system(nowenv) ;

	STARTUPINFO si = {sizeof(si)};
	PROCESS_INFORMATION pi;
	CreateProcess(NULL, nowenv, NULL, NULL, false, 0, NULL, NULL, &si, &pi) ;
	WaitForSingleObject(pi.hProcess, INFINITE);

	printf("线程%d结束: %s\n", taskindex, nowenv) ;

	return 0;
}

int main(int argc, char* argv[])
{
	HANDLE h_thread[55] ;
	FoxArg tArg[55] ;

	int taskCount = 0 ; // 任务数
	taskCount = atoi(getenv("FoxTaskCount")) ;
//	printf("任务数=<%d>\n", taskCount) ;
	

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
