import root;
import pad_layout;

string topDir = "../";

string datasets[] = {
	"run_physics_margin/274199",
	"run_physics_margin/274241",
	"run_physics_no_margin/274244",
	"run_physics_no_margin/274388",
	"run_physics_no_margin/274958",
	"run_physics_no_margin/274969",
	"run_physics_no_margin/275125",
	"run_physics_no_margin/275310",
	"run_physics_no_margin/275376",
	"run_physics_no_margin/275836",
};

string ref_label[];
pen ref_pen[];
real ref_offset[];

ref_label.push("10077"); ref_pen.push(blue); ref_offset.push(-0.1);
ref_label.push("10079"); ref_pen.push(red); ref_offset.push(-0.0);
ref_label.push("10081"); ref_pen.push(heavygreen); ref_offset.push(+0.1);

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
		for (int ri : ref_label.keys)
		{
			RootGetObject(topDir + datasets[dsi]+"/match.root", rps[rpi] + "/" + ref_label[ri] + "/g_results");
			real ax[] = { 0. };
			real ay[] = { 0. };
			robj.vExec("GetPoint", 1, ax, ay);
	
			real bsh = ay[0];
			real bsh_unc = 0.025;

			real x = dsi + ref_offset[ri];

			pen p = ref_pen[ri];
		
			draw((x, bsh), mCi+2pt+p);
			draw((x, bsh-bsh_unc)--(x, bsh+bsh_unc), p);
	
			S1 += 1.;
			Sbsh += bsh;
		}
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
