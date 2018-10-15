//#ifndef __CINT__
//#include <RooGlobalFunc.h>
//#endif   
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string.h>


#include <TCanvas.h>
#include <TAxis.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TStyle.h>
#include <TKey.h>
#include <TMacro.h>

#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooGaussian.h>
#include <RooLandau.h>
#include <RooAddPdf.h>
#include <RooFFTConvPdf.h>
#include <RooPlot.h>


using namespace RooFit; 
using namespace std;

//vector<TObject*> hist_objects;
//TObject* test_object;

int main(int argc, char **argv){
  if (argc < 2) {
    cout << "Please add a root file as argument" << endl;
    return(1);
  }	
  
  TFile *data_file = TFile::Open(argv[1], "read");
  TFile *out_file = new TFile("output.root", "RECREATE");
  
  //gStyle->SetOptFit(1111);
  //string histogram_name = "hist_s631_c0446_b0_k26"; //Tracker
  //string histogram_name = "hist_s890_c0064_b0_k30"; //ECAL

  TString name = "hist_s631_c0446_k26";
  const char* histname = name.Data();
  
  TH1F* data_histogram = nullptr;

  data_file->GetObject( histname, data_histogram );

  if ( data_histogram==nullptr ) {
    cout << "Error! no such histogram, please check ur root file!" << endl;
    return(1);
  }
  //double x_min = data_histogram->FindFirstBinAbove(0);
  //double x_max = data_histogram->FindLastBinAbove(0);
  double hist_mean = data_histogram->GetMean();
  double hist_devi = data_histogram->GetStdDev();
    
    //cout << x_min << " " << x_max << " " << hist_mean << " " << hist_devi << endl;
    RooRealVar x("ADC", "ADC", hist_mean - 5*hist_devi, hist_mean + 5*hist_devi); // mean+-5sigma
    //Construct Landau
    
    RooRealVar lmean ("lmean","mean of landau",hist_mean,hist_mean-hist_devi,hist_mean+hist_devi) ; // mean+-sigma
    RooRealVar lsigma ("lsigma","width of landau",hist_devi/2,0,hist_devi) ; // between 0 and sigma with starting parameter sigma/2
    RooLandau* landau = new RooLandau("landau","landau PDF",x,lmean,lsigma) ; 
    
    //Construct gauss
    RooRealVar gmean("gmean","mean of gauss",0) ;
    RooRealVar gsigma("gsigma","width of gauss",hist_devi/4,0,hist_devi/2) ; // between 0 and sigma/2 with starting parameter sigma/4
    RooGaussian* gauss = new RooGaussian("gauss","gauss PDF",x,gmean,gsigma) ;
		 
    
    RooPlot* frame = x.frame(Title(histname));
    RooFFTConvPdf* langauss = new RooFFTConvPdf(histname, "landau (X) gauss", x, *landau, *gauss);
    
    RooDataHist* datahist = new RooDataHist("datahist","datahist",x,Import(*data_histogram)) ;
    
    //datahist.statOn(frame,Layout(0.55,0.99,0.9)) ;
		
    ////RANGE
    datahist->plotOn(frame);
    //x.setRange("R1", hist_mean-hist_devi/4,hist_mean+hist_devi);
    langauss->fitTo(*datahist, SumW2Error(kTRUE));
    //langauss->chi2FitTo(datahist, Range("R1")) ;
    //langauss->paramOn(frame, Layout(0.7,0.99,0.9)) ;
    
    //langauss.plotOn(frame, Range("R1"), LineColor(kBlack));
    langauss->plotOn(frame, Range("Full"), LineColor(kBlack));
    //landau->fitTo(datahist) ;
    //landau->plotOn(frame, Range("Full"), LineStyle(kDashed),LineColor(kRed)) ;
    
    TCanvas* c = new TCanvas(histname,histname,1600,1200) ;
    frame->Draw() ;
    
    //c->SaveAs("./langauss_fit_to_beam_channel_tracker_test.png");
    //c->SaveAs("./langauss_fit_to_beam_channel_ECAL.png");
    //datahist.Write();
    c->Write();
    //out_file->Write();
    cout << "Mean of the Histogram: " << hist_mean << endl;
    cout << "Std.Dev. of the Histogram: " << hist_devi << endl << endl;
    
    //}
  
  out_file->Close();
  
  
  return 0;
}
