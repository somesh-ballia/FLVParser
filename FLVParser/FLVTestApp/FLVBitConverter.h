#pragma once
#include <intrin.h>
#include "FLVCommonAll.h"


class CFLVBitConverter
{
public:
	CFLVBitConverter()
	{

	}
	~CFLVBitConverter()
	{

	}

	u64 ToUINT64(u8* byBuffer, int iStartIndex)
	{
		u8 byTempBuffer[8] = {0};
		memcpy(byTempBuffer,byBuffer,8);
		return
			((u64)byTempBuffer[iStartIndex    ] << 56) |
			((u64)byTempBuffer[iStartIndex + 1] << 48) |
			((u64)byTempBuffer[iStartIndex + 2] << 40) |
			((u64)byTempBuffer[iStartIndex + 3] << 32) |
			((u64)byTempBuffer[iStartIndex + 4] << 24) |
			((u64)byTempBuffer[iStartIndex + 5] << 16) |
			((u64)byTempBuffer[iStartIndex + 6] <<  8) |
			((u64)byTempBuffer[iStartIndex + 7]      );
	};


	u32 ToUINT32(u8* byBuffer, int iStartIndex)
	{
		u8 byTempBuffer[4] = {0};
		memcpy(byTempBuffer,byBuffer,4);

		return
			((u32)byTempBuffer[iStartIndex    ] << 24) |
			((u32)byTempBuffer[iStartIndex + 1] << 16) |
			((u32)byTempBuffer[iStartIndex + 2] <<  8) |
			((u32)byTempBuffer[iStartIndex + 3]      );
	};

	u32 ToUINT24(u8* byBuffer, int iStartIndex)
	{
		u8 byTempBuffer[3] = {0};
		memcpy(byTempBuffer,byBuffer,3);

		return
			((u32)byTempBuffer[iStartIndex + 0] << 16) |
			((u32)byTempBuffer[iStartIndex + 1] <<  8) |
			((u32)byTempBuffer[iStartIndex + 2]      );
	};

	u16 ToUINT16(u8* byBuffer, int iStartIndex)
	{
		u8 byTempBuffer[2] = {0};
		memcpy(byTempBuffer,byBuffer,2);

		return (u16)(
			(byTempBuffer[iStartIndex    ] <<  8) |
			(byTempBuffer[iStartIndex + 1]      ));
	};

	u8 ToUINT8(u8* byBuffer)
	{
		u8 byTempBuffer[1] = {0};
		memcpy(byTempBuffer,byBuffer,1);

		*byTempBuffer = (*byTempBuffer & 0xF0) >> 4 | (*byTempBuffer & 0x0F) << 4;
		*byTempBuffer = (*byTempBuffer & 0xCC) >> 2 | (*byTempBuffer & 0x33) << 2;
		*byTempBuffer = (*byTempBuffer & 0xAA) >> 1 | (*byTempBuffer & 0x55) << 1;
		return (u8) *byTempBuffer;

	};
};

