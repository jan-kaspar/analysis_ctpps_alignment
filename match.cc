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

void DoMatch(TH1D *h_test, const SelectionRange &r_test, TH1D *h_ref, const SelectionRange &r_ref)
{
	// determine (local) bin ranges
	int bi_test_min = h_test->GetXaxis()->FindBin(r_test.min);
	int bi_test_max = h_test->GetXaxis()->FindBin(r_test.max);
	printf("    bin range test: %i to %i\n", bi_test_min, bi_test_max);

	int bi_ref_min = h_ref->GetXaxis()->FindBin(r_ref.min);
	int bi_ref_max = h_ref->GetXaxis()->FindBin(r_ref.max);
	printf("    bin range ref: %i to %i\n", bi_ref_min, bi_ref_max);

	// book match-quality graphs
	TGraph *g_n_bins = new TGraph(); g_n_bins->SetName("g_n_bins"); g_n_bins->SetTitle(";sh;N");
	TGraph *g_chi_sq = new TGraph(); g_chi_sq->SetName("g_chi_sq"); g_chi_sq->SetTitle(";sh;S2");
	TGraph *g_chi_sq_norm = new TGraph(); g_chi_sq_norm->SetName("g_chi_sq_norm"); g_chi_sq_norm->SetTitle(";sh;S2 / N");

	// determine step
	double step = h_test->GetBinWidth(1);

	// shift range
	int sh_min = -5. / step;
	int sh_max = +5. / step;

	double S2_norm_best = 1E1000;
	int sh_best = 0;

	for (int sh = sh_min; sh <= sh_max; ++sh)
	{
		//printf("    sh = %i\n", sh);

		// determine available (overlapping) global bin range
		int bi_gl_min = max(bi_ref_min, bi_test_min+sh);
		int bi_gl_max = min(bi_ref_max, bi_test_max+sh);

		//printf("        global bin range: %i to %i\n", bi_gl_min, bi_gl_max);

		// check number of overlapping bins
		int bins = (bi_gl_max - bi_gl_min);

		if (bins < 3)
			continue;

		double S2 = 0.;
		for (int bi = bi_gl_min; bi <= bi_gl_max; ++bi)
		{
			// local bin indeces
			int bi_ref = bi;
			int bi_test = bi - sh;

			double v_ref = h_ref->GetBinContent(bi_ref);
			double u_ref = h_ref->GetBinError(bi_ref);

			double v_test = h_test->GetBinContent(bi_test);
			double u_test = h_test->GetBinError(bi_test);

			double diff = v_test - v_ref;
			double diff_unc_sq = u_ref*u_ref + u_test*u_test;

			S2 += diff*diff / diff_unc_sq;
		}

		double S2_norm = S2 / bins;

		if (S2_norm < S2_norm_best)
		{
			S2_norm_best = S2_norm;
			sh_best = sh;
		}

		int idx = g_n_bins->GetN();
		g_n_bins->SetPoint(idx, sh, bins);
		g_chi_sq->SetPoint(idx, sh, S2);
		g_chi_sq_norm->SetPoint(idx, sh, S2_norm);
	}

	// determine uncertainty
	double fit_range = 30;	// steps
	g_chi_sq->Fit(ff_pol2, "Q", "", sh_best - fit_range, sh_best + fit_range);

	double sh_best_unc = 1. / sqrt(ff_pol2->GetParameter(2));

	// print results
	printf("    sh_best = %i +- %.1f\n", sh_best, sh_best_unc);

	// save graphs
	g_n_bins->Write();
	g_chi_sq->Write();
	g_chi_sq_norm->Write();

	// save histos
	TH1D *h_ref_sel = new TH1D(*h_ref); h_ref_sel->SetName("h_ref_sel"); h_ref_sel->SetLineColor(1);
	
	for (int bi = 1; bi <= h_ref_sel->GetNbinsX(); ++bi)
	{
		if (bi < bi_ref_min || bi > bi_ref_max)
		{
			h_ref_sel->SetBinContent(bi, 0.);
			h_ref_sel->SetBinError(bi, 0.);
		}
	}
	
	TH1D *h_test_bef = new TH1D(*h_test); h_test_bef->SetName("h_test_bef"); h_test_bef->SetLineColor(2);
	TH1D *h_test_aft = new TH1D(*h_test); h_test_aft->SetName("h_test_aft"); h_test_aft->SetLineColor(4); h_test_aft->Reset();

	for (int bi = 1; bi <= h_test->GetNbinsX(); ++bi)
	{
		if (bi < bi_test_min || bi > bi_test_max)
		{
			h_test_bef->SetBinContent(bi, 0.);
			h_test_bef->SetBinError(bi, 0.);
		} else {
			h_test_aft->SetBinContent(bi + sh_best, h_test->GetBinContent(bi));
			h_test_aft->SetBinError(bi + sh_best, h_test->GetBinError(bi));
		}
	}

	TCanvas *h_cmp = new TCanvas("h_cmp");
	h_ref_sel->Draw("");
	h_test_bef->Draw("same");
	h_test_aft->Draw("same");
	h_cmp->Write();

	TGraph *g_results = new TGraph();
	g_results->SetName("g_results");
	g_results->SetPoint(0, 0, sh_best);
	g_results->SetPoint(1, 0, sh_best * step);
	g_results->SetPoint(2, 0, sh_best_unc);
	g_results->SetPoint(3, 0, sh_best_unc * step);
	g_results->Write();
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
		TH1D *h_test = (TH1D *) f_in->Get(("profiles/" + rp + "/h_stddev").c_str());

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
			TH1D *h_ref = (TH1D *) f_ref->Get(("profiles/" + rp + "/h_stddev").c_str());

			auto it_ref = ref.selectionRangesX.find(rp);
			if (it_ref == ref.selectionRangesX.end())
			{
				printf("ERROR: can't find RP in ref.selectionRangesX.\n");
				continue;
			}

			gDirectory = ref_dir;
			DoMatch(h_test, it_test->second, h_ref, it_ref->second);

			delete f_ref;
		}
	}

	// clean up
	delete f_out;
	return 0;
}
