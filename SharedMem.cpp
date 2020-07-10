#include <windows.h>
//#include <Classes.hpp>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include "SharedMem.h"


#define BUF_SIZE 256
TCHAR szName[]=TEXT("Global\MyFileMappingObject");
HANDLE hMapFile;
INT8U* pBuf;

void CreateSharedMemory() //вызывается из клавиатуры
{
   hMapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security
                 PAGE_READWRITE,          // read/write access
                 0,                       // maximum object size (high-order DWORD)
                 BUF_SIZE,                // maximum object size (low-order DWORD)
                 szName);                 // name of mapping object
   if (hMapFile == NULL)
   {
      printf(TEXT("Could not create file mapping object (%d).\n"),
             GetLastError());
      return;
   }
   pBuf = (INT8U*) MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
                        0,
                        0,
                        BUF_SIZE);
   if (pBuf == NULL)
   {
      printf(TEXT("Could not map view of file (%d).\n"),
             GetLastError());

      CloseHandle(hMapFile);
	  hMapFile=NULL;
	  return;
   }

	INT8U *pHead=pBuf;
	INT8U *pTail=pBuf+1;
	*pHead=0;
	*pTail=0;
}

// input: 2 - establish connect, 0 - for error
void OpenSharedMemory(int status)	//вызывается в проекте
{
   hMapFile = OpenFileMapping(
                   FILE_MAP_ALL_ACCESS,   // read/write access
                   FALSE,                 // do not inherit the name
                   szName);               // name of mapping object
   if (hMapFile == NULL)
   {
      printf(TEXT("Could not open file mapping object (%d).\n"),
             GetLastError());
      return ;
   }

   pBuf = (INT8U*) MapViewOfFile(hMapFile, // handle to map object
               FILE_MAP_ALL_ACCESS,  // read/write permission
               0,
               0,
               BUF_SIZE);

   if (pBuf == NULL)
   {
      printf(TEXT("Could not map view of file (%d).\n"),
             GetLastError());
      CloseHandle(hMapFile);
	  hMapFile=NULL;
	  return;
   }

	INT8U *pHead=pBuf;
	INT8U *pTail=pBuf+1;
	*pHead=status;
	*pTail=status;
}

void WriteSharedMemory(char *Data,int How) //запись данных
{
	if (pBuf == NULL) return;

	INT8U *pHead=pBuf;
	INT8U *pTail=pBuf+1;
	INT8U *pData=pBuf+2;
	INT8U Head=*pHead;
	INT8U Tail=*pTail;
	if((Head < 2) || (Tail < 2)) return;

	INT8U Len=BUF_SIZE - Head;
	if(How <= Len)
	{
		memcpy(pBuf+Head,Data,How);
	}
	else
	{
		memcpy(pBuf+Head,Data,Len);
		memcpy(pBuf+2,Data+Len,How-Len);
	}
	Head+=How;
	if(Head >= BUF_SIZE)
		Head -= (BUF_SIZE-2);
	*pHead = Head;
}

int ReadSharedMemory(char *Data)	//чтение данных
{
	if (pBuf == NULL) return 0;

	INT8U *pHead=pBuf;
	INT8U *pTail=pBuf+1;
	INT8U *pData=pBuf+2;
	INT8U Head=*pHead;
	INT8U Tail=*pTail;
	if((Head < 2) || (Tail < 2) || (Head==Tail)) return 0;
	int How;
	if(Head > Tail)
	{
		memcpy(Data,pBuf+Tail,Head-Tail);
		How=Head-Tail;
	}
	else
	{
		memcpy(Data,pBuf+Tail,BUF_SIZE-Tail);
		memcpy(Data+(BUF_SIZE-Tail),pBuf+2,Head-2);
		How=(BUF_SIZE-Tail)+(Head-2);
	}

	*pTail = Head;
	return How;
}

void CloseSharedMemory()	//закрытие канала общей памяти
{
	if(pBuf)
		UnmapViewOfFile(pBuf);
	if(hMapFile)
		CloseHandle(hMapFile);
}


