#ifndef VZENSEDSVol_API_H
#define VZENSEDSVol_API_H
#ifdef PS_EXPORT_ON
#ifdef _WIN32
#define VZENSE_API_EXPORT __declspec(dllexport)
#else
#define VZENSE_API_EXPORT __attribute__((visibility("default")))
#endif
#else
#ifdef _WIN32
#define VZENSE_API_EXPORT __declspec(dllimport)
#else
#define VZENSE_API_EXPORT __attribute__((visibility("default")))
#endif
#endif

#ifdef __cplusplus
#define VZENSE_C_API_EXPORT extern "C" VZENSE_API_EXPORT
#else
#define VZENSE_C_API_EXPORT VZENSE_API_EXPORT
#define bool uint8_t
#endif
#include <stdint.h>

typedef enum 
{
	INVAILD = 0,
	CUBE = 1,
	IRREGULAR = 2
}VzVolObjectType;
/**
 * @brief Specifies the type of image frame.
 */
typedef enum {
	VzVolDepthFrame = 0,                           //!< Depth frame with 16 bits per pixel in millimeters.
	VzVolIRFrame = 1,                              //!< IR frame with 8 bits per pixel.
	VzVolColorFrame = 3,                           //!< Color frame with 24 bits per pixel in RGB/BGR format.
	VzVolTransformColorImgToDepthSensorFrame = 4,  //!< Color frame with 24 bits per pixel in RGB/BGR format, that is transformed to depth sensor space where the resolution is the same as the depth frame's resolution.\n 
												//!< This frame type can be enabled using ::VZ_SetTransformColorImgToDepthSensorEnabled().
	VzVolTransformDepthImgToColorSensorFrame = 5,  //!< Depth frame with 16 bits per pixel, in millimeters, that is transformed to color sensor space where the resolution is same as the color frame's resolution.\n 
																							//!< This frame type can be enabled using ::VZ_SetTransformDepthImgToColorSensorEnabled().
	VzVolConfidenceFrame = 8,                      //!< Confidence frame with 16 bits per pixel.
}VzVolFrameType;

/**
 * @brief Specifies the image pixel format.
 */
typedef enum {
	VzVolPixelFormatDepthMM16 = 0,        //!< Depth image pixel format, 16 bits per pixel in mm.
	VzVolPixelFormatGray8 = 2,            //!< Gray image pixel format, 8 bits per pixel.

	//Color
	VzVolPixelFormatRGB888 = 3,           //!< Color image pixel format, 24 bits per pixel RGB format.
	VzVolPixelFormatBGR888 = 4           //!< Color image pixel format, 24 bits per pixel BGR format.
}VzVolPixelFormat;

/**
 * @brief Return status codes for all APIs.\n
 * 		  <code>VzRetOK = 0</code> means the API successfully completed its operation.\n
 * 		  All other codes indicate a device, parameter, or API usage error.
 */
typedef enum
{
	VzVolRetOK = 0,   //!< The function completed successfully.
	VzVolRetNoDeviceConnected = -1,   //!< There is no depth camera connected or the camera has not been connected correctly. Check the hardware connection or try unplugging and re-plugging the USB cable.
	VzVolRetInvalidDeviceIndex = -2,   //!< The input device index is invalid.
	VzVolRetDevicePointerIsNull = -3,   //!< The device structure pointer is null.
	VzVolRetInvalidFrameType = -4,   //!< The input frame type is invalid.
	VzVolRetFramePointerIsNull = -5,   //!< The output frame buffer is null.
	VzVolRetNoPropertyValueGet = -6,   //!< Cannot get the value for the specified property.
	VzVolRetNoPropertyValueSet = -7,   //!< Cannot set the value for the specified property.
	VzVolRetPropertyPointerIsNull = -8,   //!< The input property value buffer pointer is null.
	VzVolRetPropertySizeNotEnough = -9,   //!< The input property value buffer size is too small to store the specified property value.
	VzVolRetInvalidDepthRange = -10,  //!< The input depth range mode is invalid.
	VzVolRetGetFrameReadyTimeOut = -11,  //!< Capture the next image frame time out.
	VzVolRetInputPointerIsNull = -12,  //!< An input pointer parameter is null.
	VzVolRetCameraNotOpened = -13,  //!< The camera has not been opened.
	vzVolRetInvalidCameraType = -14,  //!< The specified type of camera is invalid.
	VzVolRetInvalidParams = -15,  //!< One or more of the parameter values provided are invalid.
	VzVolRetCurrentVersionNotSupport = -16,  //!< This feature is not supported in the current version.
	VzVolRetUpgradeImgError = -17,  //!< There is an error in the upgrade file.
	VzVolRetUpgradeImgPathTooLong = -18,  //!< Upgrade file path length greater than 260.
	VzVolRetUpgradeCallbackNotSet = -19,  //!< VZ_SetUpgradeStatusCallback is not called.
	VzVolRetProductNotSupport = -20,  //!< The current product does not support this operation.
	VzVolRetNoProductProfile = -21,  //!< No product profile found.
	VzVolRetNoAdapterConnected = -100,	//!< There is no adapter connected
	VzVolRetReInitialized = -101,	//!< The SDK has been Initialized
	VzVolRetNoInitialized = -102,	//!< The SDK has not been Initialized
	VzVolRetCameraOpened = -103,	//!< The camera has been opened.
	VzVolRetCmdError = -104,	//!< Set/Get cmd control error
	VzVolRetCmdSyncTimeOut = -105,	//!< Set cmd ok.but time out for the sync return 
	VzVolRetIPNotMatch = -106, //!< IP is not in the same network segment
	VzVolRetNotStopStream = -107, //!< Please invoke VZ_StopStream first to close the data stream
	VzVolRetCalcVolumeError = -108,	             //!< An unknown error occurred.
	VzVolRetOthers = -255,	             //!< An unknown error occurred.
}VzVolReturnStatus;

typedef enum {
	VzVolConnectUNKNOWN = 0,
	VzVolUnconnected = 1,
	VzVolConnected = 2,
	VzVolOpened = 3,
	VzVolUpgradeUnconnected = 4,
	VzVolUpgradeConnected = 5,
}VzVolConnectStatus;

#pragma pack (push, 1)
typedef void* VzDeviceHandle;
typedef struct
{
	int	x;
	int	y;
}VzVolMarkPoint;
typedef struct
{
	int	    length;  //the length of the object: unit is mm
	int	    width;   //the width of the object: unit is mm
	int	    height;  //the height of the object: unit is mm
	VzVolObjectType	type;  //the object type: 0 - cube; 1 - irregular
	VzVolMarkPoint   Point0;
	VzVolMarkPoint   Point1;
	VzVolMarkPoint   Point2;
	VzVolMarkPoint   Point3;
}VzVolResult;
typedef struct
{
	uint32_t       frameIndex;          //!< The index of the frame.
	VzVolFrameType    frameType;           //!< The type of frame. See ::PsFrameType for more information.
	VzVolPixelFormat  pixelFormat;         //!< The pixel format used by a frame. See ::PsPixelFormat for more information.
	uint8_t*       pFrameData;          //!< A buffer containing the frame¡¯s image data.
	uint32_t       dataLen;             //!< The length of pFrame, in bytes.
	float          exposureTime;        //!< The exposure time, in milliseconds.
	uint8_t        depthRange;          //!< The depth range mode of the current frame. Used only for depth frames.
	uint16_t       width;               //!< The width of the frame, in pixels.
	uint16_t       height;              //!< The height of the frame, in pixels.
	uint64_t       deviceTimestamp;     //!< The timestamp of the frame from the device.
}VzVolFrame;
typedef struct
{
	VzVolFrame frame;
	VzVolResult result;
}VzVolInfo;
typedef struct
{
	char productName[64];
	char uri[256];
	char alias[64];
	char serialNumber[64];
	char ip[17];
	VzVolConnectStatus status;
}VzVolDeviceInfo;

#pragma pack (pop)
typedef void(*PtrDeviceHotplugStateCallback)(const VzVolDeviceInfo* pInfo, int state, void* pUserData);

VZENSE_C_API_EXPORT VzVolReturnStatus VzVol_Initialize();
VZENSE_C_API_EXPORT VzVolReturnStatus VzVol_Shutdown();
VZENSE_C_API_EXPORT VzVolReturnStatus VzVol_OpenDevice(VzDeviceHandle* pDeviceHandler, VzVolDeviceInfo* pDeviceInfo);
VZENSE_C_API_EXPORT VzVolReturnStatus VzVol_CloseDevice(VzDeviceHandle* pDeviceHandler);
VZENSE_C_API_EXPORT VzVolReturnStatus VzVol_SetBackGround(VzDeviceHandle device);
VZENSE_C_API_EXPORT VzVolReturnStatus VzVol_GetVolInfo(VzDeviceHandle device, VzVolInfo* volinfo, bool doalgo = false);
VZENSE_C_API_EXPORT VzVolReturnStatus VzVol_RegDeviceHotplugStateCallbackFunc(const PtrDeviceHotplugStateCallback pCallback, const void* pUserData);

#endif /* VZENSEDSVIL_API_H */
