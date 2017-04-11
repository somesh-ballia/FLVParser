// FLVTestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FLVContainer.h"
#include "CommonAll.h"
#include "FLV.h"
#include "FileWriter.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define INPUT_FILE_ONE "D:\\Youtube_flv\\2.flv"
#define INPUT_FILE_TWO "D:\\Youtube_flv\\1.flv"
#define INPUT_FILE_THREE "D:\\Youtube_flv\\3.flv"
#define OUTPUT_VIDEO_FILE_ONE "D:\\Youtube_flv\\123Video.flv"
#define OUTPUT_AUDIO_FILE_ONE "D:\\Youtube_flv\\123Audio.flv"

#define FORWARD_SEEK_LIMIT	1000

int TestMain()
{
	CFLVContainer FLVContainer1;
	CFLVContainer FLVContainer2;
	CFLVContainer FLVContainer3;
	if (SUCCESS == FLVContainer1.Initialize((u8*)INPUT_FILE_ONE,strlen(INPUT_FILE_ONE),FORWARD_SEEK_LIMIT) \
	   && SUCCESS == FLVContainer2.Initialize((u8*)INPUT_FILE_TWO,strlen(INPUT_FILE_TWO),FORWARD_SEEK_LIMIT) \
	   && SUCCESS == FLVContainer3.Initialize((u8*)INPUT_FILE_THREE,strlen(INPUT_FILE_THREE),FORWARD_SEEK_LIMIT))
	{
		CFileWriter	FileWriter;
		if (SUCCESS == FileWriter.Initialize(OUTPUT_AUDIO_FILE_ONE,strlen(OUTPUT_AUDIO_FILE_ONE),OUTPUT_VIDEO_FILE_ONE,strlen(OUTPUT_VIDEO_FILE_ONE)))
		{
			u8 byBuffer[BUFFER_SIZE]  = {0};
			u8* byChunkBuffer = byBuffer;
			u8	byRetGetChunk = SUCCESS;
			u16 unChunkSize = 0;			
			FILE	*pf_videoWrite = fopen(OUTPUT_VIDEO_FILE_ONE,"ab+");
			FILE	*pf_audioWrite = fopen(OUTPUT_AUDIO_FILE_ONE,"ab+");
			if (NULL != pf_audioWrite && NULL != pf_videoWrite)
			{		
				// for file 1
				while(SUCCESS == byRetGetChunk)
				{
					CFLVChunk	FLVChunk;
					byRetGetChunk = FLVContainer1.GetChunk(FLVChunk);	
					//if(FLVChunk.m_enmChunkType != FLV_CHUNK_META)
					{
						printf("\n1A TS:%d FD: %d",FLVChunk.m_pobjFLVTag.m_uiTimeStamp,FLVContainer1.GetFrameDuration());
						FileWriter.WriteData(FLVChunk);	
					}
				}

				// for file 2
				byRetGetChunk = SUCCESS;
				while(SUCCESS == byRetGetChunk)
				{
					CFLVChunk	FLVChunk;
					byRetGetChunk = FLVContainer2.GetChunk(FLVChunk);
					if(FLVChunk.m_enmChunkType != FLV_CHUNK_META)
					{
						printf("\n2A TS:%d FD: %d",FLVChunk.m_pobjFLVTag.m_uiTimeStamp,FLVContainer2.GetFrameDuration());
						FileWriter.WriteData(FLVChunk);	
					}
						
				}

				// for file 3
				byRetGetChunk = SUCCESS;
				while(SUCCESS == byRetGetChunk)
				{
					CFLVChunk	FLVChunk;
					byRetGetChunk = FLVContainer3.GetChunk(FLVChunk);
					if(FLVChunk.m_enmChunkType != FLV_CHUNK_META)
					{
						printf("\n3A TS:%d FD: %d",FLVChunk.m_pobjFLVTag.m_uiTimeStamp,FLVContainer3.GetFrameDuration());
						FileWriter.WriteData(FLVChunk);	
					}

				}
				fclose(pf_videoWrite);
				fclose(pf_audioWrite);
			}
		}
		FLVContainer1.Deinitialize();
		FileWriter.DeInitialize();
	}
	else
	{
		
	}
	printf("\nDONE");
	_getch();
	return 0;
}


