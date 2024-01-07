#pragma once
#include "Base/Base.h"
#include "FilePath.h"

namespace XHuang
{

namespace FileSystem
{
    bool Load_SPV_File(const FilePath& path, Vector<char>& outData);
}

} // namespace XHuang