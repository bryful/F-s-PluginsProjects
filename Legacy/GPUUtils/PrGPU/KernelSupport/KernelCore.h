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

#ifndef PRGPU_KERNELSUPPORT_KERNELCORE_H
#define PRGPU_KERNELSUPPORT_KERNELCORE_H

/**
**  Device target. Use these defines to #if/#endif API specific code
**  For OpenCL or Metal, you'll need to define the corresponding
**  GF_DEVICE_TARGET_* macro before including this file
*/
#if DIRECTX
    /**
    ** The DirectX support here uses the Mesa toolchain to compile OpenCL C to DXIL vis SPIR-V and NIR.
    */
    #define GF_DEVICE_TARGET_OPENCL 1
#endif

#if defined(__CUDACC__)
    #define GF_DEVICE_TARGET_CUDA 1
    #define GF_DEVICE_TARGET_OPENCL 0
    #define GF_DEVICE_TARGET_HLSL 0
    #define GF_DEVICE_TARGET_HOST 0
    #define GF_DEVICE_TARGET_METAL 0
    #define GF_DEVICE_TARGET_DEVICE 1

#elif defined(GF_DEVICE_TARGET_OPENCL)
    #define GF_DEVICE_TARGET_CUDA 0
    #define GF_DEVICE_TARGET_HLSL 0
    #define GF_DEVICE_TARGET_HOST 0
    #define GF_DEVICE_TARGET_METAL 0
    #define GF_DEVICE_TARGET_DEVICE 1

#elif defined(GF_DEVICE_TARGET_HLSL)
    #define GF_DEVICE_TARGET_CUDA 0
    #define GF_DEVICE_TARGET_OPENCL 0
    #define GF_DEVICE_TARGET_HOST 0
    #define GF_DEVICE_TARGET_METAL 0
    #define GF_DEVICE_TARGET_DEVICE 1

#elif defined(GF_DEVICE_TARGET_METAL)
    #define GF_DEVICE_TARGET_CUDA 0
    #define GF_DEVICE_TARGET_OPENCL 0
    #define GF_DEVICE_TARGET_HLSL 0
    #define GF_DEVICE_TARGET_HOST 0
    #define GF_DEVICE_TARGET_DEVICE 1

#else
    #define GF_DEVICE_TARGET_CUDA 0
    #define GF_DEVICE_TARGET_OPENCL 0
    #define GF_DEVICE_TARGET_HLSL 0
    #define GF_DEVICE_TARGET_METAL 0
    #define GF_DEVICE_TARGET_HOST 1
    #define GF_DEVICE_TARGET_DEVICE 0
#endif

#if GF_DEVICE_TARGET_METAL
    #include <metal_common>
    #include <metal_compute>
    #include <metal_integer>
    #include <metal_texture>
using namespace metal;
#endif

/**
** Device sections - one-liner target dependent code insertions
** Examples:
**      GF_CUDA_SECTION(width=16;)
**      GF_OPENCL_SECTION(#pragma UNROLL(32))
** The section macros have a hidden superpower: they can defer processing of preprocessor directives until second time the text is preprocessed.
** This is useful for when source code is preprocessed twice, once on the host during host compilation, and then again at runtime in the device compiler.
** This allows us to filter out non-kernel code from the source sent to the device compiler and pull in the required includes, while still keeping line numbers correct.
** Line numbers are important for locating compiler messages in the kernel source and for debugging tools to work correctly.
** Double preprocessing means special treatment is needed for preprocessor directives aimed at the device, such as "pragma"
**
** Don't use section macros for more than one liners or structures. Line number tracking doesn't work within them.
*/
#ifndef GF_HOST_SECTION
    #if GF_DEVICE_TARGET_HOST
        #define GF_HOST_SECTION(...) __VA_ARGS__
        #define GF_SHARED_SECTION(NS, ...)                                                                             \
            namespace NS                                                                                               \
            {                                                                                                          \
            __VA_ARGS__                                                                                                \
            }
        #define GF_DEVICE_SECTION(...)
    #else
        #define GF_HOST_SECTION(...)
        #define GF_SHARED_SECTION(NS, ...) __VA_ARGS__
        #define GF_DEVICE_SECTION(...) __VA_ARGS__
    #endif

    #if GF_DEVICE_TARGET_CUDA
        #define GF_CUDA_SECTION(...) __VA_ARGS__
    #else
        #define GF_CUDA_SECTION(...)
    #endif

    #if GF_DEVICE_TARGET_OPENCL
        #define GF_OPENCL_SECTION(...) __VA_ARGS__
    #else
        #define GF_OPENCL_SECTION(...)
    #endif

    #if GF_DEVICE_TARGET_HLSL
        #define GF_HLSL_SECTION(...) __VA_ARGS__
    #else
        #define GF_HLSL_SECTION(...)
    #endif

    #if GF_DEVICE_TARGET_METAL
        #define GF_METAL_SECTION(...) __VA_ARGS__
    #else
        #define GF_METAL_SECTION(...)
    #endif
#endif

/*
** The GF_KERNEL_FUNCTION macro is used to declare a kernel and pass values as parameters (OpenCL/CUDA) or in a struct (metal).
** The macro will create an API-specific kernel entry point which will call a device function that it defines, leaving you to fill in the body.
** The macro uses Boost preprocessor sequences to express a type/name pair:
**      (float)(inValue)
** These pairs are then nested into a sequence of parameters:
**      ((float)(inAge))((int)(inMarbles))
** There are different categories of parameters, such as buffers, values, and kernel position.
** Each category sequence is a separate macro parameter.
** Example usage:
**  GF_KERNEL_FUNCTION(RemoveFlicker,               //kernel name, then comma,
**      ((GF_PTR(float4))(inSrc))                   //all buffers and textures go after the first comma
**      ((GF_PTR(float4))(outDest)),
**      ((int)(inDestPitch))                        //After the second comma, all values to be passed
**      ((DevicePixelFormat)(inDeviceFormat))
**      ((int)(inWidth))
**      ((int)(inHeight)),
**      ((uint2)(inXY)(KERNEL_XY))                  //After the third comma, the position arguments.
**      ((uint2)(inBlockID)(BLOCK_ID)))
**  {
**      <do something interesting here>
**  }
** In the example above, the host does not pass the position values when invoking the kernel.
** Position values are filled in automatically by the unmarshalling code.
** See the example plugins for usage.
*/
#define GF_KERNEL_FUNCTION(name, buffers, values, positions)                                                           \
    GF_UNMARSHAL(name, buffers, values, positions, BOOST_PP_NIL)

/**
** Declare kernels that use shared memory slightly differently, depending in whether it is statically or dynamically sized.
** Statically sized shared memory:
**  GF_KERNEL_FUNCTION_SHARED is used for kernels that need statically sized shared memory.
**  The GF_KERNEL_FUNCTION_SHARED macro takes a fifth argument that is a sequence:
**      ((sharedMemDeclarationInKernel)(sharedMemForFunctionSignature)(argumentToPassToFunction))
**  In practice, it looks something like:
**      ((GF_SHARED(float) inMoo[42])(GF_SHARED_PTR(float)inMoo)(&inMoo[0]))
**  or
**      ((GF_SHARED(float4) block2D[17][18])(GF_SHARED_PTR_QUALIFIER float4 (*block2D)[18])(&block2D[0])))
**  Shared memory parameters to functions must be pointers in OpenCL, so arrays must be passed by pointer.
**
** Dynamically sized shared memory:
**  Use GF_KERNEL_FUNCTION, not GF_KERNEL_FUNCTION_SHARED.
**  Dynamic shared memory is a setting on the kernel for CUDA, and passed through the parameter list for non-CUDA.
**  To support both, there's an overlap in the APIs below.
**  Host:
**      Declare kernel parameter as shared dynamic. (This parameter will be skipped for CUDA):
**          GF_NONCUDA_SHARED_DYNAMIC(float,smem),
**      Before calling kernel, set shared size (for CUDA):
**          GF::SetSharedSize(inDevice, deviceKernel, smem_byte_count);
**      Call kernel, passing size of shared buffer in parameter list (skipped for CUDA, due to the declaration above):
**          GF::SizeType(smem_byte_count)
**  Kernel:
**      Declare parameter in buffer section of GF_KERNEL_FUNCTION parameter list (no-op for CUDA):
**          GF_NONCUDA_SHARED_DYNAMIC(float,smem)
**      Declare external pointer in body (no-op for non-CUDA)
**          GF_CUDA_SHARED_DYNAMIC(float, smem);
**   A kernel may have at most one dynamically shared section.
*/
#define GF_KERNEL_FUNCTION_SHARED(name, buffers, values, positions, shares)                                            \
    GF_UNMARSHAL(name, buffers, values, positions, BOOST_PP_SEQ_TO_LIST(shares))

/**
**  Device function declaration.
**      GF_DEVICE_FUNCTION float Average(float a, float b) {...
*/
#ifdef __NVCC__ /* CUDA compiler for device or host */
    #define GF_DEVICE_FUNCTION __inline__ __device__

#elif GF_DEVICE_TARGET_HOST
    #define GF_DEVICE_FUNCTION __inline__

#elif GF_DEVICE_TARGET_OPENCL
    #define GF_DEVICE_FUNCTION static __inline__

#elif GF_DEVICE_TARGET_HLSL
    #define GF_DEVICE_FUNCTION inline

#elif GF_DEVICE_TARGET_METAL
    #define GF_DEVICE_FUNCTION
#endif

/*
**  GF_HOST_FUNCTION declaration is mostly useful (along with GF_DEVICE_TARGET_* and __NVCC__)
**  for CUDA style programming that mixes host and device code in the same file.
*/
#ifdef __NVCC__
    #define GF_HOST_FUNCTION __host__

#else
    #define GF_HOST_FUNCTION
#endif

/**
**  Memory - pointers
**
**  GF_PTR declares a pointer to global memory with Read-write access:
**      GF_PTR(float4) myImage;
**
**  GF_PTR_READ_ONLY declares a pointer to global memory with Read-only access:
**      GF_PTR_READ_ONLY(float4) myReadOnlyImage;
**
** GF_THREAD_PTR declares a pointer to thread private memory:
**      float2 xy;
**      GF_THREAD_PTR(float2) xyPtr = GF_REF(xy);
** For multiple elements, use GF_THREAD_ARRAY
** Example below pulls in an array of 3 float2 elements from a 2D array
** GF_THREAD_ARRAY(float2, vertices, 3) = vertex2DArr[0]
*/
#if GF_DEVICE_TARGET_CUDA || GF_DEVICE_TARGET_HOST
    #define GF_PTR(Type) Type*
    #define GF_PTR_READ_ONLY(Type) const Type*
    #define GF_THREAD_PTR(Type) Type*
    #define GF_THREAD_ARRAY(Type, Name, Size) GF_THREAD_PTR(Type) Name

#elif GF_DEVICE_TARGET_OPENCL
    #define GF_PTR(Type) __global Type*
    #define GF_PTR_READ_ONLY(Type) __global const Type*
    #define GF_THREAD_PTR(Type) Type*
    #define GF_THREAD_ARRAY(Type, Name, Size) GF_THREAD_PTR(Type) Name

#elif GF_DEVICE_TARGET_HLSL
    #define GF_PTR(Type) RWByteAddressBuffer
    #define GF_PTR_READ_ONLY(Type) ByteAddressBuffer
    #define GF_THREAD_PTR(Type) Type
    #define GF_THREAD_ARRAY(Type, Name, Size) GF_THREAD_PTR(Type) Name[Size]

#elif GF_DEVICE_TARGET_METAL
    #define GF_PTR(Type) device Type*
    #define GF_PTR_READ_ONLY(Type) device const Type*
    #define GF_THREAD_PTR(Type) Type thread*
    #define GF_THREAD_ARRAY(Type, Name, Size) GF_THREAD_PTR(Type) Name
#endif

/**
**  Memory - alignment
**
**  GF_ALIGN structure alignment example:
**      struct GF_ALIGN(8)
**      {
**          float a, b;
**      }
*/
#if GF_DEVICE_TARGET_CUDA
    #define GF_ALIGN(inSize) __align__(inSize)

#elif GF_DEVICE_TARGET_OPENCL
    #define GF_ALIGN(inSize) __attribute__((aligned(inSize)))

#elif GF_DEVICE_TARGET_HOST
    #if defined(_MSC_VER)
        #define GF_ALIGN(inSize) __declspec(align(inSize))
    #else
        #define GF_ALIGN(inSize) __attribute__((aligned(inSize)))
    #endif

#elif GF_DEVICE_TARGET_HLSL
    // HLSL enforces 16 byte packing for cbuffer but legacy binding rules can quite finicky and weird
    // So, we bind constant buffers as StructuredBuffer to work around them
    // TODO: Figure out any specific binding needs
    #define GF_ALIGN(inSize)

#elif GF_DEVICE_TARGET_METAL
    #define GF_ALIGN(inSize) alignas(inSize)
#endif

/**
**  Unchanging values declared in global scope on GPU
**  Example:
**      GF_CONSTANT(float) kConversionFactors[9] = ...
**  This may be accessed as an array:
**      x = y * kConversionFactors[9];
**  It has a special pointer type using GF_STATIC_CONSTANT_PTR
*/
#ifdef __NVCC__
    #define GF_CONSTANT(Type) __constant__ Type
    #define GF_STATIC_CONSTANT_PTR(Type) GF_CONSTANT_PTR(Type)

#elif GF_DEVICE_TARGET_HOST
    #define GF_CONSTANT(Type) Type

#elif GF_DEVICE_TARGET_OPENCL
    #define GF_CONSTANT(Type) __constant Type
    #define GF_STATIC_CONSTANT_PTR(Type) GF_CONSTANT(Type*)

#elif GF_DEVICE_TARGET_HLSL
    #define GF_CONSTANT(Type) static Type
    #define GF_STATIC_CONSTANT_PTR(Type) GF_CONSTANT(Type)

#elif GF_DEVICE_TARGET_METAL
    #define GF_CONSTANT(Type) constant Type
    #define GF_STATIC_CONSTANT_PTR(Type) GF_CONSTANT_PTR(Type)
#endif

/**
**  Uniform buffer - constant for a given kernel launch
**  The compute APIs treat these differently -
**  CUDA passes these as a global,
**  OpenCL receives them as a buffer in global address space
**  Metal receives them as a buffer in constant address space
**  HLSL receives them as a UAV Structured Buffer to work around the legacy packing requirements for CBVs
**  Portable kernel code needs to set up for both, and the macros will stub out the inappropriate path
**  The kernel file declares a global:
**      GF_CUDA_CONSTANT(Curve, inCurve)
**  It also declares the buffer as a kernel buffer parameter:
**      GF_KERNEL_FUNCTION(CurveKernel,
**          ((const GF_PTR(float4))(inImage))
**          ((GF_NONCUDA_CONSTANT_TYPE(Curve))(GF_NONCUDA_CONSTANT_NAME(inCurve))), ...
**  The body of the function accesses it using GF_GET_CONSTANT, which returns a GF_CONSTANT_PTR:
**      GF_CONSTANT_PTR(Curve) curveBuffer = GF_GET_CONSTANT(inCurve);
**  If the host is using CUDA, it uses cuModuleGetGlobal and friends to set up the constant,
**  and passes NULL as the kernel buffer parameter.
**  To pass arrays of objects as parameters to a function, use GF_CONSTANT_ARRAY(Type, Name, Size)
**  For other APIs, pass the the buffer as the kernel buffer parameter.
*/
#if GF_DEVICE_TARGET_CUDA || GF_DEVICE_TARGET_HOST
    #ifdef __NVCC__
        #define GF_CUDA_CONSTANT(Type, Name) __device__ GF_CONSTANT(Type) Name;
    #else
        #define GF_CUDA_CONSTANT(Type, Name) GF_CONSTANT(Type) Name;
    #endif
    #define GF_NONCUDA_CONSTANT_TYPE(Type) const void*
    #define GF_NONCUDA_CONSTANT_NAME(Name) Constant##Name
    #define GF_GET_CONSTANT(inDeclarator) (&inDeclarator)
    #define GF_CONSTANT_PTR(Type) GF_PTR_READ_ONLY(Type)
    #define GF_CONSTANT_ARRAY(Type, Name, Size) GF_CONSTANT_PTR(Type) Name

#elif GF_DEVICE_TARGET_OPENCL
    #define GF_CUDA_CONSTANT(Type, Name)
    #define GF_NONCUDA_CONSTANT_TYPE(Type) const GF_PTR(Type)
    #define GF_NONCUDA_CONSTANT_NAME(Name) (Name)
    #define GF_GET_CONSTANT(inDeclarator) inDeclarator
    #define GF_CONSTANT_PTR(Type) GF_PTR_READ_ONLY(Type)
    #define GF_CONSTANT_ARRAY(Type, Name, Size) GF_CONSTANT_PTR(Type) Name

#elif GF_DEVICE_TARGET_HLSL
    #define GF_CUDA_CONSTANT(Type, Name)
    #define GF_NONCUDA_CONSTANT_TYPE(Type) StructuredBuffer<Type>
    #define GF_NONCUDA_CONSTANT_NAME(Name) Name
    #define GF_GET_CONSTANT(inDeclarator) inDeclarator[0]
    #define GF_CONSTANT_PTR(Type) const Type
    #define GF_CONSTANT_ARRAY(Type, Name, Size) const Type Name[Size]

#elif GF_DEVICE_TARGET_METAL
    #define GF_CUDA_CONSTANT(Type, Name)
    #define GF_NONCUDA_CONSTANT_TYPE(Type) GF_CONSTANT_PTR(Type)
    #define GF_NONCUDA_CONSTANT_NAME(Name) (Name)
    #define GF_GET_CONSTANT(inDeclarator) inDeclarator
    #define GF_CONSTANT_PTR(Type) const constant Type*
    #define GF_CONSTANT_ARRAY(Type, Name, Size) GF_CONSTANT_PTR(Type) Name
#endif

/**
**  Alignment for structs, pointers of which will be accessed in constant address space
**  with GF_CONSTANT_PTR(struct_name). Such structs need to be declared as
**  typedef struct GF_ALIGN(GF_ALIGNMENT_FOR_CONSTANT_PTR)
**  {
**      //Struct definition
**      ...
**  } struct_name;
*/
#define GF_ALIGNMENT_FOR_CONSTANT_PTR (16)

/**
**  Memory shared within a thread group.
**  See comments above GF_KERNEL_FUNCTION_SHARED for usage examples
*/
#if GF_DEVICE_TARGET_CUDA || GF_DEVICE_TARGET_HOST
    #ifdef __NVCC__
        #define GF_SHARED(Type) volatile __shared__ Type
    #else
        #define GF_SHARED(Type) Type
    #endif
    #define GF_SHARED_PTR(Type) volatile Type*
    #define GF_SHARED_PTR_QUALIFIER volatile
    #define GF_CUDA_SHARED_DYNAMIC(Type, Name) extern GF_SHARED(Type) Name[]
    #if GF_DEVICE_TARGET_CUDA
        #define GF_NONCUDA_SHARED_DYNAMIC(Type, Name)
    #else //host
        #define GF_NONCUDA_SHARED_DYNAMIC(Type, Name)                                                                  \
            GF::ArgumentFramework<GF::kArgumentFrameworkSharedMemory, GF_SHARED(Type*)>
    #endif

#elif GF_DEVICE_TARGET_OPENCL
    #define GF_SHARED(Type) __local Type
    #define GF_SHARED_PTR(Type) __local Type*
    #define GF_SHARED_PTR_QUALIFIER __local
    #define GF_CUDA_SHARED_DYNAMIC(Type, Name)
    // DirectX needs to know whether a kernel uses dynamic shared memory. The DirectX kernel build machinery
    // uses the DX_SHARED_DYNAMIC_FLAG comment to flag these kernels. See MediaCore/Make/dvaclc.
    #define GF_NONCUDA_SHARED_DYNAMIC(Type, Name) ((GF_SHARED_PTR(Type))(Name)) /* DX_SHARED_DYNAMIC_FLAG */

#elif GF_DEVICE_TARGET_HLSL
    #define GF_SHARED(Type) groupshared Type
    #define GF_SHARED_PTR(Type) Type
    #define GF_SHARED_PTR_QUALIFIER groupshared
    #define GF_CUDA_SHARED_DYNAMIC(Type, Name)
    // HLSL does not support setting dynamic amount of shared memory. We keep an estimate of the amount of shared memory an entry point
    // can expect and capture that value in DXDynamicSharedMemory.json and set that in post processing.
    #define GF_NONCUDA_SHARED_DYNAMIC(Type, Name) ((GF_SHARED(Type))(Name[__HLSL_SHARED_MEMORY_SIZE]))

#elif GF_DEVICE_TARGET_METAL
    #define GF_SHARED(Type) threadgroup Type
    #define GF_SHARED_PTR(Type) threadgroup Type*
    #define GF_SHARED_PTR_QUALIFIER threadgroup
    #define GF_CUDA_SHARED_DYNAMIC(Type, Name)
    #define GF_NONCUDA_SHARED_DYNAMIC(Type, Name) ((GF_SHARED_PTR(Type))(Name))
#endif

/**
** Shared memory arguments to device local functions
** GF_SHARED_PARAM(Type, Name, Size) - Pass shared memory of Type with a specific size
** GF_SHARED_ARRAY_DYNAMIC(Type, Name) - Pass shared memory of dynamic size
*/
#if GF_DEVICE_TARGET_HLSL
    #define GF_SHARED_PARAM(Type, Name, Size) Type Name##_shared_param_unused[Size]
    #define GF_SHARED_ARRAY_DYNAMIC(Type, Name) GF_SHARED_PARAM(Type, Name, __HLSL_SHARED_MEMORY_SIZE)
#else
    #define GF_SHARED_PARAM(Type, Name, Size) GF_SHARED_PTR(Type) Name
    #define GF_SHARED_ARRAY_DYNAMIC(Type, Name) GF_SHARED_PTR(Type) Name
#endif

/**
** Pointers and References
** Use the following macros for referencing and dereferencing resources
** (*data) -> GF_DEREF(data)
** (&data) -> GF_REF(data)
**
** To receive objects passed by reference, use the following Macros
** GF_PASSED_BY_REF(Type) - Receive a single object by reference
** GF_ARR_PASSED_BY_REF(Type, Name, Size) - Receive an array by reference
*/
#if GF_DEVICE_TARGET_HLSL
    #define GF_DEREF(Name) (Name)
    #define GF_REF(Name) (Name)
    #define GF_PASSED_BY_REF(Type) inout Type
    #define GF_ARR_PASSED_BY_REF(Type, Name, Size) GF_PASSED_BY_REF(Type) Name[Size]
#else
    #define GF_DEREF(Name) (*Name)
    #define GF_REF(Name) (&Name)
    #define GF_PASSED_BY_REF(Type) GF_THREAD_PTR(Type)
    #define GF_ARR_PASSED_BY_REF(Type, Name, Size) GF_PASSED_BY_REF(Type) Name
#endif

/**
** Simplified array access
** For edges that rely on subscripting, we explicitly typecast the pointer to the type for which data is being read
*/
#if GF_DEVICE_TARGET_HLSL
    #define GF_READINDEX_TYPE(Type, Name, Index) (Name).Load<Type>((Index) * sizeof(Type))
    #define GF_WRITEINDEX_TYPE(Type, Value, Name, Index) (Name).Store<Type>(((Index) * sizeof(Type)), (Type)(Value))
#else
    #define GF_READINDEX_TYPE(Type, Name, Index) ((GF_PTR_READ_ONLY(Type))(Name))[Index]
    #define GF_WRITEINDEX_TYPE(Type, Value, Name, Index) ((GF_PTR(Type))(Name))[Index] = Value
#endif

/**
**  Optimized 2D array access
*/
#if GF_DEVICE_TARGET_OPENCL
    #define GF_FASTMULTIPLY(inX, inY) mul24((inX), (inY))
#else
    #define GF_FASTMULTIPLY(inX, inY) ((inX) * (inY))
#endif
#define GF_READ2D_TYPE(Type, Name, inRowStride, inX, inY)                                                              \
    GF_READINDEX_TYPE(Type, Name, GF_FASTMULTIPLY((inY), (inRowStride)) + (inX))
#define GF_WRITE2D_TYPE(Type, Value, Name, inRowStride, inX, inY)                                                      \
    GF_WRITEINDEX_TYPE(Type, Value, Name, GF_FASTMULTIPLY((inY), (inRowStride)) + (inX))

/**
**  Textures
**  Textures have different usage patterns in the different APIs, so we need to follow both patterns and let the macros stub the unneeded path for the current API.
**  A typical texturing kernel file uses this pattern:
**      Declare the texture at global scope:
**          GF_TEXTURE_GLOBAL(float4, inSrcTexture, GF_DOMAIN_NATURAL, GF_RANGE_NATURAL_CUDA, GF_EDGE_CLAMP, GF_FILTER_LINEAR)
**      Declare the texture as a kernel parameter:
**          GF_KERNEL_FUNCTION(ApplyCowSpotsKernel,
**              ((GF_TEXTURE_TYPE(float))(GF_TEXTURE_NAME(inSrcTexture)))...
**      Sample the texture inside the body of the kernel:
**          float4 color = GF_READTEXTUREPIXEL(inSrcTexture, srcX + 0.5, srcY + 0.5);
**  The macros:
**      GF_TEXTURE_GLOBAL(
**          PixelType,  //The type of the pixel, e.g. float4
**          Name,       //A name for the texture. Must match that used in GF_TEXTURE_NAME
**          inDomain,   //The position type used - GF_DOMAIN_UNIT for 0-1, GF_DOMAIN_NATURAL for 0-width (or height)
**          inRange,    //The output range for integer textures - GF_RANGE_UNIT for 0-1
**          inEdge,     //The edge treatment such as GF_EDGE_CLAMP or GF_EDGE_BORDER
**          inFilter)   //The interpolation filter such as GF_FILTER_NEAREST or GF_FILTER_LINEAR
**      For CUDA, these properties are set by the host, and the GF_TEXTURE_GLOBAL properties are ignored.
**      You'll want to ensure the two sets match, so that your results match across APIs!
**
**      GF_TEXTURE_TYPE(ChannelType) - the type of a single channel in the texture. This can be used in declaring kernels and device functions.
**
**      GF_TEXTURE_NAME(Name) - the name must match that used in GF_TEXTURE_GLOBAL
**
**      GF_READTEXTURE(Name, inX, inY) - sample texture of given name at given position. Name must match that used in GF_TEXTURE_GLOBAL.
*/
#if GF_DEVICE_TARGET_CUDA || GF_DEVICE_TARGET_HOST
    #define GF_TEXTURE_TYPE(ChannelType) void*
    #define GF_TEXTURE_TYPE_3D(ChannelType) void*
    #define GF_TEXTURE_TYPE_VECTOR(ChannelType, NumChannels) GF_TEXTURE_TYPE(ChannelType)
    #define GF_TEXTURE_TYPE_3D_VECTOR(ChannelType, NumChannels) GF_TEXTURE_TYPE_3D(ChannelType)
    #define GF_TEXTURE_TYPE_WRITE_ONLY(ChannelType) GF_TEXTURE_TYPE(ChannelType)
    #define GF_TEXTURE_NAME(Name) Texture##Name
    #define GF_GET_TEXTURE(Name) 0
    #define GF_READTEXTURE(Name, inX, inY) tex2D(Name, (inX), (inY))
    #define GF_READTEXTURE3D(Name, inX, inY, inZ) tex3D(Name, (inX), (inY), (inZ))
    #define GF_WRITETEXTURE(Name, inX, inY, inData) surf2Dwrite(Name, inData, (inX), (inY))
    #define GF_TEXTURE_GLOBAL(PixelType, Name, inDomain, inRange, inEdge, inFilter)                                    \
        static texture<PixelType, cudaTextureType2D, inRange> Name;
    #define GF_TEXTURE_GLOBAL_3D(PixelType, Name, inDomain, inRange, inEdge, inFilter)                                 \
        static texture<PixelType, cudaTextureType3D, inRange> Name;
    #define GF_TEXTURE_GLOBAL_WRITE_ONLY(Name) surface<void, cudaSurfaceType2D> Name;

    // CUDA Only: For normalized coordinate support, pass CU_TRSF_NORMALIZED_COORDINATES to LoadTexture on the host side when initializing textures
    #define GF_DOMAIN_NATURAL // positions in pixels
    #define GF_DOMAIN_UNIT    // position normalized

    #define GF_RANGE_NATURAL_CUDA cudaReadModeElementType // unnormalized result (for CUDA only)
    #define GF_RANGE_UNIT cudaReadModeNormalizedFloat     // non-float texture values normalized

    #define GF_EDGE_CLAMP  // clamp to edge pixel
    #define GF_EDGE_BORDER // clamp to border value (black)

    #define GF_FILTER_NEAREST
    #define GF_FILTER_LINEAR

#elif GF_DEVICE_TARGET_OPENCL
    #define GF_TEXTURE_TYPE(ChannelType) __read_only image2d_t
    #define GF_TEXTURE_TYPE_3D(ChannelType) __read_only image3d_t
    #define GF_TEXTURE_TYPE_VECTOR(ChannelType, NumChannels) GF_TEXTURE_TYPE(ChannelType)
    #define GF_TEXTURE_TYPE_3D_VECTOR(ChannelType, NumChannels) GF_TEXTURE_TYPE_3D(ChannelType)
    #define GF_TEXTURE_TYPE_WRITE_ONLY(ChannelType) __write_only image2d_t
    #define GF_TEXTURE_NAME(Name) Name
    #define GF_GET_TEXTURE(Name) Name

    #define GF_READTEXTURE(Name, inX, inY) read_imagef(Name, kSampler_##Name, (float2)((inX), (inY)))
    #define GF_READTEXTURE3D(Name, inX, inY, inZ)                                                                      \
        read_imagef(Name, kSampler_##Name, (float4)((inX), (inY), (inZ), (1.0f)))
    #define GF_WRITETEXTURE(Name, inX, inY, inData) write_imagef(Name, (int2)((inX), (inY)), inData)
    #define GF_TEXTURE_GLOBAL(PixelType, Name, inDomain, inRange, inEdge, inFilter)                                    \
        constant sampler_t kSampler_##Name = inDomain | inEdge | inFilter;
    #define GF_TEXTURE_GLOBAL_3D GF_TEXTURE_GLOBAL
    #define GF_TEXTURE_GLOBAL_WRITE_ONLY(Name)

    #define GF_DOMAIN_NATURAL CLK_NORMALIZED_COORDS_FALSE
    #define GF_DOMAIN_UNIT CLK_NORMALIZED_COORDS_TRUE

    #define GF_RANGE_NATURAL_CUDA GF_RANGE_UNIT
    #define GF_RANGE_UNIT // non-float texture values normalized

    #define GF_EDGE_CLAMP CLK_ADDRESS_CLAMP_TO_EDGE
    #define GF_EDGE_BORDER CLK_ADDRESS_CLAMP

    #define GF_FILTER_NEAREST CLK_FILTER_NEAREST
    #define GF_FILTER_LINEAR CLK_FILTER_LINEAR

#elif GF_DEVICE_TARGET_HLSL
    #define GF_TEXTURE_TYPE(ChannelType) Texture2D<ChannelType>
    #define GF_TEXTURE_TYPE_3D(ChannelType) Texture3D<ChannelType>
    #define GF_TEXTURE_TYPE_VECTOR(ChannelType, NumChannels) GF_TEXTURE_TYPE(ChannelType##NumChannels)
    #define GF_TEXTURE_TYPE_3D_VECTOR(ChannelType, NumChannels) GF_TEXTURE_TYPE_3D(ChannelType##NumChannels)
    #define GF_TEXTURE_TYPE_WRITE_ONLY(ChannelType) RWTexture2D<ChannelType>
    #define GF_TEXTURE_NAME(Name) Name
    #define GF_GET_TEXTURE(Name) Name

    #define GF_READTEXTURE(Name, inX, inY) DirectXRead2DTexture(kSampler_##Name, Name, (inX), (inY))
    #define GF_READTEXTURE3D(Name, inX, inY, inZ) DirectXRead3DTexture(kSampler_##Name, Name, (inX), (inY), (inZ))
    #define GF_WRITETEXTURE(Name, inX, inY, inData) Name[float2((inX), (inY))] = inData

    #define GF_TEXTURE_GLOBAL(PixelType, Name, inDomain, inRange, inEdge, inFilter)                                    \
        SamplerState kSampler_##Name##__HLSL_SAMPLER;                                                                  \
        filter = inFilter, addressU = inEdge, addressV = inEdge, addressW = inEdge
    #define GF_TEXTURE_GLOBAL_3D(PixelType, Name, inDomain, inRange, inEdge, inFilter)                                 \
        SamplerState kSampler_##Name##__HLSL_SAMPLER;                                                                  \
        filter = inFilter, addressU = inEdge, addressV = inEdge, addressW = inEdge
    #define GF_TEXTURE_GLOBAL_WRITE_ONLY(Name)

    #define GF_DOMAIN_UNIT
    #define GF_DOMAIN_NATURAL
    #define GF_RANGE_UNIT
    #define GF_RANGE_NORMAL

    #define GF_EDGE_CLAMP TEXTURE_ADDRESS_CLAMP
    #define GF_EDGE_BORDER TEXTURE_ADDRESS_BORDER

    #define GF_FILTER_NEAREST FILTER_MIN_MAG_MIP_POINT
    #define GF_FILTER_LINEAR FILTER_MIN_MAG_MIP_LINEAR

#elif GF_DEVICE_TARGET_METAL
template <typename ChannelType>
using GFMetalTextureType = texture2d<ChannelType, access::sample>;
template <typename ChannelType>
using GFMetal3DTextureType = texture3d<ChannelType, access::sample>;
template <typename ChannelType>
using GFMetalTextureTypeWriteOnly = texture3d<ChannelType, access::write>;
    #define GF_TEXTURE_TYPE(ChannelType)                                                                               \
        GFMetalTextureType<ChannelType> //boost preprocessor sequences don't like commas
    #define GF_TEXTURE_TYPE_3D(ChannelType)                                                                            \
        GFMetal3DTextureType<ChannelType> //boost preprocessor sequences don't like commas
    #define GF_TEXTURE_TYPE_VECTOR(ChannelType, NumChannels) GF_TEXTURE_TYPE(ChannelType)
    #define GF_TEXTURE_TYPE_3D_VECTOR(ChannelType, NumChannels) GF_TEXTURE_TYPE_3D(ChannelType)
    #define GF_TEXTURE_TYPE_WRITE_ONLY(ChannelType)                                                                    \
        GFMetalTextureTypeWriteOnly<ChannelType> //boost preprocessor sequences don't like commas
    #define GF_TEXTURE_NAME(Name) Name
    #define GF_GET_TEXTURE(Name) Name
    #define GF_READTEXTURE(Name, inX, inY) Name.sample(kSampler_##Name, make_float2((inX), (inY)))
    #define GF_READTEXTURE3D(Name, inX, inY, inZ) Name.sample(kSampler_##Name, make_float3((inX), (inY), (inZ)))
    #define GF_WRITETEXTURE(Name, inX, inY, inData) Name.write(inData, make_float2((inX), (inY)))
    #define GF_TEXTURE_GLOBAL(PixelType, Name, inDomain, inRange, inEdge, inFilter)                                    \
        constexpr sampler kSampler_##Name(inDomain, s_address::inEdge, t_address::inEdge, inFilter);
    #define GF_TEXTURE_GLOBAL_WRITE_ONLY(Name)

    #define GF_TEXTURE_GLOBAL_3D GF_TEXTURE_GLOBAL
    #define GF_DOMAIN_NATURAL coord::pixel
    #define GF_DOMAIN_UNIT coord::normalized

    #define GF_RANGE_NATURAL GF_RANGE_UNIT
    #define GF_RANGE_UNIT // non-float texture values normalized

    #define GF_EDGE_CLAMP clamp_to_edge
    #define GF_EDGE_BORDER clamp_to_zero

    #define GF_FILTER_NEAREST filter::nearest
    #define GF_FILTER_LINEAR filter::linear
#endif

#if GF_DEVICE_TARGET_HLSL
// HLSL does not support non-normalized coordinates when sampling data from textures.
// These functions extract the width and height from the texture and use it to create normalized coordinates for HLSL
template <typename T>
T DirectXRead2DTexture(SamplerState inSampler, Texture2D<T> inTexture, float inX, float inY)
{
    int width, height;
    inTexture.GetDimensions(width, height);
    return inTexture.SampleLevel(inSampler, float2(float(inX) / width, float(inY) / height), 0);
}

template <typename T>
T DirectXRead3DTexture(SamplerState inSampler, Texture3D<T> inTexture, float inX, float inY, float inZ)
{
    int width, height, depth;
    inTexture.GetDimensions(width, height, depth);
    return inTexture.SampleLevel(inSampler, float3(float(inX) / width, float(inY) / height, float(inZ) / depth), 0);
}
#endif

/**
**  Vector constructors
*/
#if GF_DEVICE_TARGET_OPENCL
    #ifndef make_int2
        #define make_int2(inX, inY) (int2)(inX, inY)
    #endif
    #ifndef make_uint2
        #define make_uint2(inX, inY) (uint2)(inX, inY)
    #endif
    #ifndef make_float4
        #define make_float4(inX, inY, inZ, inW) (float4)(inX, inY, inZ, inW)
    #endif
    #ifndef make_float3
        #define make_float3(inX, inY, inZ) (float3)(inX, inY, inZ)
    #endif
    #ifndef make_float2
        #define make_float2(inX, inY) (float2)(inX, inY)
    #endif
    #ifndef make_uchar2
        #define make_uchar2(inX, inY) (uchar2)(inX, inY)
    #endif
    #ifndef make_uchar4
        #define make_uchar4(inX, inY, inZ, inW) (uchar4)(inX, inY, inZ, inW)
    #endif
    #ifndef make_ushort2
        #define make_ushort2(inX, inY) (ushort2)(inX, inY)
    #endif
    #ifndef make_ushort4
        #define make_ushort4(inX, inY, inZ, inW) (ushort4)(inX, inY, inZ, inW)
    #endif
    #ifndef make_uint4
        #define make_uint4(inX, inY, inZ, inW) (uint4)(inX, inY, inZ, inW)
    #endif
    #ifndef make_int4
        #define make_int4(inX, inY, inZ, inW) (int4)(inX, inY, inZ, inW)
    #endif

#elif GF_DEVICE_TARGET_HLSL
    #define INT_MAX 2147483647
    #define short int16_t
    #define short2 int16_t2
    #define short4 int16_t4
    #define ushort uint16_t
    #define ushort2 uint16_t2
    #define convert_ushort2 uint16_t2
    #define ushort4 uint16_t4
    #define long int
    #define make_int2(inX, inY) int2(inX, inY)
    #define make_float4(inX, inY, inZ, inW) float4(inX, inY, inZ, inW)
    #define make_float3(inX, inY, inZ) float3(inX, inY, inZ)
    #define make_float2(inX, inY) float2(inX, inY)
    #define make_half4(inX, inY, inZ, inW) half4(inX, inY, inZ, inW)
    #define make_half2(inX, inY) half2(inX, inY)
    #define make_ushort2(inX, inY) uint16_t2(inX, inY)
    #define make_ushort4(inX, inY, inZ, inW) uint16_t4(inX, inY, inZ, inW)
    #define make_uint2(inX, inY) uint2(inX, inY)
    #define make_uint4(inX, inY, inZ, inW) uint4(inX, inY, inZ, inW)
    #define make_int4(inX, inY, inZ, inW) int4(inX, inY, inZ, inW)
    // HLSL does not support 8 bit integers. We use uint to carry uchar values
    #define make_uchar2(inX, inY) uint2(inX, inY)
    #define make_uchar4(inX, inY, inZ, inW) uint4(inX, inY, inZ, inW)

#elif GF_DEVICE_TARGET_METAL
    #define make_int2(inX, inY) int2(inX, inY)
    #define make_float4(inX, inY, inZ, inW)                                                                            \
        float4(static_cast<float>(inX), static_cast<float>(inY), static_cast<float>(inZ), static_cast<float>(inW))
    #define make_float3(inX, inY, inZ) float3(static_cast<float>(inX), static_cast<float>(inY), static_cast<float>(inZ))
    #define make_float2(inX, inY) float2(static_cast<float>(inX), static_cast<float>(inY))
    #define make_half4(inX, inY, inZ, inW)                                                                             \
        half4(static_cast<half>(inX), static_cast<half>(inY), static_cast<half>(inZ), static_cast<half>(inW))
    #define make_half2(inX, inY) half2(static_cast<half>(inX), static_cast<half>(inY))
    #define make_uchar2(inX, inY) uchar2(inX, inY)
    #define make_uchar4(inX, inY, inZ, inW) uchar4(inX, inY, inZ, inW)
    #define make_ushort2(inX, inY) ushort2(inX, inY)
    #define make_ushort4(inX, inY, inZ, inW) ushort4(inX, inY, inZ, inW)
    #define make_uint2(inX, inY) uint2(inX, inY)
    #define make_uint4(inX, inY, inZ, inW) uint4(inX, inY, inZ, inW)
    #define make_int4(inX, inY, inZ, inW) int4(inX, inY, inZ, inW)
#endif

/**
** Memory transaction synchronization within thread group
**
** SyncThreadsShared = block until all thread block transactions with shared memory are complete
** SyncThreadsDevice = block until all thread block transactions with device memory are complete
** SyncThreads       = block until all thread block transactions with shared memory and device memory are complete
**
** Important: All threads must call SyncThreads/SyncThreadsShared if any do, otherwise the kernel will hang on some devices.
*/
#if GF_DEVICE_TARGET_CUDA
GF_DEVICE_FUNCTION void SyncThreads()
{
    __syncthreads();
}
    #define SyncThreadsShared() SyncThreads();
    #define SyncThreadsDevice() SyncThreads();

#elif GF_DEVICE_TARGET_OPENCL
GF_DEVICE_FUNCTION void SyncThreads()
{
    barrier(CLK_LOCAL_MEM_FENCE);
}
    #define SyncThreadsShared() SyncThreads();
    #define SyncThreadsDevice() SyncThreads();

#elif GF_DEVICE_TARGET_HLSL
GF_DEVICE_FUNCTION void SyncThreads()
{
    AllMemoryBarrierWithGroupSync();
}
    #define SyncThreadsShared() GroupMemoryBarrierWithGroupSync();
    #define SyncThreadsDevice() DeviceMemoryBarrierWithGroupSync();

#elif GF_DEVICE_TARGET_METAL
GF_DEVICE_FUNCTION void SyncThreads()
{
    threadgroup_barrier(static_cast<metal::mem_flags>(
        static_cast<int>(metal::mem_flags::mem_device) | static_cast<int>(metal::mem_flags::mem_threadgroup)));
}

GF_DEVICE_FUNCTION void SyncThreadsShared()
{
    threadgroup_barrier(metal::mem_flags::mem_threadgroup);
}

GF_DEVICE_FUNCTION void SyncThreadsDevice()
{
    threadgroup_barrier(metal::mem_flags::mem_device);
}
#endif

/**
**  various ported functions
*/
#if GF_DEVICE_TARGET_CUDA
template <typename Type>
GF_DEVICE_FUNCTION Type clamp(Type inX, Type inY, Type inZ)
{
    return min(max(inX, inY), inZ);
}
#endif

#if GF_DEVICE_TARGET_OPENCL
GF_DEVICE_FUNCTION float saturate(float inX)
{
    return clamp(inX, 0.0f, 1.0f);
}
#endif

// Mesa toolchain has problems with pragma unroll parameter so elide the parameter.
#if GF_DEVICE_TARGET_OPENCL && DIRECTX
    #define UNROLL(...) unroll
#else
    #define UNROLL(...) unroll __VA_ARGS__
#endif

#include "PrGPU/KernelSupport/KernelWrapper.h" //depends on some of the above #defines

#endif
