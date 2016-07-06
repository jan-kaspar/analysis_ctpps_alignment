#include "input_files.h"
#include "common.h"
#include "parameters.h"

#include "stat.h"

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"

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

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------

struct Profile
{
	string name;
	TH2D *h;

	vector<Stat> st;

	Profile(const string &_name, TH2D *_h) : name(_name), h(_h), st(h->GetNbinsX(), Stat(1))
	{
	}

	void Fill(double x, double y)
	{
		int bi = h->GetXaxis()->FindBin(x);

		if (bi < 1 || bi > h->GetNbinsX())
			return;

		int vi = bi - 1;
		st[vi].Fill(y);
	}

	void Write()
	{
		TDirectory *dir_top = gDirectory;	
		gDirectory = gDirectory->mkdir(name.c_str());

		int bins = h->GetXaxis()->GetNbins();
		double x_min = h->GetXaxis()->GetXmin();
		double x_max = h->GetXaxis()->GetXmax();

		TH1D *h_entries = new TH1D("h_entries", ";x", bins, x_min, x_max);
		TH1D *h_mean = new TH1D("h_mean", ";x", bins, x_min, x_max);
		TH1D *h_stddev = new TH1D("h_stddev", ";x", bins, x_min, x_max);

		for (int bi = 1; bi <= bins; ++bi)
		{
			int vi = bi - 1;

			h_entries->SetBinContent(bi, st[vi].GetEntries());

			h_mean->SetBinContent(bi, st[vi].GetMean(0));
			h_mean->SetBinError(bi, st[vi].GetMeanUnc(0));

			h_stddev->SetBinContent(bi, st[vi].GetStdDev(0));
			h_stddev->SetBinError(bi, st[vi].GetStdDevUncGauss(0));
		}

		h_entries->Write();
		h_mean->Write();
		h_stddev->Write();

		gDirectory = dir_top;
	}
};

//----------------------------------------------------------------------------------------------------

struct TrackData
{
	bool valid = false;
	double x = 0.;
	double y = 0.;

	void operator= (const TotemRPLocalTrack &ft)
	{
		valid = ft.isValid();
		x = ft.getX0();
		y = ft.getY0();
	}
};

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

int main()
{
	// apply settings
	ApplySettings();

	// get input
	InitInputFiles();
	fwlite::ChainEvent ev(input_files);

	// ouput file
	TFile *f_out = new TFile("distributions.root", "recreate");

	// book histograms
	TH2D *h2_y_vs_x_L_1_F_no_sel = new TH2D("h2_y_vs_x_L_1_F_no_sel", ";x;y", 100, 0., 20., 100, -15., +15.);
	TH2D *h2_y_vs_x_L_1_N_no_sel = new TH2D("h2_y_vs_x_L_1_N_no_sel", ";x;y", 100, 0., 20., 100, -15., +15.);
	TH2D *h2_y_vs_x_R_1_N_no_sel = new TH2D("h2_y_vs_x_R_1_N_no_sel", ";x;y", 100, 0., 20., 100, -15., +15.);
	TH2D *h2_y_vs_x_R_1_F_no_sel = new TH2D("h2_y_vs_x_R_1_F_no_sel", ";x;y", 100, 0., 20., 100, -15., +15.);

	TH2D *h2_x_L_1_F_vs_x_L_1_N = new TH2D("h2_x_L_1_F_vs_x_L_1_N", ";x_L_1_N;x_L_1_F", 100, 0., 20., 100, 0., 20.);
	TProfile *p_x_L_1_F_vs_x_L_1_N = new TProfile("p_x_L_1_F_vs_x_L_1_N", ";x_L_1_N;x_L_1_F", 100, 0., 20.);
	TH1D *h_q_cut1 = new TH1D("h_q_cut1", ";cq1", 100, -2., 2.);

	TH2D *h2_x_R_1_F_vs_x_R_1_N = new TH2D("h2_x_R_1_F_vs_x_R_1_N", ";x_R_1_N;x_R_1_F", 100, 0., 20., 100, 0., 20.);
	TProfile *p_x_R_1_F_vs_x_R_1_N = new TProfile("p_x_R_1_F_vs_x_R_1_N", ";x_R_1_N;x_R_1_F", 100, 0., 20.);
	TH1D *h_q_cut2 = new TH1D("h_q_cut2", ";cq2", 100, -2., 2.);

	TH2D *h2_y_vs_x_L_1_F_sel = new TH2D("h2_y_vs_x_L_1_F_sel", ";x;y", 800, 0., 20., 100, -15., +15.);
	TH2D *h2_y_vs_x_L_1_N_sel = new TH2D("h2_y_vs_x_L_1_N_sel", ";x;y", 800, 0., 20., 100, -15., +15.);
	TH2D *h2_y_vs_x_R_1_N_sel = new TH2D("h2_y_vs_x_R_1_N_sel", ";x;y", 800, 0., 20., 100, -15., +15.);
	TH2D *h2_y_vs_x_R_1_F_sel = new TH2D("h2_y_vs_x_R_1_F_sel", ";x;y", 800, 0., 20., 100, -15., +15.);

	Profile p_y_vs_x_L_1_F_sel("L_1_F", h2_y_vs_x_L_1_F_sel);
	Profile p_y_vs_x_L_1_N_sel("L_1_N", h2_y_vs_x_L_1_N_sel);
	Profile p_y_vs_x_R_1_N_sel("R_1_N", h2_y_vs_x_R_1_N_sel);
	Profile p_y_vs_x_R_1_F_sel("R_1_F", h2_y_vs_x_R_1_F_sel);

	// loop over the chain entries
	unsigned int ev_count = 0;
	for (ev.toBegin(); ! ev.atEnd(); ++ev)
	{
		/*
		if (ev_count > 1000000)
			break;
		ev_count++;
		*/

		// default track data
		TrackData tr_L_1_F;
		TrackData tr_L_1_N;
		TrackData tr_R_1_N;
		TrackData tr_R_1_F;

		// get track data
		fwlite::Handle< DetSetVector<TotemRPLocalTrack> > tracks;
		tracks.getByLabel(ev, "totemRPLocalTrackFitter");

		for (const auto &ds : *tracks)
		{
			const auto &rpId = ds.detId();

			for (const auto &tr : ds)
			{
				if (rpId == 3)
					tr_L_1_F = tr;
				if (rpId == 2)
					tr_L_1_N = tr;
				if (rpId == 102)
					tr_R_1_N = tr;
				if (rpId == 103)
					tr_R_1_F = tr;
			}
		}

		// process track data
		if (tr_L_1_F.valid)
			h2_y_vs_x_L_1_F_no_sel->Fill(tr_L_1_F.x, tr_L_1_F.y);
		if (tr_L_1_N.valid)
			h2_y_vs_x_L_1_N_no_sel->Fill(tr_L_1_N.x, tr_L_1_N.y);
		if (tr_R_1_N.valid)
			h2_y_vs_x_R_1_N_no_sel->Fill(tr_R_1_N.x, tr_R_1_N.y);
		if (tr_R_1_F.valid)
			h2_y_vs_x_R_1_F_no_sel->Fill(tr_R_1_F.x, tr_R_1_F.y);

		if (tr_L_1_N.valid && tr_L_1_F.valid)
		{
			h2_x_L_1_F_vs_x_L_1_N->Fill(tr_L_1_N.x, tr_L_1_F.x);
			p_x_L_1_F_vs_x_L_1_N->Fill(tr_L_1_N.x, tr_L_1_F.x);

			double cq = tr_L_1_F.x + cut1_a * tr_L_1_N.x + cut1_c;
			h_q_cut1->Fill(cq);

			if (fabs(cq) < n_si * cut1_si)
			{
				h2_y_vs_x_L_1_N_sel->Fill(tr_L_1_N.x, tr_L_1_N.y);
				h2_y_vs_x_L_1_F_sel->Fill(tr_L_1_F.x, tr_L_1_F.y);

				p_y_vs_x_L_1_N_sel.Fill(tr_L_1_N.x, tr_L_1_N.y);
				p_y_vs_x_L_1_F_sel.Fill(tr_L_1_F.x, tr_L_1_F.y);
			}
		}

		if (tr_R_1_N.valid && tr_R_1_F.valid)
		{
			h2_x_R_1_F_vs_x_R_1_N->Fill(tr_R_1_N.x, tr_R_1_F.x);
			p_x_R_1_F_vs_x_R_1_N->Fill(tr_R_1_N.x, tr_R_1_F.x);

			double cq = tr_R_1_F.x + cut2_a * tr_R_1_N.x + cut2_c;
			h_q_cut2->Fill(cq);

			if (fabs(cq) < n_si * cut2_si)
			{
				h2_y_vs_x_R_1_N_sel->Fill(tr_R_1_N.x, tr_R_1_N.y);
				h2_y_vs_x_R_1_F_sel->Fill(tr_R_1_F.x, tr_R_1_F.y);

				p_y_vs_x_R_1_N_sel.Fill(tr_R_1_N.x, tr_R_1_N.y);
				p_y_vs_x_R_1_F_sel.Fill(tr_R_1_F.x, tr_R_1_F.y);
			}
		}
	}

	// save histograms
	gDirectory = f_out;

	// save histograms
	gDirectory = f_out->mkdir("before selection");
	h2_y_vs_x_L_1_F_no_sel->Write();
	h2_y_vs_x_L_1_N_no_sel->Write();
	h2_y_vs_x_R_1_N_no_sel->Write();
	h2_y_vs_x_R_1_F_no_sel->Write();

	TDirectory *d_cuts = f_out->mkdir("cuts");
	
	gDirectory = d_cuts->mkdir("cut 1");
	h2_x_L_1_F_vs_x_L_1_N->Write();
	p_x_L_1_F_vs_x_L_1_N->Write();
	h_q_cut1->Write();

	gDirectory = d_cuts->mkdir("cut 2");
	h2_x_R_1_F_vs_x_R_1_N->Write();
	p_x_R_1_F_vs_x_R_1_N->Write();
	h_q_cut2->Write();

	gDirectory = f_out->mkdir("after selection");
	h2_y_vs_x_L_1_F_sel->Write();
	h2_y_vs_x_L_1_N_sel->Write();
	h2_y_vs_x_R_1_N_sel->Write();
	h2_y_vs_x_R_1_F_sel->Write();

	gDirectory = f_out->mkdir("profiles");
	p_y_vs_x_L_1_F_sel.Write();
	p_y_vs_x_L_1_N_sel.Write();
	p_y_vs_x_R_1_N_sel.Write();
	p_y_vs_x_R_1_F_sel.Write();

	// clean up
	delete f_out;
	return 0;
}
