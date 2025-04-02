void Draw_pads() {
    TCanvas *c = new TCanvas("c", "Canvas with Pads", 800, 800);

    // 定义行列数
    int nx = 7, ny = 6;

    // 定义 pad 的边界（无间隙）
    double xmargin = 0.0, ymargin = 0.0;
    double xstep = (1.0 - 2*xmargin) / nx;
    double ystep = (1.0 - 2*ymargin) / ny;

    TPad *pads[nx][ny];

    // 创建 pad 并设置参数
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            double xlow  = xmargin + i * xstep;
            double xhigh = xmargin + (i + 1) * xstep;
            double ylow  = ymargin + (ny - 1 - j) * ystep;
            double yhigh = ymargin + (ny - j) * ystep;

            pads[i][j] = new TPad(Form("pad_%d_%d", i, j), "", xlow, ylow, xhigh, yhigh);
            pads[i][j]->SetMargin(0, 0, 0, 0); // 移除 pad 的边距
            pads[i][j]->Draw();
        }
    }

    // 只保留最左和最下的坐标轴
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            pads[i][j]->cd();
            TH1F *h = new TH1F(Form("h_%d_%d", i, j), "", 10, 0, 10);
            h->FillRandom("gaus", 100);
            h->Draw();

            // 只在最左侧的 pad 显示 Y 轴
            if (i == 0) gPad->SetLeftMargin(0.5);
            // 只在最下方的 pad 显示 X 轴
            if (j == ny-1) gPad->SetBottomMargin(0.5);
        }
    }

    c->SaveAs("Draw_pads.png");
}

void Draw_pads() {

}