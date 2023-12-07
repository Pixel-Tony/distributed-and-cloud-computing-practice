#ifndef _IMAGE_H
#define _IMAGE_H

#if _WIN32
    #define STBI_WINDOWS_UTF8
#endif

#define STBI_NO_HDR
// #define STBI_ONLY_PNG
#define STBI_NO_FAILURE_STRINGS

#include <stb_image.h>

#endif /* _IMAGE_H */