#ifndef __BLADE_CORE_MEMORY_H__
#define __BLADE_CORE_MEMORY_H__

#include <malloc.h>

#include <TypeDefine.h>
#include <Utility.h>

namespace BladeEngine
{

	class IMalloc : public INoncopyable
	{
	public:
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

    class SystemMalloc : public IMalloc
    {
    private:
        static SystemMalloc s_Instance;

    public:
        static SystemMalloc& GetInstance() { return s_Instance;  }

    public:
        void* Alloc(SIZE_T inSize) { return malloc(inSize); }
        void* Realloc(void* inBlock, SIZE_T inSize) { return realloc(inBlock, inSize); }
        void Free(void* inBlock) { free(inBlock); }
#ifdef DEBUG
        void* Alloc(SIZE_T inSize, TCHAR* inFile, uint32 inLine) { return malloc(inSize); }
        void* Realloc(void* inBlock, SIZE_T inSize, TCHAR* inFile, uint32 inLine) { return realloc(inBlock, inSize); }
        void Free(void* inBlock) { free(inBlock); }
        void Dump() {}
#endif // DEBUG
    };
}

#endif // !__BLADE_CORE_MEMORY_H__
