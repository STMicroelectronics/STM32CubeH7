


#include <stdlib.h>
/*
void *operator new(size_t size) throw() {
    return malloc(size);
}

void operator delete(void *p) throw() {
    free(p);
}
*/
/*
extern "C" int __aeabi_atexit(void *object,
                              void (*destructor)(void *),
                              void *dso_handle)
{
    object = object; destructor=destructor; dso_handle=dso_handle;

    return 0;
}
*/
extern "C"
{
    /*
    void *malloc(size_t s) {
        while(1);
        return (void *)0;
    }
    void free(void *) {
        while(1);
    }
    void *calloc(size_t a, size_t b) {
        return (void *)0;
    }
    */
    void* __dso_handle = (void*)NULL;
    void __cxa_atexit(void (*Arg)(void*), void* Arg2, void* Arg3) {}
    void  __cxa_guard_acquire(void) {}
    void  __cxa_guard_release(void) {}

    void __cxa_pure_virtual()
    {
        while (1);
    }
    int _read(int fd, void* buf, int count)
    {
        return 0;
    }

    int _lseek(int fildes, int offset, int whence)
    {
        return 0;
    }

    int _isatty(int fildes)
    {
        return 0;
    }

    int _fstat(int fildes, void* buf)
    {
        return 0;
    }

    int _close(int fildes)
    {
        return 0;
    }

    int _write(int fd, const void* buf, int count)
    {
        return 0;
    }

    int _kill(int a, int b)
    {
        while (1);
        return 0;
    }

    int _getpid(void)
    {
        return 0;
    }
    int _sbrk(int a)
    {
        //align a to 4 bytes
        if (a & 3)
        {
            a += (4 - (a & 0x3));
        }

        extern long __heap_start__;
        extern long __heap_end__;
        static char* heap_ptr = (char*)&__heap_start__;

        if (heap_ptr + a < (char*)&__heap_end__)
        {
            int res = (int)heap_ptr;
            heap_ptr += a;
            return res;
        }
        else
        {
            return -1;
        }
    }
    void _exit(int a)
    {
        while (1);
    }
    void __assert_func(const char* file, int line, const char* func, const char* failedexpr)
    {
        while (1) {}
    }
}
