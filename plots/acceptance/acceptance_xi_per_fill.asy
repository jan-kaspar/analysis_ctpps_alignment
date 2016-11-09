import root;
import pad_layout;

string topDir = "../../";

string dataset_ref = "period1_alignment/10077";

string datasets[] = {
	"period1_physics_margin/fill_4947",
	"period1_physics_margin/fill_4953",
	"period1_physics_margin/fill_4961",
	"period1_physics_margin/fill_4964",
	"period1_physics/fill_4964",
	"period1_physics_margin/fill_4976",
	"period1_physics/fill_4985",
	"period1_physics/fill_4988",
	"period1_physics/fill_4990",

	"period1_physics/fill_5005",
	"period1_physics/fill_5013",
	"period1_physics/fill_5017",
	"period1_physics/fill_5020",
	"period1_physics/fill_5021",
	"period1_physics/fill_5024",
	"period1_physics/fill_5026",
	"period1_physics/fill_5027",
	"period1_physics/fill_5028",
	"period1_physics/fill_5029",
	"period1_physics/fill_5030",
	"period1_physics/fill_5038",
	"period1_physics/fill_5043",
	"period1_physics/fill_5045",
	"period1_physics/fill_5048",
	"period1_physics/fill_5052",

	"period1_physics/fill_5261",
	"period1_physics/fill_5264",
	"period1_physics/fill_5265",
	"period1_physics/fill_5266",
	"period1_physics/fill_5267",
	"period1_physics/fill_5274",
	"period1_physics/fill_5275",
	"period1_physics/fill_5276",
	"period1_physics/fill_5277",
	"period1_physics/fill_5279",
	"period1_physics/fill_5287",
	"period1_physics/fill_5288",
};

int rp_ids[];
string rp_labels[];
real rp_norm_min[];
real rp_norm_max[];

rp_ids.push(3); rp_labels.push("L-210-fr-hr"); rp_norm_min.push(0.085); rp_norm_max.push(0.115);
rp_ids.push(2); rp_labels.push("L-210-nr-hr"); rp_norm_min.push(0.085); rp_norm_max.push(0.115);
rp_ids.push(102); rp_labels.push("R-210-nr-hr"); rp_norm_min.push(0.095); rp_norm_max.push(0.160);
rp_ids.push(103); rp_labels.push("R-210-fr-hr"); rp_norm_min.push(0.095); rp_norm_max.push(0.160);

string alignment = "method x";

string cut_option = "without cuts";

real x_min[] = {0.05, 0.05, 0.05,  0.06};
real x_max[] = {0.11, 0.11, 0.14,  0.14};
real y_min[] = {0.03, 0.03, 0.015, 0.015};
real y_max[] = {0.04, 0.04, 0.020, 0.020};

xSizeDef = 15.5cm;
xTicksDef = LeftTicks(0.005, 0.001);

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

void DrawOne(string dataset, int rpi, pen p, string label)
{
	string f = topDir + dataset + "/reconstruction_test.root";
	RootObject obj = RootGetObject(f, alignment+"/"+cut_option+"/" + format("h_xi_%u", rp_ids[rpi]));

	real norm = GetNormalisation(obj, rp_norm_min[rpi], rp_norm_max[rpi]);

	draw(scale(1., 1./norm), obj, "vl", p, replace(label, "_", "\_"));
}

//----------------------------------------------------------------------------------------------------

for (int dsi : datasets.keys)
{
	write("* " + datasets[dsi]);

	NewPage();

	for (int rpi : rp_ids.keys)
	{
		NewPad(false);
		label("{\SetFontSizesXX " + replace(rp_labels[rpi], "_", "\_") + "}");
	}

	NewRow();

	for (int rpi : rp_ids.keys)
	{
		NewPad("$\xi$");

		DrawOne(dataset_ref, rpi, blue, "reference: " + dataset_ref);
		DrawOne(datasets[dsi], rpi, red, datasets[dsi]);
	
		xlimits(0.02, 0.105, Crop);
	}

	AddToLegend("<(" + cut_option + ")");
	AddToLegend("<(" + alignment + ")");

	frame f_legend = BuildLegend();

	NewPad(false);
	attach(f_legend);
}

GShipout(hSkip=1mm, vSkip=1mm, margin=1mm);
