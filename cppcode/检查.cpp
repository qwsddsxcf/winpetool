#include "检查.h"
#include <windows.h>
#include <stdio.h>

int checkiswin32pe(char* filedata)
{
    PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)filedata;
    if (dos_header->e_magic != IMAGE_DOS_SIGNATURE) {
        return 0;
    }
    PIMAGE_NT_HEADERS32 nt_headers = (PIMAGE_NT_HEADERS32)(filedata + dos_header->e_lfanew);
    if (nt_headers->Signature != IMAGE_NT_SIGNATURE) {
        return 0;
    }

    if (nt_headers->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
        return 0;
    }

    return 1;
}
