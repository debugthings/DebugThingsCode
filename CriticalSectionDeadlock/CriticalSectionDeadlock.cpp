// CriticalSectionDeadlock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

HANDLE hStdOut;
CRITICAL_SECTION cs1;
CRITICAL_SECTION cs2;

DWORD WINAPI Thread1(LPVOID lpParam);
DWORD WINAPI Thread2(LPVOID lpParam);




int _tmain(int argc, _TCHAR* argv[])
{
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	InitializeCriticalSection(&cs1);
	InitializeCriticalSection(&cs2);
	DWORD tid1;
	DWORD tid2;
	HANDLE t1 = CreateThread(NULL, 0, Thread1, NULL, 0, &tid1);
	HANDLE t2 = CreateThread(NULL, 0, Thread2, NULL, 0, &tid2);
	HANDLE multiHandle[2] = { t1, t2 };
	WaitForMultipleObjects(2, multiHandle, TRUE, INFINITE);
	return 0;
}

DWORD WINAPI  Thread1(LPVOID lpParam)
{
	int i = 0;
	do
	{
		EnterCriticalSection(&cs2);
		Sleep(5); // Artificial do work
		EnterCriticalSection(&cs1);
		SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
		std::cout << ++i << "Thread 1" << std::endl;
		Sleep(250);
		LeaveCriticalSection(&cs2);
		LeaveCriticalSection(&cs1);
	} while (true);
}

DWORD WINAPI  Thread2(LPVOID lpParam)
{
	int i = 0;
	do
	{
		EnterCriticalSection(&cs1);
		Sleep(5); // Artificial do work
		EnterCriticalSection(&cs2);
		
		SetConsoleTextAttribute(hStdOut, FOREGROUND_BLUE);
		std::cout << ++i << " Thread 2" << std::endl;
		Sleep(250);
		LeaveCriticalSection(&cs1);
		LeaveCriticalSection(&cs2);
		
	} while (true);
}