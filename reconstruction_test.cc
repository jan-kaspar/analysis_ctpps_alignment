#include "common.h"

#include "shared_track.h"
#include "shared_alignment.h"
#include "shared_reconstruction.h"

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

struct PlotGroup
{
	map<unsigned int, TH1D*> h_xi;

	PlotGroup()
	{
		h_xi[2] = new TH1D("", "", 200, 0., 0.2);
		h_xi[3] = new TH1D("", "", 200, 0., 0.2);
		h_xi[102] = new TH1D("", "", 200, 0., 0.2);
		h_xi[103] = new TH1D("", "", 200, 0., 0.2);
	}

	void Write() const
	{
		for (const auto &it : h_xi)
		{
			char buf[100];
			sprintf(buf, "h_xi_%u", it.first);
			it.second->Write(buf);
		}
	}
};

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

int main()
{
	// apply settings
	ApplySettings();

	InitReconstruction();

	AlignmentResultsCollection alignments;
	alignments.Load("match.out");
	//alignments.Write(stdout);

	// get input
	InitInputFiles();
	fwlite::ChainEvent ev(input_files);

	printf("* events in input chain: %llu\n", ev.size());

	// ouput file
	TFile *f_out = new TFile("reconstruction_test.root", "recreate");

	// book histograms
	vector<string> methods = {
		"none",
		"method x",
		"method y",
	};

	// map: method idx, cuts on(1)/off(0) --> plots
	map<unsigned int, map<unsigned int, PlotGroup>> plots;

	// loop over the chain entries
	unsigned long int ev_count = 0;
	unsigned long int tr_sel_count = 0;
	for (ev.toBegin(); ! ev.atEnd(); ++ev)
	{
		/*
		// TODO: comment out
		if (ev_count > 10000000)
			break;
		*/
		ev_count++;

		/*
		// TODO: comment out
		if (tr_sel_count > 2000000)
			break;
		*/

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
		double cq1 = tr[3].x + cut1_a * tr[2].x + cut1_c;
		bool cut1_val = (fabs(cq1) < n_si * cut1_si);

		double cq2 = tr[103].x + cut2_a * tr[102].x + cut2_c;
		bool cut2_val = (fabs(cq2) < n_si * cut2_si);

		double cq3 = tr[3].y + cut3_a * tr[2].y + cut3_c;
		bool cut3_val = (fabs(cq3) < n_si * cut3_si);

		double cq4 = tr[103].y + cut4_a * tr[102].y + cut4_c;
		bool cut4_val = (fabs(cq4) < n_si * cut4_si);

		bool cuts_L = tr[2].valid && tr[3].valid && (!cut1_apply || cut1_val) && (!cut3_apply || cut3_val);
		bool cuts_R = tr[102].valid && tr[103].valid && (!cut2_apply || cut2_val) && (!cut4_apply || cut4_val);

		// increase counter
		if (cuts_L || cuts_R)
			tr_sel_count++;

		// loop over alignments
		for (unsigned int mi = 0; mi < methods.size(); ++mi)
		{
			// apply alignments
			TrackDataCollection tr_al;

			if (methods[mi] == "none")
				tr_al = tr;
			else
				tr_al = alignments[methods[mi]].Apply(tr);

			// loop over RPs
			for (unsigned int rpId : {2, 3, 102, 103})
			{
				bool left = ((rpId / 100) == 0);

				ProtonData proton = ReconstructProton({{rpId, tr_al[rpId]}}, left);

				if (!proton.valid)
					continue;
				
				plots[mi][0].h_xi[rpId]->Fill(proton.xi);

				bool cuts = (left) ? cuts_L : cuts_R;
				if (cuts)
					plots[mi][1].h_xi[rpId]->Fill(proton.xi);
			}
		}
	}

	printf("* events processed: %lu\n", ev_count);
	printf("* tracks selected: %lu\n", tr_sel_count);

	// save plots
	for (const auto &mit : plots)
	{
		TDirectory *m_dir = f_out->mkdir(methods[mit.first].c_str());

		for (const auto &cit : mit.second)
		{
			TDirectory *c_dir = m_dir->mkdir( (cit.first) ? "with cuts" : "without cuts" );

			gDirectory = c_dir;

			cit.second.Write();
		}
	}

	// clean up
	delete f_out;
	return 0;
}
