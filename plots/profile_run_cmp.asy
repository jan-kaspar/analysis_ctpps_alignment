import root;
import pad_layout;

string topDir = "../";

string datasets[] = {
	//"run_alignment/10077",
	"run_alignment/10079",
	//"run_alignment/10080",
	//"run_alignment/10081",
	//"run_alignment/10082",

	"run_physics/with_margin",
	"run_physics/no_margin",
};

string quantities[] = {
	"mean",
	"stddev"
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
		NewPad("$x\ung{mm}$", "$y\ung{mm}$");

		for (int dsi : datasets.keys)
		{
			pen p = StdPen(dsi);

			RootGetObject(topDir + datasets[dsi]+"/distributions.root", "profiles/" + rps[rpi] + "/h_" + q);
			draw(robj, "vl,eb", p);
		}

		if (q == "mean")
			limits((0, -2), (20, +2), Crop);

		if (q == "stddev")
			limits((0, 0), (20, +3), Crop);
	}
}

GShipout(hSkip=1mm, vSkip=1mm);
