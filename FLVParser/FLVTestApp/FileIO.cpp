#include "StdAfx.h"
#include "FileIO.h"


CFileIO::CFileIO(void)
{
	memset(m_pszBuffer,0,99999);
}


CFileIO::~CFileIO(void)
{
}

u8 CFileIO::Initialize(u8* pszFileName,u16 unFileNameLength, \
					   u64 &dwFileSize, u64 &dwCurrentOffset )
{
	u8 byRetValue = FAILURE;
	m_FileOffset = 0;
	dwCurrentOffset = m_FileOffset;
	m_FileStream.open((char*)pszFileName,ios::binary);
	if(NULL != m_FileStream)
	{
		m_FileStream.seekg(0,ios::end);
		dwFileSize = m_FileStream.tellg();
		m_FileStream.seekg(0,ios::beg);
		m_FileSize = dwFileSize;
		byRetValue = SUCCESS;
	}
	return byRetValue;
}

u8 CFileIO::Deinitialize()
{
	u8 byRetValue = FAILURE;

	m_FileStream.close();
	byRetValue = SUCCESS;

	return byRetValue;
}

u8 CFileIO::SetOffset(u32 lOffset)
{
	u8 byRetValue = SUCCESS;
	
	return byRetValue;
}

long CFileIO::GetOffset()
{
	return m_FileOffset;
}

u8 CFileIO::FetchBuffer(u8* &pszBuffer,u16 unBufferSize)
{
	u8 byRetValue = FAILURE;
	// memory for pszBuffer is already created;
	if (NULL != m_FileStream)
	{
		if(0 < unBufferSize)
		{
			m_FileStream.read((char*)m_pszBuffer,unBufferSize);
			if (m_FileStream)
			{
				m_FileOffset+=unBufferSize;
				pszBuffer = (u8*)m_pszBuffer;
				byRetValue = SUCCESS;
			}
			else
			{
				if(0 < m_FileStream.gcount())
				{
					// for last chunk of file
					byRetValue = SUCCESS;
				}
				else
				{
					printf("\nError Reading file");
				}			
			}			
		}
	}

	return byRetValue;
}