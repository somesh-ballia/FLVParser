#ifndef _FLV_H
#define _FLV_H


#define FLV_FILE_HEADER_SIZE		9
#define FLV_TAG_HEADER_SIZE			11
#define FLV_AUDIO_HEADER_SIZE		1
#define FLV_VIDEO_HEADER_SIZE		1

#define FLV_SIGNATURE	0x464C56
#define FLV_VERSION		0x01

/* offsets for packed values */
#define FLV_AUDIO_SAMPLESSIZE_OFFSET 1
#define FLV_AUDIO_SAMPLERATE_OFFSET  2
#define FLV_AUDIO_CODECID_OFFSET     4

#define FLV_VIDEO_FRAMETYPE_OFFSET   4

/* bitmasks to isolate specific values */
#define FLV_AUDIO_CHANNEL_MASK    0x01
#define FLV_AUDIO_SAMPLESIZE_MASK 0x02
#define FLV_AUDIO_SAMPLERATE_MASK 0x0c
#define FLV_AUDIO_CODECID_MASK    0xf0

#define FLV_VIDEO_CODECID_MASK    0x0f
#define FLV_VIDEO_FRAMETYPE_MASK  0xf0

#define AMF_END_OF_OBJECT         0x09

#define KEYFRAMES_TAG            "keyframes"
#define KEYFRAMES_TIMESTAMP_TAG  "times"
#define KEYFRAMES_BYTEOFFSET_TAG "filepositions"

enum ENMChunkType
{
	FLV_CHUNK_UNKNOWN,
	FLV_CHUNK_VIDEO,
	FLV_CHUNK_AUDIO,
	FLV_CHUNK_META
};

enum {
    FLV_HEADER_FLAG_HASVIDEO = 1,
    FLV_HEADER_FLAG_HASAUDIO = 4,
};

enum {
    FLV_TAG_TYPE_AUDIO = 0x08,
    FLV_TAG_TYPE_VIDEO = 0x09,
    FLV_TAG_TYPE_META  = 0x12,
};

enum {
    FLV_STREAM_TYPE_VIDEO,
    FLV_STREAM_TYPE_AUDIO,
    FLV_STREAM_TYPE_DATA,
    FLV_STREAM_TYPE_NB,
};

enum {
    FLV_MONO   = 0,
    FLV_STEREO = 1,
};

enum {
    FLV_SAMPLESSIZE_8BIT  = 0,
    FLV_SAMPLESSIZE_16BIT = 1 << FLV_AUDIO_SAMPLESSIZE_OFFSET,
};

enum {
    FLV_SAMPLERATE_SPECIAL = 0, /**< signifies 5512Hz and 8000Hz in the case of NELLYMOSER */
    FLV_SAMPLERATE_11025HZ = 1 << FLV_AUDIO_SAMPLERATE_OFFSET,
    FLV_SAMPLERATE_22050HZ = 2 << FLV_AUDIO_SAMPLERATE_OFFSET,
    FLV_SAMPLERATE_44100HZ = 3 << FLV_AUDIO_SAMPLERATE_OFFSET,
};

enum {
    FLV_CODECID_PCM                   = 0,
    FLV_CODECID_ADPCM                 = 1 << FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_MP3                   = 2 << FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_PCM_LE                = 3 << FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_NELLYMOSER_16KHZ_MONO = 4 << FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_NELLYMOSER_8KHZ_MONO  = 5 << FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_NELLYMOSER            = 6 << FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_PCM_ALAW              = 7 << FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_PCM_MULAW             = 8 << FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_AAC                   = 10<< FLV_AUDIO_CODECID_OFFSET,
    FLV_CODECID_SPEEX                 = 11<< FLV_AUDIO_CODECID_OFFSET,
};

enum {
    FLV_CODECID_H263    = 2,
    FLV_CODECID_SCREEN  = 3,
    FLV_CODECID_VP6     = 4,
    FLV_CODECID_VP6A    = 5,
    FLV_CODECID_SCREEN2 = 6,
    FLV_CODECID_H264    = 7,
    FLV_CODECID_REALH263= 8,
    FLV_CODECID_MPEG4   = 9,
};

enum {
    FLV_FRAME_KEY            = 1 << FLV_VIDEO_FRAMETYPE_OFFSET, ///< key frame (for AVC, a seekable frame)
    FLV_FRAME_INTER          = 2 << FLV_VIDEO_FRAMETYPE_OFFSET, ///< inter frame (for AVC, a non-seekable frame)
    FLV_FRAME_DISP_INTER     = 3 << FLV_VIDEO_FRAMETYPE_OFFSET, ///< disposable inter frame (H.263 only)
    FLV_FRAME_GENERATED_KEY  = 4 << FLV_VIDEO_FRAMETYPE_OFFSET, ///< generated key frame (reserved for server use only)
    FLV_FRAME_VIDEO_INFO_CMD = 5 << FLV_VIDEO_FRAMETYPE_OFFSET, ///< video info/command frame
};

typedef enum {
    AMF_DATA_TYPE_NUMBER      = 0x00,
    AMF_DATA_TYPE_BOOL        = 0x01,
    AMF_DATA_TYPE_STRING      = 0x02,
    AMF_DATA_TYPE_OBJECT      = 0x03,
    AMF_DATA_TYPE_NULL        = 0x05,
    AMF_DATA_TYPE_UNDEFINED   = 0x06,
    AMF_DATA_TYPE_REFERENCE   = 0x07,
    AMF_DATA_TYPE_MIXEDARRAY  = 0x08,
    AMF_DATA_TYPE_OBJECT_END  = 0x09,
    AMF_DATA_TYPE_ARRAY       = 0x0a,
    AMF_DATA_TYPE_DATE        = 0x0b,
    AMF_DATA_TYPE_LONG_STRING = 0x0c,
    AMF_DATA_TYPE_UNSUPPORTED = 0x0d,
} AMFDataType;

struct STSound
{
	unsigned _int8  uiSoundFormat		: 4;
	unsigned _int8  uiSoundRate			: 2;
	unsigned _int8  uiSoundSize			: 1;
	unsigned _int8	uiSoundType			: 1;
};

struct STVideo
{
	unsigned _int8  uiFrameType			: 4;
	unsigned _int8  uiCodecID			: 4;
};

struct STHeader
{
	unsigned _int8  uiFlagReserved0 :  5;
	unsigned _int8  uiFlagsHasAudio :  1;
	unsigned _int8  uiFlagReserved1 :  1;
	unsigned _int8  uiFlagsHasVideo	:  1;
};

union HEADER
{
	HEADER()
	{
		stHeader.uiFlagReserved0		 = 0;
		stHeader.uiFlagsHasAudio		 = 0;
		stHeader.uiFlagReserved1		 = 0;
		stHeader.uiFlagsHasVideo		 = 0;
	}
	unsigned _int8	header;
	STHeader		stHeader;
};

union VIDEO
{
	VIDEO()
	{
		stVideo.uiFrameType					= 0;
		stVideo.uiCodecID					= 0;
	}

	unsigned _int8  video;
	STVideo			stVideo;
};

union SOUND
{
	SOUND()
	{
		stSound.uiSoundFormat				= 0;
		stSound.uiSoundRate					= 0;
		stSound.uiSoundSize					= 0;
		stSound.uiSoundType					= 0;
	}
	unsigned _int8  sound;
	STSound			stSound;
};
// FOR File Header
class CFLVFileHeader
{
public:
	CFLVFileHeader()
	{
		m_uiSignature					 = 0;
		m_uiVersion						 = 0;		
		m_uiDataOffset					 = 0;
	}

	~CFLVFileHeader()
	{
		m_uiSignature					 = 0;
		m_uiVersion						 = 0;		
		m_uiDataOffset					 = 0;
	}

	unsigned _int32	m_uiSignature	:  24;
	unsigned _int32	m_uiVersion		:  8;
	HEADER			m_header;
	unsigned _int32 m_uiDataOffset	:  32;

};

// For TAG structure
class CFLVTags
{
public:

	CFLVTags()
	{
		m_uiTagType					= 0;
		m_uiDataSize				= 0;
		m_uiTimeStamp				= 0;
		m_uiTimeStampExtended		= 0;
		m_uiStreamID				= 0;
	}

	~CFLVTags()
	{
		m_uiTagType					= 0;
		m_uiDataSize				= 0;
		m_uiTimeStamp				= 0;
		m_uiTimeStampExtended		= 0;
		m_uiStreamID				= 0;
	}

	unsigned _int8	m_uiTagType				: 8;
	unsigned _int32	m_uiDataSize		    : 24;
	unsigned _int32	m_uiTimeStamp		    : 24;
	unsigned _int8	m_uiTimeStampExtended	: 8;
	unsigned _int32	m_uiStreamID		    : 24;

};

// For AUDIO Data Structure
class CFLVAudioData
{
public:
	CFLVAudioData()
	{
		
	}

	~CFLVAudioData()
	{		
		
	}

	//SOUND			 m_sound;
	uint8			m_sound;
};


// For VideoData
class CFLVVideoData
{
public:
	CFLVVideoData()
	{		
		
	}

	~CFLVVideoData()
	{		
	
	}

	VIDEO			m_video;
};

class CFLVScriptData
{
	uint8	m_Type;

};


class CFLVChunk
{
public:
	CFLVChunk()
	{
		m_enmChunkType = FLV_CHUNK_UNKNOWN;
		m_dwDataBufferLength = 0;
		m_uiPreviousTagSize = 0;
		m_dwFrameDuration = 0;
	}

	~CFLVChunk()
	{
			
	}

public:
	
	void ReInitialize()
	{
		if (NULL != m_pbyDataBuffer)
		{
			delete m_pbyDataBuffer;			
		}

		m_pbyDataBuffer = NULL;
		m_enmChunkType = FLV_CHUNK_UNKNOWN;
		m_dwDataBufferLength = 0;
		m_uiPreviousTagSize = 0;
	}

	ENMChunkType	m_enmChunkType;
	unsigned int	m_uiPreviousTagSize;
	CFLVTags		m_pobjFLVTag;
	CFLVAudioData	m_pobjFLVAudio;
	CFLVVideoData	m_pobjFLVVideo;
	u8*				m_pbyDataBuffer;
	u32				m_dwDataBufferLength;
	u32				m_dwFrameDuration;
};

#endif /* _FLV_H */
