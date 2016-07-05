import root;
import pad_layout;

string topDir = "../";

string datasets[] = {
	"run_physics_margin/274199",
	"run_physics_margin/274241",
	"run_physics_no_margin/274244",
	"run_physics_no_margin/274388",
};

string reference = "10077";

string rps[] = {
	"L_1_F",
	"L_1_N",
	"R_1_N",
	"R_1_F",
};

yTicksDef = RightTicks(0.1, 0.05);

//----------------------------------------------------------------------------------------------------

for (int rpi : rps.keys)
{
	NewPad(false);
	label("{\SetFontSizesXX " + replace(rps[rpi], "_", "\_") + "}");
}

NewRow();

for (int rpi : rps.keys)
{
	NewPad("dataset", "shift$\ung{mm}$");

	real S1=0, Sbsh=0;
	for (int dsi : datasets.keys)
	{
		RootGetObject(topDir + datasets[dsi]+"/match.root", rps[rpi] + "/" + reference + "/g_results");
		real ax[] = { 0. };
		real ay[] = { 0. };
		robj.vExec("GetPoint", 0, ax, ay);

		real bsh = ay[0] * 0.1;	// index to mm
		real bsh_unc = 0.05;
	
		//limits((2, 0), (15, 3), Crop);
		draw((dsi, bsh), mCi+2pt+red);
		draw((dsi, bsh-bsh_unc)--(dsi, bsh+bsh_unc), red);

		S1 += 1.;
		Sbsh += bsh;
	}

	real bsh_mean = Sbsh / S1;

	for (int dsi : datasets.keys)
	{
		string bits[] = split(replace(datasets[dsi], "_", "\_"), "/");
		label(rotate(90) * Label(bits[1], Fill(white)), (dsi, bsh_mean));
	}

	xlimits(-1, datasets.length);
}

GShipout(hSkip=1mm, vSkip=1mm);
