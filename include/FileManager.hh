#ifndef FileManager_h
#define FileManager_h 1


#include <string>
#include <cstdlib>
#include <iostream>



class FileManager {
    
    private:

        FileManager();
        std::string FileName;
        std::string SavingPath;
        std::string PathFortmp;
        std::string PathForSIMDATA;
        std::string PathForANALYSIS;
        std::string PathForFIGURES;
        std::string Year;
        std::string Date;
        std::string Time;
        unsigned int PID;
        static FileManager* fFileManager;

    public:
        
        static FileManager* GetInstance();
        virtual ~FileManager();
        static bool FileManagerFlag;
        void PrepareCustomDirectory(const std::string& ss);
        void PrepareSavingDirectory();
        void SetFileName(std::string);
        std::string GetFileName(){ return FileName;};
        std::string GetDate(){ return Date;};
        std::string GetTime(){ return Time;};
        std::string GetPID(){ return std::to_string(PID);};
        std::string GetSavingPath(){ return SavingPath;};
        std::string GetPathFortmp(){ return PathFortmp;};
        std::string GetPathForSIMDATA(){ return PathForSIMDATA;};
        std::string GetPathForANALYSIS(){ return PathForANALYSIS;};
        std::string GetPathForFIGURES(){ return PathForFIGURES;};
        unsigned int GetPIDAsInt(){ return PID;};
        

        
    
};



#endif
