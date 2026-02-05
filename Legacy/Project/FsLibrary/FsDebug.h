#pragma once
#ifndef FSDEBUG_H
#define FSDEBUG_H


#include <stdio.h>

#ifdef AE_OS_WIN
	#include <Windows.h>
	#include <shlobj.h>
#else
#include <unistd.h>

#endif


//***********************************************************************
static void writeLog(char *s)
{
	FILE *fp;
	char *nm = (char*)"log.txt";

#ifdef AE_OS_WIN
	if ( fopen_s(&fp, nm,"a") ==0){
		fputs(s,fp);
		fputc('\n',fp);
		fclose(fp);
	}
#else
	fp = fopen(nm,"a");
	if (fp!=NULL) {
		fputs(s,fp);
		fputc('\n',fp);
		fclose(fp);
	}
#endif
}
//***********************************************************************
static void writeCurrentPath()
{
	char path[512] ="";
#ifdef AE_OS_WIN
	GetCurrentDirectory(512,path);
	writeLog(path);
	SHGetSpecialFolderPath(NULL,path,CSIDL_APPDATA,0);
	writeLog(path);
	GetModuleFileName(NULL,path,512);
	writeLog(path);
#else
	getcwd(path,512);
#endif
}




#endif
