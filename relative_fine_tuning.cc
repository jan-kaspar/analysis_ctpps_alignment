#include "common.h"

#include "shared_track.h"
#include "shared_alignment.h"
#include "shared_reconstruction.h"
#include "shared_fill_info.h"

#include "input_files.h"
#include "parameters.h"

#include "stat.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"
#include "DataFormats/CTPPSReco/interface/TotemRPUVPattern.h"
#include "DataFormats/CTPPSReco/interface/TotemRPCluster.h"
#include "DataFormats/TotemDigi/interface/TotemRPDigi.h"
#include "DataFormats/TotemDigi/interface/TotemVFATStatus.h"
#include "DataFormats/TotemRPDetId/interface/TotemRPDetId.h"

#include <vector>
#include <string>

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------

void WriteCutPlot(TH2D *h, double a, double c, double tol, const string &label)
{
	TCanvas *canvas = new TCanvas();
	canvas->SetName(label.c_str());
	canvas->SetLogz(1);

	h->Draw("colz");

	double x_min = -30.;
	double x_max = +30.;

	TGraph *g_up = new TGraph();
	g_up->SetName("g_up");
	g_up->SetPoint(0, x_min, -a*x_min - c + tol);
	g_up->SetPoint(1, x_max, -a*x_max - c + tol);
	g_up->SetLineColor(1);
	g_up->Draw("l");

	TGraph *g_down = new TGraph();
	g_down->SetName("g_down");
	g_down->SetPoint(0, x_min, -a*x_min - c - tol);
	g_down->SetPoint(1, x_max, -a*x_max - c - tol);
	g_down->SetLineColor(1);
	g_down->Draw("l");

	canvas->Write();
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

int main()
{
	// apply settings
	ApplySettings();

	InitReconstruction();

	const string alignmentMethodTag = "method x";

	// get fill info
	InitFillInfoCollection();
	const auto &fillInfo = fillInfoCollection.FindByFill(lhc_fill, rps_have_margin);

	//  load alignment
	AlignmentResultsCollection alignmentCollection;
	alignmentCollection.Load("../../shared_alignment/data/collect_alignments_2017_01_17.out");

	printf("* alignment tag: %s\n", fillInfo.alignmentTag.c_str());

	const auto alignments = alignmentCollection.find(fillInfo.alignmentTag)->second;

	// override cut settings
	map<unsigned int, double> cut_n_si;
	cut_n_si[1] = 5.;
	cut_n_si[2] = 5.;
	cut_n_si[3] = 10.;
	cut_n_si[4] = 10.;

	// get input
	InitInputFiles();
	fwlite::ChainEvent ev(input_files);

	printf("* events in input chain: %llu\n", ev.size());

	// ouput file
	TFile *f_out = new TFile("relative_fine_tuning.root", "recreate");

	// book cut histograms
	map<unsigned int, TH1D *> cut_h_before, cut_h_after;
	map<unsigned int, TH2D *> cut_h2_before, cut_h2_after;
	for (unsigned int i = 1; i <= 4; ++i)
	{
		double x_min, x_max, y_min, y_max, q_min, q_max;
		if (i == 1) { x_min = 0.; x_max = 30. ; y_min = 0.; y_max = 30.; q_min = -2.; q_max = +2.; }
		if (i == 2) { x_min = 0.; x_max = 30. ; y_min = 0.; y_max = 30.; q_min = -2.; q_max = +2.; }
		if (i == 3) { x_min = -10.; x_max = 10. ; y_min = -10.; y_max = 10.; q_min = -2.; q_max = +2.; }
		if (i == 4) { x_min = -10.; x_max = 10. ; y_min = -10.; y_max = 10.; q_min = -2.; q_max = +2.; }

		unsigned n_bins_1D = 100, n_bins_2D = 100;

		cut_h_before[i] = new TH1D("", "", n_bins_1D, q_min, q_max);
		cut_h_after[i] = new TH1D("", "", n_bins_1D, q_min, q_max);

		cut_h2_before[i] = new TH2D("", "", n_bins_2D, x_min, x_max, n_bins_2D, y_min, y_max);
		cut_h2_after[i] = new TH2D("", "", n_bins_2D, x_min, x_max, n_bins_2D, y_min, y_max);
	}

	// book histograms
	TProfile *p_x_L_diffFN_vs_x_L_N = new TProfile("p_x_L_diffFN_vs_x_L_N", ";x_{LN};x_{LF} - x_{LN}", 100, 0., 20.);
	TProfile *p_x_R_diffFN_vs_x_R_N = new TProfile("p_x_R_diffFN_vs_x_R_N", ";x_{RN};x_{RF} - x_{RN}", 100, 0., 20.);

	// loop over the chain entries
	unsigned long int ev_count = 0;
	unsigned long int tr_sel_count = 0;
	for (ev.toBegin(); ! ev.atEnd(); ++ev)
	{
		ev_count++;

		// TODO: comment out
		if (tr_sel_count > 1000000)
			break;

		// get track data
		fwlite::Handle< DetSetVector<TotemRPLocalTrack> > tracks;
		tracks.getByLabel(ev, "totemRPLocalTrackFitter");

		TrackDataCollection tr;

		for (const auto &ds : *tracks)
		{
			const auto &rpId = ds.detId();

			for (const auto &track : ds)
			{
				if (rpId == 2 || rpId == 3)
					tr[rpId] = track;

				if (rpId == 102 || rpId == 103)
					tr[rpId] = track;
			}
		}

		// cut evaluation
		map<unsigned int, double> cut_q, cut_X, cut_Y;
		map<unsigned int, bool> cut_val;

		for (unsigned int i = 1; i <= 4; ++i)
		{
			double Y, X, a, c, si;
			if (i == 1) { Y = tr[3].x; X = tr[2].x; a = cut1_a; c = cut1_c; si = cut1_si; }
			if (i == 2) { Y = tr[103].x; X = tr[102].x; a = cut2_a; c = cut2_c; si = cut2_si; }
			if (i == 3) { Y = tr[3].y; X = tr[2].y; a = cut3_a; c = cut3_c; si = cut3_si; }
			if (i == 4) { Y = tr[103].y; X = tr[102].y; a = cut4_a; c = cut4_c; si = cut4_si; }

			cut_X[i] = X;
			cut_Y[i] = Y;

			cut_q[i] = Y + a*X + c;
			cut_val[i] = (fabs(cut_q[i]) < cut_n_si[i] * si);
		}

		bool all_tracks_L = tr[2].valid && tr[3].valid;
		bool all_tracks_R = tr[102].valid && tr[103].valid;

		bool cuts_L = all_tracks_L && (!cut1_apply || cut_val[1]) && (!cut3_apply || cut_val[3]);
		bool cuts_R = all_tracks_R && (!cut2_apply || cut_val[2]) && (!cut4_apply || cut_val[4]);

		for (unsigned int i = 1; i <= 4; ++i)
		{
			bool all_tracks, all_cuts;
			if (i == 1 || i == 3) { all_tracks = all_tracks_L; all_cuts = cuts_L; }
			if (i == 2 || i == 4) { all_tracks = all_tracks_R; all_cuts = cuts_R; }

			if (all_tracks)
			{
				cut_h_before[i]->Fill(cut_q[i]);
				cut_h2_before[i]->Fill(cut_X[i], cut_Y[i]);
			}

			if (all_cuts)
			{
				cut_h_after[i]->Fill(cut_q[i]);
				cut_h2_after[i]->Fill(cut_X[i], cut_Y[i]);
			}
		}

		// increase counter
		if (cuts_L || cuts_R)
			tr_sel_count++;

		// apply alignments
		TrackDataCollection tr_al;
		tr_al = alignments.Apply(tr);

		// make plots
		if (tr_al[2].valid && tr_al[3].valid)
		{
			const double &x_N = tr_al[2].x;
			const double &x_F = tr_al[3].x;

			p_x_L_diffFN_vs_x_L_N->Fill(x_N, x_F - x_N);
		}

		if (tr_al[102].valid && tr_al[103].valid)
		{
			const double &x_N = tr_al[102].x;
			const double &x_F = tr_al[103].x;

			p_x_R_diffFN_vs_x_R_N->Fill(x_N, x_F - x_N);
		}
	}

	printf("* events processed: %lu\n", ev_count);
	printf("* tracks selected: %lu\n", tr_sel_count);

	// make fits
	TF1 *ff = new TF1("ff", "[0] + [1]*x");
	TF1 *ff_sl_fixed = new TF1("ff_sl_fixed", "[0] + [1]*x");

	double x_min, x_max, slope_fix;
   
	x_min = yAlignmentRangeX["L_1_N"].min; x_max = 9., slope_fix = -0.0083;

	printf("\n>> arm L: fit with slope free\n");
	printf("    x_min = %.3f, x_max = %.3f\n", x_min, x_max);
	ff->SetParameters(0., 0.);
	p_x_L_diffFN_vs_x_L_N->Fit(ff, "", "", x_min, x_max);

	printf("\n>> arm L: fit with slope fixed\n");
	printf("    x_min = %.3f, x_max = %.3f, slope_fix = %.4f\n", x_min, x_max, slope_fix);
	ff_sl_fixed->SetParameters(0., slope_fix);
	ff_sl_fixed->FixParameter(1, slope_fix);
	p_x_L_diffFN_vs_x_L_N->Fit(ff_sl_fixed, "+", "", x_min, x_max);

	const double De_L = ff_sl_fixed->GetParameter(0);
   
	x_min = yAlignmentRangeX["R_1_N"].min; x_max = 6., slope_fix = -0.0515;

	printf("\n>> arm L: fit with slope free\n");
	printf("    x_min = %.3f, x_max = %.3f\n", x_min, x_max);
	ff->SetParameters(0., 0.);
	p_x_R_diffFN_vs_x_R_N->Fit(ff, "", "", x_min, x_max);

	printf("\n>> arm L: fit with slope fixed\n");
	printf("    x_min = %.3f, x_max = %.3f, slope_fix = %.4f\n", x_min, x_max, slope_fix);
	ff_sl_fixed->SetParameters(0., slope_fix);
	ff_sl_fixed->FixParameter(1, slope_fix);
	p_x_R_diffFN_vs_x_R_N->Fit(ff_sl_fixed, "+", "", x_min, x_max);

	const double De_R = ff_sl_fixed->GetParameter(0);

	// save plots
	TDirectory *d_cuts = f_out->mkdir("cuts");
	for (unsigned int i = 1; i <= 4; ++i)
	{
		char buf[100];
		sprintf(buf, "cut %u", i);
		gDirectory = d_cuts->mkdir(buf);

		cut_h_before[i]->Write("h_before");
		cut_h_after[i]->Write("h_after");

		double a=0., c=0., si=0.;
		if (i == 1) { a = cut1_a; c = cut1_c; si = cut1_si; }
		if (i == 2) { a = cut2_a; c = cut2_c; si = cut2_si; }
		if (i == 3) { a = cut3_a; c = cut3_c; si = cut3_si; }
		if (i == 4) { a = cut4_a; c = cut4_c; si = cut4_si; }

		WriteCutPlot(cut_h2_before[i], a, c, cut_n_si[i] * si, "canvas_before");
		WriteCutPlot(cut_h2_after[i], a, c, cut_n_si[i] * si, "canvas_after");
	}

	gDirectory = f_out;
	p_x_L_diffFN_vs_x_L_N->Write();
	p_x_R_diffFN_vs_x_R_N->Write();

	// save results
	AlignmentResults alignments_out = alignments;
	alignments_out[3].sh_x -= De_L / 2.;
	alignments_out[2].sh_x += De_L / 2.;
	alignments_out[103].sh_x -= De_R / 2.;
	alignments_out[102].sh_x += De_R / 2.;

	AlignmentResultsCollection al_coll_out;
	al_coll_out["relative_fine_tuning"] = alignments_out;
	al_coll_out.Write("relative_fine_tuning.out");

	// clean up
	delete f_out;
	return 0;
}
