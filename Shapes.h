#ifndef FORMATOS
#define FORMATOS
#include "Engine.h"




class Rect {
private:
    int ImageSizeX, ImageSizeY, chanels, Red = 255, Green = 255, Blue = 255;

    bool SETCOLISION = false;
    Game* jogo;
    Rect* ColisionRect;
    cl_mem MemMeshTexture;
public:
    int Myid;
    std::string Name = "Rect";
    bool CanColide = false;
    bool CanDraw = true;
    bool colided = false;
	float size=0,x, y,z=1, larguraRect, alturaRect, tamanhototal=2;
    bool MeshSet = false;
	char* Mesh;
	Rect(float x1, float y1, float Largura1,float Altura1, Game *jogoponteiro);

	void update();

	void draw();

	void loadImage(const char* ImageFile, int FileLargura, int FIleAltura);

	void FreeDraw(float x, float y, float largura ,float altura);

	void CpuDraw();

	void SetColor(int Red, int Grenn, int Blue);

    void Colision();

    void SetColision(Rect &ColisionRect1);

    ~Rect();

};
const int AmountRects = 100;
class RectMain {
public:
    int NumRects = -1;
    const int TotalNumRects = AmountRects;
    Rect* RectListUpdate[AmountRects];
    Rect* CreateRect(float x1, float y1, float Largura1, float Altura1, Game* jogo);
    void DeleteRect(int Rect_Myid);
    void DrawRects();
    void UpdateColsion();
    void InsertRect(Rect& rect);
    void RemoveRect(int Rect_Myid);
    void Syncrects();
};

class Animation {

    bool Ydone = false;
    bool Xdone = false;
    float* StartX;
    float* StartY;
    float* animPOSx;
    float* animPOSy;
    float* animTEMPO;
    float stopTime = 0;
    float resettime = 0;
    int index = 0;
    int tamanho = 1;
    Game* jogo;

    void Animation2();
public:
    bool IsAnimating = false;
    bool done = false;
    Animation(Game& jogoecia);

    void AnimationInfo(float* time, float* Xfinal, float* Yfinal, int size, float& StartX, float& StartY);

    void AnimPlay();

    void AnimStop();

    void update();
};

#endif // FORMATOS

