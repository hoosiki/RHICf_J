#include "Riostream.h"
int main()
{

    Double_t mean[17];
    Double_t RMS[17];
    Double_t RMSX[17];
    Double_t Angle[17];
    Double_t Res[17];
    Double_t Lin[17];
    Int_t j=17;


    TFile* file =  TFile::Open("ZDC.root");
    TNtuple* N1 = (TNtuple*) gROOT->FindObject("zdc5");
    TNtuple* N2 = (TNtuple*) gROOT->FindObject("zdc10");
    TNtuple* N3 = (TNtuple*) gROOT->FindObject("zdc15");
    TNtuple* N4 = (TNtuple*) gROOT->FindObject("zdc20");
    TNtuple* N5 = (TNtuple*) gROOT->FindObject("zdc25");
    TNtuple* N6 = (TNtuple*) gROOT->FindObject("zdc30");
    TNtuple* N7 = (TNtuple*) gROOT->FindObject("zdc35");
    TNtuple* N8 = (TNtuple*) gROOT->FindObject("zdc40");
    TNtuple* N9 = (TNtuple*) gROOT->FindObject("zdc45");
    TNtuple* N10 = (TNtuple*) gROOT->FindObject("zdc50");
    TNtuple* N11 = (TNtuple*) gROOT->FindObject("zdc55");
    TNtuple* N12 = (TNtuple*) gROOT->FindObject("zdc60");
    TNtuple* N13 = (TNtuple*) gROOT->FindObject("zdc65");
    TNtuple* N14 = (TNtuple*) gROOT->FindObject("zdc70");
    TNtuple* N15 = (TNtuple*) gROOT->FindObject("zdc75");
    TNtuple* N16 = (TNtuple*) gROOT->FindObject("zdc80");
    TNtuple* N17 = (TNtuple*) gROOT->FindObject("zdc85");
    
    TH1D* h1 = new TH1D("h1", "", 100, 0., 40.);
    TH1D* h2 = new TH1D("h2", "", 100, 0., 40.);
    TH1D* h3 = new TH1D("h3", "", 100, 0., 40.);
    TH1D* h4 = new TH1D("h4", "", 100, 0., 40.);
    TH1D* h5 = new TH1D("h5", "", 100, 0., 40.);
    TH1D* h6 = new TH1D("h6", "", 100, 0., 40.);
    TH1D* h7 = new TH1D("h7", "", 100, 0., 40.);
    TH1D* h8 = new TH1D("h8", "", 100, 0., 40.);
    TH1D* h9 = new TH1D("h9", "", 100, 0., 40.);
    TH1D* h10 = new TH1D("h10", "", 100, 0., 40.);
    TH1D* h11 = new TH1D("h11", "", 100, 0., 40.);
    TH1D* h12 = new TH1D("h12", "", 100, 0., 40.);
    TH1D* h13 = new TH1D("h13", "", 100, 0., 40.);
    TH1D* h14 = new TH1D("h14", "", 100, 0., 40.);
    TH1D* h15 = new TH1D("h15", "", 100, 0., 40.);
    TH1D* h16 = new TH1D("h16", "", 100, 0., 40.);
    TH1D* h17 = new TH1D("h17", "", 100, 0., 40.);

    N1->Draw("NOfPhoton >> h1", "NOfPhoton>=1","goff");
    N2->Draw("NOfPhoton >> h2", "NOfPhoton>=1","goff");
    N3->Draw("NOfPhoton >> h3", "NOfPhoton>=1","goff");
    N4->Draw("NOfPhoton >> h4", "NOfPhoton>=1","goff");
    N5->Draw("NOfPhoton >> h5", "NOfPhoton>=1","goff");
    N6->Draw("NOfPhoton >> h6", "NOfPhoton>=1","goff");
    N7->Draw("NOfPhoton >> h7", "NOfPhoton>=1","goff");
    N8->Draw("NOfPhoton >> h8", "NOfPhoton>=1","goff");
    N9->Draw("NOfPhoton >> h9", "NOfPhoton>=1","goff");
    N10->Draw("NOfPhoton >> h10", "NOfPhoton>=1","goff");
    N11->Draw("NOfPhoton >> h11", "NOfPhoton>=1","goff");
    N12->Draw("NOfPhoton >> h12", "NOfPhoton>=1","goff");
    N13->Draw("NOfPhoton >> h13", "NOfPhoton>=1","goff");
    N14->Draw("NOfPhoton >> h14", "NOfPhoton>=1","goff");
    N15->Draw("NOfPhoton >> h15", "NOfPhoton>=1","goff");
    N16->Draw("NOfPhoton >> h16", "NOfPhoton>=1","goff");
    N17->Draw("NOfPhoton >> h17", "NOfPhoton>=1","goff");

    TF1* f1 = new TF1("f1", "gaus", 0, 40);
    TF1* f2 = new TF1("f2", "gaus", 0, 40);
    TF1* f3 = new TF1("f3", "gaus", 0, 40);
    TF1* f4 = new TF1("f4", "gaus", 0, 40);
    TF1* f5 = new TF1("f5", "gaus", 0, 40);
    TF1* f6 = new TF1("f6", "gaus", 0, 40);
    TF1* f7 = new TF1("f7", "gaus", 0, 40);
    TF1* f8 = new TF1("f8", "gaus", 0, 40);
    TF1* f9 = new TF1("f9", "gaus", 0, 40);
    TF1* f10 = new TF1("f10", "gaus", 0, 40);
    TF1* f11 = new TF1("f11", "gaus", 0, 40);
    TF1* f12 = new TF1("f12", "gaus", 0, 40);
    TF1* f13 = new TF1("f13", "gaus", 0, 40);
    TF1* f14 = new TF1("f14", "gaus", 0, 40);
    TF1* f15 = new TF1("f15", "gaus", 0, 40);
    TF1* f16 = new TF1("f16", "gaus", 0, 40);
    TF1* f17 = new TF1("f17", "gaus", 0, 40);

   
    
    h1->Fit("f1");
    h2->Fit("f2");
    h3->Fit("f3");
    h4->Fit("f4");
    h5->Fit("f5");
    h6->Fit("f6");
    h7->Fit("f7");
    h8->Fit("f8");
    h9->Fit("f9");
    h10->Fit("f10");
    h11->Fit("f11");
    h12->Fit("f12");
    h13->Fit("f13");
    h14->Fit("f14");
    h15->Fit("f15");
    h16->Fit("f16");
    h17->Fit("f17");



    mean[0] = f1->GetParameter(1);
    mean[1] = f2->GetParameter(1);
    mean[2] = f3->GetParameter(1);
    mean[3] = f4->GetParameter(1);
    mean[4] = f5->GetParameter(1);
    mean[5] = f6->GetParameter(1);
    mean[6] = f7->GetParameter(1);
    mean[7] = f8->GetParameter(1);
    mean[8] = f9->GetParameter(1);
    mean[9] = f10->GetParameter(1);
    mean[10] = f11->GetParameter(1);
    mean[11] = f12->GetParameter(1);
    mean[12] = f13->GetParameter(1);
    mean[13] = f14->GetParameter(1);
    mean[14] = f15->GetParameter(1);
    mean[15] = f16->GetParameter(1);
    mean[16] = f17->GetParameter(1);
    RMS[0] = f1->GetParameter(2);
    RMS[1] = f2->GetParameter(2);
    RMS[2] = f3->GetParameter(2);
    RMS[3] = f4->GetParameter(2);
    RMS[4] = f5->GetParameter(2);
    RMS[5] = f6->GetParameter(2);
    RMS[6] = f7->GetParameter(2);
    RMS[7] = f8->GetParameter(2);
    RMS[8] = f9->GetParameter(2);
    RMS[9] = f10->GetParameter(2);
    RMS[10] = f11->GetParameter(2);
    RMS[11] = f12->GetParameter(2);
    RMS[12] = f13->GetParameter(2);
    RMS[13] = f14->GetParameter(2);
    RMS[14] = f15->GetParameter(2);
    RMS[15] = f16->GetParameter(2);
    RMS[16] = f17->GetParameter(2);
    Angle[0] = 5;
    Angle[1] = 10;
    Angle[2] = 15;
    Angle[3] = 20;
    Angle[4] = 25;
    Angle[5] = 30;
    Angle[6] = 35;
    Angle[7] = 40;
    Angle[8] = 45;
    Angle[9] = 50;
    Angle[10] = 55;
    Angle[11] = 60;
    Angle[12] = 65;
    Angle[13] = 70;
    Angle[14] = 75;
    Angle[15] = 80;
    Angle[16] = 85;

    for(int i=0; i<17; i++)
    {

        cout << mean[i] << "-----" << RMS[i] << endl;
        //cout << RMS[i]/mean[i] << endl;
        //Res[i] = RMS[i]/mean[i];
        //Lin[i] = 1/sqrt(Angle[i]);
        

    }


   TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",200,10,700,500);

   //c1->SetFillColor();
   c1->SetGrid();


    TGraph* gr = new TGraph(j, Angle, mean);
    //TGraph* g = new TGraph(j, Angle, Res);
    //graph1 -> Draw("AP");

    //TF1* t1 = new TF1("t1","[0]/x**0.5",0,1.);
   
    //t1->SetParameter(0,1);
    //gr -> Fit("gauss");
    //gr->Fit("t1","R+");
   gr->SetLineColor(2);
   gr->SetLineWidth(1);
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(20);
   gr->SetMinimum(0);

   gr->SetTitle("Angle VS NumOfPhotons");
   gr->GetXaxis()->SetTitle("Angle[deg]");
   gr->GetYaxis()->SetTitle("NumOfPhoton");



   // f1->Draw();
   gr->Draw("ACP");

   gStyle->SetOptFit(1111111);

    return 0;
}
