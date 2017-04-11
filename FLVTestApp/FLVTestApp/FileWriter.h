#pragma once
#include "CommonAll.h"
#include "FLV.h"
#include <fstream>

using namespace std;
class CFileWriter
{
public:
	CFileWriter(void);
	~CFileWriter(void);
	u8 Initialize(char* pszAudioFileName,int iAudioFileNameLen, \
				  char* pszVideoFileName, int iVideoFileNameLen);
	u8 DeInitialize();

	u8 WriteData(CFLVChunk	&FLVChunk);

private:
	u8 WriteFLVHeader();
	u8 WriteAudioInVideoFile(CFLVChunk	&FLVChunk);
	u8 WriteVideoInAudioFile(CFLVChunk	&FLVChunk);
private:
	ofstream	m_VideoFile;
	ofstream	m_AudioFile;
	bool		m_isFirstAudioFrame;
	bool		m_isFirstVideoFrame;
};

