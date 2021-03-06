#pragma once

#include "RCore.h"
#include "RSurface.h"

RR_EXTERN_C_BEG

RR_DEFINE_HANDLE(RrSwapchainKHR);
RR_DEFINE_HANDLE(RrSurfaceKHR);
RR_DEFINE_HANDLE(RrLogicalDevice);
RR_DEFINE_HANDLE(RrImage);
RR_DEFINE_HANDLE(RrSemaphore);
RR_DEFINE_HANDLE(RrFence);
RR_DEFINE_HANDLE(RrQueue);

typedef enum RrSwapchainCreateFlagBitsKHR
{
	RrSwapchainCreateSplitInstanceBindRegionsKHR = 0x00000001,
	RrSwapchainCreateProtectedKHR = 0x00000002,
	RrSwapchainCreateMutableFormatKHR = 0x00000004
} RrSwapchainCreateFlagBitsKHR;
typedef uint32_t RrSwapchainCreateFlagsKHR;

typedef struct RrPresentInfoKHR
{
	uint32_t waitSemaphoreCount;
	const RrSemaphore* pWaitSemaphores;
	uint32_t swapchainCount;
	const RrSwapchainKHR* pSwapchains;
	const uint32_t* pImageIndices;
	RrError* pResults;
} RrPresentInfoKHR;

typedef struct RrSwapchainCreateInfoKHR
{
	RrSwapchainCreateFlagsKHR flags;
	RrSurfaceKHR surface;
	uint32_t minImageCount;
	RrFormat imageFormat;
	RrColorSpaceKHR imageColorSpace;
	RrExtent2D imageExtent;
	uint32_t imageArrayLayers;
	RrImageUsageFlags imageUsage;
	RrSharingMode imageSharingMode;
	uint32_t queueFamilyIndexCount;
	const uint32_t* pQueueFamilyIndices;
	RrSurfaceTransformFlagsKHR preTransform;
	RrCompositeAlphaFlagsKHR compositeAlpha;
	RrPresentModeKHR presentMode;
	RrBool32 clipped;
	RrSwapchainKHR oldSwapchain;
} RrSwapchainCreateInfoKHR;


RR_API RrError RrCreateSwapchainKHR(RrLogicalDevice device,
	const RrSwapchainCreateInfoKHR* pCreateInfo, RrSwapchainKHR* pSwapchain);

RR_API RrError RrGetSwapchainImagesKHR(RrLogicalDevice device, RrSwapchainKHR swapchain,
	uint32_t* pSwapchainImageCount, RrImage* pSwapchainImages);

RR_API void RrDestroySwapchainKHR(RrLogicalDevice device, RrSwapchainKHR swapchain);

RR_API RrError RrAcquireNextImageKHR(RrLogicalDevice device, RrSwapchainKHR swapchain,
	uint64_t timeout, RrSemaphore semaphore, RrFence fence, uint32_t* pImageIndex);

RR_API RrError RrQueuePresentKHR(RrQueue queue, const RrPresentInfoKHR* pPresentInfo);

RR_EXTERN_C_END
