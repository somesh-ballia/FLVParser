// FLVTestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FLVContainer.h"
#include "CommonAll.h"
#include "FLV.h"
#include "FileWriter.h"
#include "TestMerge.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

//#define INPUT_FILE "D:\\Youtube_flv\\123.flv"
#define INPUT_FILE "D:\\MMFSSVideoReader\\FLV\\TC2_final\\MediaData.flv"
#define OUTPUT_VIDEO_FILE "D:\\MMFSSVideoReader\\FLV\\TC2_final\\1Video.flv"
#define OUTPUT_AUDIO_FILE "D:\\MMFSSVideoReader\\FLV\\TC2_final\\1Audio.flv"

#define FORWARD_SEEK_LIMIT	1000
int main(int argc, char* argv[])
{
	//m! for split test
	int iCount = 0;
	bool bWriteData = false;
	bool bIsFirstVideoPacket = true;
	//m!
	int MaxDuration = 0;
	if (false)
	{
		// to merge file
		TestMain();
	}
	else
	{
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

						if(0 <= iCount)
						{
							bWriteData = true;
						}

						if(FLVChunk.m_enmChunkType != FLV_CHUNK_META)
						{
							if(bIsFirstVideoPacket && FLVChunk.m_enmChunkType == FLV_CHUNK_VIDEO)
							{
								printf("\nA TS:%d FD: %d",FLVChunk.m_pobjFLVTag.m_uiTimeStamp,FLVChunk.m_dwFrameDuration);
								if(FLVChunk.m_dwFrameDuration > MaxDuration)
								{
									MaxDuration = FLVChunk.m_dwFrameDuration;
								}
								FileWriter.WriteData(FLVChunk);	
								bIsFirstVideoPacket = false;
							}
							else
							{
								if(bWriteData)
								{
									printf("\nA TS:%d FD: %d",FLVChunk.m_pobjFLVTag.m_uiTimeStamp,FLVChunk.m_dwFrameDuration);
									if(FLVChunk.m_dwFrameDuration > MaxDuration)
									{
										MaxDuration = FLVChunk.m_dwFrameDuration;
									}
									FileWriter.WriteData(FLVChunk);	
								}
								
							}
														
						}

						FLVChunk.ReInitialize();						
						iCount++;
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
	}
	
	return 0;
}


