//  Description: 本程序用于展示 Sn124 的 z 极化数据的 pxp + pxn 与 pxp - pxn 的关系。

//
//  输入：
//      - Sn124 的 z 极化数据 from imqmd 
//      - 并未考虑 不同b的统计上的关系。

//
//  输出：
//          C->SaveAs("img/hist2d_sn124_pz.pdf");
//         C->SaveAs("img/hist2d_sn124_pz.root");
//
//
//  依赖：
//      - ROOT 框架
//
//  作者：tianbating
//  日期：2025-04-02

// ...existing code...

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


int zpol_Sn124_tbt()
{

    string target = "Sn124";
    Int_t bmin = 5, bmax = 10;
    vector<string> gams{"050", "060", "070", "080"};
    vector<string> pols{"znp", "zpn"};


    Int_t nx = bmax - bmin + 2, ny = gams.size();

    string xlab = "#it{p}_{x}^{p} + #it{p}_{x}^{n} (MeV/c)";
    string ylab = "#it{p}_{x}^{p} - #it{p}_{x}^{n} (MeV/c)";



    TH2D* hist2D[nx][ny];

    // 初始化数组
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            hist2D[i][j] = new TH2D(Form("hist2D_%d_%d", i, j), Form("Histogram %d %d", i, j), 
                                    100, -100, 700, 100, -400, 400); 
        }
    }

    
    for (Int_t i = 0; i < gams.size(); i++)
    {
        for (Int_t b = bmin; b <= bmax; b++)
        {
            for (string p : pols)
            {

            // sim_D_work/input_tree/z_pol/b_discrete/d+Sn124E190g100znp/dbreakb01.dat
                string folder = "../input_tree/z_pol/b_discrete/d+" + target + "E190g" +
                                gams[i] + p + "/";
                string filename = string{"dbreakb"} + Form("%02d", b) + ".dat";
                std::cout << folder + filename << std::endl;

                ifstream inputfile{folder + filename};
                
                // 检查 inputfile 是否成功打开
                if (!inputfile.is_open()) {
                    cerr << "Error opening file" << endl;
                    return 1; // 返回非零值表示错误
                }

                // 或者使用流对象的布尔值检查
                if (!inputfile) {
                    cerr << "Error opening file" << endl;
                    return 1; // 返回非零值表示错误
                }

                string info, header, line;
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
                    // if (abs(phi) > 90)
                    //     phi = phi > 0 ? phi - 180 : phi + 180;

                    pp.RotateZ(phi ), pn.RotateZ(phi );
                    pxp = pp.x(), pyp = pp.y(), pzp = pp.z();
                    pxn = pn.x(), pyn = pn.y(), pzn = pn.z();

                    // if (pzp + pzn > 1150 && abs(pzp - pzn) < 100)
                    // {
                        hist2D[nx-1][i]->Fill(pxp + pxn, pxp - pxn);
                        hist2D[b - bmin][i]->Fill(pxp + pxn, pxp - pxn);

                    // }
                }
            }
        }

    }
 
    double globalMin = hist2D[0][0]->GetMinimum();
    double globalMax = hist2D[0][0]->GetMaximum();
    
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            double min = hist2D[i][j]->GetMinimum();
            double max = hist2D[i][j]->GetMaximum();
            if (min < globalMin) globalMin = min;
            if (max > globalMax) globalMax = max;
        }
    }

    // 设置所有直方图的最小值和最大值
    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            hist2D[i][j]->SetMinimum(globalMin);
            hist2D[i][j]->SetMaximum(globalMax);

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
        "#it{P}_{x}^{p} + #it{P}_{x}^{n} (MeV/c)");
    leftTitle->SetTextAngle(90);
    leftTitle->SetTextSize(0.04);
    leftTitle->SetTextFont(42); // 必须设置字体才能显示LaTeX
    leftTitle->SetTextColor(kBlack);
    leftTitle->SetTextAlign(23); // 垂直居中+右侧对齐
    leftTitle->Draw();

    // 下边标题（X轴方向）
    TLatex *downTitle = new TLatex(        0.5,   // X位置：水平居中
        0.04,  // Y位置：底部边距内侧
        "#it{P}_{x}^{p} - #it{P}_{x}^{n} (MeV/c)");
    downTitle->SetTextSize(0.04);
    downTitle->SetTextFont(42);
    downTitle->SetTextColor(kBlack);
    downTitle->SetTextAlign(22); // 水平居中+底部对齐
    downTitle->Draw();



    // 设置颜色图谱
    // gStyle->SetPalette(kRainBow);
    const Int_t NRGBs = 5;
    const Int_t NCont = 400;

    Double_t stops[NRGBs] = { 0.00, 0.04, 0.08, 0.3,  1.00 };

    // Double_t red[NRGBs]   = { 0.1, 0.25, 0.50, 0.75, 1.00 };
    // Double_t green[NRGBs] = { 0.1, 0.25, 0.50, 0.75, 1.00 };
    // Double_t blue[NRGBs]  = { 0.7, 0.5, 0.4, 0.25, 0.00 };

    // Double_t red[NRGBs]   = { 0.455, 0.000, 0.000, 0.000, 0.596, 0.976 };
    // Double_t green[NRGBs] = { 0.580, 0.706, 0.816, 0.898, 0.957, 0.973 };
    // Double_t blue[NRGBs]  = { 0.851, 0.937, 0.902, 0.757, 0.569, 0.443 };

    // Double_t red[5]   = { 0.208, ,0.000, 0.000 };
    // Double_t green[5] = { 0.651, ,0.710, 0.722 };
    // Double_t blue[5]  = { 0.855, ,0.859, 0.796 };
   Double_t red[5]   = { 0.102, 0.306,0.824 ,1.000,0.996};
    Double_t green[5] = { 0.671,0.843 ,0.761 ,0.471, 0.231};
    Double_t blue[5]  = { 0.859, 0.522, 0.208, 0.345, 0.659};


    // Double_t red[6]   = { 0.153, 0.121, 0.333, 0.545, 0.761, 0.976 };
    // Double_t green[6] = { 0.855, 0.898, 0.941, 0.969, 0.980, 0.973 };
    // Double_t blue[6]  = { 0.945, 0.878, 0.776, 0.651, 0.529, 0.443 };

    // Double_t red[6]   = { 0.113, 0.075, 0.267, 0.486, 0.722, 0.976 };
    // Double_t green[6] = { 0.616, 0.706, 0.792, 0.871, 0.929, 0.973 };
    // Double_t blue[6]  = { 0.678, 0.686, 0.647, 0.576, 0.498, 0.443 };

    // Double_t red[NRGBs]   = { 0.00, 0.3, 1.00, 0.8, 0.51 };
    // Double_t green[NRGBs] = { 0.00, 1.00, 1.00, 0.20, 0.00 };
    // Double_t blue[NRGBs]  = { 1.00, 0.5, 0.12, 0.00, 0.00 };

    // Double_t blue[NRGBs]  = { 0.75, 0.85, 0.90, 0.95, 1.00 };

    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);



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
 
        //  TH1F *hist2D[i][j] = (TH1F*) h->Clone(TString::Format("h_%d_%d",i,j).Data());

        hist2D[i][j]->GetXaxis()->SetRangeUser(-90.0, 650.0);
        hist2D[i][j]->GetYaxis()->SetRangeUser(-380.0, 380.0);

 
        //  // Format for y axis
         hist2D[i][j]->GetYaxis()->SetLabelFont(43);
         hist2D[i][j]->GetYaxis()->SetLabelSize(8);

 
        //  // Format for x axis
         hist2D[i][j]->GetXaxis()->SetLabelFont(43);
         hist2D[i][j]->GetXaxis()->SetLabelSize(8);



            // hist2D[i][j]->SetTitle(""); // 去掉直方图的标题
            hist2D[i][j]->SetXTitle(""); // 去掉 x 轴标题
            hist2D[i][j]->SetYTitle(""); // 去掉 y 轴标题

  
            hist2D[i][j]->Draw("COL"); // 绘制直方图，使用 "COLZ" 选项

            // 禁用统计框
            hist2D[i][j]->SetStats(0); // 关闭统计信息显示

        



        }
    }
    C->cd(0); // 返回主画布

    // 创建一个大的颜色刻度条
    TPaletteAxis *palette = new TPaletteAxis(0.921, bMargin, 0.94, 1-tMargin, hist2D[0][0]); // 调整位置和大小
    // palette->SetTitle("Color Scale"); // 设置标题
    palette->SetLabelFont(42);
    palette->SetLabelSize(0.04);
    palette->SetFillColor(0); // 设置背景透明
    palette->Draw();

    // TPaletteAxis *palette = new TPaletteAxis(0.92, 0.2, 0.96, 0.8, hist);
    // palette->SetTitle("Color Scale");
    // palette->SetLabelFont(42);
    // palette->SetLabelSize(0.04);
    // palette->SetFillColor(0); // 透明背景

    // 保存为 PDF 文件
    C->SaveAs("img/hist2d_sn124_pz.pdf");
     C->SaveAs("img/hist2d_sn124_pz.root");
    return 1;
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