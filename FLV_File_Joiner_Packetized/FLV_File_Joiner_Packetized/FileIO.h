#ifndef _FILE_IO_H
#define _FILE_IO_H
#include <iostream>
#include <fstream>
#include "CommonAll.h"

using namespace std;
class CFileIO
{
public:
	CFileIO(void);
	~CFileIO(void);
	u8 Initialize(u8* pszFileName,u16 unFileNameLength, \
				  u64 &dwFileSize, u64 &dwCurrentOffset );
	u8 Deinitialize();
	u8 SetOffset(u32 lOffset);
	long GetOffset();
	u8 FetchBuffer(u8* &pszBuffer,u16 unBufferSize);

private:
	ifstream	m_FileStream;
	long		m_FileOffset;
	long		m_FileSize;
};

#endif