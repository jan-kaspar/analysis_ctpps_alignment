#include "common.h"
#include "parameters.h"

#include "stat.h"

#include "TFile.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TF1.h"

#include <vector>
#include <string>

//----------------------------------------------------------------------------------------------------

using namespace std;

//----------------------------------------------------------------------------------------------------

TF1 *ff_pol2 = new TF1("ff_pol2", "[0] + [1]*x + [2]*x*x");

//----------------------------------------------------------------------------------------------------

struct ReferenceInfo
{
	string label;
	string path;

	map<string, SelectionRange> selectionRangesX;

	ReferenceInfo(const string &_l, const string &_p, const map<string, SelectionRange> &_sr) : label(_l), path(_p), selectionRangesX(_sr)
	{
	}
};

//----------------------------------------------------------------------------------------------------

void BuildHistogram(TGraph *g_input, double x_shift, const SelectionRange &range, TH1D *h)
{
	// erase previous content
	h->Reset();

	// determine bin range
	int bi_min = h->GetXaxis()->FindBin(range.min + x_shift) + 1;
	int bi_max = h->GetXaxis()->FindBin(range.max + x_shift) - 1;

	// build stats
	vector<Stat> stats(h->GetNbinsX(), Stat(1));
	double *xa = g_input->GetX();
	double *ya = g_input->GetY();
	for (int i = 0; i < g_input->GetN(); ++i)
	{
		// determine bin
		int bi = h->GetXaxis()->FindBin(xa[i] + x_shift);
		if (bi < bi_min || bi > bi_max)
			continue;

		// fill stats
		int idx = bi - 1;
		stats[idx].Fill(ya[i]);
	}

	// convert stats into histogram
	for (int bi = 1; bi <= h->GetNbinsX(); ++bi)
	{
		int idx = bi - 1;
		const Stat &stat = stats[idx];

		if (stat.GetEntries() < 10)
			continue;

		double N = stat.GetEntries();
		double N_unc = sqrt(N);

		h->SetBinContent(bi, N);
		h->SetBinError(bi, N_unc);
	}
}

//----------------------------------------------------------------------------------------------------

void BuildStdDevProfile(TGraph *g_input, double x_shift, const SelectionRange &range, TH1D *h)
{
	// erase previous content
	h->Reset();

	// determine bin range
	int bi_min = h->GetXaxis()->FindBin(range.min + x_shift) + 1;
	int bi_max = h->GetXaxis()->FindBin(range.max + x_shift) - 1;

	// build stats
	vector<Stat> stats(h->GetNbinsX(), Stat(1));
	double *xa = g_input->GetX();
	double *ya = g_input->GetY();
	for (int i = 0; i < g_input->GetN(); ++i)
	{
		// determine bin
		int bi = h->GetXaxis()->FindBin(xa[i] + x_shift);
		if (bi < bi_min || bi > bi_max)
			continue;

		// fill stats
		int idx = bi - 1;
		stats[idx].Fill(ya[i]);
	}

	// convert stats into histogram
	for (int bi = 1; bi <= h->GetNbinsX(); ++bi)
	{
		int idx = bi - 1;
		const Stat &stat = stats[idx];

		if (stat.GetEntries() < 10)
			continue;

		h->SetBinContent(bi, stat.GetStdDev(0));
		h->SetBinError(bi, stat.GetStdDevUnc(0));
	}
}

//----------------------------------------------------------------------------------------------------

void DoMatchMethodX(TGraph *g_test, const SelectionRange &r_test, TGraph *g_ref, const SelectionRange &r_ref)
{
	// prepare reference histogram
	TH1D *h_ref = new TH1D("h_ref", ";x", 140, 2., 16.);
	BuildHistogram(g_ref, 0., r_ref, h_ref);

	// book match-quality graphs
	TGraph *g_n_bins = new TGraph(); g_n_bins->SetName("g_n_bins"); g_n_bins->SetTitle(";sh;N");
	TGraph *g_chi_sq = new TGraph(); g_chi_sq->SetName("g_chi_sq"); g_chi_sq->SetTitle(";sh;S2");
	TGraph *g_chi_sq_norm = new TGraph(); g_chi_sq_norm->SetName("g_chi_sq_norm"); g_chi_sq_norm->SetTitle(";sh;S2 / N");

	// optimalisation variables
	double S2_norm_best = 1E100;
	double sh_best = 0.;	// mm

	// prepare container for test histograms
	TH1D *h_test = new TH1D(*h_ref);
	h_test->SetName("h_test");

	// loop over shifts
	double sh_min = -5.;	// mm
	double sh_max = 0.;		// mm
	double sh_step = 0.025;	// mm
	for (double sh = sh_min; sh <= sh_max; sh += sh_step)
	{
		// build test histogram
		BuildHistogram(g_test, sh, r_test, h_test);

		// determine normalisation
		int N_bins = 0;
		double S_test=0., S_ref=0.;

		for (int bi = 1; bi <= h_ref->GetNbinsX(); ++bi)
		{
			double v_ref = h_ref->GetBinContent(bi);
			double v_test = h_test->GetBinContent(bi);

			if (v_ref <= 0. || v_test <= 0.)
				continue;

			N_bins += 1;
			S_test += v_test;
			S_ref += v_ref;
		}

		if (N_bins < 20)
			continue;

		//printf("        sh=%.3f: N_bins=%u, S_test=%.3f, S_ref=%.3f\n", sh, N_bins, S_test, S_ref);

		// calculate chi^2
		N_bins = 0;
		double S2 = 0.;	

		for (int bi = 1; bi <= h_ref->GetNbinsX(); ++bi)
		{
			double v_ref = h_ref->GetBinContent(bi);
			double u_ref = h_ref->GetBinError(bi);

			double v_test = h_test->GetBinContent(bi);
			double u_test = h_test->GetBinError(bi);

			if (v_ref <= 0. || v_test <= 0.)
				continue;

			v_ref /= S_ref;
			u_ref /= S_ref;

			v_test /= S_test;
			u_test /= S_test;

			double diff = v_test - v_ref;
			double diff_unc_sq = u_ref*u_ref + u_test*u_test;

			N_bins++;
			S2 += diff*diff / diff_unc_sq;
		}

		double S2_norm = S2 / N_bins;

		if (S2_norm < S2_norm_best)
		{
			S2_norm_best = S2_norm;
			sh_best = sh;
		}

		// fill graphs
		int idx = g_n_bins->GetN();
		g_n_bins->SetPoint(idx, sh, N_bins);
		g_chi_sq->SetPoint(idx, sh, S2);
		g_chi_sq_norm->SetPoint(idx, sh, S2_norm);
	}

	// determine uncertainty
	double fit_range = 0.5;	// mm
	g_chi_sq->Fit(ff_pol2, "Q", "", sh_best - fit_range, sh_best + fit_range);

	double sh_best_unc = 1. / sqrt(ff_pol2->GetParameter(2));

	// print results
	printf("        sh_best = (%.3f +- %.3f) mm\n", sh_best, sh_best_unc);

	// save histograms
	BuildHistogram(g_test, 0., r_test, h_test);

	TH1D *h_test_aft = new TH1D(*h_test);
	BuildHistogram(g_test, sh_best, r_test, h_test_aft);

	double S_test=0., S_ref=0.;
	for (int bi = 1; bi <= h_ref->GetNbinsX(); ++bi)
	{
		double v_ref = h_ref->GetBinContent(bi);
		double v_test = h_test_aft->GetBinContent(bi);

		if (v_ref <= 0. || v_test <= 0.)
			continue;

		S_test += v_test;
		S_ref += v_ref;
	}

	h_ref->Scale(1./S_ref);
	h_test->Scale(1./S_test);
	h_test_aft->Scale(1./S_test);

	TCanvas *c_cmp = new TCanvas("c_cmp");
	h_ref->SetLineColor(1); h_ref->SetName("h_ref_sel"); h_ref->Draw("");
	h_test->SetLineColor(6); h_test->SetName("h_test_bef"); h_test->Draw("same");
	h_test_aft->SetLineColor(2); h_test_aft->SetName("h_test_aft"); h_test_aft->Draw("same");
	c_cmp->Write();

	// save graphs
	g_n_bins->Write();
	g_chi_sq->Write();
	g_chi_sq_norm->Write();

	// save results	
	TGraph *g_results = new TGraph();
	g_results->SetName("g_results");
	g_results->SetPoint(0, 0, sh_best);
	g_results->SetPoint(1, 0, sh_best_unc);
	g_results->Write();

	// cleaning
	delete h_ref;
	delete h_test;
	delete h_test_aft;
	delete c_cmp;
}

//----------------------------------------------------------------------------------------------------

void DoMatchMethodY(TGraph *g_test, const SelectionRange &r_test, TGraph *g_ref, const SelectionRange &r_ref)
{
	// prepare reference histogram
	TH1D *h_ref = new TH1D("h_ref", ";x", 140, 2., 16.);
	BuildStdDevProfile(g_ref, 0., r_ref, h_ref);

	// book match-quality graphs
	TGraph *g_n_bins = new TGraph(); g_n_bins->SetName("g_n_bins"); g_n_bins->SetTitle(";sh;N");
	TGraph *g_chi_sq = new TGraph(); g_chi_sq->SetName("g_chi_sq"); g_chi_sq->SetTitle(";sh;S2");
	TGraph *g_chi_sq_norm = new TGraph(); g_chi_sq_norm->SetName("g_chi_sq_norm"); g_chi_sq_norm->SetTitle(";sh;S2 / N");

	// optimalisation variables
	double S2_norm_best = 1E100;
	double sh_best = 0.;	// mm

	// prepare container for test histograms
	TH1D *h_test = new TH1D(*h_ref);
	h_test->SetName("h_test");

	// loop over shifts
	double sh_min = -5.;	// mm
	double sh_max = 0.;		// mm
	double sh_step = 0.025;	// mm
	for (double sh = sh_min; sh <= sh_max; sh += sh_step)
	{
		// build test histogram
		BuildStdDevProfile(g_test, sh, r_test, h_test);

		// calculate chi^2
		int N_bins = 0;
		double S2 = 0.;	

		for (int bi = 1; bi <= h_ref->GetNbinsX(); ++bi)
		{
			double v_ref = h_ref->GetBinContent(bi);
			double u_ref = h_ref->GetBinError(bi);

			double v_test = h_test->GetBinContent(bi);
			double u_test = h_test->GetBinError(bi);

			if (v_ref <= 0. || v_test <= 0.)
				continue;

			double diff = v_test - v_ref;
			double diff_unc_sq = u_ref*u_ref + u_test*u_test;

			N_bins++;
			S2 += diff*diff / diff_unc_sq;
		}

		if (N_bins < 3)
			continue;

		double S2_norm = S2 / N_bins;

		if (S2_norm < S2_norm_best)
		{
			S2_norm_best = S2_norm;
			sh_best = sh;
		}

		int idx = g_n_bins->GetN();
		g_n_bins->SetPoint(idx, sh, N_bins);
		g_chi_sq->SetPoint(idx, sh, S2);
		g_chi_sq_norm->SetPoint(idx, sh, S2_norm);
	}

	// determine uncertainty
	double fit_range = 0.5;	// mm
	g_chi_sq->Fit(ff_pol2, "Q", "", sh_best - fit_range, sh_best + fit_range);

	double sh_best_unc = 1. / sqrt(ff_pol2->GetParameter(2));

	// print results
	printf("        sh_best = (%.3f +- %.3f) mm\n", sh_best, sh_best_unc);

	// save histograms
	TCanvas *c_cmp = new TCanvas("c_cmp");
	h_ref->SetLineColor(1); h_ref->SetName("h_ref_sel"); h_ref->Draw("");

	BuildStdDevProfile(g_test, 0., r_test, h_test);
	h_test->SetLineColor(4); h_test->SetName("h_test_bef"); h_test->Draw("same");

	TH1D *h_test_aft = new TH1D(*h_test);
	BuildStdDevProfile(g_test, sh_best, r_test, h_test_aft);
	h_test_aft->SetLineColor(2); h_test_aft->SetName("h_test_aft"); h_test_aft->Draw("same");
	
	c_cmp->Write();

	// save graphs
	g_n_bins->Write();
	g_chi_sq->Write();
	g_chi_sq_norm->Write();

	// save results	
	TGraph *g_results = new TGraph();
	g_results->SetName("g_results");
	g_results->SetPoint(0, 0, sh_best);
	g_results->SetPoint(1, 0, sh_best_unc);
	g_results->Write();

	// cleaning
	delete h_ref;
	delete h_test;
	delete h_test_aft;
	delete c_cmp;
}

//----------------------------------------------------------------------------------------------------

void DoMatch(TGraph *g_test, const SelectionRange &r_test, TGraph *g_ref, const SelectionRange &r_ref)
{
	TDirectory *d_top = gDirectory;

	gDirectory = d_top->mkdir("method y");
	printf("    method y\n");
	DoMatchMethodY(g_test, r_test, g_ref, r_ref);

	gDirectory = d_top->mkdir("method x");
	printf("    method x\n");
	DoMatchMethodX(g_test, r_test, g_ref, r_ref);

	gDirectory = d_top;
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

int main()
{
	// apply settings
	ApplySettings();

	// list of RPs
	vector<string> rps = {
		"L_1_F",
		"L_1_N",
		"R_1_N",
		"R_1_F"
	};

	// list of references
	vector<ReferenceInfo> refInfo;
	map<string, SelectionRange> ref_ranges;

	ref_ranges.clear();
	ref_ranges["L_1_F"] = SelectionRange(3.7, 11.0);
	ref_ranges["L_1_N"] = SelectionRange(3.8, 10.5);
	ref_ranges["R_1_N"] = SelectionRange(2.5, 10.0);
	ref_ranges["R_1_F"] = SelectionRange(3.0,  9.5);
	refInfo.push_back(ReferenceInfo("10077", "/afs/cern.ch/work/j/jkaspar/analyses/ctpps/alignment/run_alignment/10077/", ref_ranges));

	ref_ranges.clear();
	ref_ranges["L_1_F"] = SelectionRange(4.0, 11.0);
	ref_ranges["L_1_N"] = SelectionRange(4.4, 11.0);
	ref_ranges["R_1_N"] = SelectionRange(3.0, 10.0);
	ref_ranges["R_1_F"] = SelectionRange(3.0, 10.0);
	refInfo.push_back(ReferenceInfo("10079", "/afs/cern.ch/work/j/jkaspar/analyses/ctpps/alignment/run_alignment/10079/", ref_ranges));

	ref_ranges.clear();
	ref_ranges["L_1_F"] = SelectionRange(4.0, 11.0);
	ref_ranges["L_1_N"] = SelectionRange(4.4, 11.0);
	ref_ranges["R_1_N"] = SelectionRange(3.0, 10.0);
	ref_ranges["R_1_F"] = SelectionRange(3.0, 10.0);
	refInfo.push_back(ReferenceInfo("10081", "/afs/cern.ch/work/j/jkaspar/analyses/ctpps/alignment/run_alignment/10081/", ref_ranges));

	/*
	ref_ranges.clear();
	ref_ranges["L_1_F"] = SelectionRange(9.2, 15.5);
	ref_ranges["L_1_N"] = SelectionRange(7.2, 13.4);
	ref_ranges["R_1_N"] = SelectionRange(7.2, 15.0);
	ref_ranges["R_1_F"] = SelectionRange(6.4, 14.0);
	refInfo.push_back(ReferenceInfo("274241", "/afs/cern.ch/work/j/jkaspar/analyses/ctpps/alignment/run_physics_margin/274241", ref_ranges));
	*/

	// get input
	TFile *f_in = new TFile("distributions.root");

	// ouput file
	TFile *f_out = new TFile("match.root", "recreate");

	// processing per rp
	for (const auto &rp : rps)
	{
		printf("-------------------- %s --------------------\n", rp.c_str());

		TDirectory *rp_dir = f_out->mkdir(rp.c_str());
		TGraph *g_test = (TGraph *) f_in->Get(("after selection/g_y_vs_x_" + rp + "_sel").c_str());

		auto it_test = selectionRangesX.find(rp);
		if (it_test == selectionRangesX.end())
		{
			printf("ERROR: can't find RP in selectionRangesX.\n");
			continue;
		}

		for (const auto &ref : refInfo)
		{
			printf("* %s\n", ref.label.c_str());
		
			TDirectory *ref_dir = rp_dir->mkdir(ref.label.c_str());
		
			TFile *f_ref = TFile::Open((ref.path + "/distributions.root").c_str());
			TGraph *g_ref = (TGraph *) f_ref->Get(("after selection/g_y_vs_x_" + rp + "_sel").c_str());

			auto it_ref = ref.selectionRangesX.find(rp);
			if (it_ref == ref.selectionRangesX.end())
			{
				printf("ERROR: can't find RP in ref.selectionRangesX.\n");
				continue;
			}

			gDirectory = ref_dir;
			DoMatch(g_test, it_test->second, g_ref, it_ref->second);

			delete f_ref;
		}
	}

	// clean up
	delete f_out;
	return 0;
}
