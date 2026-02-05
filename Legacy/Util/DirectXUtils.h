/*************************************************************************
 * ADOBE CONFIDENTIAL
 * ___________________
 *
 * Copyright 2023 Adobe
 * All Rights Reserved.
 *
 * NOTICE: All information contained herein is, and remains
 * the property of Adobe and its suppliers, if any. The intellectual
 * and technical concepts contained herein are proprietary to Adobe
 * and its suppliers and are protected by all applicable intellectual
 * property laws, including trade secret and copyright laws.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Adobe.
 **************************************************************************/

#pragma once
#include <d3d12.h>
#include <d3dcompiler.h>
#include <memory>
#include <wrl.h>
#include <xstring>

struct ShaderObject
{
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPipelineState;
};
using ShaderObjectPtr = std::shared_ptr<ShaderObject>;

struct DXContext
{
	DXContext() = default;
	~DXContext();

	// Initializes the DirectX Context
	bool Initialize(ID3D12Device* inDevice, ID3D12CommandQueue* inCommandQueue);

	// Creates a Shader Object from CSO and RootSignature
	bool LoadShader(
		LPCWSTR inCSOPath,
		LPCWSTR inRootSignaturePath,
		ShaderObjectPtr& outShaderObject);

	// Reserve Descriptor Heap Slots
	bool ReserveDescriptorHeapSlots(UINT inNumDescriptors, UINT& outHeapOffset);

	// Wait and Reset for next execution
	void CloseWaitAndReset();

	// Command Execution Objects
	Microsoft::WRL::ComPtr<ID3D12Device> mDevice;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

	// Descriptor Heap Objects
	UINT mHeapOffset = 0;
	UINT mHandleSize;
	D3D12_CPU_DESCRIPTOR_HANDLE mCPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE mGPUHandle;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescriptorHeap;

	// Fence Objects
	UINT mFenceValue = 0;
	HANDLE mFenceEvent = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
};
using DXContextPtr = std::shared_ptr<DXContext>;

class DXShaderExecution
{
public:
	DXShaderExecution(
		const DXContextPtr& inContext,
		const ShaderObjectPtr& inShaderObject,
		const UINT inNumDescriptors);
	~DXShaderExecution() = default;

	/************************* Bind Calls ********************************/
	/* The Idx at which a specific type of element is bound should exactly match the one
	* in the RootSignature of the shader. In provided examples, we bind CBVs at Idx 0,
	* UAVs at Idx 1 and SRVs (if present) at Idx 2 (see DescriptorSlotIndex in DirectXUtils.cpp) */
	bool SetParamBuffer(void* inParamBuffer, UINT inParamBufferSize);

	/* Multiple calls to Set UAV/SRVs should follow the the order in which the buffers
	* are expected in the shader
	*/
	bool SetUnorderedAccessView(ID3D12Resource* inBuffer, UINT inBufferSize);
	bool SetShaderResourceView(ID3D12Resource* inBuffer, UINT inBufferSize);
	/*********************************************************************/

	// Executes the shader. Ensure all descriptors have been used
	bool Execute(const UINT inGridSizeX, const UINT inGridSizeY);

private:
	DXContextPtr mContext;
	ShaderObjectPtr mShaderObject;
	UINT mReservedDescriptorSlotBase;
	Microsoft::WRL::ComPtr<ID3D12Resource> mHostParamBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> mDeviceParamBuffer;
	bool mIsUAVBaseSet = false;
	bool mIsSRVBaseSet = false;
	bool mIsValid = true;
};

// Returns the Path of the CSO and Signature file
// The shaders are expected in "DirectX_Assets/<inModuleName>.(cso|rs)"
bool GetShaderPath(
	const wchar_t* inModuleName,
	std::wstring& outCSOPath,
	std::wstring& outSigPath);
