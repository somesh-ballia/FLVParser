#include "StdAfx.h"
#include "FileWriter.h"
#include <stdio.h>
#include <Winsock2.h>

#define FLV_VERSION	0x01
#define SIZE_ONE	1
#define SIZE_TWO	2
#define SIZE_THREE	3
#define SIZE_FOUR	4

CFileWriter::CFileWriter(void)
{
}


CFileWriter::~CFileWriter(void)
{
}


u8 CFileWriter::Initialize(char* pszAudioFileName,int iAudioFileNameLen, \
							char* pszVideoFileName, int iVideoFileNameLen)
{
	u8 byRetValue = FAILURE;

	if (0 < strlen(pszAudioFileName) && 0 < strlen(pszVideoFileName))
	{
		m_VideoFile.open(pszVideoFileName,ios::binary | ios::out);
		m_AudioFile.open(pszAudioFileName,ios::binary | ios::out);

		if (m_AudioFile && m_VideoFile)
		{
			if (SUCCESS == WriteFLVHeader())
			{
				m_isFirstAudioFrame = true;
				m_isFirstVideoFrame = true;
				byRetValue = SUCCESS;
			}			
		}
	}
	return byRetValue;
}

u8 CFileWriter::DeInitialize()
{
	u8 byRetValue = FAILURE;

	m_AudioFile.close();
	m_VideoFile.close();
	byRetValue = SUCCESS;

	return byRetValue;
}

u8 CFileWriter::WriteFLVHeader()
{
	u8 byRetValue = FAILURE;
	char pszBuffer[FLV_FILE_HEADER_SIZE] = {0};

	// AUDIO HEADER
	pszBuffer[0]= 'F';
	pszBuffer[1]= 'L';
	pszBuffer[2]= 'V';
	pszBuffer[3]= 0x01;
	pszBuffer[4]= 0x05;
	pszBuffer[5]= 0x00;
	pszBuffer[6]= 0x00;
	pszBuffer[7]= 0x00;
	pszBuffer[8]= 0x09;

	m_AudioFile.write(pszBuffer,FLV_FILE_HEADER_SIZE);
	m_VideoFile.write(pszBuffer,FLV_FILE_HEADER_SIZE);

	byRetValue = SUCCESS;
	return byRetValue;
}

u8 CFileWriter::WriteData(CFLVChunk &FLVChunk)
{
	u8 byRetValue = FAILURE;

	uint8	uiTemp8 = 0;
	u16		uiTemp16 = 0;
	u32		uiTemp32 = 0;
	char*	pszTemp = NULL;

	if (m_AudioFile && m_VideoFile)
	{
		switch(FLVChunk.m_enmChunkType)
		{
		case FLV_CHUNK_AUDIO:
			
			WriteAudioInVideoFile(FLVChunk);
			// Writing previous tag size
			uiTemp32 = htonl(FLVChunk.m_uiPreviousTagSize);
			m_AudioFile.write((char*)&uiTemp32,SIZE_FOUR);

			// writing TAG TYPE
			uiTemp8 = FLVChunk.m_pobjFLVTag.m_uiTagType;
			m_AudioFile.write((char*)&uiTemp8,SIZE_ONE);

			// writing Data Size
			uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiDataSize);
			pszTemp = (char*)&uiTemp32;
			m_AudioFile.write(&pszTemp[1],SIZE_THREE);

			// Writing TimeStamp
			uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiTimeStamp);
			pszTemp = (char*)&uiTemp32;
			m_AudioFile.write(&pszTemp[1],SIZE_THREE);

			// Writing TimeStamp extended
			uiTemp8 = FLVChunk.m_pobjFLVTag.m_uiTimeStampExtended;
			m_AudioFile.write((char*)&uiTemp8,SIZE_ONE);

			// Writing Stream ID
			uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiStreamID);
			pszTemp = (char*)&uiTemp32;
			m_AudioFile.write(&pszTemp[1],SIZE_THREE);

			// Writing audio Header
			uiTemp8 = FLVChunk.m_pobjFLVAudio.m_sound;
			m_AudioFile.write((char*)&uiTemp8,SIZE_ONE);

			// Writing Payload Data Bytes
			m_AudioFile.write((char*)FLVChunk.m_pbyDataBuffer,FLVChunk.m_dwDataBufferLength);

			byRetValue = SUCCESS;
			break;


		case FLV_CHUNK_VIDEO:
			//WriteVideoInAudioFile(FLVChunk);
			// Writing previous tag size
			uiTemp32 = htonl(FLVChunk.m_uiPreviousTagSize);
			m_VideoFile.write((char*)&uiTemp32,SIZE_FOUR);	

			// writing TAG TYPE
			uiTemp8 = FLVChunk.m_pobjFLVTag.m_uiTagType;
			m_VideoFile.write((char*)&uiTemp8,SIZE_ONE);

			// writing Data Size
			uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiDataSize);
			pszTemp = (char*)&uiTemp32;
			m_VideoFile.write(&pszTemp[1],SIZE_THREE);

			// Writing TimeStamp
			uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiTimeStamp);
			pszTemp = (char*)&uiTemp32;
			m_VideoFile.write(&pszTemp[1],SIZE_THREE);

			// Writing TimeStamp extended
			uiTemp8 = FLVChunk.m_pobjFLVTag.m_uiTimeStampExtended;
			m_VideoFile.write((char*)&uiTemp8,SIZE_ONE);

			// Writing Stream ID
			uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiStreamID);
			pszTemp = (char*)&uiTemp32;
			m_VideoFile.write(&pszTemp[1],SIZE_THREE);

			// writing video header
			uiTemp8 = FLVChunk.m_pobjFLVVideo.m_video.stVideo.uiCodecID;
			FLVChunk.m_pobjFLVVideo.m_video.stVideo.uiCodecID = FLVChunk.m_pobjFLVVideo.m_video.stVideo.uiFrameType;
			FLVChunk.m_pobjFLVVideo.m_video.stVideo.uiFrameType = uiTemp8;
			uiTemp8 = FLVChunk.m_pobjFLVVideo.m_video.video;
			m_VideoFile.write((char*)&uiTemp8,SIZE_ONE);

			// Writing Payload Data Bytes
			m_VideoFile.write((char*)FLVChunk.m_pbyDataBuffer,FLVChunk.m_dwDataBufferLength);

			byRetValue = SUCCESS;
			break;
		case FLV_CHUNK_META:

			// *****************************AUDIO***************************************

			// Writing previous tag size
			uiTemp32 = htonl(FLVChunk.m_uiPreviousTagSize);
			m_AudioFile.write((char*)&uiTemp32,SIZE_FOUR);

			// writing TAG TYPE
			uiTemp8 = FLVChunk.m_pobjFLVTag.m_uiTagType;
			m_AudioFile.write((char*)&uiTemp8,SIZE_ONE);

			// writing Data Size
			uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiDataSize);
			pszTemp = (char*)&uiTemp32;
			m_AudioFile.write(&pszTemp[1],SIZE_THREE);

			// Writing TimeStamp
			uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiTimeStamp);
			pszTemp = (char*)&uiTemp32;
			m_AudioFile.write(&pszTemp[1],SIZE_THREE);

			// Writing TimeStamp extended
			uiTemp8 = FLVChunk.m_pobjFLVTag.m_uiTimeStampExtended;
			m_AudioFile.write((char*)&uiTemp8,SIZE_ONE);

			// Writing Stream ID
			uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiStreamID);
			pszTemp = (char*)&uiTemp32;
			m_AudioFile.write(&pszTemp[1],SIZE_THREE);
			
			// Writing Payload Data Bytes
			m_AudioFile.write((char*)FLVChunk.m_pbyDataBuffer,FLVChunk.m_dwDataBufferLength);


			//************************************VIDEO********************************


			// Writing previous tag size
			uiTemp32 = htonl(FLVChunk.m_uiPreviousTagSize);
			m_VideoFile.write((char*)&uiTemp32,SIZE_FOUR);	

			// writing TAG TYPE
			uiTemp8 = FLVChunk.m_pobjFLVTag.m_uiTagType;
			m_VideoFile.write((char*)&uiTemp8,SIZE_ONE);

			// writing Data Size
			uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiDataSize);
			pszTemp = (char*)&uiTemp32;
			m_VideoFile.write(&pszTemp[1],SIZE_THREE);

			// Writing TimeStamp
			uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiTimeStamp);
			pszTemp = (char*)&uiTemp32;
			m_VideoFile.write(&pszTemp[1],SIZE_THREE);

			// Writing TimeStamp extended
			uiTemp8 = FLVChunk.m_pobjFLVTag.m_uiTimeStampExtended;
			m_VideoFile.write((char*)&uiTemp8,SIZE_ONE);

			// Writing Stream ID
			uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiStreamID);
			pszTemp = (char*)&uiTemp32;
			m_VideoFile.write(&pszTemp[1],SIZE_THREE);

			// Writing Payload Data Bytes
			m_VideoFile.write((char*)FLVChunk.m_pbyDataBuffer,FLVChunk.m_dwDataBufferLength);

			byRetValue = SUCCESS;
			break;
		default:
			break;
		}
	}	
	
	return byRetValue;
}

u8 CFileWriter::WriteAudioInVideoFile(CFLVChunk &FLVChunk)
{
	u8 byRetValue = FAILURE;

	uint8	uiTemp8 = 0;
	u16		uiTemp16 = 0;
	u32		uiTemp32 = 0;
	char*	pszTemp = NULL;
	
		// Writing previous tag size
		uiTemp32 = htonl(FLVChunk.m_uiPreviousTagSize);
		m_VideoFile.write((char*)&uiTemp32,SIZE_FOUR);

		// writing TAG TYPE
		uiTemp8 = FLVChunk.m_pobjFLVTag.m_uiTagType;
		m_VideoFile.write((char*)&uiTemp8,SIZE_ONE);

		// writing Data Size
		uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiDataSize);
		pszTemp = (char*)&uiTemp32;
		m_VideoFile.write(&pszTemp[1],SIZE_THREE);

		// Writing TimeStamp
		uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiTimeStamp);
		pszTemp = (char*)&uiTemp32;
		m_VideoFile.write(&pszTemp[1],SIZE_THREE);

		// Writing TimeStamp extended
		uiTemp8 = FLVChunk.m_pobjFLVTag.m_uiTimeStampExtended;
		m_VideoFile.write((char*)&uiTemp8,SIZE_ONE);

		// Writing Stream ID
		uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiStreamID);
		pszTemp = (char*)&uiTemp32;
		m_VideoFile.write(&pszTemp[1],SIZE_THREE);

		// Writing audio Header
		uiTemp8 = FLVChunk.m_pobjFLVAudio.m_sound;
		m_VideoFile.write((char*)&uiTemp8,SIZE_ONE);

		// Writing Payload Data Bytes
		m_VideoFile.write((char*)FLVChunk.m_pbyDataBuffer,FLVChunk.m_dwDataBufferLength);

	m_isFirstAudioFrame = false;
	return byRetValue;
}

u8 CFileWriter::WriteVideoInAudioFile(CFLVChunk &FLVChunk)
{
	u8 byRetValue = FAILURE;
	uint8	uiTemp8 = 0;
	u16		uiTemp16 = 0;
	u32		uiTemp32 = 0;
	char*	pszTemp = NULL;

	
		// Writing previous tag size
		uiTemp32 = htonl(FLVChunk.m_uiPreviousTagSize);
		m_AudioFile.write((char*)&uiTemp32,SIZE_FOUR);	

		// writing TAG TYPE
		uiTemp8 = FLVChunk.m_pobjFLVTag.m_uiTagType;
		m_AudioFile.write((char*)&uiTemp8,SIZE_ONE);

		// writing Data Size
		uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiDataSize);
		pszTemp = (char*)&uiTemp32;
		m_AudioFile.write(&pszTemp[1],SIZE_THREE);

		// Writing TimeStamp
		uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiTimeStamp);
		pszTemp = (char*)&uiTemp32;
		m_AudioFile.write(&pszTemp[1],SIZE_THREE);

		// Writing TimeStamp extended
		uiTemp8 = FLVChunk.m_pobjFLVTag.m_uiTimeStampExtended;
		m_AudioFile.write((char*)&uiTemp8,SIZE_ONE);

		// Writing Stream ID
		uiTemp32 = htonl(FLVChunk.m_pobjFLVTag.m_uiStreamID);
		pszTemp = (char*)&uiTemp32;
		m_AudioFile.write(&pszTemp[1],SIZE_THREE);

		// writing video header
		uiTemp8 = FLVChunk.m_pobjFLVVideo.m_video.stVideo.uiCodecID;
		FLVChunk.m_pobjFLVVideo.m_video.stVideo.uiCodecID = FLVChunk.m_pobjFLVVideo.m_video.stVideo.uiFrameType;
		FLVChunk.m_pobjFLVVideo.m_video.stVideo.uiFrameType = uiTemp8;
		uiTemp8 = FLVChunk.m_pobjFLVVideo.m_video.video;
		m_AudioFile.write((char*)&uiTemp8,SIZE_ONE);

		// Writing Payload Data Bytes
		m_AudioFile.write((char*)FLVChunk.m_pbyDataBuffer,FLVChunk.m_dwDataBufferLength);

	
	m_isFirstVideoFrame = false;
	return byRetValue;
}