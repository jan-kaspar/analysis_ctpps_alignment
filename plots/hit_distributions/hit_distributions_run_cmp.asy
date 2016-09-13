import root;
import pad_layout;

string topDir = "../../";

string datasets[] = {
//	"run_alignment/10077",
//	"run_alignment/10079",
//	"run_alignment/10080",
//	"run_alignment/10081",
//	"run_alignment/10082",

//	"run_physics/with_margin",
//	"run_physics/no_margin",

	"run_physics_no_margin/fill_5261",
	"run_physics_no_margin/fill_5264",
	"run_physics_no_margin/fill_5265",
	"run_physics_no_margin/fill_5266",
	"run_physics_no_margin/fill_5267",
	"run_physics_no_margin/fill_5274",
	"run_physics_no_margin/fill_5275",
	"run_physics_no_margin/fill_5276",
	"run_physics_no_margin/fill_5277",
	"run_physics_no_margin/fill_5279",
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

		TH2_x_min = 5; TH2_x_max = 13;
		TH2_y_min = -3; TH2_y_max = +3;

		TH2_z_min = 1; TH2_z_max = 10^4;

		RootGetObject(topDir + datasets[dsi]+"/distributions.root", "after selection/h2_y_vs_x_" + rps[rpi] + "_sel");
		draw(robj, "def");

		//limits((0, -15), (20, +15), Crop);
	}
}

GShipout(hSkip=1mm, vSkip=1mm);
