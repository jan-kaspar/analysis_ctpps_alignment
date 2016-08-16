import root;
import pad_layout;

string topDir = "../../";

string datasets[] = {
//	"run_physics_margin/274199",
//	"run_physics_margin/274241",
//	"run_physics_no_margin/274244",
//	"run_physics_no_margin/274388",
//	"run_physics_no_margin/274958",
//	"run_physics_no_margin/274969",
//	"run_physics_no_margin/275125",
//	"run_physics_no_margin/275310",
//	"run_physics_no_margin/275376",
//	"run_physics_no_margin/275836",

	"run_physics_margin/fill_4947",
	"run_physics_margin/fill_4953",
	"run_physics_margin/fill_4961",
	"run_physics_margin/fill_4964",
	
	"run_physics_no_margin/fill_4964",
	"run_physics_margin/fill_4976",

	"run_physics_no_margin/fill_4985",
	"run_physics_no_margin/fill_4988",
	"run_physics_no_margin/fill_4990",
	"run_physics_no_margin/fill_5005",
	"run_physics_no_margin/fill_5013",
	"run_physics_no_margin/fill_5017",
	"run_physics_no_margin/fill_5020",
	"run_physics_no_margin/fill_5021",
	"run_physics_no_margin/fill_5024",
	"run_physics_no_margin/fill_5026",
	"run_physics_no_margin/fill_5027",
	"run_physics_no_margin/fill_5028",
	"run_physics_no_margin/fill_5029",
	"run_physics_no_margin/fill_5030",
	"run_physics_no_margin/fill_5038",
	"run_physics_no_margin/fill_5043",
	"run_physics_no_margin/fill_5045",
	"run_physics_no_margin/fill_5048",
	"run_physics_no_margin/fill_5052",
};

//string reference = "10077";
string reference = "10081";

string rps[] = {
	"L_1_F",
	"L_1_N",
	"R_1_N",
	"R_1_F",
};

xSizeDef = 8cm;

xTicksDef = LeftTicks(2., 1.);
yTicksDef = RightTicks(0.5, 0.1);

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
		NewPad("$x\ung{mm}$", "std.~dev.~of $y\ung{mm}$");
	
		string p_base = rps[rpi] + "/" + reference + "/method y/c_cmp|";
		draw(RootGetObject(topDir + dataset+"/match.root", p_base + "h_ref_sel"), "d0,eb", black);
		draw(RootGetObject(topDir + dataset+"/match.root", p_base + "h_test_bef"), "d0,eb", blue);
		draw(RootGetObject(topDir + dataset+"/match.root", p_base + "h_test_aft"), "d0,eb", red);
	
		limits((5, 0), (17, 3), Crop);
	}
}

GShipout(hSkip=1mm, vSkip=1mm);
