#pragma once

#include "RCore.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrBuffer);
RR_DEFINE_HANDLE(RrLogicalDevice);
RR_DEFINE_HANDLE(RrDeviceMemory);

typedef enum RrBufferUsageFlagBits
{
	RrBufferUsageTransferSrc = 0x00000001,
	RrBufferUsageTransferDst = 0x00000002,
	RrBufferUsageUniformTexelBuffer = 0x00000004,
	RrBufferUsageStorageTexelBuffer = 0x00000008,
	RrBufferUsageUniformBuffer = 0x00000010,
	RrBufferUsageStorageBuffer = 0x00000020,
	RrBufferUsageIndexBuffer = 0x00000040,
	RrBufferUsageVertexBuffer = 0x00000080,
	RrBufferUsageIndirectbuffer = 0x00000100,
	RrBufferUsageShaderDeviceAddress = 0x00020000,
	RrBufferUsageTransformFeedbackBufferEXT = 0x00000800,
	RrBufferUsageTransformFeedbackCounterBufferEXT = 0x00001000,
	RrBufferUsageConditionalRenderingEXT = 0x00000200,
	RrBufferUsageAccelerationStructureBuildInputReadOnlyKHR = 0x00080000,
	RrBufferUsageAccelerationStructureStorageKHR = 0x00100000,
	RrBufferUsageShaderBindingTableKHR = 0x00000400,
	RrBufferUsageRayTracingNV = RrBufferUsageShaderBindingTableKHR,
	RrBufferUsageShaderDeviceAddressEXT = RrBufferUsageShaderDeviceAddress,
	RrBufferUsageShaderDeviceAddressKHR = RrBufferUsageShaderDeviceAddress,
} RrBufferUsageFlagBits;
typedef uint32_t RrBufferUsageFlags;

typedef struct RrBufferCreateInfo
{
	RrDeviceSize size;
	RrBufferUsageFlags usage;
	RrSharingMode sharingMode;
	uint32_t queueFamilyIndexCount;
	const uint32_t* pQueueFamilyIndices;
} RrBufferCreateInfo;

RR_API RrError RrCreateBuffer(
	RrLogicalDevice device, const RrBufferCreateInfo* pCreateInfo, RrBuffer* pBuffer);

RR_API void RrBufferGetMemoryRequirements(
	RrLogicalDevice device, RrBuffer buffer, RrMemoryRequirements* pMemRequirements);

RR_API RrError RrBindBufferMemory(
	RrLogicalDevice device, RrBuffer buffer, RrDeviceMemory memory, RrDeviceSize memoryOffset);

RR_API void RrDestroyBuffer(RrLogicalDevice device, RrBuffer buffer);

RR_EXTERN_C_END
