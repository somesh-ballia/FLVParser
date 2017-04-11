#ifndef _FLV_PARSER_H
#define _FLV_PARSER_H
#include "CommonAll.h"
#include "BitConverter.h"

class CFLVFileHeader;
class CFLVTags;
class CFLVAudioData;
class CFLVVideoData;
class CFileIO;
class CFLVChunk;

class CFLVParser
{
public:
	CFLVParser(void);
	~CFLVParser(void);
	u8	Initialize(u8* pszFileName,u16 unFileNameLength);
	u8	Deinitialize();
	u32 GetPreviousTagSize();
	u32 GetDataSize();
	u32 GetTimeStamo();
	u8	GetTimeStampExtended();
	u32 GetStreamID();	
	u8	ReadTag(CFLVChunk	&FLVChunk);
	unsigned long  GetOffset();
	unsigned long  GetFileSize();

private:

	u8 ValidateFLVFile();
	u8 AllocateMemory(u8* &pszMemory,u32 dwMemorySize);

	CFileIO*		m_pobjCFileIO;

	u64				m_dwFileSize;
	u64				m_dwCurrentOffset;
	CBitConverter	m_bitConverter;
};

#endif