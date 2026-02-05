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

#ifndef PRGPU_KERNELSUPPORT_KERNELHALF_H
#define PRGPU_KERNELSUPPORT_KERNELHALF_H

/**
** Half4ToFloat* and Float4ToHalf* are implementation details and are not portable to OpenCL.
** These exist as an aid to KernelMemory.h
*/
#if GF_DEVICE_TARGET_CUDA

    #include <cuda_fp16.h>

struct half4
{
    half x, y, z, w;
};

GF_DEVICE_FUNCTION float HalfToFloat(half inV)
{
    return inV;
}

GF_DEVICE_FUNCTION half FloatToHalf(float inV)
{
    return inV;
}

GF_DEVICE_FUNCTION float2 Half2ToFloat2(half2 inV)
{
    return make_float2(inV.x, inV.y);
}

GF_DEVICE_FUNCTION half2 Float2ToHalf2(float2 inV)
{
    return make_half2(inV.x, inV.y);
}

GF_DEVICE_FUNCTION float4 Half4ToFloat4(half4 inV)
{
    float4 out;
    out.x = inV.x;
    out.y = inV.y;
    out.z = inV.z;
    out.w = inV.w;

    return out;
}

GF_DEVICE_FUNCTION half4 Float4ToHalf4(float4 inV)
{
    half4 out;
    out.x = inV.x;
    out.y = inV.y;
    out.z = inV.z;
    out.w = inV.w;

    return out;
}

#elif GF_DEVICE_TARGET_HLSL
GF_DEVICE_FUNCTION float HalfToFloat(half inV)
{
    return float(inV);
}

GF_DEVICE_FUNCTION half FloatToHalf(float inV)
{
    return half(inV);
}

GF_DEVICE_FUNCTION float2 Half2ToFloat2(half2 inV)
{
    return float2(inV);
}

GF_DEVICE_FUNCTION half2 Float2ToHalf2(float2 inV)
{
    return half2(inV);
}

GF_DEVICE_FUNCTION float4 Half4ToFloat4(half4 inV)
{
    return float4(inV);
}

GF_DEVICE_FUNCTION half4 Float4ToHalf4(float4 inV)
{
    return half4(inV);
}

#elif GF_DEVICE_TARGET_METAL

float HalfToFloat(half inV)
{
    return inV;
}

half FloatToHalf(float inV)
{
    return inV;
}

half2 Float2ToHalf2(float2 inV)
{
    return make_half2(inV.x, inV.y);
}

float2 Half2ToFloat2(half2 inV)
{
    return make_float2(inV.x, inV.y);
}

float4 Half4ToFloat4(half4 inV)
{
    return make_float4(inV.x, inV.y, inV.z, inV.w);
}

half4 Float4ToHalf4(float4 inV)
{
    return make_half4(inV.x, inV.y, inV.z, inV.w);
}

#endif

#endif //PRGPU_KERNELSUPPORT_KERNELHALF_H
