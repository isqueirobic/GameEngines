#include "SoundEngine.h"

Sound::Sound(LPCWSTR FilePath) {
	Name = FilePath;
}

void Sound::Play()
{
	
	PlaySound(Name, NULL, SND_ASYNC);
}

void Sound::ChagePath(LPCWSTR FilePath)
{
	Name = FilePath;
}
