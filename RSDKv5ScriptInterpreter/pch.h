// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "Game.hpp"
#include "framework.h"

namespace RSDK
{
    inline void StrCopy(char* dest, const char* src)
    {
        strcpy(dest, src);
    }

    inline void StrAdd(char* dest, const char* src)
    {
        int32 destStrPos = 0;
        int32 srcStrPos = 0;
        while (dest[destStrPos]) ++destStrPos;
        while (true) {
            if (!src[srcStrPos]) {
                break;
            }
            dest[destStrPos++] = src[srcStrPos++];
        }
        dest[destStrPos] = 0;
    }

    inline bool StrComp(const char* stringA, const char* stringB)
    {
        bool32 match = true;
        bool32 finished = false;
        while (!finished) {
            if (*stringA == *stringB || *stringA == *stringB + ' ' || *stringA == *stringB - ' ') {
                if (*stringA) {
                    ++stringA;
                    ++stringB;
                }
                else {
                    finished = true;
                }
            }
            else {
                match = false;
                finished = true;
            }
        }

        return match;
    }
    
    inline int32 StrLength(const char* string)
    {
        int32 len = 0;
        for (len = 0; string[len]; len++)
            ;
        return len;
    }

    int32 FindStringToken(const char* string, const char* token, uint8 stopID);

    namespace Legacy
    {
        bool32 ConvertStringToInteger(const char* text, int32* value);
    }
}

#define MEM_ZERO(x) memset(&(x), 0, sizeof((x)))

#endif //PCH_H
