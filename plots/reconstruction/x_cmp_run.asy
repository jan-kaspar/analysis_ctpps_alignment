import root;
import pad_layout;

string topDir = "../../";

string datasets[] = {
	"run_physics_margin/fill_4947",
//	"run_physics_margin/fill_4953",
//	"run_physics_margin/fill_4961",
	"run_physics_margin/fill_4964",
	
//	"run_physics_no_margin/fill_4964",
//	"run_physics_margin/fill_4976",

//	"run_physics_no_margin/fill_4985",
	"run_physics_no_margin/fill_4988",
//	"run_physics_no_margin/fill_4990",
//	"run_physics_no_margin/fill_5005",
//	"run_physics_no_margin/fill_5013",
//	"run_physics_no_margin/fill_5017",
//	"run_physics_no_margin/fill_5020",
//	"run_physics_no_margin/fill_5021",
//	"run_physics_no_margin/fill_5024",
	"run_physics_no_margin/fill_5026",
//	"run_physics_no_margin/fill_5027",
//	"run_physics_no_margin/fill_5028",
//	"run_physics_no_margin/fill_5029",
//	"run_physics_no_margin/fill_5030",
//	"run_physics_no_margin/fill_5038",
//	"run_physics_no_margin/fill_5043",
//	"run_physics_no_margin/fill_5045",
	"run_physics_no_margin/fill_5048",
//	"run_physics_no_margin/fill_5052",

	"run_physics_no_margin/fill_5261",
//	"run_physics_no_margin/fill_5264",
	"run_physics_no_margin/fill_5265",
	"run_physics_no_margin/fill_5266",
	"run_physics_no_margin/fill_5267",

	"run_alignment/10077",
//	"run_alignment/10081",
};

string datasets[] = {
	"run_physics_margin/fill_4947",
//	"run_physics_margin/fill_4964",

	"run_physics_no_margin/fill_4988",
	"run_physics_no_margin/fill_5026",
	"run_physics_no_margin/fill_5048",

	"run_physics_no_margin/fill_5261",
//	"run_physics_no_margin/fill_5266",

	"run_alignment/10077",
//	"run_alignment/10081",
};

int rp_ids[];
string rp_labels[];
real rp_norm_min[];
real rp_norm_max[];

rp_ids.push(3); rp_labels.push("L-210-fr-hr"); rp_norm_min.push(9.0); rp_norm_max.push(11.0);
rp_ids.push(2); rp_labels.push("L-210-nr-hr"); rp_norm_min.push(9.0); rp_norm_max.push(11.0);
rp_ids.push(102); rp_labels.push("R-210-nr-hr"); rp_norm_min.push(7.0); rp_norm_max.push(10.0);
rp_ids.push(103); rp_labels.push("R-210-fr-hr"); rp_norm_min.push(7.0); rp_norm_max.push(10.0);

string alignments[] = {
//	"none",
	"method x",
//	"method y",
};

string cut_option = "without cuts";

xSizeDef = 8cm;
xTicksDef = LeftTicks(1., 0.5);

//----------------------------------------------------------------------------------------------------

real GetNormalisation(RootObject obj, real x_min, real x_max)
{
	RootObject x_axis = obj.oExec("GetXaxis");
	int bin_min = x_axis.iExec("FindBin", x_min);
	int bin_max = x_axis.iExec("FindBin", x_max);

	real S = 0.;
	for (int bin = bin_min; bin <= bin_max; ++bin)
	{
		S += obj.rExec("GetBinContent", bin);
	}

	return S;
}

//----------------------------------------------------------------------------------------------------

NewPad(false);
label(cut_option);

NewRow();

NewPad(false);
for (int rpi : rp_ids.keys)
{
	NewPad(false);
	label("{\SetFontSizesXX " + replace(rp_labels[rpi], "_", "\_") + "}");
}

for (int ai : alignments.keys)
{
	NewRow();

	NewPad(false);
	label(rotate(90)*Label("{\SetFontSizesXX " + alignments[ai] + "}"));

	for (int rpi : rp_ids.keys)
	{
		NewPad("$x\ung{mm}$");

		TH1_x_min = 1;

		for (int dsi : datasets.keys)
		{
			bool alignmentRun = (find(datasets[dsi], "run_alignment") != -1);

			string alignments_eff = alignments[ai];
			if (alignmentRun)
				alignments_eff = "none";

			string f = topDir + datasets[dsi] + "/reconstruction_test.root";
			RootObject obj = RootGetObject(f, alignments_eff+"/"+cut_option+"/" + format("h_x_%u", rp_ids[rpi]));

			real norm = GetNormalisation(obj, rp_norm_min[rpi], rp_norm_max[rpi]);

			pen p = (alignmentRun) ? black+2pt : StdPen(dsi+1);

			draw(scale(1., 1./norm), obj, "vl", p, replace(datasets[dsi], "_", "\_"));
		}
	
		//limits((1, 0), (15., 0.10), Crop);
		xlimits(1, 15., Crop);
	}

	frame f_legend = BuildLegend();

	NewPad(false);
	attach(f_legend);
}


GShipout(hSkip=1mm, vSkip=1mm, margin=1mm);
