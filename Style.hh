
#include "TROOT.h"
#include "TSystem.h"
#include "TApplication.h"
#include "TStyle.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TLine.h"
#include "TF1.h"
#include "TEllipse.h"
#include "TBox.h"
#include "TCut.h"
#include "TLegend.h"
#include "TAttLine.h"
#include "TLatex.h"


void Style(TStyle* jStyle, int dim){
// ----- Style -------------
  jStyle->SetFrameLineWidth(2);  // doesnt work! 
  jStyle->SetFillColor(0);
  jStyle->SetCanvasColor(0);
  jStyle->SetCanvasBorderMode(0);
  jStyle->SetPadColor(0);
  jStyle->SetPadBorderMode(0);
  jStyle->SetFrameFillColor(0);
  jStyle->SetFrameBorderMode(0);
  jStyle->SetStatColor(0);
  jStyle->SetTitleFillColor(0);
//
  jStyle->SetOptStat(1110);   // 1110
//   jStyle->SetOptFit(01111);
  jStyle->SetStatW(0.25);
  jStyle->SetStatH(0.15);
  jStyle->SetStatX(.97);
  jStyle->SetStatY(.93);
//
  jStyle->SetLabelSize(0.05, "x");
  jStyle->SetLabelSize(0.05, "y");
  jStyle->SetTitleOffset(1.0, "x");
  jStyle->SetTitleOffset(1.0, "y");
  jStyle->SetTitleSize(0.06, "x");
  jStyle->SetTitleSize(0.06, "y");
  jStyle->SetTitleFontSize(0.05);
//
  jStyle->SetOptTitle(1);
  jStyle->SetTitleBorderSize(0);
  jStyle->SetTitleFontSize(0.05);
  jStyle->SetTitleX(0.25);
  jStyle->SetTitleY(0.98);
  
  if(dim==1)
    {
      jStyle->SetPadRightMargin(0.1);
      jStyle->SetPadTopMargin(0.1);
      jStyle->SetPadLeftMargin(0.12);
      jStyle->SetPadBottomMargin(0.14);
    }
  if(dim==2)
    {
      jStyle->SetPalette(1,0);
      jStyle->SetOptStat(0);
      jStyle->SetPadRightMargin(0.12);
      jStyle->SetPadTopMargin(0.1);
      jStyle->SetPadLeftMargin(0.12);
      jStyle->SetPadBottomMargin(0.14);
      jStyle->SetLabelSize(0.05, "x");
      jStyle->SetLabelSize(0.05, "y");
      jStyle->SetTitleOffset(1.0, "x");
      jStyle->SetTitleOffset(0.8, "y");
      jStyle->SetTitleSize(0.06, "x");
      jStyle->SetTitleSize(0.06, "y");
      jStyle->SetTitleFontSize(0.05);
    }
// 
  gROOT->SetStyle("jStyle");
}
