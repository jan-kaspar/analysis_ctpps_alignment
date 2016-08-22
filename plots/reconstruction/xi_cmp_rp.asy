import root;
import pad_layout;

string topDir = "../../";

string datasets[] = {
	"run_alignment/10077",
	"run_alignment/10081",

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
};

int rp_ids[];
string rp_labels[];
pen rp_pens[];

rp_ids.push(3); rp_labels.push("L_1_F"); rp_pens.push(black);
rp_ids.push(2); rp_labels.push("L_1_N"); rp_pens.push(red);
rp_ids.push(102); rp_labels.push("R_1_N"); rp_pens.push(blue);
rp_ids.push(103); rp_labels.push("R_1_F"); rp_pens.push(heavygreen);

string alignments[] = {
//	"none",
	"method x",
	"method y",
};

string cut_option = "with cuts";

xSizeDef = 10cm;
xTicksDef = LeftTicks(0.05, 0.01);

//----------------------------------------------------------------------------------------------------

NewPad(false);
label(cut_option);

NewRow();

NewPad(false);
for (int ai : alignments.keys)
{
	NewPad(false);
	label("{\SetFontSizesXX " + replace(alignments[ai], "_", "\_") + "}");
}

for (int dsi : datasets.keys)
{
	NewRow();

	NewPad(false);
	string bits[] = split(replace(datasets[dsi], "_", "\_"), "/");
	label("\vbox{\SetFontSizesXX\hbox{" + bits[0] + "}\hbox{" + bits[1] + "}}");

	for (int ai : alignments.keys)
	{
		NewPad("$\xi$");
		scale(Linear, Log);

		for (int rpi : rp_ids.keys)
		{
			string f = topDir + datasets[dsi] + "/reconstruction_test.root";
			RootObject obj = RootGetObject(f, alignments[ai]+"/"+cut_option+"/" + format("h_xi_%u", rp_ids[rpi]));
			draw(obj, "vl", rp_pens[rpi], replace(rp_labels[rpi], "_", "\_"));
		}

		//ylimits(1e1, 2e5, Crop);
	}

	frame f_legend = BuildLegend();

	NewPad(false);
	attach(f_legend);
}


GShipout(hSkip=1mm, vSkip=1mm, margin=1mm);
