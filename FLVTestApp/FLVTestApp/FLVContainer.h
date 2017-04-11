#ifndef _FLV_CONTRINER_H
#define _FLV_CONTRINER_H
#include "CommonAll.h"
#include "FLV.h"
class CFileIO;
class CFLVParser;

class CFLVContainer
{
public:
	CFLVContainer(void);
	~CFLVContainer(void);
	u8 Initialize(u8* pszFileName,u16 unFileNameLength,u32 dwForwardSeekLimit);
	u8 Deinitialize();
	u8 GetChunk(CFLVChunk  &FLVChunk);
	u8 GetFileInfo();
	unsigned int GetFrameDuration();

private:
	void UpdateChunkSequence();
	void UpdateNextTimeStamp();
	void UpdateFrameCounters(CFLVChunk FLVChunk);
	u32  GetAverageAudioFrameDuration();
	u32  GetAverageVideoFrameDuration();
	

private:	
	CFLVParser* m_pobjFLVParser;
	CFLVChunk   m_CurrentChunk;
	CFLVChunk	m_NextChunk;	
	bool		m_isSeeked;
	u64			m_dwTotalVideoFrameDuration;
	u64			m_dwTotalAudioFrameDuration;
	u64			m_dwVideoFrameCounter;
	u64			m_dwAudioFrameCounter;
	bool		m_isFirstPacket;
	bool		m_isLastPacket;
	u32			m_dwForwardSeekLimit;
	u32			m_dwNextTimeStamp;

};

#endif