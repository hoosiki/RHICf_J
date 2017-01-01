#include "Riostream.h"
void makeroot()
{

    TString dir = gSystem -> UnixPathName(__FILE__);
    dir.ReplaceAll("makeroot.C","");
    dir.ReplaceAll("/./","/");
    ifstream in5;
    ifstream in10;
    ifstream in15;
    ifstream in20;
    ifstream in25;
    ifstream in30;
    ifstream in35;
    ifstream in40;
    ifstream in45;
    ifstream in50;
    ifstream in55;
    ifstream in60;
    ifstream in65;
    ifstream in70;
    ifstream in75;
    ifstream in80;
    ifstream in85;

    in5.open(Form("%sNDEDEW5.out",dir.Data()));
    in10.open(Form("%sNDEDEW10.out",dir.Data()));
    in15.open(Form("%sNDEDEW15.out",dir.Data()));
    in20.open(Form("%sNDEDEW20.out",dir.Data()));
    in25.open(Form("%sNDEDEW25.out",dir.Data()));
    in30.open(Form("%sNDEDEW30.out",dir.Data()));
    in35.open(Form("%sNDEDEW35.out",dir.Data()));
    in40.open(Form("%sNDEDEW40.out",dir.Data()));
    in45.open(Form("%sNDEDEW45.out",dir.Data()));
    in50.open(Form("%sNDEDEW50.out",dir.Data()));
    in55.open(Form("%sNDEDEW55.out",dir.Data()));
    in60.open(Form("%sNDEDEW60.out",dir.Data()));
    in65.open(Form("%sNDEDEW65.out",dir.Data()));
    in70.open(Form("%sNDEDEW70.out",dir.Data()));
    in75.open(Form("%sNDEDEW75.out",dir.Data()));
    in80.open(Form("%sNDEDEW80.out",dir.Data()));
    in85.open(Form("%sNDEDEW85.out",dir.Data()));
    

    Int_t x;
    Float_t y,z;

    TFile* f= new TFile("ZDC.root","RECREATE");
    TNtuple* zdc1 = new TNtuple("zdc5","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc2 = new TNtuple("zdc10","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc3 = new TNtuple("zdc15","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc4 = new TNtuple("zdc20","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc5 = new TNtuple("zdc25","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc6 = new TNtuple("zdc30","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc7 = new TNtuple("zdc35","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc8 = new TNtuple("zdc40","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc9 = new TNtuple("zdc45","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc10 = new TNtuple("zdc50","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc11 = new TNtuple("zdc55","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc12 = new TNtuple("zdc60","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc13 = new TNtuple("zdc65","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc14 = new TNtuple("zdc70","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc15 = new TNtuple("zdc75","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc16 = new TNtuple("zdc80","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");
    TNtuple* zdc17 = new TNtuple("zdc85","NDEDEW", "NOfPhoton:EOfPhoton:DEInW");

    while(1)
    {

        in5 >> x >> y >> z;
        if(!in5.good()) break;
        if(x<500)
        zdc1 -> Fill(x,y,z);

    }
    while(1)
    {

        in10 >> x >> y >> z;
        if(!in10.good()) break;
        if(x<500)
        zdc2 -> Fill(x,y,z);

    }
    while(1)
    {

        in15 >> x >> y >> z;
        if(!in15.good()) break;
        if(x<500)
        zdc3 -> Fill(x,y,z);

    }
    while(1)
    {

        in20 >> x >> y >> z;
        if(!in20.good()) break;
        if(x<500)
        zdc4 -> Fill(x,y,z);

    }
    while(1)
    {

        in25 >> x >> y >> z;
        if(!in25.good()) break;
        if(x<500)
        zdc5 -> Fill(x,y,z);

    }
    while(1)
    {

        in30 >> x >> y >> z;
        if(!in30.good()) break;
        if(x<500)
        zdc6 -> Fill(x,y,z);

    }
    while(1)
    {

        in35 >> x >> y >> z;
        if(!in35.good()) break;
        if(x<500)
        zdc7 -> Fill(x,y,z);

    }
    while(1)
    {

        in40 >> x >> y >> z;
        if(!in40.good()) break;
        if(x<500)
        zdc8 -> Fill(x,y,z);

    }
    while(1)
    {

        in45 >> x >> y >> z;
        if(!in45.good()) break;
        if(x<500)
        zdc9 -> Fill(x,y,z);

    }
    while(1)
    {

        in50 >> x >> y >> z;
        if(!in50.good()) break;
        if(x<500)
        zdc10 -> Fill(x,y,z);

    }
    while(1)
    {

        in55 >> x >> y >> z;
        if(!in55.good()) break;
        if(x<500)
        zdc11 -> Fill(x,y,z);

    }
    while(1)
    {

        in60 >> x >> y >> z;
        if(!in60.good()) break;
        if(x<500)
        zdc12 -> Fill(x,y,z);

    }
    while(1)
    {

        in65 >> x >> y >> z;
        if(!in65.good()) break;
        if(x<500)
        zdc13 -> Fill(x,y,z);

    }
    while(1)
    {

        in70 >> x >> y >> z;
        if(!in70.good()) break;
        if(x<500)
        zdc14 -> Fill(x,y,z);

    }
    while(1)
    {

        in75 >> x >> y >> z;
        if(!in75.good()) break;
        if(x<500)
        zdc15 -> Fill(x,y,z);

    }
    while(1)
    {

        in80 >> x >> y >> z;
        if(!in80.good()) break;
        if(x<500)
        zdc16 -> Fill(x,y,z);

    }
    while(1)
    {

        in85 >> x >> y >> z;
        if(!in85.good()) break;
        if(x<500)
        zdc17 -> Fill(x,y,z);

    }
    in5.close();
    in10.close();
    in15.close();
    in20.close();
    in25.close();
    in30.close();
    in35.close();
    in40.close();
    in45.close();
    in50.close();
    in55.close();
    in60.close();
    in65.close();
    in70.close();
    in75.close();
    in80.close();
    in85.close();

    f -> Write();

}
