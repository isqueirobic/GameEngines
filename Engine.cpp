#include "Engine.h"

Game::Game(int largura, int altura) {
	Screenaltura = altura;
	Screenlargura = largura;
	tamanhoreal = largura * altura*3;
	Screen = new unsigned char[tamanhoreal];
    
    glfwInit();
    window = glfwCreateWindow(Screenlargura, Screenaltura, "GAME", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    clGetPlatformIDs(1, &platform, NULL);
    // pegar cpu ou gpu
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    // contexto OpenCL
    context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    // queue
    queue = clCreateCommandQueue(context, device, 0, NULL);
    // carregar o buffer de memoria daa array
    ScreenGpuMem = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(unsigned char) * tamanhoreal, NULL, NULL);



    // compilar e tacar na gpu
    ProgramClearList = clCreateProgramWithSource(context, 1, &SourceClearList, NULL, NULL);
    clBuildProgram(ProgramClearList, 1, &device, NULL, NULL, NULL);
    KernelClearList = clCreateKernel(ProgramClearList, "CleanArray", NULL); // NOME PRECISA SER O MESMO DA FUNÇÃO

    ProgramDrawRect = clCreateProgramWithSource(context, 1, &SourceDrawRectusGPU, NULL, NULL);
    clBuildProgram(ProgramDrawRect, 1, &device, NULL, NULL, NULL);
    KernelDrawRect = clCreateKernel(ProgramDrawRect, "DrawRectGPU", NULL);

    ProgramDrawRectWithMesh = clCreateProgramWithSource(context, 1, &SourceDrawRectWithMesh, NULL, NULL);
    clBuildProgram(ProgramDrawRectWithMesh, 1, &device, NULL, NULL, NULL);
    KernelDrawRectWithMesh = clCreateKernel(ProgramDrawRectWithMesh, "DrawRectWithMeshGPU", NULL);
    
    


}

void Game::ClearListProgram() {
    size_t globalWorkSize = tamanhoreal;
    int ch = 255;
    clSetKernelArg(KernelClearList, 0, sizeof(cl_mem), &ScreenGpuMem); // seta os argumentos o segundo numero é o index do argumento
    clSetKernelArg(KernelClearList, 1, sizeof(int), &ch);
    clEnqueueNDRangeKernel(queue, KernelClearList, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    clFinish(queue);

    // Read the modified data back to the host
    clEnqueueReadBuffer(queue, ScreenGpuMem, CL_TRUE, 0, sizeof(unsigned char) * tamanhoreal, Screen, 0, NULL, NULL);

}
//__global unsigned char* Screen, int x2, int y2, int largura, int altura, int largurapssada, int red, int green, int blue, int ScreenLargura, int ScreenAltura
void Game::draw(float x, float y, float largura, float altura, int red, int green, int blue) {
    size_t globalWorkSize = tamanhoreal;
    clSetKernelArg(KernelDrawRect, 0, sizeof(cl_mem), &ScreenGpuMem);
    clSetKernelArg(KernelDrawRect, 1, sizeof(float), &x);
    clSetKernelArg(KernelDrawRect, 2, sizeof(float), &y);
    clSetKernelArg(KernelDrawRect, 3, sizeof(float), &largura);
    clSetKernelArg(KernelDrawRect, 4, sizeof(float), &altura);
    clSetKernelArg(KernelDrawRect, 5, sizeof(int), &Screenlargura);
    clSetKernelArg(KernelDrawRect, 6, sizeof(int), &red);
    clSetKernelArg(KernelDrawRect, 7, sizeof(int), &green);
    clSetKernelArg(KernelDrawRect, 8, sizeof(int), &blue);
    clSetKernelArg(KernelDrawRect, 9, sizeof(int), &Screenaltura);
    clEnqueueNDRangeKernel(queue, KernelDrawRect, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    clFinish(queue);
    clEnqueueReadBuffer(queue, ScreenGpuMem, CL_TRUE, 0, sizeof(unsigned char) * tamanhoreal, Screen, 0, NULL, NULL);
}
// DrawRectWithMeshGPU(__global unsigned char* Screen, float x, float y, float largura, float altura, int largurapssada, __global char* ch, float tx, float ty, int ScreenAltura)
void Game::DrawRectWithMesh(float x22, float y22, float largura2, float altura2, cl_mem ch2, float tx, float ty, int Red, int Green, int Blue) {
    size_t globalWorkSize = tamanhoreal;
    
    clSetKernelArg(KernelDrawRectWithMesh, 0, sizeof(cl_mem), &ScreenGpuMem);
    clSetKernelArg(KernelDrawRectWithMesh, 1, sizeof(float), &x22);
    clSetKernelArg(KernelDrawRectWithMesh, 2, sizeof(float), &y22);
    clSetKernelArg(KernelDrawRectWithMesh, 3, sizeof(float), &largura2);
    clSetKernelArg(KernelDrawRectWithMesh, 4, sizeof(float), &altura2);
    clSetKernelArg(KernelDrawRectWithMesh, 5, sizeof(int), &Screenlargura);
    clSetKernelArg(KernelDrawRectWithMesh, 6, sizeof(cl_mem), &ch2);
    clSetKernelArg(KernelDrawRectWithMesh, 7, sizeof(float), &tx);
    clSetKernelArg(KernelDrawRectWithMesh, 8, sizeof(float), &ty);
    clSetKernelArg(KernelDrawRectWithMesh, 9, sizeof(int), &Screenaltura);
    clSetKernelArg(KernelDrawRectWithMesh, 10, sizeof(int), &Red);
    clSetKernelArg(KernelDrawRectWithMesh, 11, sizeof(int), &Green);
    clSetKernelArg(KernelDrawRectWithMesh, 12, sizeof(int), &Blue);
    clEnqueueNDRangeKernel(queue, KernelDrawRectWithMesh, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    clFinish(queue);
    clEnqueueReadBuffer(queue, ScreenGpuMem, CL_TRUE, 0, sizeof(unsigned char) * tamanhoreal, Screen, 0, NULL, NULL);
}

int Game::GetScreenPixel(int x, int y)
{
    return int(Screen[(y * Screenlargura + x)*3]);
}

void Game::ClearBuffer() {
    glRasterPos2i(-1, 1);
    glPixelZoom(1, -1);
    closewindow = glfwWindowShouldClose(window);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(Screenlargura, Screenaltura, GL_RGB, GL_UNSIGNED_BYTE, Screen);
    glfwSwapBuffers(window);
    glfwPollEvents();
    delete[] Screen;
    Screen = nullptr;
    Screen = new unsigned char[tamanhoreal];
    clEnqueueWriteBuffer(queue, ScreenGpuMem, CL_TRUE, 0, sizeof(unsigned char) * tamanhoreal, Screen, 0, NULL, NULL);

}

void Game::CalculateDeltaTime(const std::chrono::high_resolution_clock::time_point& previousTime) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    deltaTime = currentTime - previousTime;
    deltaTimeSeconds = deltaTime.count();
}


