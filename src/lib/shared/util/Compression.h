#ifndef SHARED_ZIP_H
#define SHARED_ZIP_H

#include "types.h"
#include "zlib.h"

bool ZipBufferToBuffer(uint8 * outBufeer, ULONG & uOutLen, uint8 * inBuffer, ULONG uInLen, int iCompressLevel)     // 成功返回 Z_OK
{
	return Z_OK == compress2(outBufeer ,&uOutLen , inBuffer ,uInLen , iCompressLevel);
}

bool UnZipBufferToBuffer(uint8 * outBufeer, ULONG &uOutLen, uint8 * inBuffer, ULONG uInLen)
{
	return Z_OK == uncompress(outBufeer,&uOutLen,inBuffer,uInLen);
}

ULONG ComputeComSize(ULONG inSize)
{
	return compressBound(inSize);
}

#endif	//SHARED_ZIP_H

