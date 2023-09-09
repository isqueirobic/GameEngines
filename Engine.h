#ifndef LaEngine
#define LaEngine
#pragma comment(lib, "winmm.lib")
#include "CL/cl.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <thread>
#include <vector>
#define PI 3.14159265359
class Game {
	const char* SourceDrawRectusGPU = "__kernel void DrawRectGPU(__global unsigned char* Screen, float x2, float y2, float largura, float altura, int largurapssada, int red, int green, int blue, int ScreenAltura) {"
		"int x1 = get_global_id(0);"
		"int x = x1 / largurapssada;"
		"x += x2;"
		"int y = x1 % largurapssada;"
		"y += y2;"
		"int index = (y * largurapssada) + x;"
		"index *= 3;"
		"if (x > 0 && x < largurapssada && y > 0 && y < ScreenAltura) {"
		"if (x >= x2 && x <= x2 + largura && y >= y2 && y <= y2 + altura) {"

		"Screen[index] = red;"
		"Screen[index + 1] = green;"
		"Screen[index + 2] = blue;"
		"}"
		"}"
		"}";

	const char* SourceDrawRectWithMesh = "__kernel void DrawRectWithMeshGPU(__global unsigned char* Screen, float x, float y, float largura, float altura, int largurapssada,__global char* ch, float tx, float ty, int ScreenAltura, int Red, int Grenn, int Blue) {"
		"int X1 = get_global_id(0);"
		"Screen[0]= 255;"
		"Screen[1]= 255;"
		"Screen[2]= 255;"
		"int X = X1 / largurapssada;"
		"X += x;"
		"int Y = X1 % largurapssada;"
		"Y += y;"
		"int index = (Y * largurapssada) + X;"
		"index *= 3;"
		"if (X > 0 && X < largurapssada && Y > 0 && Y < ScreenAltura)"
		"{"
		"if (X >= x && X <= (x + largura)-2 && Y >= y && Y <= (y + altura)-2) {"
		"int y1 = (Y - y) / ty;"
		"float x1 = (X - x) / tx;"
		"int index2 = y1 * largura / tx + x1;"

		"if (ch[index2] != ' ') {"
		"Screen[index] = Red;"
		"Screen[index + 1] = Grenn;"
		"Screen[index + 2] = Blue;"
		"}"

		"}"
		"}"


		"}";
	
	cl_platform_id platform;
	
	cl_program ProgramDrawRect;
	cl_kernel KernelDrawRect;
	cl_program ProgramClearList;
	cl_kernel KernelClearList;
	cl_program ProgramDrawRectWithMesh;
	cl_kernel KernelDrawRectWithMesh;
	cl_mem ScreenGpuMem;
	
	char* screencuda = 0;
	std::chrono::duration<double> deltaTime;
	
	
	
	
public:
	float deltaTimeSeconds = 1;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
	int Screenaltura;
	int Screenlargura;
	GLFWwindow* window;
	const char* SourceClearList = "__kernel void CleanArray(__global unsigned char* Array, int ch){"
        "int index = get_global_id(0);"
        "Array[index] = ch;"
		"}";
	int tamanhoreal;
	unsigned char* Screen;
	bool closewindow = false;
	Game(int ScreenLargura1 , int ScreenAltura1);
	void draw(float x, float y, float largura, float altura, int red=255, int green=255, int blue=255);
	void ClearBuffer();

	void ClearListProgram();
	void CalculateDeltaTime(const std::chrono::high_resolution_clock::time_point& previousTime);
	void DrawRectWithMesh(float x22, float y22, float largura2, float altura2, cl_mem ch2, float tx, float ty, int Red=255, int Green=255, int Blue=255);
	int GetScreenPixel(int x, int y);

};

#endif // !LaEngine
