#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <windows.h>
#include "SharedMem.h"
#include "rtems.h"
#include <unistd.h>

#define BUF_SIZE 128
TCHAR szName[]=TEXT("Global\MyFileMappingObject");
HANDLE hMapFile;
INT8U* pBuf;

void CreateSharedMemory();
void OpenSharedMemory();
void WriteSharedMemory(char *Data, int How);
int ReadSharedMemory(char *Data);
void CloseSharedMemory();


int main(void) {
	
	unsigned char buffer[7] = {0x00};
	unsigned char ident = 0x00, code = 0x00;
	
	unsigned int counter = 0, handler= 0;
	
	OpenSharedMemory(2);
	while (TRUE) {
		unsigned short key_code = 0x00;
		key_code = ReadSharedMemory(buffer);
		ident = buffer[3];
		code = buffer[4];
		key_code = code | ident << 8;
		counter++;
		if (code > 0) {
			printf("Buffer: [%c%c%c%c%c%c%c], keycode=[%2x %2x], i=%d, handlers=%d, KEY=%4x\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], ident, code, counter, handler, key_code);
			handler++;
		}
	
//		if (code > 0)
//			Sleep(5000);
		
		//Sleep((unsigned int)100);
		if (BUF_SIZE/3>=256)
			if (handler>=BUF_SIZE/3) {
				CloseSharedMemory();
				OpenSharedMemory(2);
				handler=0;
				counter=0;
			}
		
		memset(&buffer, 0x00, sizeof(buffer));	
	//	counter = handler = 0;	
	}
	
	
	
	return 0;
}





void CreateSharedMemory() //?????????? ?? ??????????
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

void OpenSharedMemory()	//?????????? ? ???????
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
	*pHead=2;
	*pTail=2;
}

void WriteSharedMemory(char *Data,int How) //?????? ??????
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

int ReadSharedMemory(char *Data)	//?????? ??????
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

void CloseSharedMemory()	//???????? ?????? ????? ??????
{
	if(pBuf)
		UnmapViewOfFile(pBuf);
	if(hMapFile)
		CloseHandle(hMapFile);
}

