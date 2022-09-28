#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "VzenseVol_API.h"
#include <thread>
 
using namespace std;
using namespace cv;

VzDeviceHandle g_DeviceHandle = 0;

VzVolDeviceInfo g_DeviceInfo = { 0 };
Point g_Pos(320, 240);
int g_Slope = 1500;
void ShowMenu();
static void Opencv_Depth(uint32_t slope, int height, int width, uint8_t*pData, cv::Mat& dispImg);

void on_MouseHandle(int event, int x, int y, int flags, void * param)
{
	if (EVENT_RBUTTONDOWN == event)
	{
		g_Pos.x = x;
		g_Pos.y = y;
	}
}
void HotPlugStateCallback(const VzVolDeviceInfo* pInfo, int state, void* pUserData)
{
	cout << pInfo->uri << " " << (state == 0 ? "add" : "remove") << endl;
}
int main(int argc, char *argv[])
{
	uint32_t deviceCount = 0;

	VzVolReturnStatus status = VzVol_Initialize();
	if (status != VzVolReturnStatus::VzVolRetOK)
	{
		cout << "VzVol_Initialize failed! " << status << endl;
		system("pause");
		return -1;
	}
	VzVol_RegDeviceHotplugStateCallbackFunc(HotPlugStateCallback, NULL);

	status = VzVol_OpenDevice(&g_DeviceHandle,&g_DeviceInfo);
	if (status != VzVolReturnStatus::VzVolRetOK)
	{
		cout << "VzVol_OpenDevice failed! " << status << endl;
		system("pause");
		return -1;
	}

	ShowMenu();

	cv::Mat imageMat;
	const string depthImageWindow = "Depth Image";
	cv::namedWindow(depthImageWindow, cv::WINDOW_AUTOSIZE);
	setMouseCallback(depthImageWindow, on_MouseHandle, nullptr);

	bool domeasure = false;
	for (;;)
	{
		VzVolInfo volinfo = { 0 };
		status = VzVol_GetVolInfo(g_DeviceHandle, &volinfo, domeasure);
		//cout << "VzVol_GetVolInfo  " << status << endl;

		if (status == VzVolReturnStatus::VzVolRetOK&&volinfo.frame.pFrameData != NULL)
		{
			static int index = 0;
			static float fps = 0;
			static int64 start = cv::getTickCount();

			int64 current = cv::getTickCount();
			int64 diff = current - start;
			index++;
			if (diff > cv::getTickFrequency())
			{
				fps = index * cv::getTickFrequency() / diff;
				index = 0;
				start = current;
			}

			//Display the Depth Image
			Opencv_Depth(g_Slope, volinfo.frame.height, volinfo.frame.width, volinfo.frame.pFrameData, imageMat);
			char text[30] = "";
			sprintf(text, "%.2f", fps);
			putText(imageMat, text, Point(0, 15), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));

			line(imageMat, Point(volinfo.result.Point0.x, volinfo.result.Point0.y), Point(volinfo.result.Point1.x, volinfo.result.Point1.y), Scalar(0, 255, 255), 2);
			line(imageMat, Point(volinfo.result.Point1.x, volinfo.result.Point1.y), Point(volinfo.result.Point2.x, volinfo.result.Point2.y), Scalar(0, 255, 255), 2);
			line(imageMat, Point(volinfo.result.Point2.x, volinfo.result.Point2.y), Point(volinfo.result.Point3.x, volinfo.result.Point3.y), Scalar(0, 255, 255), 2);
			line(imageMat, Point(volinfo.result.Point3.x, volinfo.result.Point3.y), Point(volinfo.result.Point0.x, volinfo.result.Point0.y), Scalar(0, 255, 255), 2);
			char textrst[100];
			Scalar color;
			if (volinfo.result.type == VzVolObjectType::CUBE)
			{
				color = Scalar(255, 255, 255);
			}
			else
			{
				color = Scalar(0, 0, 0);
			}
		 
			sprintf(textrst, "L:  %d mm", volinfo.result.length);
			putText(imageMat, textrst, Point(10, 40), FONT_HERSHEY_DUPLEX, 0.7, color);
			sprintf(textrst, "W:  %d mm", volinfo.result.width);
			putText(imageMat, textrst, Point(10, 60), FONT_HERSHEY_DUPLEX, 0.7, color);
			sprintf(textrst, "H:  %d mm", volinfo.result.height);
			putText(imageMat, textrst, Point(10, 80), FONT_HERSHEY_DUPLEX, 0.7, color);

			cv::imshow(depthImageWindow, imageMat);
		 
		}
		else if(status == VzVolReturnStatus::VzVolRetCalcVolumeError)
		{
			static int index = 0;
			static float fps = 0;
			static int64 start = cv::getTickCount();

			int64 current = cv::getTickCount();
			int64 diff = current - start;
			index++;
			if (diff > cv::getTickFrequency())
			{
				fps = index * cv::getTickFrequency() / diff;
				index = 0;
				start = current;
			}
			Opencv_Depth(g_Slope, volinfo.frame.height, volinfo.frame.width, volinfo.frame.pFrameData, imageMat);
			char text[30] = "";
			sprintf(text, "%.2f", fps);
			putText(imageMat, text, Point(0, 15), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));
			cv::imshow(depthImageWindow, imageMat);
		}
		unsigned char key = waitKey(1);
		if (key == 'B' || key == 'b')
		{
			std::cout << "SetBackGround begin!!" << endl;

			status = VzVol_SetBackGround(g_DeviceHandle);
			if (status != VzVolReturnStatus::VzVolRetOK)
			{
				cout << "VzVol_SetBackGround failed! " << status << endl;
			}
			std::cout << "SetBackGround end!!" << endl;
		}
		else if (key == 'D' || key == 'd')
		{
			domeasure = !domeasure;
			cout << "set Measure "<< domeasure << endl;
 		}
		else if (key == 27)	//ESC Pressed
		{
			break;
		}
	}

	status = VzVol_CloseDevice(&g_DeviceHandle);
	cout << "CloseDevice status: " << status << endl;

	status = VzVol_Shutdown();
	cout << "Shutdown status: " << status << endl;
	cv::destroyAllWindows();

	return 0;
}

void ShowMenu()
{
	cout << "\n--------------------------------------------------------------------" << endl;
	cout << "--------------------------------------------------------------------" << endl;
	cout << "Press following key to set corresponding feature:" << endl;
	cout << "B/b: SetBackGround" << endl;
	cout << "Esc: Program quit " << endl;
	cout << "--------------------------------------------------------------------" << endl;
	cout << "--------------------------------------------------------------------\n" << endl;
}

static void Opencv_Depth(uint32_t slope, int height, int width, uint8_t*pData, cv::Mat& dispImg)
{
	cv::Mat depthMat = cv::Mat(height, width, CV_16UC1, pData);
	Point2d pointxy = g_Pos;
	int val = depthMat.at<ushort>(pointxy);
	char text[20];
#ifdef _WIN32
	sprintf_s(text, "%d", val);
#else
	snprintf(text, sizeof(text), "%d", val);
#endif
	depthMat.convertTo(dispImg, CV_8U, 255.0 / slope);
	applyColorMap(dispImg, dispImg, cv::COLORMAP_RAINBOW);
	int color;
	if (val > 2500)
		color = 0;
	else
		color = 4096;
	circle(dispImg, pointxy, 4, Scalar(color, color, color), -1, 8, 0);
	putText(dispImg, text, pointxy, FONT_HERSHEY_DUPLEX, 2, Scalar(color, color, color));
}
 