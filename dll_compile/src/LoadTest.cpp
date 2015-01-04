#include "windows.h"
#include "stdlib.h"
#include "stdio.h"

int main()
{
    printf("+:load testing.");
    __asm__
        (
            ".intel_syntax;"
            "int 0x3;"
         );
    HANDLE lret = LoadLibrary("libMyLibrary.dll");
    if(lret == NULL)
    {
        MessageBox(NULL,"LOADLibrary FAILED","load failed",MB_OK);
    }
    else{printf("load dll into 0x%08x",lret);}

    return 0;
}