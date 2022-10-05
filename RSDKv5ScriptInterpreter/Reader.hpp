#ifndef READER_H
#define READER_H

#if RETRO_RENDERDEVICE_SDL2 || RETRO_AUDIODEVICE_SDL2 || RETRO_INPUTDEVICE_SDL2
#define FileIO                                          SDL_RWops
#define fOpen(path, mode)                               SDL_RWFromFile(path, mode)
#define fRead(buffer, elementSize, elementCount, file)  SDL_RWread(file, buffer, elementSize, elementCount)
#define fSeek(file, offset, whence)                     SDL_RWseek(file, offset, whence)
#define fTell(file)                                     SDL_RWtell(file)
#define fClose(file)                                    SDL_RWclose(file)
#define fWrite(buffer, elementSize, elementCount, file) SDL_RWwrite(file, buffer, elementSize, elementCount)
#else
#define FileIO                                          FILE
#define fOpen(path, mode)                               fopen(path, mode)
#define fRead(buffer, elementSize, elementCount, file)  fread(buffer, elementSize, elementCount, file)
#define fSeek(file, offset, whence)                     fseek(file, offset, whence)
#define fTell(file)                                     ftell(file)
#define fClose(file)                                    fclose(file)
#define fWrite(buffer, elementSize, elementCount, file) fwrite(buffer, elementSize, elementCount, file)
#endif

namespace RSDK
{

struct FileInfo {
    int32 fileSize;
    FileIO *file;
    int32 readPos;
    int32 fileOffset;
};

enum FileModes { FMODE_NONE, FMODE_RB, FMODE_WB, FMODE_RB_PLUS };

static const char *openModes[3] = { "rb", "wb", "rb+" };

inline bool32 CheckBigEndian()
{
    uint32 x = 1;
    uint8 *c = (uint8 *)&x;
    return ((int32)*c) == 0;
}

inline void InitFileInfo(FileInfo *info)
{
    info->file            = NULL;
    info->fileSize        = 0;
    info->readPos         = 0;
    info->fileOffset      = 0;
}

bool32 LoadFile(FileInfo *info, const char *filename, uint8 fileMode);

inline void CloseFile(FileInfo *info)
{
    if (info->file)
        fClose(info->file);

    info->file = NULL;
}

inline void Seek_Set(FileInfo *info, int32 count)
{
    if (info->readPos != count) {
        info->readPos = count;
        fSeek(info->file, info->fileOffset + info->readPos, SEEK_SET);
    }
}

inline void Seek_Cur(FileInfo* info, int32 count)
{
    info->readPos += count;

    fSeek(info->file, count, SEEK_CUR);
}

inline size_t ReadBytes(FileInfo* info, void* data, int32 count)
{
    size_t bytesRead = 0;

    bytesRead = fRead(data, 1, count, info->file);

    info->readPos += bytesRead;
    return bytesRead;
}

inline uint8 ReadInt8(FileInfo *info)
{
    int8 result      = 0;
    size_t bytesRead = 0;

    bytesRead = fRead(&result, 1, sizeof(int8), info->file);

    info->readPos += bytesRead;
    return result;
}

inline int16 ReadInt16(FileInfo *info)
{
    union {
        uint16 result;
        uint8 b[sizeof(result)];
    } buffer;
    memset(&buffer, 0, sizeof(buffer));

    size_t bytesRead = 0;

    bytesRead = fRead(buffer.b, 1, sizeof(int16), info->file);

    // if we're on a big endian machine, swap the byte order
    // this is done AFTER reading & decrypting since they expect little endian order on all systems
    if (CheckBigEndian()) {
        uint8 bytes[sizeof(buffer)];
        memcpy(bytes, &buffer, sizeof(buffer));

        int32 max = sizeof(buffer) - 1;
        for (int32 i = 0; i < sizeof(buffer) / 2; ++i) {
            uint8 store    = bytes[i];
            bytes[i]       = bytes[max - i];
            bytes[max - i] = store;
        }
        memcpy(&buffer, bytes, sizeof(buffer));
    }

    info->readPos += bytesRead;
    return buffer.result;
}

inline int32 ReadInt32(FileInfo *info, bool32 swapEndian)
{
    union {
        uint32 result;
        uint8 b[sizeof(result)];
    } buffer;
    memset(&buffer, 0, sizeof(buffer));

    size_t bytesRead = 0;

    bytesRead = fRead(buffer.b, 1, sizeof(int32), info->file);

    if (swapEndian) {
        uint8 bytes[sizeof(buffer)];
        memcpy(bytes, &buffer, sizeof(buffer));

        int32 max = sizeof(buffer) - 1;
        for (int32 i = 0; i < sizeof(buffer) / 2; ++i) {
            uint8 store    = bytes[i];
            bytes[i]       = bytes[max - i];
            bytes[max - i] = store;
        }
        memcpy(&buffer, bytes, sizeof(buffer));
    }

    // if we're on a big endian machine, swap the byte order
    // this is done AFTER reading & decrypting since they expect little endian order on all systems
    if (CheckBigEndian()) {
        uint8 bytes[sizeof(buffer)];
        memcpy(bytes, &buffer, sizeof(buffer));

        int32 max = sizeof(buffer) - 1;
        for (int32 i = 0; i < sizeof(buffer) / 2; ++i) {
            uint8 store    = bytes[i];
            bytes[i]       = bytes[max - i];
            bytes[max - i] = store;
        }
        memcpy(&buffer, bytes, sizeof(buffer));
    }

    info->readPos += bytesRead;
    return buffer.result;
}
inline int64 ReadInt64(FileInfo *info)
{
    union {
        uint64 result;
        uint8 b[sizeof(result)];
    } buffer;
    memset(&buffer, 0, sizeof(buffer));

    size_t bytesRead = 0;

    bytesRead = fRead(buffer.b, 1, sizeof(int64), info->file);

    // if we're on a big endian machine, swap the byte order
    // this is done AFTER reading & decrypting since they expect little endian order on all systems
    if (CheckBigEndian()) {
        uint8 bytes[sizeof(buffer)];
        memcpy(bytes, &buffer, sizeof(buffer));

        int32 max = sizeof(buffer) - 1;
        for (int32 i = 0; i < sizeof(buffer) / 2; ++i) {
            uint8 store    = bytes[i];
            bytes[i]       = bytes[max - i];
            bytes[max - i] = store;
        }
        memcpy(&buffer, bytes, sizeof(buffer));
    }

    info->readPos += bytesRead;
    return buffer.result;
}

inline float ReadSingle(FileInfo *info)
{
    union {
        float result;
        uint8 b[sizeof(result)];
    } buffer;
    memset(&buffer, 0, sizeof(buffer));

    size_t bytesRead = 0;

    bytesRead = fRead(buffer.b, 1, sizeof(float), info->file);

    // if we're on a big endian machine, swap the byte order
    // this is done AFTER reading & decrypting since they expect little endian order on all systems
    if (CheckBigEndian()) {
        uint8 bytes[sizeof(buffer)];
        memcpy(bytes, &buffer, sizeof(buffer));

        int32 max = sizeof(buffer) - 1;
        for (int32 i = 0; i < sizeof(buffer) / 2; ++i) {
            uint8 store    = bytes[i];
            bytes[i]       = bytes[max - i];
            bytes[max - i] = store;
        }
        memcpy(&buffer, bytes, sizeof(buffer));
    }

    info->readPos += bytesRead;
    return buffer.result;
}

inline void ReadString(FileInfo *info, char *buffer)
{
    uint8 size = ReadInt8(info);
    ReadBytes(info, buffer, size);
    buffer[size] = 0;
}

} // namespace RSDK

#endif
