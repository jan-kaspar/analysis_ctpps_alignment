import root;
import pad_layout;

string topDir = "../../";

string datasets[] = {
	"run_physics_margin/274199",

	//"run_physics_no_margin/274244",
	"run_physics_no_margin/274958",
	//"run_physics_no_margin/275125",
	//"run_physics_no_margin/275376",
	"run_physics_no_margin/275836",
};

int rp_ids[];
string rp_labels[];
pen rp_pens[];

rp_ids.push(3); rp_labels.push("L_1_F"); rp_pens.push(black);
rp_ids.push(2); rp_labels.push("L_1_N"); rp_pens.push(red);
rp_ids.push(102); rp_labels.push("R_1_N"); rp_pens.push(blue);
rp_ids.push(103); rp_labels.push("R_1_F"); rp_pens.push(heavygreen);

string alignments[] = {
	"none",
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

		for (int rpi : rp_ids.keys)
		{
			string f = topDir + datasets[dsi] + "/reconstruction_test.root";
			RootObject obj = RootGetObject(f, alignments[ai]+"/"+cut_option+"/" + format("h_xi_%u", rp_ids[rpi]));
			draw(obj, "vl", rp_pens[rpi], replace(rp_labels[rpi], "_", "\_"));
		}
	}

	frame f_legend = BuildLegend();

	NewPad(false);
	attach(f_legend);
}


GShipout(hSkip=1mm, vSkip=1mm, margin=1mm);
