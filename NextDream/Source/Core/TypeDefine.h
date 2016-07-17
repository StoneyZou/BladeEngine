#ifndef __BLADE_CORE_TYPE_DEFINE_H__
#define __BLADE_CORE_TYPE_DEFINE_H__

#ifndef NULL
    #define NULL 0
#endif

template<class T1, class T2, bool Flag>
class _TypeSelector
{
public:
	typedef T2 Type;
};

template<class T1, class T2>
class _TypeSelector<T1, T2, true>
{
public:
	typedef T1 Type;
};

template<class T1, class T2, int Size>
class _TypeSelectorBySize : public _TypeSelector<T1, T2, sizeof(T1) == Size>
{
};

struct _GenericPlatformTypes
{
    // Unsigned base types.
    typedef unsigned char       uint8;      // 8-bit  unsigned.
    typedef unsigned short int  uint16;     // 16-bit unsigned.
    typedef unsigned int        uint32;     // 32-bit unsigned.
    typedef unsigned long long  uint64;     // 64-bit unsigned.

    // Signed base types.
    typedef	signed char         int8;       // 8-bit  signed.
    typedef signed short int    int16;      // 16-bit signed.
    typedef signed int          int32;      // 32-bit signed.
    typedef signed long long    int64;      // 64-bit signed.

    typedef unsigned char       byte;

                                            // Character types.
    typedef char                ANSICHAR;   // An ANSI character       -                  8-bit fixed-width representation of 7-bit characters.
    typedef wchar_t             WIDECHAR;   // A wide character        - In-memory only.  ?-bit fixed-width representation of the platform's natural wide character set.  Could be different sizes on different platforms.
    typedef uint8               CHAR8;      // An 8-bit character type - In-memory only.  8-bit representation.  Should really be char8_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
    typedef uint16              CHAR16;     // A 16-bit character type - In-memory only.  16-bit representation.  Should really be char16_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
    typedef uint32              CHAR32;     // A 32-bit character type - In-memory only.  32-bit representation.  Should really be char32_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
    typedef WIDECHAR            TCHAR;      // A switchable character  - In-memory only.  Either ANSICHAR or WIDECHAR, depending on a licensee's requirements.

    typedef _TypeSelectorBySize<uint32, uint64, sizeof(void*)>::Type UPTRINT;           // unsigned int the same size as a pointer
    typedef _TypeSelectorBySize<int32, int64, sizeof(void*)>::Type PTRINT;              // signed int the same size as a pointer
    typedef UPTRINT SIZE_T;                                                             // unsigned int the same size as a pointer
    typedef PTRINT SSIZE_T;                                                             // unsigned int the same size as a pointer

    typedef int32                   TYPE_OF_NULL;
    typedef decltype(nullptr)       TYPE_OF_NULLPTR;
};

typedef _GenericPlatformTypes::uint8		uint8;
typedef _GenericPlatformTypes::uint16		uint16;
typedef _GenericPlatformTypes::uint32		uint32;
typedef _GenericPlatformTypes::uint64		uint64;

typedef _GenericPlatformTypes::int8			int8;
typedef _GenericPlatformTypes::int16		int16;
typedef _GenericPlatformTypes::int32        int32;
typedef _GenericPlatformTypes::int64        int64;

typedef _GenericPlatformTypes::byte         byte;

typedef _GenericPlatformTypes::ANSICHAR     ANSICHAR;
typedef _GenericPlatformTypes::WIDECHAR     WIDECHAR;
typedef _GenericPlatformTypes::CHAR8        CHAR8;
typedef _GenericPlatformTypes::CHAR16       CHAR16;
typedef _GenericPlatformTypes::CHAR32       CHAR32;
typedef _GenericPlatformTypes::TCHAR        TCHAR;

typedef _GenericPlatformTypes::UPTRINT      UPTRINT;
typedef _GenericPlatformTypes::PTRINT       PTRINT;

typedef _GenericPlatformTypes::SIZE_T       SIZE_T;
typedef _GenericPlatformTypes::SSIZE_T      SSIZE_T;

#if _MSC_VER >= 1900
	#define _static_check_type_size(Type, Size)	\
		class _sizeof_type_##Type##_is_not_##Size##_bit	\
		{	\
			char check1[sizeof(##Type##) - (##Size## >> 3) + 1];	\
			char check2[(##Size## >> 3) - sizeof(##Type##) + 1];	\
		};
#else
	#define _static_check_type_size(Type, Size)	\
		class _sizeof_type_##Type##_is_not_##Size##_bit	\
		{	\
			char check1[sizeof(##Type##) - (##Size## >> 3)];	\
			char check2[(##Size## >> 3) - sizeof(##Type##)];	\
		};
#endif

_static_check_type_size(int32, 32);
_static_check_type_size(uint32, 32);
_static_check_type_size(int64, 64);
_static_check_type_size(uint64, 64);


#endif // !__BLADE_CORE_TYPE_DEFINE_H__
