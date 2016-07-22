import root;
import pad_layout;

string topDir = "../";

string datasets[] = {
	"run_alignment/10077",
	"run_alignment/10079",
	"run_alignment/10080",
	"run_alignment/10081",
	"run_alignment/10082",

//	"run_physics/with_margin",
//	"run_physics/no_margin",
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

for (int dsi : datasets.keys)
{
	NewRow();

	NewPad(false);
	label("{\SetFontSizesXX " + replace(datasets[dsi], "_", "\_") + "}");

	for (int rpi : rps.keys)
	{
		NewPad("$x\ung{mm}$", "$y\ung{mm}$");

		scale(Linear, Linear, Log);

		RootGetObject(topDir + datasets[dsi]+"/distributions.root", "after selection/h2_y_vs_x_" + rps[rpi] + "_sel");
		draw(robj, "def");

		limits((0, -15), (20, +15), Crop);
	}
}

GShipout(hSkip=1mm, vSkip=1mm);
