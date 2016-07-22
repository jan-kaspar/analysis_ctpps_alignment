import root;
import pad_layout;

string topDir = "../../";

string datasets[] = {
	"run_physics_margin/274199",
	"run_physics_margin/274241",

	"run_physics_no_margin/274244",
	"run_physics_no_margin/274958",
	"run_physics_no_margin/275125",
	//"run_physics_no_margin/275376",
	"run_physics_no_margin/275836",
};

int rp_ids[];
string rp_labels[];
real rp_norm_min[];
real rp_norm_max[];

rp_ids.push(3); rp_labels.push("L_1_F"); rp_norm_min.push(0.085); rp_norm_max.push(0.115);
rp_ids.push(2); rp_labels.push("L_1_N"); rp_norm_min.push(0.085); rp_norm_max.push(0.115);
rp_ids.push(102); rp_labels.push("R_1_N"); rp_norm_min.push(0.095); rp_norm_max.push(0.160);
rp_ids.push(103); rp_labels.push("R_1_F"); rp_norm_min.push(0.095); rp_norm_max.push(0.160);

string alignments[] = {
//	"none",
	"method x",
	"method y",
};

string cut_option = "with cuts";

xSizeDef = 10cm;
xTicksDef = LeftTicks(0.05, 0.01);

//----------------------------------------------------------------------------------------------------

real GetNormalisation(RootObject obj, real xi_min, real xi_max)
{
	RootObject x_axis = obj.oExec("GetXaxis");
	int bin_min = x_axis.iExec("FindBin", xi_min);
	int bin_max = x_axis.iExec("FindBin", xi_max);

	real S = 0.;
	for (int bin = bin_min; bin <= bin_max; ++bin)
	{
		S += obj.rExec("GetBinContent", bin);
	}

	return S;
}

//----------------------------------------------------------------------------------------------------

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
	label("{\SetFontSizesXX " + alignments[ai] + "}");

	for (int rpi : rp_ids.keys)
	{
		NewPad("$\xi$");

		for (int dsi : datasets.keys)
		{
			string f = topDir + datasets[dsi] + "/reconstruction_test.root";
			RootObject obj = RootGetObject(f, alignments[ai]+"/"+cut_option+"/" + format("h_xi_%u", rp_ids[rpi]));

			real norm = GetNormalisation(obj, rp_norm_min[rpi], rp_norm_max[rpi]);

			draw(scale(1., 1./norm), obj, "vl", StdPen(dsi), replace(datasets[dsi], "_", "\_"));
		}
	}

	frame f_legend = BuildLegend();

	NewPad(false);
	attach(f_legend);
}


GShipout(hSkip=1mm, vSkip=1mm);
