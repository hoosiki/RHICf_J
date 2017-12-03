#include "FileManager.hh"
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <sys/types.h>
#include <unistd.h>
#include <typeinfo>

using namespace std;


bool FileManager::FileManagerFlag = false;

FileManager* FileManager::fFileManager = NULL;

FileManager::FileManager() 
{
    time_t RawTime; // rawtime
    time(&RawTime);
    struct tm* TimeInfo; // insert time information into tm struct: sec, min, hour, mday, mon, year, wday, yday, isdst
    TimeInfo = localtime(&RawTime);


    string tmpStr1;

    tmpStr1 = to_string((TimeInfo->tm_year)+1900); // extract year information
    Year = tmpStr1;


    tmpStr1 = "";
    if ((TimeInfo->tm_mon)<9) // extract month information
    {
        tmpStr1 = "0" + to_string((TimeInfo->tm_mon)+1);
    }else 
    {
        tmpStr1 = to_string((TimeInfo->tm_mon+1));
    }
    Date = tmpStr1;




    tmpStr1 = ""; // extract day information
    if ((TimeInfo->tm_mday)<9) 
    {
        tmpStr1 = "0"+to_string((TimeInfo->tm_mday)+1);
    }else 
    {
        tmpStr1 = to_string((TimeInfo->tm_mday+1));
    }
    Date += tmpStr1; 




    tmpStr1 = ""; // extract hour information
    if ((TimeInfo->tm_hour)<9) 
    {
        tmpStr1 = "0"+to_string((TimeInfo->tm_hour)+1);
    }else 
    {
        tmpStr1 = to_string((TimeInfo->tm_hour+1));
    }
    Time = tmpStr1; 



    tmpStr1 = ""; // extract min information
    if ((TimeInfo->tm_min)<9) 
    {
        tmpStr1 = "0"+to_string((TimeInfo->tm_min)+1);
    }else 
    {
        tmpStr1 = to_string((TimeInfo->tm_min+1));
    }
    Time += tmpStr1; 



    PID = getpid(); // extract pid information


    tmpStr1 = ""; // set saving directory path
    tmpStr1 = string(getenv("DATA_DIR"));
    SavingPath = tmpStr1 + "/" + Year + Date;
    
    tmpStr1 = ""; // set saving directory for temporary files
    PathFortmp = SavingPath + "/" + "tmp"; 

    tmpStr1 = ""; // set saving directory for simulation data
    PathForSIMDATA = SavingPath + "SIMDATA"; 

    tmpStr1 = ""; // set saving directory for anaylsis result files
    PathForANALYSIS = SavingPath + "ANALYSIS"; 

    tmpStr1 = ""; // set saving directory for figures
    PathForFIGURES = SavingPath + "FIGURES"; 

    tmpStr1 = ""; // set default filename 
    FileName = "data" + Date + Time + GetPID();

}

FileManager::~FileManager()
{
    FileManagerFlag = false;
}

void FileManager::SetFileName(std::string filename)
{
    FileName = filename + GetTime() + GetPID();
}

FileManager* FileManager::GetInstance()
{
    if(!fFileManager)
    {
        fFileManager = new FileManager();
        FileManagerFlag = true;
    }
    return fFileManager;


}
void FileManager::PrepareCustomDirectory(const string& ss) // method for making directory
{
    string Command1 = "test -d $DATA_DIR/" + Year + Date + "/" + ss + " || mkdir -p " + "$DATA_DIR/" + Year + Date + "/" + ss;  
    system(Command1.c_str());
}

void FileManager::PrepareSavingDirectory() // method for making directory
{
    string Command2 = "test -d " + PathFortmp + " || mkdir -p " + PathFortmp; // make directory for temporary files
    system(Command2.c_str());
    
    string Command3 = "test -d " + PathForSIMDATA + " || mkdir -p " + PathForSIMDATA; // make directory for simulation data files
    system(Command3.c_str());

    string Command4 = "test -d " + PathForANALYSIS + " || mkdir -p " + PathForANALYSIS; // make directory for analysis files
    system(Command4.c_str());

    string Command5 = "test -d " + PathForFIGURES + " || mkdir -p " + PathForFIGURES; // make directory for figures files
    system(Command5.c_str());
}

