#include "Shapes.h"

Rect::Rect(float x1, float y1,float Largura1 ,float Altura1 , Game *jogoponteiro) // the joy of creating
{
	jogo = jogoponteiro;
	x = x1;
	y = y1;
	larguraRect = Largura1;
	alturaRect = Altura1;
	Mesh = new char[2];
}



void Rect::update() // you can use this but i recomend use the RectMain class
{
	if (SETCOLISION) {
		Colision();
	}
	if (CanDraw) {
		draw();
	}
	
}

void Rect::draw() // ask gently if our game class could draw this
{
	z = z == 0 ? 0.0001 : z;
	float X, Y;
	float num1 = larguraRect/z;
	float num2 = alturaRect/z;
	X = num1 / ImageSizeX;
	Y = num2 / ImageSizeY;
	jogo->DrawRectWithMesh(x/z, y/z, num1, num2, MemMeshTexture, X, Y, Red, Green, Blue);
}

void Rect::loadImage(const char* ImageFile, int FileLargura, int FIleAltura) // load our txt file in a 1d array bc idk how to read png files
{
	//delete[] Mesh;
	int Temp = tamanhototal;
	Mesh = nullptr; // rest our mesh
	Mesh = new char[FIleAltura * FileLargura]; // set the new size
	alturaRect = FIleAltura; // same here
	larguraRect = FileLargura; // and here
	std::fstream file(ImageFile); // read the txt file
	file.read(Mesh, FIleAltura * FileLargura); // write the txt file on the array
	tamanhototal = FIleAltura * FileLargura;
	ImageSizeX = FileLargura;
	ImageSizeY = FIleAltura;
	if (!MeshSet)
	{
		MeshSet = true;
		MemMeshTexture = clCreateBuffer(jogo->context, CL_TRUE, sizeof(char) * tamanhototal, NULL, NULL); // Create the buffer bc why not
	}
	else if (tamanhototal > Temp){
		clReleaseMemObject(MemMeshTexture);
		MemMeshTexture = clCreateBuffer(jogo->context, CL_TRUE, sizeof(char) * tamanhototal, NULL, NULL); // Creatw again with a bigger size
	}
	//MemMeshTexture = jogo->WriteBuffer(MemMeshTexture, Mesh, tamanhototal);
	clEnqueueWriteBuffer(jogo->queue, MemMeshTexture, CL_TRUE, 0, sizeof(char) * tamanhototal, Mesh, 0, NULL, NULL); // update bc we have to
	
}



void Rect::FreeDraw(float x, float y,float largura ,float altura ) // draw wherever u want with out consuming more memory
{
	float X, Y;
	float num1 = largura / z;
	float num2 = altura / z;
	X = num1 / ImageSizeX;
	Y = num2 / ImageSizeY;
	
	jogo->DrawRectWithMesh(x/z, y/z, num1, num2, MemMeshTexture, X, Y);
}

void Rect::CpuDraw() { // If your gpu is trash use the cpu why not ?
	float y2 = alturaRect / ImageSizeY;
	float x2 = larguraRect / ImageSizeX;
	for (int y = this->y; y < alturaRect + this->x; y++) {
		for (int x = this->x; x < larguraRect + this->x; x++) {
			int index = y * jogo->Screenlargura + x;
			index *= 3;
			int index2 = (int)(y / y2) * (larguraRect/x2) + (x / x2);
			if (Mesh[index2] != ' ') {
				jogo->Screen[index] = 255;
				jogo->Screen[index + 1] = 255;
				jogo->Screen[index + 2] = 255;
			}
		}
	}
}

void Rect::SetColor(int r, int g , int b) { // colors
	Red = r;
	Green = g;
	Blue = b;
}

void Rect::SetColision(Rect &Colsion1) { // set if we hit this mf the colided variable gona be true
	ColisionRect = &Colsion1;
	SETCOLISION = true;
	CanColide = true;
}

void Rect::Colision() { // most simple Colsion 2d detection
	if (x < ColisionRect->x + ColisionRect->larguraRect &&
		x + larguraRect > ColisionRect->x &&
		y < ColisionRect->y + ColisionRect->alturaRect &&
		y + alturaRect > ColisionRect->y) {
		colided = true;
	}
	else {
		colided = false;
	}
}

Rect::~Rect() {
	delete jogo;
	delete ColisionRect;
	clReleaseMemObject(MemMeshTexture);
	delete[] Mesh;
}



void Animation::Animation2() { //logic
	float numX = animPOSx[index] - *StartX; // will see if the speed is + or -
	float numY = animPOSy[index] - *StartY; // same but in the Y axis
	float velocidadeX = (animPOSx[index] / animTEMPO[index]); // set de velocity based on the distance and time we set
	float velocidadeY = (animPOSx[index] / animTEMPO[index]); // same Y axis
	stopTime += jogo->deltaTimeSeconds; // clock to stop
	if (numX < 0) {
		velocidadeX = -velocidadeX * 2;
	}
	if (numY < 0) {
		velocidadeY = -velocidadeY * 2;
	}

	if (*StartY >= animPOSy[index] && *StartY <= animPOSy[index] + 10) { // it reach at the point ? yes Y axis is done
		Ydone = true;
	}
	if (*StartX >= animPOSx[index] && *StartX <= animPOSx[index] + 10) { // same X axis
		Xdone = true;
	}
	if (!Xdone) {
		*StartX += velocidadeX * jogo->deltaTimeSeconds; // velocity of the animation
	}

	if (!Ydone) {
		*StartY += velocidadeY * jogo->deltaTimeSeconds;
	}

	if (Ydone && Xdone || stopTime == animTEMPO[index]) { // if both axis and time are done we increase the position, setting the next position target or if we run out of index we stop the animation
		if (index == tamanho - 1) {
			IsAnimating = false;
			done = true;
		}
		else {
			index++;
			Ydone = false;
			Xdone = false;
		}

	}

}


Animation::Animation(Game& jogoecia2) { // intalizate this shit up
	jogo = &jogoecia2;
}

void Animation::AnimationInfo(float* time, float* Xfinal, float* Yfinal, int size, float& StartX, float& StartY) { // set the position based on a array of floating points the size is the lengh of all arrays
	IsAnimating = false;
	Ydone = false;
	Xdone = false;
	animPOSx = Xfinal;
	animPOSy = Yfinal;
	animTEMPO = time;
	stopTime = 0;
	tamanho = size;
	this->StartX = &StartX;
	this->StartY = &StartY;
}

void Animation::AnimPlay() { // play
	IsAnimating = true; // this what make relly play
	stopTime = 0;
	Ydone = false;
	Xdone = false;
	done = false;
	index = 0;

}

void Animation::AnimStop() { // pause
	IsAnimating = false;
}

void Animation::update() { // since our program is not multhread unfortnaly we gotta usa the update method
	if (IsAnimating) {
		Animation2();
	}
	if (done) {
		resettime += jogo->deltaTimeSeconds;
		if (resettime == 2) {
			done = false;
			resettime = 0;
		}
	}
}

void RectMain::InsertRect(Rect &rect) { // Will insert a rect into the array and give it a id
	NumRects += 1;
	rect.Myid = NumRects;
	RectListUpdate[NumRects] = &rect;
}

void RectMain::RemoveRect(int index) { // Remove The Rect based on the index
	int id = index;
	printf("%d", id);  
	for (int i = id; i < NumRects;i++) { // Reagroup the list for better performance 
		if (RectListUpdate[i] == NULL) {
			break;
		}

		RectListUpdate[i] = RectListUpdate[i+1];
		RectListUpdate[i]->Myid -= 1;

	}

	NumRects -= 1;

	
	
}

void RectMain::DrawRects() { // draw the rects on the array
	for (int i = 0; i < NumRects+1; i++) {
		if (RectListUpdate[i] != NULL)
		{
			if (RectListUpdate[i]->CanDraw) {
				RectListUpdate[i]->draw();
			}
		}
	}
	
}

void RectMain::UpdateColsion() { // update the colsion of the rects in the array
	for (int i = 0; i < NumRects+1; i++) {
		if(RectListUpdate[i] != NULL)
		{
			if (RectListUpdate[i]->CanColide) {
				RectListUpdate[i]->Colision();
			}
		}
	}
}

Rect* RectMain::CreateRect(float x1, float y1, float Largura1, float Altura1, Game* jogo) { // return a rect to the variable and then put this rect on the array and give the rect a id
	NumRects += 1;
	RectListUpdate[NumRects] = new Rect(x1, y1, Largura1, Altura1, jogo);
	RectListUpdate[NumRects]->Myid = NumRects;
	return RectListUpdate[NumRects];
	
}

