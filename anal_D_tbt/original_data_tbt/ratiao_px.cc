#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <TFile.h>
#include <TMath.h>
#include <TVector3.h>
#include <TFile.h>
#include <TMath.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TH2D.h>
#include <iostream>
#include <TText.h>
#include <TLatex.h>
#include <TLine.h>
#include <TStyle.h>
#include <TROOT.h>
void CanvasPartition(TCanvas *C,const Int_t Nx = 2,const Int_t Ny = 2,
                     Float_t lMargin = 0.15, Float_t rMargin = 0.05,
                     Float_t bMargin = 0.15, Float_t tMargin = 0.05);
double XtoPad(double x);
double YtoPad(double x);

int ratiao_px()
{



    std::string target = "Sn124";
    Int_t bmin = 5, bmax = 10;
    std::vector<std::string> gams{"050", "060", "070", "080"};
    std::vector<std::string> pols{"znp", "zpn"};

    std::ofstream txt{"results/Sn112/zpol_rdm_cut1.txt"};
    Int_t nx = bmax - bmin + 2, ny = gams.size();

    std::string xlab = "#it{p}_{x}^{p} (MeV/c)";
    std::string ylab = "#{p}_{x}^{n} (MeV/c)";

    TH2D* hist2D[nx][ny];


    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            hist2D[i][j] = new TH2D(Form("hist2D_%d_%d", i, j), Form("Histogram %d %d", i, j), 
                                    100, -100, 700, 100, -100, 700); 
        }
    }

    
    for (Int_t i = 0; i < gams.size(); i++)
    {
        for (Int_t b = bmin; b <= bmax; b++)
        {
            for (std::string p : pols)
            {
                std::string folder = "../input_tree/z_pol/b_discrete/d+" + target + "E190g" +
                                gams[i] + p + "/";
                std::string filename = std::string{"dbreakb"} + Form("%02d", b) + ".dat";
                std::cout << folder + filename << std::endl;

                std::ifstream inputfile{folder + filename};
                
                // 检查 inputfile 是否成功打开
                if (!inputfile.is_open()) {
                    std::cerr << "Error opening file" << std::endl;
                    return 1; // 返回非零值表示错误
                }

                // 或者使用流对象的布尔值检查
                if (!inputfile) {
                    std::cerr << "Error opening file" << std::endl;
                    return 1; // 返回非零值表示错误
                }

                std::string info, header, line;
                getline(inputfile, info);
                getline(inputfile, header);

                
                hist2D[b - bmin][i]->SetTitle(("gammas" + gams[i] + "b" + Form("%02d", b)).c_str()); // 去掉直方图的标题


                Int_t eventNo;
                Double_t pxp, pyp, pzp, pxn, pyn, pzn;

                 // std::cout << "pxp + pxn "<<pxp + pxn <<std::endl;
                while (inputfile >> eventNo)
                {
                    inputfile >> pxp >> pyp >> pzp >> pxn >> pyn >> pzn;
                    TVector3 pp = {pxp, pyp, pzp}, pn = {pxn, pyn, pzn};
                    Double_t phi = (pp + pn).Phi();


                    pp.RotateZ(phi ), pn.RotateZ(phi );
                    pxp = pp.x(), pyp = pp.y(), pzp = pp.z();
                    pxn = pn.x(), pyn = pn.y(), pzn = pn.z();

                    // if (pzp + pzn > 1150 && abs(pzp - pzn) < 100)
                    // {
                        hist2D[nx-1][i]->Fill(pxp ,  pxn);
                        hist2D[b - bmin][i]->Fill(pxp, pxn);

                    // }
                }
            
            }
        }
    }

    
    auto C = (TCanvas*) gROOT->FindObject("C");
    if (C) delete C;
    C = new TCanvas("C","canvas",1024,640);
    C->SetFillStyle(4000);

   // Margins
    Float_t lMargin = 0.08;
    Float_t rMargin = 0.08;
    Float_t bMargin = 0.08;
    Float_t tMargin = 0.05;
 
    CanvasPartition(C,nx,ny,lMargin,rMargin,bMargin,tMargin);
 

 
   TPad *pad[nx][ny];
 


    C->cd(0); 

    // 左边标题（Y轴方向）
    TLatex *leftTitle = new TLatex(        0.01,  // X位置：左边距内侧
        0.5,   // Y位置：垂直居中
        "#it{P}_{x}^{p}  (MeV/c)");
    leftTitle->SetTextAngle(90);
    leftTitle->SetTextSize(0.04);
    leftTitle->SetTextFont(42); // 必须设置字体才能显示LaTeX
    leftTitle->SetTextColor(kBlack);
    leftTitle->SetTextAlign(23); // 垂直居中+右侧对齐
    leftTitle->Draw();

    // 下边标题（X轴方向）
    TLatex *downTitle = new TLatex(        0.5,   // X位置：水平居中
        0.04,  // Y位置：底部边距内侧
        "#it{P}_{x}^{n}  (MeV/c)");
    downTitle->SetTextSize(0.04);
    downTitle->SetTextFont(42);
    downTitle->SetTextColor(kBlack);
    downTitle->SetTextAlign(22); // 水平居中+底部对齐
    downTitle->Draw();



    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
        C->cd(0);
                  // Get the pads previously created.
         pad[i][j] = (TPad*) C->FindObject(TString::Format("pad_%d_%d",i,j).Data());
         pad[i][j]->Draw();
         pad[i][j]->SetFillStyle(4000);
         pad[i][j]->SetFrameFillStyle(4000);
         pad[i][j]->cd();
 
         // Size factors
         Float_t xFactor = pad[0][0]->GetAbsWNDC()/pad[i][j]->GetAbsWNDC();
         Float_t yFactor = pad[0][0]->GetAbsHNDC()/pad[i][j]->GetAbsHNDC();
 
        //  TH1F *hist2D[i][j] = (TH1F*) h->Clone(Tstd::String::Format("h_%d_%d",i,j).Data());

        hist2D[i][j]->GetXaxis()->SetRangeUser(-90.0, 650.0);
        hist2D[i][j]->GetYaxis()->SetRangeUser(-90.0, 650.0);

 
        //  // Format for y axis
        hist2D[i][j]->GetYaxis()->SetLabelFont(43);
        hist2D[i][j]->GetYaxis()->SetLabelSize(8);
        hist2D[i][j]->GetXaxis()->SetLabelFont(43);
        hist2D[i][j]->GetXaxis()->SetLabelSize(8);

        hist2D[i][j]->SetXTitle(""); // 去掉 x 轴标题
        hist2D[i][j]->SetYTitle(""); // 去掉 y 轴标题

  
            hist2D[i][j]->Draw("COL"); // 绘制直方图，使用 "COLZ" 选项

            // 禁用统计框
            hist2D[i][j]->SetStats(0); // 关闭统计信息显示

        



        }
    }
    C->cd(0); // 返回主画布

    C->SaveAs("./img/show_px.pdf");

    return 0;
}





void CanvasPartition(TCanvas *C,const Int_t Nx,const Int_t Ny,
                     Float_t lMargin, Float_t rMargin,
                     Float_t bMargin, Float_t tMargin)
{
   if (!C) return;
 
   // Setup Pad layout:
   Float_t vSpacing = 0.0;
   Float_t vStep  = (1.- bMargin - tMargin - (Ny-1) * vSpacing) / Ny;
 
   Float_t hSpacing = 0.0;
   Float_t hStep  = (1.- lMargin - rMargin - (Nx-1) * hSpacing) / Nx;
 
   Float_t vposd,vposu,vmard,vmaru,vfactor;
   Float_t hposl,hposr,hmarl,hmarr,hfactor;
 
   for (Int_t i=0;i<Nx;i++) {
 
      if (i==0) {
         hposl = 0.0;
         hposr = lMargin + hStep;
         hfactor = hposr-hposl;
         hmarl = lMargin / hfactor;
         hmarr = 0.0;
      } else if (i == Nx-1) {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep + rMargin;
         hfactor = hposr-hposl;
         hmarl = 0.0;
         hmarr = rMargin / (hposr-hposl);
      } else {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep;
         hfactor = hposr-hposl;
         hmarl = 0.0;
         hmarr = 0.0;
      }
 
      for (Int_t j=0;j<Ny;j++) {
 
         if (j==0) {
            vposd = 0.0;
            vposu = bMargin + vStep;
            vfactor = vposu-vposd;
            vmard = bMargin / vfactor;
            vmaru = 0.0;
         } else if (j == Ny-1) {
            vposd = vposu + vSpacing;
            vposu = vposd + vStep + tMargin;
            vfactor = vposu-vposd;
            vmard = 0.0;
            vmaru = tMargin / (vposu-vposd);
         } else {
            vposd = vposu + vSpacing;
            vposu = vposd + vStep;
            vfactor = vposu-vposd;
            vmard = 0.0;
            vmaru = 0.0;
         }
 
         C->cd(0);
 
         auto name = TString::Format("pad_%d_%d",i,j);
         auto pad = (TPad*) C->FindObject(name.Data());
         if (pad) delete pad;
         pad = new TPad(name.Data(),"",hposl,vposd,hposr,vposu);
         pad->SetLeftMargin(hmarl);
         pad->SetRightMargin(hmarr);
         pad->SetBottomMargin(vmard);
         pad->SetTopMargin(vmaru);
 
         pad->SetFrameBorderMode(0);
         pad->SetBorderMode(0);
         pad->SetBorderSize(0);
 
         pad->Draw();
      }
   }
}
 
double XtoPad(double x)
{
   double xl,yl,xu,yu;
   gPad->GetPadPar(xl,yl,xu,yu);
   double pw = xu-xl;
   double lm = gPad->GetLeftMargin();
   double rm = gPad->GetRightMargin();
   double fw = pw-pw*lm-pw*rm;
   return (x*fw+pw*lm)/pw;
}
 
double YtoPad(double y)
{
   double xl,yl,xu,yu;
   gPad->GetPadPar(xl,yl,xu,yu);
   double ph = yu-yl;
   double tm = gPad->GetTopMargin();
   double bm = gPad->GetBottomMargin();
   double fh = ph-ph*bm-ph*tm;
   return (y*fh+bm*ph)/ph;
}