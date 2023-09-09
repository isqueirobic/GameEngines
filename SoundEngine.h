#ifndef SOM
#define SOM
#include "Engine.h"
class Sound {
	LPCWSTR Name;
public:
	Sound(LPCWSTR FilePath);
	void Play();
	void ChagePath(LPCWSTR FilePath);
#endif // !SOM


};