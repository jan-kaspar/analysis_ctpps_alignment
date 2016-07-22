import root;
import pad_layout;

string topDir = "../../";

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

string datasets[] = {
	"run_physics_no_margin/274244",
	"run_physics_no_margin/274388",
	"run_physics_no_margin/274958",
	"run_physics_no_margin/274969",
	"run_physics_no_margin/275310",
	"run_physics_no_margin/275836",
};

string reference = "10081";

string rps[] = {
	"L_1_F",
	"L_1_N",
	"R_1_N",
	"R_1_F",
};

xSizeDef = 8cm;

yTicksDef = RightTicks(0.005, 0.001);

//----------------------------------------------------------------------------------------------------

NewPad(false);

for (int rpi : rps.keys)
{
	NewPad(false);
	label("{\SetFontSizesXX " + replace(rps[rpi], "_", "\_") + "}");
}

for (int dsi : datasets.keys)
{
	string dataset = datasets[dsi];

	NewRow();

	NewPad(false);
	string bits[] = split(replace(dataset, "_", "\_"), "/");
	label("\vbox{\SetFontSizesXX\hbox{" + bits[0] + "}\hbox{" + bits[1] + "}}");
	
	for (int rpi : rps.keys)
	{
		NewPad("$x\ung{mm}$", "entries (scaled)");
	
		string p_base = rps[rpi] + "/" + reference + "/method x/c_cmp|";
		draw(RootGetObject(topDir + dataset+"/match.root", p_base + "h_ref_sel"), "d0,eb", black);
		draw(RootGetObject(topDir + dataset+"/match.root", p_base + "h_test_bef"), "d0,eb", blue);
		draw(RootGetObject(topDir + dataset+"/match.root", p_base + "h_test_aft"), "d0,eb", red);
	
		limits((3, 0.005), (16, 0.04), Crop);
	}
}

GShipout(hSkip=1mm, vSkip=1mm);
