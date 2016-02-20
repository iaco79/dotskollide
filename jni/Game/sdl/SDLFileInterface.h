
#ifndef SDLFILEINTERFACE_H
#define SDLFILEINTERFACE_H


class SDLFileInterface 
{
	private:
		static SDLFileInterface* gInstance;
		static bool gInstanceFlag;
		
		SDLFileInterface(irr::io::IFileSystem* fileSystem);

    public:
		static void Initialize(irr::io::IFileSystem* fileSystem);

		static SDLFileInterface* getInstance();


        /** Default constructor */
        

        /** Default destructor */
        virtual ~SDLFileInterface();

        // Opens a file.
		int Open(SDL_RWops* rwops, irr::core::stringc path);

		size_t GetSize(SDL_RWops* rwops);

        // Closes a previously opened file.
        void Close(SDL_RWops* rwops);

        // Reads data from a previously opened file.
        size_t Read(void* buffer, size_t size, SDL_RWops* rwops);

        // Seeks to a point in a previously opened file.
        bool Seek(SDL_RWops* rwops, long offset, int origin);

        // Returns the current position of the file pointer.
        size_t Tell(SDL_RWops* rwops);

    protected:
        irr::io::IFileSystem* FileSystem;
    private:
};

#endif // IRRROCKETFILEINTERFACE_H
