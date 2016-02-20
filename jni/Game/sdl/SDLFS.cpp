
#include <IFileSystem.h>
#include "SDL.h"
#include "SDLFileInterface.h"
#include "SDLFS.h"


  int SDLCALL irr_sdl_fs_open(SDL_RWops * context, const char *filename, const char *mode)
  {
	  return SDLFileInterface::getInstance()->Open(context, irr::core::stringc(filename));
	 
  }

	Sint64 irr_sdl_fs_size(struct SDL_RWops * context)
	{
		return SDLFileInterface::getInstance()->GetSize(context);
		
	}

    /**
     *  Seek to \c offset relative to \c whence, one of stdio's whence values:
     *  RW_SEEK_SET, RW_SEEK_CUR, RW_SEEK_END
     *  
     *  \return the final offset in the data stream.
     */
    Sint64 irr_sdl_fs_seek (struct SDL_RWops * context, Sint64 offset,
                             int whence)
	{

		if (SDLFileInterface::getInstance()->Seek(context,offset, whence))
		{
			return SDLFileInterface::getInstance()->Tell(context);
		}

		return -1;
   
   }

    /**
     *  Read up to \c maxnum objects each of size \c size from the data
     *  stream to the area pointed at by \c ptr.
     *  
     *  \return the number of objects read, or 0 at error or end of file.
     */
    size_t irr_sdl_fs_read(struct SDL_RWops * context, void *ptr,
                             size_t size, size_t maxnum)
	{
		size_t totalbytes =  (maxnum * size);

		size_t read = SDLFileInterface::getInstance()->Read(ptr, totalbytes, context);

		if(size>0)
			return totalbytes/size;
		return 0;
		
     }

    /**
     *  Write exactly \c num objects each of size \c size from the area
     *  pointed at by \c ptr to data stream.
     *  
     *  \return the number of objects written, or 0 at error or end of file.
     */
    size_t irr_sdl_fs_write  (struct SDL_RWops * context, const void *ptr,
                              size_t size, size_t num)
	{
		return 0;
   
   }

    /**
     *  Close and free an allocated SDL_RWops structure.
     *  
     *  \return 0 if successful or -1 on write error when flushing data.
     */
    int irr_sdl_fs_close (struct SDL_RWops * context)
	{
		SDLFileInterface::getInstance()->Close(context);

		return 0;
	}
   
   

