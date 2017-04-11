#include "StdAfx.h"
#include "FLVContainer.h"
#include "FLVParser.h"

CFLVContainer::CFLVContainer(void):m_pobjFLVParser(NULL)
{
}


CFLVContainer::~CFLVContainer(void)
{
}

u8 CFLVContainer::Initialize(u8* pszFileName,u16 unFileNameLength,u32 dwForwardSeekLimit)
{
	u8 byRetValue = FAILURE;
	if (NULL == m_pobjFLVParser)
	{
		m_pobjFLVParser = new CFLVParser();
		if (SUCCESS == m_pobjFLVParser->Initialize(pszFileName,unFileNameLength))
		{
			m_isFirstPacket = true;	
			m_isLastPacket = false;
			m_isSeeked = false;
			m_dwTotalAudioFrameDuration = 0;
			m_dwTotalVideoFrameDuration = 0;
			m_dwForwardSeekLimit = dwForwardSeekLimit;
			m_dwVideoFrameCounter = 0;
			m_dwAudioFrameCounter = 0;
			byRetValue = SUCCESS;
		}
	}	

	return byRetValue;
}

u8 CFLVContainer::Deinitialize()
{
	u8 byRetValue = FAILURE;

	if (NULL != m_pobjFLVParser)
	{		
		m_pobjFLVParser->Deinitialize();
		delete m_pobjFLVParser;
		byRetValue = SUCCESS;
	}

	return byRetValue;
}

u8 CFLVContainer::GetChunk(CFLVChunk &FLVChunk)
{
	u8 byRetValue = FAILURE;
	UpdateChunkSequence();

	if(!m_isLastPacket)
	{
		if(m_isFirstPacket)
		{		
			if (SUCCESS == m_pobjFLVParser->ReadTag(m_CurrentChunk))
			{			
				if (SUCCESS == m_pobjFLVParser->ReadTag(m_NextChunk))
				{
					FLVChunk = m_CurrentChunk;
					FLVChunk.m_dwFrameDuration = GetFrameDuration();
					UpdateFrameCounters(FLVChunk);
					byRetValue = SUCCESS;
					m_isFirstPacket = false;
					m_isLastPacket = false;
				}
				else
				{
					printf("Failed to read chunk");
					m_isLastPacket = true;
				}
			}
			else
			{
				printf("Failed to read chunk");
				m_isLastPacket = true;
			}
		}
		else
		{
			int iRet = m_pobjFLVParser->ReadTag(m_NextChunk);
			FLVChunk = m_CurrentChunk;
			FLVChunk.m_dwFrameDuration = GetFrameDuration();
			UpdateFrameCounters(FLVChunk);
			
			if(m_isSeeked)
			{
				FLVChunk.m_pobjFLVTag.m_uiTimeStamp = m_dwNextTimeStamp;
				m_dwNextTimeStamp = FLVChunk.m_pobjFLVTag.m_uiTimeStamp + FLVChunk.m_dwFrameDuration;
			}

			// for forward seek
			if((FLVChunk.m_dwFrameDuration > m_dwForwardSeekLimit))
			{				
				switch(FLVChunk.m_enmChunkType)
				{
				case FLV_CHUNK_AUDIO :
					FLVChunk.m_dwFrameDuration = GetAverageAudioFrameDuration();
					break;
				case FLV_CHUNK_VIDEO :
					FLVChunk.m_dwFrameDuration = GetAverageVideoFrameDuration();
					break;
				default:
					break;
				}

				m_dwNextTimeStamp = FLVChunk.m_pobjFLVTag.m_uiTimeStamp + FLVChunk.m_dwFrameDuration;
				m_isSeeked = true;
			}

			byRetValue = SUCCESS;
			if(FAILURE == iRet)
			{
				m_isLastPacket = true;
			}			
		}
	}	
	
	return byRetValue;
}

u8 CFLVContainer::GetFileInfo()
{
	u8 byRetValue = FAILURE;	


	return byRetValue;
}

unsigned int CFLVContainer::GetFrameDuration()
{
	unsigned int uiFrameDuration = 0;
	if(m_NextChunk.m_pobjFLVTag.m_uiTimeStamp < m_CurrentChunk.m_pobjFLVTag.m_uiTimeStamp)
	{
		uiFrameDuration = m_CurrentChunk.m_pobjFLVTag.m_uiTimeStamp - m_NextChunk.m_pobjFLVTag.m_uiTimeStamp;
	}
	else
	{
		uiFrameDuration = m_NextChunk.m_pobjFLVTag.m_uiTimeStamp - m_CurrentChunk.m_pobjFLVTag.m_uiTimeStamp;
	}
	
	return uiFrameDuration;
}

void CFLVContainer::UpdateChunkSequence()
{
	m_CurrentChunk = m_NextChunk;
}

void CFLVContainer::UpdateFrameCounters(CFLVChunk FLVChunk)
{
	switch(FLVChunk.m_enmChunkType)
	{
	case FLV_CHUNK_AUDIO :
		m_dwAudioFrameCounter++;
		m_dwTotalAudioFrameDuration += FLVChunk.m_dwFrameDuration;
		break;
	case FLV_CHUNK_VIDEO :
		m_dwVideoFrameCounter++;
		m_dwTotalVideoFrameDuration += FLVChunk.m_dwFrameDuration;
		break;
	default:
		break;
	}
}

u32 CFLVContainer::GetAverageAudioFrameDuration()
{
	return (m_dwTotalAudioFrameDuration/m_dwAudioFrameCounter);
}

u32 CFLVContainer::GetAverageVideoFrameDuration()
{
	return (m_dwTotalVideoFrameDuration/m_dwVideoFrameCounter);
}

void CFLVContainer::UpdateNextTimeStamp()
{

	m_dwNextTimeStamp = m_NextChunk.m_pobjFLVTag.m_uiTimeStamp;
}