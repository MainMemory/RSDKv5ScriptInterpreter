#include "pch.h"

using namespace RSDK;

ManiaGlobalVariables* maniaGlobals = nullptr;

int32 RSDK::FindStringToken(const char* string, const char* token, uint8 stopID)
{
    int32 tokenCharID = 0;
    bool32 tokenMatch = true;
    int32 stringCharID = 0;
    int32 foundTokenID = 0;

    while (string[stringCharID]) {
        tokenCharID = 0;
        tokenMatch = true;
        while (token[tokenCharID]) {
            if (!string[tokenCharID + stringCharID])
                return -1;

            if (string[tokenCharID + stringCharID] != token[tokenCharID])
                tokenMatch = false;

            ++tokenCharID;
        }
        if (tokenMatch && ++foundTokenID == stopID)
            return stringCharID;

        ++stringCharID;
    }
    return -1;
}

bool32 RSDK::Legacy::ConvertStringToInteger(const char* text, int32* value)
{
    int32 charID = 0;
    bool32 negative = false;
    int32 base = 10;
    *value = 0;

    if (*text != '+' && !(*text >= '0' && *text <= '9') && *text != '-')
        return false;

    int32 strLength = StrLength(text) - 1;
    uint32 charVal = 0;

    if (*text == '-') {
        negative = true;
        charID = 1;
        --strLength;
    }
    else if (*text == '+') {
        charID = 1;
        --strLength;
    }

    if (text[charID] == '0') {
        if (text[charID + 1] == 'x' || text[charID + 1] == 'X')
            base = 0x10;
#if !RETRO_USE_ORIGINAL_CODE
        else if (text[charID + 1] == 'b' || text[charID + 1] == 'B')
            base = 0b10;
        else if (text[charID + 1] == 'o' || text[charID + 1] == 'O')
            base = 0010; // base 8
#endif

        if (base != 10) {
            charID += 2;
            strLength -= 2;
        }
    }

    while (strLength > -1) {
        bool32 flag = text[charID] < '0';
        if (!flag) {
            if (base == 0x10 && text[charID] > 'f')
                flag = true;
#if !RETRO_USE_ORIGINAL_CODE
            if (base == 0010 && text[charID] > '7')
                flag = true;
            if (base == 0b10 && text[charID] > '1')
                flag = true;
#endif
        }

        if (flag) {
            return 0;
        }
        if (strLength <= 0) {
            if (text[charID] >= '0' && text[charID] <= '9') {
                *value = text[charID] + *value - '0';
            }
            else if (text[charID] >= 'a' && text[charID] <= 'f') {
                charVal = text[charID] - 'a';
                charVal += 10;
                *value += charVal;
            }
            else if (text[charID] >= 'A' && text[charID] <= 'F') {
                charVal = text[charID] - 'A';
                charVal += 10;
                *value += charVal;
            }
        }
        else {
            int32 strlen = strLength + 1;
            charVal = 0;
            if (text[charID] >= '0' && text[charID] <= '9') {
                charVal = text[charID] - '0';
            }
            else if (text[charID] >= 'a' && text[charID] <= 'f') {
                charVal = text[charID] - 'a';
                charVal += 10;
            }
            else if (text[charID] >= 'A' && text[charID] <= 'F') {
                charVal = text[charID] - 'A';
                charVal += 10;
            }
            for (; --strlen; charVal *= base)
                ;
            *value += charVal;
        }

        --strLength;
        ++charID;
    }

    if (negative)
        *value = -*value;

    return true;
}

void InitModAPI(void)
{
    
}

#define ADD_PUBLIC_FUNC(func) Mod.AddPublicFunction(#func, (void *)(func))

extern "C"
{
	__declspec(dllexport) void UnloadMod()
	{
	}

	__declspec(dllexport) bool32 LinkModLogic(EngineInfo* info, const char* id)
	{
#if !RETRO_REV01
		LinkGameLogicDLL(info);
#else
		LinkGameLogicDLL(*info);
#endif

		maniaGlobals = (ManiaGlobalVariables*)Mod::GetGlobals();

		InitModAPI();

		return true;
	}
}