// FLVTestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "FLVContainer.h"
#include "FLVCommonAll.h"
#include "FLVDependencies.h"
#include "FileWriter.h"

//#define INPUT_FILE "D:\\Youtube_flv\\123.flv"
#define INPUT_FILE "D:\\Youtube_flv\\Single_file_two\\Outfile.raw"
#define OUTPUT_VIDEO_FILE "D:\\Youtube_flv\\123VideoOUT.flv"
#define OUTPUT_AUDIO_FILE "D:\\Youtube_flv\\123Audio.flv"

#define FORWARD_SEEK_LIMIT	1000
int main(int argc, char* argv[])
{
	int MaxDuration = 0;
	CFLVContainer FLVContainer;
	if (SUCCESS == FLVContainer.Initialize((u8*)INPUT_FILE,strlen(INPUT_FILE),FORWARD_SEEK_LIMIT))
	{
		CFileWriter	FileWriter;
		if (SUCCESS == FileWriter.Initialize(OUTPUT_AUDIO_FILE,strlen(OUTPUT_AUDIO_FILE),OUTPUT_VIDEO_FILE,strlen(OUTPUT_VIDEO_FILE)))
		{
			u8 byBuffer[BUFFER_SIZE]  = {0};
			u8* byChunkBuffer = byBuffer;
			u8	byRetGetChunk = SUCCESS;
			u16 unChunkSize = 0;			
			FILE	*pf_videoWrite = fopen(OUTPUT_VIDEO_FILE,"ab+");
			FILE	*pf_audioWrite = fopen(OUTPUT_AUDIO_FILE,"ab+");
			if (NULL != pf_audioWrite && NULL != pf_videoWrite)
			{		
				while(SUCCESS == byRetGetChunk)
				{
					CFLVChunk FLVChunk;
					byRetGetChunk = FLVContainer.GetChunk(FLVChunk);
					printf("\nA TS:%d FD: %d",FLVChunk.m_pobjFLVTag.m_uiTimeStamp,FLVChunk.m_dwFrameDuration);
					if(FLVChunk.m_dwFrameDuration > MaxDuration)
					{
						MaxDuration = FLVChunk.m_dwFrameDuration;
					}
					FileWriter.WriteData(FLVChunk);	
					//FLVChunk.ReInitialize();
				}

				fclose(pf_videoWrite);
				fclose(pf_audioWrite);
			}
		}
		FLVContainer.Deinitialize();
		FileWriter.DeInitialize();
	}
	else
	{

	}
	printf("\nMAX DUration : %d",MaxDuration);
	_getch();	
	return 0;
}


