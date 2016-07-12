import root;
import pad_layout;

string topDir = "../";

string dataset = "run_physics_no_margin/274388";
//string dataset = "run_physics_no_margin/274958";

string reference = "10081";

string rps[] = {
	"L_1_F",
	"L_1_N",
	"R_1_N",
	"R_1_F",
};

ySizeDef = 5cm;

//----------------------------------------------------------------------------------------------------

for (int rpi : rps.keys)
{
	NewPad(false);
	label("{\SetFontSizesXX " + replace(rps[rpi], "_", "\_") + "}");
}

NewRow();

for (int rpi : rps.keys)
{
	NewPad("$x\ung{mm}$", "std.~dev.~of $y\ung{mm}$");
	currentpad.yTicks = RightTicks(0.5, 0.1);

	string p_base = rps[rpi] + "/" + reference + "/h_cmp|";
	draw(RootGetObject(topDir + dataset+"/match.root", p_base + "h_ref_sel"), "d0,eb", black);
	draw(RootGetObject(topDir + dataset+"/match.root", p_base + "h_test_bef"), "d0,eb", blue);
	draw(RootGetObject(topDir + dataset+"/match.root", p_base + "h_test_aft"), "d0,eb", red);

	limits((2, 0), (15, 3), Crop);
}

NewRow();

xTicksDef = LeftTicks(50., 10.);

for (int rpi : rps.keys)
{
	NewPad("shift (steps)", "bins in overlap");

	RootGetObject(topDir + dataset+"/match.root", rps[rpi] + "/" + reference + "/g_results");
	real ax[] = { 0. };
	real ay[] = { 0. };
	robj.vExec("GetPoint", 0, ax, ay);

	draw(RootGetObject(topDir + dataset+"/match.root", rps[rpi] + "/" + reference + "/g_n_bins"), "p", magenta, mCi+1pt+magenta);

	limits((-200, 0), (+150, 300), Crop);
	yaxis(XEquals(ay[0], false), dashed);
}

NewRow();

for (int rpi : rps.keys)
{
	NewPad("shift, in steps of $100\ung{\mu m}$", "$S^2 / N$");

	RootGetObject(topDir + dataset+"/match.root", rps[rpi] + "/" + reference + "/g_results");
	real ax[] = { 0. };
	real ay[] = { 0. };
	robj.vExec("GetPoint", 0, ax, ay);

	draw(RootGetObject(topDir + dataset+"/match.root", rps[rpi] + "/" + reference + "/g_chi_sq_norm"), "p", heavygreen, mCi+1pt+heavygreen);

	limits((-200, 0), (+150, 300), Crop);
	yaxis(XEquals(ay[0], false), dashed);
}

GShipout(hSkip=1mm, vSkip=1mm);
