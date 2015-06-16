/*
 * Copyright Benjamin Wang 2014.
 * 
 * This source code may not be distributed, or used for any commercial or non-commerical   
 * purposes, without the written of Benjamin Wang or Darcy McGee. This includes, without
 * exception, any modified versions of this source code.
 * This copyright notice may not be removed.
 */

#include "CommonGraphics.h"

namespace wntr {
    
    unsigned sizeOfFormat(FORMAT format) {
		switch (format) {
            case FORMAT::BYTE:
                return sizeof(char);
                break;
            case FORMAT::BYTE2:
                return sizeof(char)*2;
                break;
            case FORMAT::BYTE3:
                return sizeof(char)*3;
                break;
            case FORMAT::BYTE4:
                return sizeof(char)*4;
                break;
            case FORMAT::UBYTE:
                return sizeof(unsigned char);
                break;
            case FORMAT::UBYTE2:
                return sizeof(unsigned char)*2;
                break;
            case FORMAT::UBYTE3:
                return sizeof(unsigned char)*3;
                break;
            case FORMAT::UBYTE4:
                return sizeof(unsigned char)*4;
                break;
            case FORMAT::SHORT:
                return sizeof(short);
                break;
            case FORMAT::SHORT2:
                return sizeof(short)*2;
                break;
            case FORMAT::SHORT3:
                return sizeof(short)*3;
                break;
            case FORMAT::SHORT4:
                return sizeof(short)*4;
                break;
            case FORMAT::USHORT:
                return sizeof(unsigned short);
                break;
            case FORMAT::USHORT2:
                return sizeof(unsigned short)*2;
                break;
            case FORMAT::USHORT3:
                return sizeof(unsigned short)*3;
                break;
            case FORMAT::USHORT4:
                return sizeof(unsigned short)*4;
                break;
            case FORMAT::INT:
                return sizeof(int);
                break;
            case FORMAT::INT2:
                return sizeof(int)*2;
                break;
            case FORMAT::INT3:
                return sizeof(int)*3;
                break;
            case FORMAT::INT4:
                return sizeof(int)*4;
                break;
            case FORMAT::UINT:
                return sizeof(unsigned int);
                break;
            case FORMAT::UINT2:
                return sizeof(unsigned int)*2;
                break;
            case FORMAT::UINT3:
                return sizeof(unsigned int)*3;
                break;
            case FORMAT::UINT4:
                return sizeof(unsigned int)*4;
                break;
            case FORMAT::FLOAT:
                return sizeof(float);
                break;
            case FORMAT::FLOAT2:
                return sizeof(float)*2;
                break;
            case FORMAT::FLOAT3:
                return sizeof(float)*3;
                break;
            case FORMAT::FLOAT4:
                return sizeof(float)*4;
                break;
            default:
                throw std::invalid_argument("error: sizeOfFormat: encountered an unsupported data format");
		}
	}

}