#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <TFile.h>
#include <TMath.h>
#include <TVector3.h>
#include <TCanvas.h>

void ypol_Sn124_tbt()
{
    string target = "Sn124";
    Int_t bmin = 5, bmax = 10;
    vector<string> gams{"050", "060", "070", "080"};
    vector<string> pols{"znp", "zpn"};
    // TFile *outfile = new TFile("results/Sn124/unpol.root","recreate");
    ofstream txt{"results/Sn124/zpol.txt"};

    Int_t nx = bmax - bmin + 2, ny = gams.size();

    TCanvas *c = new TCanvas("c", "Canvas", 800, 600);

    c->Divide(nx, ny);


    string xlab = "#it{p}_{x}^{p} + #it{p}_{x}^{n} (MeV/c)";
    string ylab = "#it{p}_{x}^{p} - #it{p}_{x}^{n} (MeV/c)";


    ROOT::Experimental::MultiPadHist<RH2D,nx,ny> hist_px{"px", nx, ny, {150,-750,100}, {150,-500,500}, xlab, ylab};

    xlab = "#it{p}_{y}^{p} + #it{p}_{y}^{n} (MeV/c)";
    ylab = "#it{p}_{y}^{p} - #it{p}_{y}^{n} (MeV/c)";
    ROOT::Experimental::MultiPadHist<RH2D,nx,ny> hist_py{"py", nx, ny, {150,-500,500}, {150,-500,500}, xlab, ylab};

    xlab = "#it{p}_{z}^{p} + #it{p}_{z}^{n} (MeV/c)";
    ylab = "#it{p}_{z}^{p} - #it{p}_{z}^{n} (MeV/c)";
    ROOT::Experimental::MultiPadHist<RH2D,nx,ny> hist_pz{"pz", nx, ny, {150,500,1500}, {150,-500,500}, xlab, ylab};

    xlab = "#it{p}_{x}^{p} + #it{p}_{x}^{n} (MeV/c)";
    ylab = "#phi (deg)";
    ROOT::Experimental::MultiPadHist<RH2D,nx,ny> hist_phi{"phi", nx, ny, {150,-750,100}, {150,-50,50}, xlab, ylab};

    xlab = "#it{p}_{x}^{p} - #it{p}_{x}^{n} (MeV/c)";
    ylab = "Counts";
    ROOT::Experimental::MultiPadHist<RH1D,nx,ny> hist_obs{"obs", 2, 2, {150,-500,500}, xlab, ylab};

    for (Int_t i = 0; i < gams.size(); i++)
    {
        for (Int_t b = bmin; b <= bmax; b++)
        {
            for (string p: pols)
            {
                string folder = "../input_tree/z_pol/b_discrete/d+" + target + "E190g" + \
                                gams[i] + p + "/";
                string filename = string{"dbreakb"} + Form("%02d", b) + ".dat";
                std::cout << folder + filename << std::endl;
                ifstream inputfile{folder + filename};

                string info, header, line;
                getline(inputfile, info);
                getline(inputfile, header);

                Int_t eventNo;
                Double_t pxp, pyp, pzp, pxn, pyn, pzn;

                while (inputfile >> eventNo)
                {
                    inputfile >> pxp >> pyp >> pzp >> pxn >> pyn >> pzn;
                    TVector3 pp = {pxp, pyp, pzp}, pn = {pxn, pyn, pzn};
                    Double_t phi = (pp + pn).Phi()*180/TMath::Pi();
                    if (abs(phi) > 90) phi = phi>0 ? phi-180 : phi+180;

                    // if (pxp + pxn < -200 && pxp + pxn > -350) {
                        // hist_px[nx - 1][i]->Fill({pxp + pxn, pxp - pxn}, b/10.);
                        // hist_px[b-bmin][i]->Fill({pxp + pxn, pxp - pxn});

                        // hist_py[nx - 1][i]->Fill({pyp + pyn, pyp - pyn}, b/10.);
                        // hist_py[b-bmin][i]->Fill({pyp + pyn, pyp - pyn});     

                        // hist_pz[nx - 1][i]->Fill({pzp + pzn, pzp - pzn}, b/10.);
                        // hist_pz[b-bmin][i]->Fill({pzp + pzn, pzp - pzn});          

                        // hist_phi[nx - 1][i]->Fill({pxp + pxn, phi}, b/10.);
                        // hist_phi[b-bmin][i]->Fill({pxp + pxn, phi});          

                        // hist_obs[i%2][i/2]->Fill(pxp - pxn, b/10.);
                    // }

                    
                }
            }
        }
        txt << hist_obs[i%2][i/2]->GetEntries() << ", ";
    }

    hist_px.SetMaximum(100);
    hist_py.SetMaximum(100);
    hist_pz.SetMaximum(100);
    hist_phi.SetMaximum(100);

    hist_px.GetCanvas()->Show();
    hist_py.GetCanvas()->Show();
    hist_pz.GetCanvas()->Show();
    hist_phi.GetCanvas()->Show();
    hist_obs.GetCanvas()->Show();
    txt.close();
}