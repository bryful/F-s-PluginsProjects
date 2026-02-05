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

#include "DirectXUtils.h"
#include <vector>

 // This represents the index on which the specific Descriptor table is attached
enum DescriptorSlotIndex
{
	kCBVSlotIndex = 0,
	kUAVSlotIndex,
	kSRVSlotIndex
};

DXContext::~DXContext()
{
	if (mFenceEvent)
	{
		::CloseHandle(mFenceEvent);
		mFenceEvent = nullptr;
	}
}

bool DXContext::Initialize(ID3D12Device* inDevice, ID3D12CommandQueue* inCommandQueue)
{
	if (!inDevice || !inCommandQueue) { return false; }

	/* Subtle: The ComPtr constructor does an internal AddRef which would balance the RelaseRef when DXContext is deleted
	* If the AddRef is not enforced, the device/command queue/resource may get destroyed prematurely while the
	* application is alive leading to catastrophic results */
	mDevice = Microsoft::WRL::ComPtr<ID3D12Device>(inDevice);
	mCommandQueue = Microsoft::WRL::ComPtr<ID3D12CommandQueue>(inCommandQueue);
	HRESULT res = mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&mCommandAllocator));
	if (FAILED(res)) { return false; }
	res = mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&mCommandList));
	if (FAILED(res)) { return false; }

	mFenceValue = 0;
	mFenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
	res = mDevice->CreateFence(mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
	if (FAILED(res)) { return false; }

	mHeapOffset = 0;
	mHandleSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// Sub-optimal: Prefer using a ring buffer style implementation
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1000;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	res = mDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescriptorHeap));
	if (FAILED(res)) { return false; }
	mCPUHandle = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	mGPUHandle = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	return true;
}

bool DXContext::LoadShader(
	LPCWSTR inCSOPath,
	LPCWSTR inRootSignaturePath,
	ShaderObjectPtr& outShaderObject)
{
	if (!inCSOPath || !inRootSignaturePath) { return false; }

	HRESULT res = S_OK;
	Microsoft::WRL::ComPtr<ID3DBlob> rootSignatureBlob;

	// Load Root Signature
	res = D3DReadFileToBlob(inRootSignaturePath, &rootSignatureBlob);
	if (FAILED(res)) { return false; }

	// Create Root Signature
	res = mDevice->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&outShaderObject->mRootSignature));
	if (FAILED(res)) { return false; }

	// Load CSO file
	FILE* file = nullptr;
	if ((_wfopen_s(&file, inCSOPath, L"rb") != 0) || !file) { return false; }
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);
	std::vector<uint8_t> csoData(size);
	fread(csoData.data(), 1, size, file);
	fclose(file);

	// Create Pipeline State
	D3D12_COMPUTE_PIPELINE_STATE_DESC pipelineStateDesc = {};
	pipelineStateDesc.pRootSignature = outShaderObject->mRootSignature.Get();
	pipelineStateDesc.CS.pShaderBytecode = csoData.data();
	pipelineStateDesc.CS.BytecodeLength = size;
	res = mDevice->CreateComputePipelineState(&pipelineStateDesc, IID_PPV_ARGS(&outShaderObject->mPipelineState));
	if (FAILED(res)) { return false; }

	return true;
}

bool DXContext::ReserveDescriptorHeapSlots(UINT inNumDescriptors, UINT& outHeapOffset)
{
	// Note: All descriptors must be on the same heap, so if we don't have enough slots available,
	// create a new heap and use that instead
	if (mHeapOffset + inNumDescriptors > mDescriptorHeap->GetDesc().NumDescriptors)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = mDescriptorHeap->GetDesc();
		if (FAILED(mDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescriptorHeap)))) { return false; }
		mHeapOffset = 0;
		mCPUHandle = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		mGPUHandle = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	}
	outHeapOffset = mHeapOffset;
	mHeapOffset += inNumDescriptors;
	return true;
}

void DXContext::CloseWaitAndReset()
{
	// Close the command list
	mCommandList->Close();

	// Execute on the Command Queue
	ID3D12CommandList* commandLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(1, commandLists);

	// Ensure work completes
	mCommandQueue->Signal(mFence.Get(), ++mFenceValue);
	if (mFence->GetCompletedValue() < mFenceValue)
	{
		mFence->SetEventOnCompletion(mFenceValue, mFenceEvent);
		WaitForSingleObject(mFenceEvent, INFINITE);
	}

	// Reset the allocator and the command list to enable further execution
	mCommandAllocator->Reset();
	mCommandList->Reset(mCommandAllocator.Get(), nullptr);
}

DXShaderExecution::DXShaderExecution(
	const DXContextPtr& inContext,
	const ShaderObjectPtr& inShaderObject,
	const UINT inNumDescriptors)
	:
	mContext(inContext),
	mShaderObject(inShaderObject)
{
	if (!mContext->ReserveDescriptorHeapSlots(inNumDescriptors, mReservedDescriptorSlotBase))
	{
		mIsValid = false;
		return;
	}

	// Set the descriptor heap on the command list
	ID3D12DescriptorHeap* heaps[] = { mContext->mDescriptorHeap.Get() };
	mContext->mCommandList->SetDescriptorHeaps(_countof(heaps), heaps);

	// Set root signature on the command list. This is required before views can be created
	mContext->mCommandList->SetComputeRootSignature(mShaderObject->mRootSignature.Get());
}

bool DXShaderExecution::Execute(const UINT inGridSizeX, const UINT inGridSizeY)
{
	if (!mIsValid || mReservedDescriptorSlotBase != mContext->mHeapOffset) { return false; }

	mContext->mCommandList->SetPipelineState(mShaderObject->mPipelineState.Get());
	mContext->mCommandList->Dispatch(inGridSizeX, inGridSizeY, 1);

	// Sub-optimal: Implement deferred execution over waiting synchronously for better performance
	mContext->CloseWaitAndReset();
	mIsValid = false;
	return true;
}

bool DXShaderExecution::SetParamBuffer(
	void* inParamBuffer,
	UINT inParamBufferSize)
{
	HRESULT res = S_OK;
	if (!mIsValid || !inParamBuffer || !inParamBufferSize) { mIsValid = false; return mIsValid; }
	if (mReservedDescriptorSlotBase >= mContext->mHeapOffset) { mIsValid = false; return mIsValid; }

	// Allocate host memory to transfer Params to GPU
	// Note: CBV memory has to be D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT aligned.
	// As we know the size of the param buffer is strictly less than D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT, 
	// we are using it instead
	D3D12_RESOURCE_DESC paramDesc = {
		D3D12_RESOURCE_DIMENSION_BUFFER, 0,
		D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT, 1, 1, 1,
		DXGI_FORMAT_UNKNOWN, 1, 0,
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR, D3D12_RESOURCE_FLAG_NONE };
	D3D12_HEAP_PROPERTIES hostHeapProperties = {
		D3D12_HEAP_TYPE_UPLOAD,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0, 0 };
	res = mContext->mDevice->CreateCommittedResource(
		&hostHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&paramDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(mHostParamBuffer.GetAddressOf()));
	if (FAILED(res)) { mIsValid = false; return mIsValid; }

	// Copy Params to the mapped host memory
	void* mappedMemory = nullptr;
	res = mHostParamBuffer->Map(0, nullptr, &mappedMemory);
	if (FAILED(res)) { mIsValid = false; return mIsValid; }
	memcpy(mappedMemory, inParamBuffer, inParamBufferSize);
	mHostParamBuffer->Unmap(0, nullptr);

	// Allocate device memory
	D3D12_HEAP_PROPERTIES deviceHeapProperties = {
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0, 0 };
	res = mContext->mDevice->CreateCommittedResource(
		&deviceHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&paramDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(mDeviceParamBuffer.GetAddressOf()));
	if (FAILED(res)) { mIsValid = false; return mIsValid; }

	// Upload param data
	mContext->mCommandList->CopyResource(mDeviceParamBuffer.Get(), mHostParamBuffer.Get());

	// The CBV will next be used as a Constant Buffer during kernel execution.
	// It has to be explicitly transitioned to the correct state for that to happen
	D3D12_RESOURCE_BARRIER transitionBarrier;
	transitionBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	transitionBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	transitionBarrier.Transition.pResource = mDeviceParamBuffer.Get();
	transitionBarrier.Transition.Subresource = 0;
	transitionBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	transitionBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	mContext->mCommandList->ResourceBarrier(1, &transitionBarrier);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = mDeviceParamBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
	mContext->mDevice->CreateConstantBufferView(&cbvDesc,
		{ mContext->mCPUHandle.ptr + (SIZE_T)mReservedDescriptorSlotBase * mContext->mHandleSize });
	mContext->mCommandList->SetComputeRootDescriptorTable(kCBVSlotIndex,
		{ mContext->mGPUHandle.ptr + (UINT64)mReservedDescriptorSlotBase * mContext->mHandleSize });
	mReservedDescriptorSlotBase++;
	return true;
}

bool DXShaderExecution::SetUnorderedAccessView(
	ID3D12Resource* inBuffer,
	UINT inBufferSize)
{
	if (!mIsValid || !inBuffer || !inBufferSize) { mIsValid = false; return mIsValid; }
	if (mReservedDescriptorSlotBase >= mContext->mHeapOffset) { mIsValid = false; return mIsValid; }

	// We are using RWByteAddressBuffer in hlsl. 
	// We interpret it as a raw typeless 4-byte (R32) buffer with 'size/4' elements
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = inBufferSize / 4;
	uavDesc.Buffer.StructureByteStride = 0;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;
	mContext->mDevice->CreateUnorderedAccessView(inBuffer, nullptr, &uavDesc,
		{ mContext->mCPUHandle.ptr + (SIZE_T)mReservedDescriptorSlotBase * mContext->mHandleSize });
	if (!mIsUAVBaseSet)
	{
		mContext->mCommandList->SetComputeRootDescriptorTable(kUAVSlotIndex,
			{ mContext->mGPUHandle.ptr + (UINT64)mReservedDescriptorSlotBase * mContext->mHandleSize });
		mIsUAVBaseSet = true;
	}
	mReservedDescriptorSlotBase++;

	// UAV barrier to prevent synchronization issues
	D3D12_RESOURCE_BARRIER uavBarrier;
	uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	uavBarrier.UAV.pResource = inBuffer;
	mContext->mCommandList->ResourceBarrier(1, &uavBarrier);

	return true;
}

bool DXShaderExecution::SetShaderResourceView(
	ID3D12Resource* inBuffer,
	UINT inBufferSize)
{
	if (!mIsValid || !inBuffer || !inBufferSize) { mIsValid = false; return mIsValid; }
	if (mReservedDescriptorSlotBase + 1 > mContext->mHeapOffset) { mIsValid = false; return mIsValid; }

	// We are using ByteAddressBuffer in hlsl. 
	// We interpret it as a raw typeless 4-byte (R32) buffer with 'size/4' elements
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = inBufferSize / 4;
	srvDesc.Buffer.StructureByteStride = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
	mContext->mDevice->CreateShaderResourceView(inBuffer, &srvDesc,
		{ mContext->mCPUHandle.ptr + (SIZE_T)mReservedDescriptorSlotBase * mContext->mHandleSize });
	if (!mIsSRVBaseSet)
	{
		mContext->mCommandList->SetComputeRootDescriptorTable(kSRVSlotIndex,
			{ mContext->mGPUHandle.ptr + (UINT64)mReservedDescriptorSlotBase * mContext->mHandleSize });
		mIsSRVBaseSet = true;
	}
	mReservedDescriptorSlotBase++;
	return true;
}

bool GetShaderPath(
	const wchar_t* inModuleName,
	std::wstring& outCSOPath,
	std::wstring& outSigPath)
{
	// Get the path of the effect binary
	wchar_t path[1024];
	HMODULE hm = NULL;
	if (!::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCSTR)&GetShaderPath, &hm))
	{
		return false;
	}
	if (::GetModuleFileNameW(hm, path, sizeof(path)) == 0)
	{
		return false;
	}
	std::wstring directory(path);
	const size_t last_slash_idx = directory.rfind('\\');
	if (std::string::npos == last_slash_idx)
	{
		return false;
	}

	directory = directory.substr(0, last_slash_idx) + L"\\DirectX_Assets\\";
	outCSOPath = directory + inModuleName + L".cso";
	outSigPath = directory + inModuleName + L".rs";
	return true;
}
