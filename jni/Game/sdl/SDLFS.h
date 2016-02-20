
#ifndef IRR_SDL_FILE_H
#define IRR_SDL_FILE_H


#ifdef __cplusplus
extern "C" {
#endif
   int SDLCALL irr_sdl_fs_open(SDL_RWops * context, const char *filename, const char *mode);

   Sint64 irr_sdl_fs_size(struct SDL_RWops * context);

    /**
     *  Seek to \c offset relative to \c whence, one of stdio's whence values:
     *  RW_SEEK_SET, RW_SEEK_CUR, RW_SEEK_END
     *  
     *  \return the final offset in the data stream.
     */ 
    Sint64 irr_sdl_fs_seek (struct SDL_RWops * context, Sint64 offset,
                             int whence);

    /**
     *  Read up to \c maxnum objects each of size \c size from the data
     *  stream to the area pointed at by \c ptr.
     *  
     *  \return the number of objects read, or 0 at error or end of file.
     */
    size_t irr_sdl_fs_read(struct SDL_RWops * context, void *ptr,
                             size_t size, size_t maxnum);

    /**
     *  Write exactly \c num objects each of size \c size from the area
     *  pointed at by \c ptr to data stream.
     *  
     *  \return the number of objects written, or 0 at error or end of file.
     */
    size_t irr_sdl_fs_write  (struct SDL_RWops * context, const void *ptr,
                              size_t size, size_t num);

    /**
     *  Close and free an allocated SDL_RWops structure.
     *  
     *  \return 0 if successful or -1 on write error when flushing data.
     */
    int irr_sdl_fs_close (struct SDL_RWops * context);

#ifdef __cplusplus
}
#endif

#endif // IRRROCKETFILEINTERFACE_H

