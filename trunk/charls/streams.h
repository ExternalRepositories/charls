// 
// (C) Jan de Vaan 2007-2010, all rights reserved. See the accompanying "License.txt" for licensed use. 
// 
#ifndef CHARLS_STREAMS
#define CHARLS_STREAMS

#include <memory>
#include <vector>
#include "util.h"



// This file defines JPEG-LS streams: The header and the actual pixel data. Header markers have fixed length, the pixeldata not. 



class JpegSegment;

enum JPEGLS_ColorXForm
{
	// default (RGB)
	COLORXFORM_NONE = 0,	

	// Color transforms as defined by HP
	COLORXFORM_HP1,
	COLORXFORM_HP2,
	COLORXFORM_HP3,

	// Defined by HP but not supported by CharLS
	COLORXFORM_RGB_AS_YUV_LOSSY,
	COLORXFORM_MATRIX
};


ByteStreamInfo FromByteArray(const void* bytes, size_t count);
ByteStreamInfo FromStream(std::basic_streambuf<char>* stream);
void SkipBytes(ByteStreamInfo* streamInfo, size_t count);

//
// JLSOutputStream: minimal implementation to write JPEG header streams
//
class JLSOutputStream
{
	friend class JpegMarkerSegment;
	friend class JpegImageDataSegment;

public:
	JLSOutputStream();
	virtual ~JLSOutputStream();

	void Init(Size size, LONG bitsPerSample, LONG ccomp);
	void AddScan(ByteStreamInfo info, const JlsParameters* pparams);
	
	void AddLSE(const JlsCustomParameters* pcustom);
	void AddColorTransform(int i);
	size_t GetBytesWritten()
		{ return _cbyteOffset; }

	size_t GetLength()
	{ return _data.count - _cbyteOffset; }

 
	size_t Write(ByteStreamInfo info);
	
	void EnableCompare(bool bCompare) 
	{ _bCompare = bCompare; }
private:
	
	BYTE* GetPos() const
	{ return _data.rawData + _cbyteOffset; }

	ByteStreamInfo OutputStream() const
	{ 
		ByteStreamInfo data = _data;
		data.count -= _cbyteOffset;
		data.rawData += _cbyteOffset;
		return data; 
	}


	void WriteByte(BYTE val)
	{ 
		ASSERT(!_bCompare || _data.rawData[_cbyteOffset] == val);
		
		if (_data.rawStream != NULL)
		{
			_data.rawStream->sputc(val);
		}
		else
		{
			_data.rawData[_cbyteOffset++] = val; 
		}
	}

	void WriteBytes(const std::vector<BYTE>& rgbyte)
	{
		for (size_t i = 0; i < rgbyte.size(); ++i)
		{
			WriteByte(rgbyte[i]);
		}		
	}

	void WriteWord(USHORT val)
	{
		WriteByte(BYTE(val / 0x100));
		WriteByte(BYTE(val % 0x100));
	}


    void Seek(size_t byteCount)
	{ 
		if (_data.rawStream != NULL)
			return;

	    _cbyteOffset += byteCount;
	}


private:
	bool _bCompare;
	ByteStreamInfo _data;
	size_t _cbyteOffset;
	LONG _lastCompenentIndex;
	std::vector<JpegSegment*> _segments;	
};


//
// JLSInputStream: minimal implementation to read JPEG header streams
//
class JLSInputStream
{
public:
	JLSInputStream(ByteStreamInfo byteStreamInfo);
	
	const JlsParameters& GetMetadata() const
		{ return _info; } 

	const JlsCustomParameters& GetCustomPreset() const
	{ return _info.custom; } 

	void Read(ByteStreamInfo info);
	void ReadHeader();
	
	void EnableCompare(bool bCompare)
		{ _bCompare = bCompare;	}

	void SetInfo(JlsParameters* info) { _info = *info; }

	void SetRect(JlsRect rect) { _rect = rect; }

	void ReadStartOfScan(bool firstComponent);
	BYTE ReadByte();

private:
	void ReadScan(ByteStreamInfo rawPixels);	
	int ReadPresetParameters();
	int ReadComment();
	int ReadStartOfFrame();
	int ReadWord();
	void ReadNBytes(std::vector<char>& dst, int byteCount);
	int ReadMarker(BYTE marker);

	// JFIF
	void ReadJfif();
	// Color Transform Application Markers & Code Stream (HP extension)
	int ReadColorSpace();
	int ReadColorXForm();
	
private:
	ByteStreamInfo _byteStream;
	BYTE* _byteStreamStart;
	bool _bCompare;
	JlsParameters _info;
	JlsRect _rect;
};




#endif
