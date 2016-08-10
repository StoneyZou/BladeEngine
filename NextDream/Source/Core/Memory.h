#ifndef __BLADE_CORE_MEMORY_H__
#define __BLADE_CORE_MEMORY_H__

#include <malloc.h>

#include <TypeDefine.h>
#include <Utility.h>

namespace BladeEngine
{
    struct IMalloc : public INoncopyable
    {
        virtual void* Alloc(SIZE_T inSize) = 0;
        virtual void* Realloc(void* inBlock, SIZE_T inSize) = 0;
        virtual void Free(void* inBlock) = 0;
#ifdef DEBUG
        virtual void* Alloc(SIZE_T inSize, TCHAR* inFile, uint32 inLine) = 0;
        virtual void* Realloc(void* inBlock, SIZE_T inSize, TCHAR* inFile, uint32 inLine) = 0;
        virtual void Free(void* inBlock) = 0;
        virtual void Dump() = 0;
#endif // DEBUG
    };

    class Malloc
    {
    private:
        static IMalloc* s_RegisterMalloc;
        
    private:
        static void* _Alloc(SIZE_T inSize) { return malloc(inSize); }
        static void* _Realloc(void* inBlock, SIZE_T inSize) { return realloc(inBlock, inSize); }
        static void _Free(void* inBlock) { free(inBlock); }
#ifdef DEBUG
        static void* _Alloc(SIZE_T inSize, TCHAR* inFile, uint32 inLine) { return malloc(inSize); }
        static void* _Realloc(void* inBlock, SIZE_T inSize, TCHAR* inFile, uint32 inLine) { return realloc(inBlock, inSize); }
        static void _Free(void* inBlock) { free(inBlock); }
        static void _Dump() {}
#endif // DEBUG

    public:
        static void* Alloc(SIZE_T inSize) {
            return s_RegisterMalloc != NULL ? s_RegisterMalloc->Alloc(inSize) : _Alloc(inSize);
        }
        static void* Realloc(void* inBlock, SIZE_T inSize) {
            return s_RegisterMalloc != NULL ? s_RegisterMalloc->Realloc(inBlock, inSize) : _Realloc(inBlock, inSize);
        }
        static void Free(void* inBlock) {
            return s_RegisterMalloc != NULL ? s_RegisterMalloc->Free(inBlock) : _Free(inBlock);
        }
#ifdef DEBUG
        static void* Alloc(SIZE_T inSize, TCHAR* inFile, uint32 inLine) {
            return s_RegisterMalloc != NULL ? s_RegisterMalloc->Alloc(inSize, inFile, inLine) : _Alloc(inSize, inFile, inLine);
        }
        static void* Realloc(void* inBlock, SIZE_T inSize, TCHAR* inFile, uint32 inLine) {
            return s_RegisterMalloc != NULL ? s_RegisterMalloc->Realloc(inBlock, inSize, inFile, inLine) : Realloc(inBlock, inSize, inFile, inLine);
    }
        static void Free(void* inBlock) {
            return s_RegisterMalloc != NULL ? s_RegisterMalloc->Free(inBlock) : Free(inBlock);
        }
        static void Dump() {
            return s_RegisterMalloc != NULL ? s_RegisterMalloc->Dump() : _Dump(inSize);
        }
#endif // DEBUG

    public:
        static void Register(IMalloc* inMalloc) { s_RegisterMalloc = inMalloc; }
        static void Unregister() { s_RegisterMalloc = NULL; }
    };
}

#endif // !__BLADE_CORE_MEMORY_H__
