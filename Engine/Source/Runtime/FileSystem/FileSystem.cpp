#include "FileSystem.h"
#include <fstream>

namespace XHuang
{

namespace FileSystem
{
    bool Load_SPV_File(const FilePath& path, Vector<char>& outData)
    {
        std::ifstream spvFile(path.ToString(), std::ios::ate | std::ios::binary);
        if (spvFile.is_open()) {
            UInt32 fileSize = spvFile.tellg();
            outData.resize(fileSize);
            spvFile.seekg(0);
            spvFile.read(outData.data(), fileSize);
            spvFile.close();
            return true;
        } else {
            // error log
            return false;
        }
    }
}

} // namepace XHuang