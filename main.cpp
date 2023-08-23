#include "DSRTArchiver.h"
#include "Utility.h"
#include <iostream>

#define TARGETPATH L".\\data.dts"
#define DECRYPT_KEY_LENGTH 6
const wchar_t DecryptKey[DECRYPT_KEY_LENGTH]={L'k',L'e',L'y',L's',L'e',L't'};

using namespace std;
int main(int argc,char** argv){
    DSRTArchiver archiver(TARGETPATH);
    wchar_t* filepath=new wchar_t[256];
    wchar_t* entry_name=new wchar_t[256];
    wchar_t* dir_path=new wchar_t[512];
    unsigned char* buffer;
    for(DSRTEntry entry :archiver.GetEntries()){
        buffer=new unsigned char[entry.GetLength()];
        {
            wstring str=entry.GetEntryName();
            entry_name[str.copy(entry_name,str.size())]=L'\0';
            ReplaceAllWchar(entry_name,'/','\\');
        }
        {
            int length=GetCurrentDirectoryW(256,dir_path);
            if(dir_path[length-1]!=L'\\')
                dir_path[length++]=L'\\';
            dir_path[length]=L'\0';
        }
        filepath[Wstrcpy(filepath,dir_path)]=L'\0';
        filepath[Wstrcat(filepath,L"\\output\\")]=L'\0';
        filepath[Wstrcat(filepath,entry_name)]=L'\0';
        MakeDirectory(filepath);
        unsigned int length=archiver.ExtractEntry(entry,buffer,(const unsigned char*)DecryptKey,DECRYPT_KEY_LENGTH);
        filepath[Wstrcat(filepath,DetectExt(buffer,entry_name))]=L'\0';
        WriteToFile(filepath,buffer,length);
        {
            wstring msg=L"Extracting ";
            msg.append(filepath);
            msg.append(L" Success");
            PrintWstring(msg.c_str(),msg.size());
        }
        
        delete[] buffer;
    }
    delete[] filepath;
    delete[] entry_name;
    delete[] dir_path;
    
    system("pause");
    return 0;
}