#include "StdAfx.h"
#include "FLVParser.h"
#include "FLV.h"
#include "FileIO.h"
#include <Winsock2.h>
#include <conio.h>
#include "BitConverter.h"

CFLVParser::CFLVParser(void):m_pobjCFileIO(NULL),m_bIsFirstPacket(true)
{
}


CFLVParser::~CFLVParser(void)
{
}


u8 CFLVParser::Initialize(u8* pszFileName,u16 unFileNameLength)
{
	u8 byRetValue = FAILURE;
	if (NULL == m_pobjCFileIO)
	{
		m_pobjCFileIO = new CFileIO();
		if(NULL != m_pobjCFileIO)
		{
			if(SUCCESS == m_pobjCFileIO->Initialize(pszFileName,unFileNameLength,m_dwFileSize,m_dwCurrentOffset))
			{
				//if(SUCCESS == ValidateFLVFile())
				{
					byRetValue = SUCCESS;
				}				
			}
		}
	}
	
	return byRetValue;
}

u8 CFLVParser::Deinitialize()
{
	u8 byRetValue = FAILURE;

	if(NULL != m_pobjCFileIO)
	{
		m_pobjCFileIO->Deinitialize();
		delete m_pobjCFileIO;
	}

	byRetValue = SUCCESS;

	return byRetValue;
}

u8 CFLVParser::ValidateFLVFile()
{
	u8 byRetValue = FAILURE;
	u32 temp = 0;
	CBitConverter bitConverter;
	u8 byBuffer[BUFFER_SIZE] = {0};
	u8* byBufferTemp = byBuffer;
	if(SUCCESS == m_pobjCFileIO->FetchBuffer(byBufferTemp,FLV_FILE_HEADER_SIZE))
	{	
		CFLVFileHeader* pobjFLVHeader = NULL;
		pobjFLVHeader = (CFLVFileHeader*)byBufferTemp;
		u32 UnSignature = bitConverter.ToUINT24(byBufferTemp,0);
		if(UnSignature == FLV_SIGNATURE)
		{
			if(FLV_VERSION == pobjFLVHeader->m_uiVersion)
			{
				byRetValue = SUCCESS;
			}			
		}		
	}
	return byRetValue;
}

u32 CFLVParser::GetPreviousTagSize()
{
	u32 dwPreviousTagSize = 0;
	u8 byBuffer[BUFFER_SIZE] = {0};
	u8* byBufferTemp = byBuffer;
	if (SUCCESS == m_pobjCFileIO->FetchBuffer(byBufferTemp,sizeof(u32)))
	{
		dwPreviousTagSize = m_bitConverter.ToUINT32(byBufferTemp,0);
		return dwPreviousTagSize;
	}
	return 0;
}

u8 CFLVParser::ReadTag(CFLVChunk &FLVChunk)
{
	u8 byRetValue = FAILURE;	
	
	m_dwCurrentOffset = m_pobjCFileIO->GetOffset();

	if(m_dwFileSize - m_dwCurrentOffset >= FLV_TAG_HEADER_SIZE)
	{
		
		if(!m_bIsFirstPacket)
		{
			FLVChunk.m_uiPreviousTagSize = GetPreviousTagSize();
		}
		else
		{
			m_bIsFirstPacket = false;
		}

		u8 byBuffer1[BUFFER_SIZE] = {0};
		u8* byBufferTemp1 = byBuffer1;

		u8 byBuffer2[BUFFER_SIZE] = {0};
		u8* byBufferTemp2 = byBuffer2;

		if (SUCCESS == m_pobjCFileIO->FetchBuffer(byBufferTemp1,FLV_TAG_HEADER_SIZE))
		{
			uint8 unData;
			u8* byDataBuffer = NULL;
			CFLVTags FLVTags;
			CFLVAudioData FLVAudio;
			CFLVVideoData FLVVideo;
			FLVTags.m_uiTagType = (unsigned _int8)byBufferTemp1[0];
			FLVTags.m_uiDataSize = m_bitConverter.ToUINT24(&byBufferTemp1[1],0);
			FLVTags.m_uiTimeStamp = m_bitConverter.ToUINT24(&byBufferTemp1[4],0);
			FLVTags.m_uiTimeStampExtended = (unsigned _int8)byBufferTemp1[7];
			FLVTags.m_uiStreamID = m_bitConverter.ToUINT24(&byBufferTemp1[8],0);

			if (0 == FLVTags.m_uiStreamID && 0 < FLVTags.m_uiTagType)
			{
				switch(FLVTags.m_uiTagType)
				{
				case FLV_TAG_TYPE_AUDIO:
					if (SUCCESS == m_pobjCFileIO->FetchBuffer(byBufferTemp2,FLV_AUDIO_HEADER_SIZE))
					{
						//unData = m_bitConverter.ToUINT8(&byBufferTemp2[0]);
						unData = byBufferTemp2[0];
						FLVAudio.m_sound = unData;
						//byDataBuffer = new u8[FLVTags.m_uiDataSize];
						if (SUCCESS == AllocateMemory(byDataBuffer,FLVTags.m_uiDataSize))
						{
							memset(byDataBuffer,0,FLVTags.m_uiDataSize);
							if(SUCCESS == m_pobjCFileIO->FetchBuffer(byDataBuffer,FLVTags.m_uiDataSize-FLV_AUDIO_HEADER_SIZE))
							{
								FLVChunk.m_enmChunkType = FLV_CHUNK_AUDIO;
								FLVChunk.m_pobjFLVTag = FLVTags;
								FLVChunk.m_pobjFLVAudio = FLVAudio;
								FLVChunk.m_pobjFLVVideo = FLVVideo;
								FLVChunk.m_pbyDataBuffer = byDataBuffer;
								FLVChunk.m_dwDataBufferLength = FLVTags.m_uiDataSize-FLV_AUDIO_HEADER_SIZE;
								byRetValue = SUCCESS;
							}
						}
						
					}
					break;
				case  FLV_TAG_TYPE_VIDEO:
					if (SUCCESS == m_pobjCFileIO->FetchBuffer(byBufferTemp2,FLV_VIDEO_HEADER_SIZE))
					{
						//unData = m_bitConverter.ToUINT8(byBufferTemp2);
						unData = byBufferTemp2[0];
						FLVVideo.m_video.video = unData;
						uint8 unTemp = FLVVideo.m_video.stVideo.uiCodecID;
						FLVVideo.m_video.stVideo.uiCodecID = FLVVideo.m_video.stVideo.uiFrameType;
						FLVVideo.m_video.stVideo.uiFrameType = unTemp;

						//byDataBuffer = new u8[FLVTags.m_uiDataSize];
						if (SUCCESS == AllocateMemory(byDataBuffer,FLVTags.m_uiDataSize))
						{
							memset(byDataBuffer,0,FLVTags.m_uiDataSize);
							if(SUCCESS == m_pobjCFileIO->FetchBuffer(byDataBuffer,FLVTags.m_uiDataSize-FLV_VIDEO_HEADER_SIZE))
							{
								FLVChunk.m_enmChunkType = FLV_CHUNK_VIDEO;
								FLVChunk.m_pobjFLVTag = FLVTags;
								FLVChunk.m_pobjFLVAudio = FLVAudio;
								FLVChunk.m_pobjFLVVideo = FLVVideo;
								FLVChunk.m_pbyDataBuffer = byDataBuffer;
								FLVChunk.m_dwDataBufferLength = FLVTags.m_uiDataSize-FLV_VIDEO_HEADER_SIZE;
								byRetValue = SUCCESS;
							}
						}

					}
					break;
				case FLV_TAG_TYPE_META:
					if(SUCCESS == m_pobjCFileIO->FetchBuffer(byBufferTemp2,FLVTags.m_uiDataSize))
					{
						//byDataBuffer = new u8[FLVTags.m_uiDataSize];
						if (SUCCESS == AllocateMemory(byDataBuffer,FLVTags.m_uiDataSize))
						{
							memset(byDataBuffer,0,FLVTags.m_uiDataSize);
							memcpy(byDataBuffer,byBufferTemp2,FLVTags.m_uiDataSize);
							FLVChunk.m_enmChunkType = FLV_CHUNK_META;
							FLVChunk.m_pobjFLVTag = FLVTags;
							FLVChunk.m_pobjFLVAudio = FLVAudio;
							FLVChunk.m_pobjFLVVideo = FLVVideo;
							FLVChunk.m_pbyDataBuffer = byDataBuffer;
							FLVChunk.m_dwDataBufferLength = FLVTags.m_uiDataSize;
							byRetValue = SUCCESS;
						}
					}
					break;
				default:
					printf("\nError in FLV File");
					break;
				}
			}
			else
			{
				printf("\nError in FLV File");
				
			}			
		}
	}

	return byRetValue;
}


u8 CFLVParser::AllocateMemory(u8* &pszMemory,u32 dwMemorySize)
{
	u8 byRetValue = FAILURE;
	pszMemory = new u8[dwMemorySize];
	if(NULL != pszMemory)
	{
		byRetValue = SUCCESS;
	}
	return byRetValue;
}

unsigned long CFLVParser::GetOffset()
{
	unsigned long ulOffset = 0;
	ulOffset = m_pobjCFileIO->GetOffset();
	return ulOffset;
}

unsigned long CFLVParser::GetFileSize()
{
	unsigned long ulFileSize = m_dwFileSize;
	return ulFileSize;
}