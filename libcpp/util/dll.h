#ifndef DLL_H
#define DLL_H

// export/import dll
#if defined(_MSC_VER) //  Microsoft
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) //  GCC
#define DLL_EXPORT __attribute__((visibility("default")))
#define DLL_IMPORT
#else // Warnning
#define DLL_EXPORT
#define DLL_IMPORT
#pragma warning Unknown dynamic link import/export semantics.
#endif

// // export/import c style dll
// #ifdef _cplusplus
// #define C_STYLE_EXPORT extern "C" DLL_EXPORT
// #define C_STYLE_IMPORT DLL_IMPORT
// #else
// #define C_STYLE_EXPORT DLL_EXPORT
// #define C_STYLE_IMPORT DLL_IMPORT
// #endif

#define C_STYLE_EXPORT extern "C" DLL_EXPORT


#if defined(WIN32)
#include <windows.h>
#define DLL_RTLD_LAZY         0
#define DLL_RTLD_NOW          0
#define DLL_RTLD_BINDING_MASK 0
#define DLL_RTLD_NOLOAD       0
#define DLL_RTLD_DEEPBIND     0
#define DLL_RTLD_GLOBAL       0
#define DLL_RTLD_LOCAL        0
#define DLL_RTLD_NODELETE     0

#else
#include <dlfcn.h>
#define DLL_RTLD_LAZY         RTLD_LAZY
#define DLL_RTLD_NOW          RTLD_NOW
#define DLL_RTLD_BINDING_MASK RTLD_BINDING_MASK
#define DLL_RTLD_NOLOAD       RTLD_NOLOAD
#define DLL_RTLD_DEEPBIND     RTLD_DEEPBIND
#define DLL_RTLD_GLOBAL       RTLD_GLOBAL
#define DLL_RTLD_LOCAL        RTLD_LOCAL
#define DLL_RTLD_NODELETE     RTLD_NODELETE
#endif


static void* dll_open(const char* filename, int flag)
{
#if defined(WIN32)
    return LoadLibrary(filename);
#else
    return dlopen(filename, flag);
#endif
}

static void* dll_get(void* handler, const char* symbol)
{
#if defined(WIN32)
    return GetProcAddress(handler, symbol);
#else
    return dlsym(handler, symbol);
#endif
}

static int dll_close(void* handler)
{
#if defined(WIN32)
    return FreeLibrary(handler);
#else
    return dlclose(handler);
#endif
}

#endif