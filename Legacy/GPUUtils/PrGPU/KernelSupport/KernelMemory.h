/*
** ADOBE CONFIDENTIAL
**
** Copyright 2017 Adobe
** All Rights Reserved.
**
** NOTICE: Adobe permits you to use, modify, and distribute this file in
** accordance with the terms of the Adobe license agreement accompanying
** it. If you have received this file from a source other than Adobe,
** then your use, modification, or distribution of it requires the prior
** written permission of Adobe.
*/

#ifndef PRGPU_KERNELSUPPORT_KERNELMEMORY_H
#define PRGPU_KERNELSUPPORT_KERNELMEMORY_H

#include "PrGPU/KernelSupport/KernelHalf.h"

/**
** ReadFloat4, ReadFloat2, ReadFloat:
**  Read from a 4, 2, or 1 channel image of 32 bit or 16 bit float in global memory
** WriteFloat4, WriteFloat2, WriteFloat:
**  Write to a 4, 2, or 1 channel image of 32 bit or 16 bit float in global memory
*/
#if GF_DEVICE_TARGET_CUDA
GF_DEVICE_FUNCTION float4 ReadFloat4(GF_PTR(float4 const) inImage, int inIndex, bool is16Bit)
{
    return is16Bit ? Half4ToFloat4(((GF_PTR(half4 const))inImage)[inIndex]) : inImage[inIndex];
}

GF_DEVICE_FUNCTION void WriteFloat4(float4 inPixel, GF_PTR(float4) outImage, int inIndex, bool is16Bit)
{
    if (is16Bit)
    {
        ((GF_PTR(half4))outImage)[inIndex] = Float4ToHalf4(inPixel);
    }
    else
    {
        outImage[inIndex] = inPixel;
    }
}

GF_DEVICE_FUNCTION float2 ReadFloat2(GF_PTR(float2 const) inImage, int inIndex, bool is16Bit)
{
    return is16Bit ? Half2ToFloat2(((GF_PTR(half2))inImage)[inIndex]) : inImage[inIndex];
}

GF_DEVICE_FUNCTION void WriteFloat2(float2 inPixel, GF_PTR(float2) outImage, int inIndex, bool is16Bit)
{
    if (is16Bit)
    {
        ((GF_PTR(half2))outImage)[inIndex] = Float2ToHalf2(inPixel);
    }
    else
    {
        outImage[inIndex] = inPixel;
    }
}

GF_DEVICE_FUNCTION float ReadFloat(GF_PTR(float const) inImage, int inIndex, bool is16Bit)
{
    return is16Bit ? HalfToFloat(((GF_PTR(half const))inImage)[inIndex]) : inImage[inIndex];
}

GF_DEVICE_FUNCTION void WriteFloat(float inPixel, GF_PTR(float) outImage, int inIndex, bool is16Bit)
{
    if (is16Bit)
    {
        ((GF_PTR(half))outImage)[inIndex] = FloatToHalf(inPixel);
    }
    else
    {
        outImage[inIndex] = inPixel;
    }
}

#elif GF_DEVICE_TARGET_OPENCL
GF_DEVICE_FUNCTION float4 ReadFloat4(const GF_PTR(float4) inImage, int inIndex, bool is16Bit)
{
    GF_OPENCL_SECTION(#if GF_OPENCL_SUPPORTS_16F)
    if (is16Bit)
    {
        return vload_half4(inIndex, (const GF_PTR(half))inImage);
    }
    GF_OPENCL_SECTION(#endif)
    return inImage[inIndex];
}

GF_DEVICE_FUNCTION void WriteFloat4(float4 inPixel, GF_PTR(float4) outImage, int inIndex, bool is16Bit)
{
    GF_OPENCL_SECTION(#if GF_OPENCL_SUPPORTS_16F)
    if (is16Bit)
    {
        vstorea_half4_rtz(inPixel, inIndex, (GF_PTR(half))outImage);
    }
    else
        GF_OPENCL_SECTION(#endif)
        {
            outImage[inIndex] = inPixel;
        }
}

GF_DEVICE_FUNCTION float2 ReadFloat2(const GF_PTR(float2) inImage, int inIndex, bool is16Bit)
{
    GF_OPENCL_SECTION(#if GF_OPENCL_SUPPORTS_16F)
    if (is16Bit)
    {
        return vload_half2(inIndex, (const GF_PTR(half))inImage);
    }
    GF_OPENCL_SECTION(#endif)
    return inImage[inIndex];
}

GF_DEVICE_FUNCTION void WriteFloat2(float2 inPixel, GF_PTR(float2) outImage, int inIndex, bool is16Bit)
{
    GF_OPENCL_SECTION(#if GF_OPENCL_SUPPORTS_16F)
    if (is16Bit)
    {
        vstorea_half2_rtz(inPixel, inIndex, (GF_PTR(half))outImage);
    }
    else
        GF_OPENCL_SECTION(#endif)
        {
            outImage[inIndex] = inPixel;
        }
}

GF_DEVICE_FUNCTION float ReadFloat(GF_PTR(float const) inImage, int inIndex, bool is16Bit)
{
    GF_OPENCL_SECTION(#if GF_OPENCL_SUPPORTS_16F)
    if (is16Bit)
    {
        return vload_half(inIndex, (const GF_PTR(half))inImage);
    }
    GF_OPENCL_SECTION(#endif)
    return inImage[inIndex];
}

GF_DEVICE_FUNCTION void WriteFloat(float inPixel, GF_PTR(float) outImage, int inIndex, bool is16Bit)
{
    GF_OPENCL_SECTION(#if GF_OPENCL_SUPPORTS_16F)
    if (is16Bit)
    {
        vstore_half_rtz(inPixel, inIndex, (GF_PTR(half))outImage);
    }
    else
        GF_OPENCL_SECTION(#endif)
        {
            outImage[inIndex] = inPixel;
        }
}

#elif GF_DEVICE_TARGET_HLSL
// We have different copies for certain functions to ensure we can read from both RO and RW buffers
GF_DEVICE_FUNCTION float4 ReadFloat4(const GF_PTR(float4) inImage, int inIndex, bool is16Bit)
{
    return is16Bit ? Half4ToFloat4(GF_READINDEX_TYPE(half4, inImage, inIndex))
                   : GF_READINDEX_TYPE(float4, inImage, inIndex);
}

GF_DEVICE_FUNCTION float4 ReadFloat4(GF_PTR_READ_ONLY(float4) inImage, int inIndex, bool is16Bit)
{
    return is16Bit ? Half4ToFloat4(GF_READINDEX_TYPE(half4, inImage, inIndex))
                   : GF_READINDEX_TYPE(float4, inImage, inIndex);
}

GF_DEVICE_FUNCTION void WriteFloat4(float4 inPixel, GF_PTR(float4) outImage, int inIndex, bool is16Bit)
{
    if (is16Bit)
    {
        GF_WRITEINDEX_TYPE(half4, Float4ToHalf4(inPixel), outImage, inIndex);
    }
    else
    {
        GF_WRITEINDEX_TYPE(float4, inPixel, outImage, inIndex);
    }
}

GF_DEVICE_FUNCTION float2 ReadFloat2(const GF_PTR(float2) inImage, int inIndex, bool is16Bit)
{
    return is16Bit ? Half2ToFloat2(GF_READINDEX_TYPE(half2, inImage, inIndex))
                   : GF_READINDEX_TYPE(float2, inImage, inIndex);
}

GF_DEVICE_FUNCTION float2 ReadFloat2(GF_PTR_READ_ONLY(float2) inImage, int inIndex, bool is16Bit)
{
    return is16Bit ? Half2ToFloat2(GF_READINDEX_TYPE(half2, inImage, inIndex))
                   : GF_READINDEX_TYPE(float2, inImage, inIndex);
}

GF_DEVICE_FUNCTION void WriteFloat2(float2 inPixel, GF_PTR(float2) outImage, int inIndex, bool is16Bit)
{
    if (is16Bit)
    {
        GF_WRITEINDEX_TYPE(half2, Float2ToHalf2(inPixel), outImage, inIndex);
    }
    else
    {
        GF_WRITEINDEX_TYPE(float2, inPixel, outImage, inIndex);
    }
}

GF_DEVICE_FUNCTION float ReadFloat(const GF_PTR(float) inImage, int inIndex, bool is16Bit)
{
    return is16Bit ? HalfToFloat(GF_READINDEX_TYPE(half, inImage, inIndex))
                   : GF_READINDEX_TYPE(float, inImage, inIndex);
}

GF_DEVICE_FUNCTION float ReadFloat(GF_PTR_READ_ONLY(float) inImage, int inIndex, bool is16Bit)
{
    return is16Bit ? HalfToFloat(GF_READINDEX_TYPE(half, inImage, inIndex))
                   : GF_READINDEX_TYPE(float, inImage, inIndex);
}

GF_DEVICE_FUNCTION void WriteFloat(float inPixel, GF_PTR(float) outImage, int inIndex, bool is16Bit)
{
    if (is16Bit)
    {
        GF_WRITEINDEX_TYPE(half, FloatToHalf(inPixel), outImage, inIndex);
    }
    else
    {
        GF_WRITEINDEX_TYPE(float, inPixel, outImage, inIndex);
    }
}

#elif GF_DEVICE_TARGET_METAL
GF_DEVICE_FUNCTION float4 ReadFloat4(GF_PTR(float4 const) inImage, int inIndex, bool is16Bit)
{
    return is16Bit ? Half4ToFloat4(((GF_PTR(half4 const))inImage)[inIndex]) : inImage[inIndex];
}

GF_DEVICE_FUNCTION void WriteFloat4(float4 inPixel, GF_PTR(float4) outImage, int inIndex, bool is16Bit)
{
    if (is16Bit)
    {
        ((GF_PTR(half4))outImage)[inIndex] = Float4ToHalf4(inPixel);
    }
    else
    {
        outImage[inIndex] = inPixel;
    }
}

GF_DEVICE_FUNCTION float2 ReadFloat2(GF_PTR(float2 const) inImage, int inIndex, bool is16Bit)
{
    return is16Bit ? Half2ToFloat2(((GF_PTR(half2 const))inImage)[inIndex]) : inImage[inIndex];
}

GF_DEVICE_FUNCTION void WriteFloat2(float2 inPixel, GF_PTR(float2) outImage, int inIndex, bool is16Bit)
{
    if (is16Bit)
    {
        ((GF_PTR(half2))outImage)[inIndex] = Float2ToHalf2(inPixel);
    }
    else
    {
        outImage[inIndex] = inPixel;
    }
}

GF_DEVICE_FUNCTION float ReadFloat(GF_PTR(float const) inImage, int inIndex, bool is16Bit)
{
    return is16Bit ? HalfToFloat(((GF_PTR(half const))inImage)[inIndex]) : inImage[inIndex];
}

GF_DEVICE_FUNCTION void WriteFloat(float inPixel, GF_PTR(float) outImage, int inIndex, bool is16Bit)
{
    if (is16Bit)
    {
        ((GF_PTR(half))outImage)[inIndex] = FloatToHalf(inPixel);
    }
    else
    {
        outImage[inIndex] = inPixel;
    }
}
#endif

#endif //PRGPU_KERNELSUPPORT_KERNELMEMORY_H
