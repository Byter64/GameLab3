#include "FileSystem.h"

//Wieso?????
#define GET_STRING(x) #x
#define GET_STRING2(x) GET_STRING(x)

namespace Engine::Files
{
    static const std::filesystem::path ASSETS = GET_STRING2(_ASSET_ROOT);
}