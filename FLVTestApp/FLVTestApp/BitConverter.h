#pragma once
#include "CommonAll.h"
#include <intrin.h>

class CBitConverter
{
public:
	CBitConverter(void);
	~CBitConverter(void);

	u64 ToUINT64(u8* byBuffer, int iStartIndex)
	{
		return
			((u64)byBuffer[iStartIndex    ] << 56) |
			((u64)byBuffer[iStartIndex + 1] << 48) |
			((u64)byBuffer[iStartIndex + 2] << 40) |
			((u64)byBuffer[iStartIndex + 3] << 32) |
			((u64)byBuffer[iStartIndex + 4] << 24) |
			((u64)byBuffer[iStartIndex + 5] << 16) |
			((u64)byBuffer[iStartIndex + 6] <<  8) |
			((u64)byBuffer[iStartIndex + 7]      );
	};


	u32 ToUINT32(u8* byBuffer, int iStartIndex)
	{
		return
			((u32)byBuffer[iStartIndex    ] << 24) |
			((u32)byBuffer[iStartIndex + 1] << 16) |
			((u32)byBuffer[iStartIndex + 2] <<  8) |
			((u32)byBuffer[iStartIndex + 3]      );
	};

	u32 ToUINT24(u8* byBuffer, int iStartIndex)
	{
		return
			((u32)byBuffer[iStartIndex + 0] << 16) |
			((u32)byBuffer[iStartIndex + 1] <<  8) |
			((u32)byBuffer[iStartIndex + 2]      );
	};

	u16 ToUINT16(u8* byBuffer, int iStartIndex)
	{
		return (u16)(
			(byBuffer[iStartIndex    ] <<  8) |
			(byBuffer[iStartIndex + 1]      ));
	};

	uint8 ToUINT8(u8* byBuffer)
	{
		*byBuffer = (*byBuffer & 0xF0) >> 4 | (*byBuffer & 0x0F) << 4;
		*byBuffer = (*byBuffer & 0xCC) >> 2 | (*byBuffer & 0x33) << 2;
		*byBuffer = (*byBuffer & 0xAA) >> 1 | (*byBuffer & 0x55) << 1;
		return (uint8) *byBuffer;
			
	};
};

