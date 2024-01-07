#pragma once
#include <filesystem>
#include "Base/String.h"

namespace XHuang
{

class FilePath
{
public:
    FilePath(const String& str) : mStdPath(str) {}
    String ToString() const { return mStdPath.string(); }
private:
    std::filesystem::path mStdPath;
};

} // namespace XHuang