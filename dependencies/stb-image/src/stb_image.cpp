#if _WIN32
    #define STBI_WINDOWS_UTF8
#endif

#define STBI_NO_HDR
#define STBI_ONLY_PNG
#define STBI_NO_FAILURE_STRINGS
#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>