import root;
import pad_layout;

string topDir = "../";

string datasets[] = {
	"run_alignment/10077",
	//"run_alignment/10079",
	//"run_alignment/10080",
	//"run_alignment/10081",
	//"run_alignment/10082",

	//"run_physics_margin/274199",
	"run_physics_no_margin/274244",
	"run_physics_no_margin/274388",
};

string quantities[] = {
	"entries",
	"mean",
	"stddev"
};

string quantity_labels[] = {
	"$N$",
	"mean of $y\ung{mm}$",
	"RMS of $y\ung{mm}$",
};

string rps[] = {
	"L_1_F",
	"L_1_N",
	"R_1_N",
	"R_1_F",
};

//----------------------------------------------------------------------------------------------------

NewPad(false);
for (int rpi : rps.keys)
{
	NewPad(false);
	label("{\SetFontSizesXX " + replace(rps[rpi], "_", "\_") + "}");
}

for (int qi : quantities.keys)
{
	string q = quantities[qi];

	NewRow();

	NewPad(false);
	label("{\SetFontSizesXX " + replace(q, "_", "\_") + "}");

	for (int rpi : rps.keys)
	{
		NewPad("$x\ung{mm}$", quantity_labels[qi]);

		for (int dsi : datasets.keys)
		{
			pen p = StdPen(dsi);

			transform t = yscale(1);
			
			/*
			if (qi == 0 && dsi == 0)
				t = yscale(0.5);

			if (qi == 0 && dsi == 2)
				t = yscale(0.38) * shift(0.5, 0);
			*/

			RootGetObject(topDir + datasets[dsi]+"/distributions.root", "profiles/" + rps[rpi] + "/h_" + q);
			draw(t, robj, "vl", p);
		}

		if (q == "mean")
			limits((0, -2), (20, +2), Crop);

		if (q == "stddev")
			limits((0, 0), (20, +3), Crop);
	}
}

GShipout(hSkip=1mm, vSkip=1mm);
