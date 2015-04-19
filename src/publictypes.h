/* 
  (C) Jan de Vaan 2007-2010, all rights reserved. See the accompanying "License.txt" for licensed use. 
*/ 
#ifndef CHARLS_PUBLICTYPES
#define CHARLS_PUBLICTYPES


enum JLS_ERROR
{
    OK                              = 0,  // The operation completed without errors.
    InvalidJlsParameters            = 1,  // One of the JLS parameters is invalid.
    ParameterValueNotSupported      = 2,  // The parameter value not supported.
    UncompressedBufferTooSmall      = 3,  // The uncompressed buffer is too small to hold all the output.
    CompressedBufferTooSmall        = 4,  // The compressed buffer too small, more input data was expected.
    InvalidCompressedData           = 5,  // This error is returned when the encoded bit stream contains a general structural problem.
    TooMuchCompressedData           = 6,  // Too much compressed data.The decoding proccess is ready but the input buffer still contains encoded data.
    ImageTypeNotSupported           = 7,  // This error is returned when the bit stream is encoded with an option that is not supported by this implementation.
    UnsupportedBitDepthForTransform = 8,  // The bit depth for transformation is not supported.
    UnsupportedColorTransform       = 9,  // The color transformation is not supported.
    UnsupportedEncoding             = 10, // This error is returned when an encoded frame is found that is not encoded with the JPEG-LS algorithm.
    UnknownJpegMarker               = 11, // This error is returned when an unknown JPEG marker code is detected in the encoded bit stream.
    MissingJpegMarkerStart          = 12, // This error is returned when the algorithm expect a 0xFF code (indicates start of a JPEG marker) but none was found.
    UnspecifiedFailure              = 13, // This error is returned when the implementation detected a failure, but no specific error is available.
    UnexpectedFailure               = 14, // This error is returned when the implementation encountered a failure it didn't expect. No guarantees can be given for the state after this error.
};

#ifdef __cplusplus

namespace charls
{
    /// <summary>
    /// Defines the interleave mode for multi-component (color) pixel data.
    /// </summary>
    enum class InterleaveMode
    {
        /// <summary>
        /// The data is encoded and stored as component for component: RRRGGGBBB.
        /// </summary>
        None   = 0,

        /// <summary>
        /// The interleave mode is by line. A full line of each component is encoded before moving to the next line.
        /// </summary>
        Line   = 1,

        /// <summary>
        /// The data is encoded and stored by sample. For color images this is the format like RGBRGBRGB.
        /// </summary>
        Sample = 2
    };

    enum class ColorTransformation
    {
        // Default (RGB)
        None = 0,

        // Color transforms as defined by HP
        // Not part of the JPEG-LS standard in any way, provided for compatibility with existing streams.
        HP1,
        HP2,
        HP3,

        // Defined by HP but not supported by CharLS
        RgbAsYuvLossy,
        Matrix,
        BigEndian = 1 << 29,
        LittleEndian = 1 << 30
    };
}

typedef charls::InterleaveMode CharlsInterleaveModeType;
typedef charls::ColorTransformation CharlsColorTransformationType;

#else

enum CharlsInterleaveMode
{
    CHARLS_IM_NONE   = 0,
    CHARLS_IM_LINE   = 1,
    CHARLS_IM_SAMPLE = 2
};

enum CharlsColorTransformation
{
    // default (RGB)
    CHARLS_COLOR_TRANSFORMATION_NONE = 0,

    // Color transforms as defined by HP
    // Not part of the JPEG-LS standard in any way, provided for compatibility with existing streams.
    CHARLS_COLOR_TRANSFORMATION_HP1,
    CHARLS_COLOR_TRANSFORMATION_HP2,
    CHARLS_COLOR_TRANSFORMATION_HP3,

    // Defined by HP but not supported by CharLS
    CHARLS_COLOR_TRANSFORMATION_RGB_AS_YUV_LOSSY,
    CHARLS_COLOR_TRANSFORMATION_MATRIX,
    CHARLS_COLOR_TRANSFORMATION_BIGENDIAN = 1 << 29,
    CHARLS_COLOR_TRANSFORMATION_LITTLEENDIAN = 1 << 30
};

typedef enum CharlsInterleaveMode CharlsInterleaveModeType;
typedef enum CharlsColorTransformation CharlsColorTransformationType;

#endif


struct JlsCustomParameters
{
    int MAXVAL;
    int T1;
    int T2;
    int T3;
    int RESET;
};


struct JlsRect
{
    int X, Y;
    int Width, Height;
};


struct JfifParameters
{
    int   Ver;
    char  units;
    int   XDensity;
    int   YDensity;
    short Xthumb;
    short Ythumb;
    void* pdataThumbnail; /* user must set buffer which size is Xthumb*Ythumb*3(RGB) before JpegLsDecode() */
};


struct JlsParameters
{
    int width;
    int height;
    int bitspersample;
    int bytesperline;	/* for [source (at encoding)][decoded (at decoding)] pixel image in user buffer */
    int components;
    int allowedlossyerror;
    CharlsInterleaveModeType ilv;
    CharlsColorTransformationType colorTransform;
    char outputBgr;
    struct JlsCustomParameters custom;
    struct JfifParameters jfif;
};




#ifdef __cplusplus

#include <iostream>
#include <cstdint>


// 
// ByteStreamInfo & FromByteArray helper function
//
// ByteStreamInfo describes the stream: either set rawStream to a valid stream, or rawData/count, not both.
// it's possible to decode to memorystreams, but using rawData will always be faster.
//
// Example use: 
//     ByteStreamInfo streamInfo = { fileStream.rdbuf() };
// or 
//     ByteStreamInfo streamInfo = FromByteArray( bytePtr, byteCount);
//
struct ByteStreamInfo
{
    std::basic_streambuf<char>* rawStream;
    uint8_t* rawData;
    std::size_t count;
};


inline ByteStreamInfo FromByteArray(const void* bytes, std::size_t count)
{
    ByteStreamInfo info = ByteStreamInfo();
    info.rawData = (uint8_t*) bytes;
    info.count = count;
    return info;
}

#endif

#endif
