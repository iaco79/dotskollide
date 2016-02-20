#include <irrlicht.h>
#include <IFileSystem.h>
#include "SDL.h"
#include "SDLFileInterface.h" 

SDLFileInterface* SDLFileInterface::gInstance= NULL;
bool SDLFileInterface::gInstanceFlag = false;


SDLFileInterface* SDLFileInterface::getInstance()
{
   if(gInstanceFlag)
    {
		return gInstance;
    }
    else
    {
        return NULL;
    }
}


void SDLFileInterface::Initialize(irr::io::IFileSystem* fileSystem) 
{
	if(!gInstanceFlag)
	{
		gInstance = new SDLFileInterface(fileSystem);
		gInstanceFlag=true;
	}
}

SDLFileInterface::SDLFileInterface(irr::io::IFileSystem* fileSystem) : FileSystem(fileSystem)
{
	
    //ctor
}

SDLFileInterface::~SDLFileInterface()
{
    //dtor
}

int SDLFileInterface::Open(SDL_RWops* rwops, irr::core::stringc path)
{
    int result =0;
	irr::io::IReadFile* rf = FileSystem->createAndOpenFile(path.c_str());	

	rwops->hidden.irrio.fs  = (uintptr_t)rf;

	if(!rf)
		result = -1;

	return result;
	
}

void SDLFileInterface::Close(SDL_RWops* rwops)
{
    
    if (!rwops)
        return;
	irr::io::IReadFile* rfile = (irr::io::IReadFile*)rwops->hidden.irrio.fs;
    rfile->drop();
}

size_t  SDLFileInterface::GetSize(SDL_RWops* rwops)
{
    if (!rwops->hidden.irrio.fs)
        return 0;
    irr::io::IReadFile* rfile = (irr::io::IReadFile*)rwops->hidden.irrio.fs;
	return rfile->getSize();



}

size_t SDLFileInterface::Read(void* buffer, size_t size, SDL_RWops* rwops)
{
    //printf("File read\n");
    if (!rwops->hidden.irrio.fs)
        return 0;
    irr::io::IReadFile* rfile = (irr::io::IReadFile*)rwops->hidden.irrio.fs;
    return rfile->read(buffer, size);

}

bool SDLFileInterface::Seek(SDL_RWops* rwops, long offset, int origin)
{
    //printf("File seek: %i : %i\n", offset, origin);
    if (!rwops->hidden.irrio.fs)
        return false;
    irr::io::IReadFile* rfile = (irr::io::IReadFile*)rwops->hidden.irrio.fs;
    if (origin == SEEK_SET)
        return rfile->seek(offset, false);
    else if (origin == SEEK_CUR)
        return rfile->seek(offset, true);
    //printf("Seek end--------------------------------------------\n");
    rfile->seek(rfile->getSize(), false);
    return rfile->seek(offset, true);
}

size_t SDLFileInterface::Tell(SDL_RWops* rwops)
{
    //printf("File tell\n");
    if (!rwops->hidden.irrio.fs)
        return 0;
    irr::io::IReadFile* rfile = (irr::io::IReadFile*)rwops->hidden.irrio.fs;
    return rfile->getPos();
}
