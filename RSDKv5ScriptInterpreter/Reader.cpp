#include "pch.h"
#include "Reader.hpp"
#include <string>
#define PrintLog RSDK::Dev::Print
using namespace RSDK::Dev;

using namespace RSDK;

#if !RETRO_USE_ORIGINAL_CODE && RETRO_REV0U
inline bool ends_with(std::string const &value, std::string const &ending)
{
    if (ending.size() > value.size())
        return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}
#endif

bool32 RSDK::LoadFile(FileInfo *info, const char *filename, uint8 fileMode)
{
    if (info->file)
        return false;

    char fullFilePath[0x100];
    strcpy(fullFilePath, filename);

    int modCount = Mod::List::GetModCount(true);
    for (int32 m = 0; m < modCount; ++m) {
        String modPath{};
        Mod::List::GetModPath(Mod::List::GetModIDByIndex(m), &modPath);
        modPath.CStr(fullFilePath);
        strcat_s(fullFilePath, "/");
        strcat_s(fullFilePath, filename);
        if (fileMode == FMODE_RB || fileMode == FMODE_WB || fileMode == FMODE_RB_PLUS) {
            info->file = fOpen(fullFilePath, openModes[fileMode - 1]);
        }
        if (info->file)
            break;
    }

    if (!info->file) {
#if !RETRO_USE_ORIGINAL_CODE
        PrintLog(PRINT_NORMAL, "File not found: %s", fullFilePath);
#endif
        return false;
    }

    info->readPos  = 0;
    info->fileSize = 0;

    if (fileMode != FMODE_WB) {
        fSeek(info->file, 0, SEEK_END);
        info->fileSize = (int32)fTell(info->file);
        fSeek(info->file, 0, SEEK_SET);
    }
#if !RETRO_USE_ORIGINAL_CODE
    PrintLog(PRINT_NORMAL, "Loaded file %s", fullFilePath);
#endif
    return true;
}
