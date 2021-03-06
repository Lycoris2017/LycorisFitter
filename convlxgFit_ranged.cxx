//** This is manually supervised fit procedure **
//
// by Mengqing <mengqing.wu@deys.de>
// 
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

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooLandau.h"
#include "RooAddPdf.h"
#include "RooFFTConvPdf.h"
#include "RooPlot.h"
#include "RooFitResult.h"

using namespace RooFit; 
using namespace std;

void SetRangeCutX(TH1F& hist, double integralCut);
void fitLan(TH1F* dh, TFile& fout, double xmin=0, double xmax=0);
void fitLxG(TH1F* dh, TFile& fout, double xmin=0, double xmax=0);

Double_t ScaleX(Double_t x){
	Double_t v;
	v= 10*x ;
	return v;
}
void ScaleAxis(TAxis *a, Double_t (*Scale)(Double_t))
{
  if (!a) return; // just a precaution
  if (a->GetXbins()->GetSize())
    {
      // an axis with variable bins
      // note: bins must remain in increasing order, hence the "Scale"
      // function must be strictly (monotonically) increasing
      TArrayD X(*(a->GetXbins()));
      for(Int_t i = 0; i < X.GetSize(); i++) X[i] = Scale(X[i]);
      a->Set((X.GetSize() - 1), X.GetArray()); // new Xbins
    }
  else
    {
      // an axis with fix bins
      // note: we modify Xmin and Xmax only, hence the "Scale" function
      // must be linear (and Xmax must remain greater than Xmin)
      a->Set( a->GetNbins(),
              Scale(a->GetXmin()), // new Xmin
              Scale(a->GetXmax()) ); // new Xmax
    }
  return;
}

void ScaleXaxis(TH1 *h, Double_t (*Scale)(Double_t))
{
  if (!h) return; // just a precaution
  ScaleAxis(h->GetXaxis(), Scale);
  return;
}


int main(int argc, char **argv){
  if (argc < 3) {
    cout << "Please add a root file as argument.\n Usage: ./temp xx.root histname [0.1 10]" << endl;
    return(1);
  }	

  //** START-- SET YOUR Parameters.
  //gStyle->SetOptFit(1111);
  TString name;
  name = argv[2];
  
  double xmin=0.1;
  double xmax=10;
  if (argc == 5){
    double a = atof(argv[3]);
    double b = atof(argv[4]);
    if (a<b){
      xmin = a;
      xmax = b;
    }
  }
  cout << "Fit range : "<< xmin << " to " << xmax << endl;

  //** END -- SET YOUR Parameters.
  
  const char* histname = name.Data();

  TFile *data_file = TFile::Open(argv[1], "read");
  TH1F* data_histogram = nullptr;
  data_file->GetObject( histname, data_histogram );
  if ( data_histogram==nullptr ) {
    cout << "Error! no such histogram, please check ur root file!" << endl;
    return(1);
  }

  data_histogram->SetDirectory(0);
  data_file->Close();
  //SetRangeCutX(*data_histogram, 0.9);

  string x; 
  cout << "Normalize data hist to 1? [y/n]"; // Type a number and press enter
  cin >> x; // Get user input from the keyboard
  if (x=="y" || x=="Y"  ){
    cout << "Normalize to 1..." << endl;
    data_histogram->Scale(1/data_histogram->Integral());
  }
  else
    cout << "No Normalize." << endl;

  /**** different scale function used to tune your fit ****/
  //data_histogram->Scale(103210);
  //ScaleXaxis(data_histogram, ScaleX);

  TFile *out_file = new TFile("output_ranged.root", "RECREATE");
  data_histogram->Write();
    
  //fitLan(data_histogram, *out_file, xmin, xmax);
  fitLxG(data_histogram, *out_file, xmin, xmax);

  out_file->Close();
  
  return 0;
}

void fitLan(TH1F* dh, TFile& fout, double xmin, double xmax){
	double xmean;
	//** default setting for fC distribution FIT
	if (xmin==0 || xmax==0){
		xmin = 40;
		xmax = 60;
		xmean = 48;
	} else xmean = (xmin+xmax)/2;
	//** end of default

  TH1F *m_dh = (TH1F*) dh->Clone();
  m_dh->SetDirectory(0);

  //m_dh ->Rebin(4);

  TString outhist = dh->GetName();
  outhist+="_lanOnly";
    
  // Create observables
  //RooRealVar x("poi","ADC",40,60) ;
  RooRealVar x("poi","Charge [fC]",xmin, xmax) ;
 
  // Create Landau
  RooRealVar sigma("sigma","sigma", 5, 0.1, 10) ;
  RooRealVar mean("mean","mean", xmean, xmin, xmax) ;
  RooLandau *landau = new RooLandau("landau","landau",x, mean, sigma) ;

  // Import Data
  RooDataHist* datahist = new RooDataHist("datahist","datahist",x,Import(*m_dh)) ;
  
  // Fit pdf to data
  landau->fitTo(*datahist,  SumW2Error(kTRUE)) ;

	  
  //-- Ploting:
  RooPlot* frame = x.frame(Title(outhist), Bins( datahist->numEntries() ));
  frame->SetName("frame_fitTo_lanOnly");

  frame->GetXaxis()->SetTitleSize(0.05);
  frame->GetYaxis()->SetTitleSize(0.05);
  frame->GetXaxis()->SetLabelSize(0.05);
  frame->GetYaxis()->SetLabelSize(0.05);
  frame->GetXaxis()->SetTitleFont(42);
  frame->GetYaxis()->SetTitleFont(42);
  
  datahist->plotOn(frame, Name("datahist"), LineColor(kBlue), MarkerColor(kBlue));

  landau->plotOn( frame, Name("landau"),LineStyle(kDashed),LineColor(kRed) );

  //langauss->plotOn(frame, Range("Full"), LineColor(kRed));
  //landau->plotOn(frame, Range("Full"), LineStyle(kDashed),LineColor(kBlue)) ;

  //datahist->statOn(frame,Layout(0.6,0.85,0.9)) ;
  //landau->paramOn(frame, Layout(0.65,0.95,0.7)) ;
  landau->paramOn(frame,  Layout(0.5,0.8,0.9), Format("NEU", AutoPrecision(1)) ) ;

  frame->GetYaxis()->SetTitle("Clusters");
	  
  TCanvas* c = new TCanvas(outhist,outhist,800,600) ;
  frame->Draw() ;
  fout.cd();
  frame->Write();
  datahist->Write();
  c->Write();
  
  return;
}

void fitLxG(TH1F* dh, TFile& fout, double xmin, double xmax){
  /*
  * fit landau (x) gauss to input data hsit;
  * will write down canvas + fitres to the input fout;
  */

	double xmean;
	//** default setting for fC distribution FIT
	if (xmin==0 || xmax==0){
		xmin = 40;
		xmax = 60;
		xmean = 48;
	} else xmean = (xmin+xmax)/2;
	//** end of default
	
	
  TH1F *m_dh = (TH1F*) dh->Clone();
  m_dh->SetDirectory(0);
  //m_dh->Rebin(4);
  const char* histname = m_dh ->GetName();


  /*--  Start LxG fit --*/

  //-- Construct observable
  RooRealVar x ("poi", "fC", xmin, xmax); // mean+-5sigma
  
  //-- Import Data
  RooDataHist* datahist = new RooDataHist("datahist","datahist",x,Import(*m_dh)) ;
  cout << "[MQ] How many bins of datahist? => " << datahist->numEntries() << endl;
  
  //-- trim the data
  //RooDataHist* dtrim = datahist->reduce("x>45");

  /*-- Create Model --*/

  //-- Construct Landau
  RooRealVar lmpv ("lmpv","mpv of landau", xmean, xmin, xmax) ;
  RooRealVar lsigma ("lsigma","width of landau", 5, 0.1, 10) ; 
  RooLandau* landau = new RooLandau("landau","landau PDF", x, lmpv, lsigma) ; 

  //-- Construct Landau Only fit
  RooRealVar mean ("mean","mean of landau", xmean, xmin, xmax );
  RooRealVar sigma("sigma","sigma", 1, 0.1, 10) ;
  RooLandau* landauOnly = new RooLandau("landau","landau", x, mean, sigma) ; 
    
  //-- Construct gauss
  RooRealVar gmean("gmean","mean of gauss", 0) ;
  RooRealVar gsigma("gsigma","width of gauss", 0.3, 0.1, 1.5) ;
  RooGaussian* gauss = new RooGaussian("gauss","gauss PDF", x, gmean, gsigma) ;
  
  //-- Construct convolution pdf
  cout << "[MQ] How many bins of observable-x => " << x.getBins("cache") << endl;
  x.setBins( datahist->numEntries(), "cache");
  cout << "[MQ] Change bins of observable-x => " << x.getBins("cache") << endl;

  RooFFTConvPdf* langauss = new RooFFTConvPdf(histname, "landau (X) gauss", x, *landau, *gauss);
  
  //-- Fit pdf to Data:
  RooFitResult * fitres = langauss->fitTo(*datahist, SumW2Error(kTRUE),RooFit::Save());

  
  string input; 
  cout << "Do a landau-only fit? [y/n]"; // Type a number and press enter
  cin >> input; // Get user input from the keyboard
  bool dolanfit = (input=="y");
  if (dolanfit){
	  printf("[INFO] Do also landauOnly.\n");
	  landauOnly->fitTo(*datahist,  SumW2Error(kTRUE)) ;
  }
  else
	  printf("[INFO] No fit to landauOnly.\n");

  cout<< "[Mengqing] Print RooFitResult >>";
  fitres->Print();
  cout<< "[Mengqing] Finished printing <<\n" <<endl;

  //-- Ploting:
  RooPlot* frame = x.frame(Title(histname));
  frame->SetName("frame_fitTo_langauss");

  frame->GetXaxis()->SetTitleSize(0.05);
  frame->GetYaxis()->SetTitleSize(0.05);
  frame->GetXaxis()->SetLabelSize(0.05);
  frame->GetYaxis()->SetLabelSize(0.05);
  frame->GetXaxis()->SetTitleFont(42);
  frame->GetYaxis()->SetTitleFont(42);
  
  datahist->plotOn(frame, LineColor(kViolet), MarkerColor(kViolet));

  langauss ->plotOn( frame, LineColor(kRed) );
  //landau ->plotOn( frame, LineStyle(kDashed),LineColor(kBlue) );
  if (dolanfit)
	  landauOnly ->plotOn( frame, LineStyle(kDashed), LineColor(kBlue) );
  
  //langauss->plotOn(frame, Range("Full"), LineColor(kRed));
  //landau->plotOn(frame, Range("Full"), LineStyle(kDashed),LineColor(kBlue)) ;

  //datahist->statOn(frame,Layout(0.55,0.85,0.9)) ;
  langauss->paramOn(frame, Layout(0.5,0.6,0.75), Format("NEU", AutoPrecision(1)) ) ;
  TString parambox = histname;
  parambox += "_paramBox";
  frame->getAttText(parambox)->SetTextColor(kBlack);
  frame->getAttFill(parambox)->SetFillStyle(0);
  frame->getAttLine(parambox)->SetLineWidth(0);
	  
  TCanvas* c = new TCanvas(histname,histname,800,600) ;
  frame->Draw() ;

  fout.cd();
  //datahist.Write();
  fitres->Write();
  frame->Write();
  c->Write();

  return;
  
};

void SetRangeCutX(TH1F& hist, double integralCut){
  /*
  *  find the main distribution area == 99% integral
  */

  if (! (integralCut >0 && integralCut<1.00 ) ) integralCut = 0.90; 
      
  // init value from 100% integral
  int binxmin = hist.FindFirstBinAbove(0);
  int binxmax = hist.FindLastBinAbove(0);

  auto xaxis = hist.GetXaxis();
    
  double xmin = xaxis ->GetBinCenter( binxmin);
  double xmax = xaxis ->GetBinCenter( binxmax);
  
  double mu = hist.GetMean();
  double sig  = hist.GetStdDev();

  int binxmu = xaxis->FindBin(mu);
  printf("[MQ] Init value, x-bins->(%d, %d), x->(%.2f, %.2f), x^ = %.2f (bin@%d), sig(x) = %.2f\n\n", binxmin, binxmax, xmin, xmax, mu, binxmu, sig);

  int binxminCut = binxmu;
  int binxmaxCut = binxmu;

  double integral100 = hist.Integral();

  // do an add-up from the mu-bin w/ 1-bin step
  do{
    if ( binxminCut> binxmin)
      binxminCut -=1;
    if ( binxmaxCut< binxmax)
      binxmaxCut +=1;
    
  }while( hist.Integral(binxminCut, binxmaxCut) < integralCut*integral100 );

  double xminCut = xaxis ->GetBinCenter( binxminCut);
  double xmaxCut = xaxis ->GetBinCenter( binxmaxCut);

  xaxis->SetRangeUser(xminCut, xmaxCut);
    
  printf("[MQ] Final value, x-bins->(%d, %d), x->(%.2f, %.2f), x^ = %.2f, sig(x) = %.2f\n\n", binxminCut, binxmaxCut, xminCut, xmaxCut, hist.GetMean(), hist.GetStdDev() );


  return;
}
