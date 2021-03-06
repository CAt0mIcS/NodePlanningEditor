#include "Rpch.h"
#include <RayRenderer/Core/RCore.h>


RrError GetError(int code)
{
	switch (code)
	{
	case VK_SUCCESS: return RrErrorSuccess;
	case VK_NOT_READY: return RrErrorNotReady;
	case VK_TIMEOUT: return RrErrorTimeout;
	case VK_EVENT_SET: return RrErrorEventSet;
	case VK_EVENT_RESET: return RrErrorEventReset;
	case VK_INCOMPLETE: return RrErrorIncomplete;
	case VK_ERROR_OUT_OF_HOST_MEMORY: return RrErrorOutOfHostMemory;
	case VK_ERROR_OUT_OF_DEVICE_MEMORY: return RrErrorOutOfDeviceMemory;
	case VK_ERROR_INITIALIZATION_FAILED: return RrErrorInitializationFailed;
	case VK_ERROR_DEVICE_LOST: return RrErrorDeviceLost;
	case VK_ERROR_MEMORY_MAP_FAILED: return RrErrorMemoryMapFailed;
	case VK_ERROR_LAYER_NOT_PRESENT: return RrErrorLayerNotPresent;
	case VK_ERROR_EXTENSION_NOT_PRESENT: return RrErrorExtensionNotPresent;
	case VK_ERROR_FEATURE_NOT_PRESENT: return RrErrorFeatureNotPresent;
	case VK_ERROR_INCOMPATIBLE_DRIVER: return RrErrorIncompatibleDriver;
	case VK_ERROR_TOO_MANY_OBJECTS: return RrErrorTooManyObjects;
	case VK_ERROR_FORMAT_NOT_SUPPORTED: return RrErrorFormatNotSupported;
	case VK_ERROR_FRAGMENTED_POOL: return RrErrorFragmentedPool;
	case VK_ERROR_UNKNOWN: return RrErrorUnknown;
	case VK_ERROR_OUT_OF_POOL_MEMORY: return RrErrorOutOfPoolMemory;
	case VK_ERROR_INVALID_EXTERNAL_HANDLE: return RrErrorInvalidExternalHandle;
	case VK_ERROR_FRAGMENTATION: return RrErrorFragmentation;
	case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return RrErrorInvalidOpaqueCaptureAddress;
	case VK_ERROR_SURFACE_LOST_KHR: return RrErrorSurfaceLostKHR;
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return RrErrorNativeWindowInUseKHR;
	case VK_SUBOPTIMAL_KHR: return RrErrorSuboptimalKHR;
	case VK_ERROR_OUT_OF_DATE_KHR: return RrErrorOutOfDateKHR;
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return RrErrorIncompatibleDriver;
	case VK_ERROR_VALIDATION_FAILED_EXT: return RrErrorValidationFailedEXT;
	case VK_ERROR_INVALID_SHADER_NV: return RrErrorInvalidShaderNV;
	case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
		return RrErrorInvalidDRMFormatModifierPlaneLayoutEXT;
	case VK_ERROR_NOT_PERMITTED_EXT: return RrErrorNotPermittedEXT;
	case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return RrErrorFullScreenExclusiveModeLostEXT;
	case VK_THREAD_IDLE_KHR: return RrErrorThreadIdleKHR;
	case VK_THREAD_DONE_KHR: return RrErrorThreadDoneKHR;
	case VK_OPERATION_DEFERRED_KHR: return RrErrorOperationDeferredKHR;
	case VK_OPERATION_NOT_DEFERRED_KHR: return RrErrorOperationNotDeferredKHR;
	case VK_PIPELINE_COMPILE_REQUIRED_EXT: return RrErrorPipelineCompileRequiredEXT;
	}

	return RrErrorUnknown;
}
