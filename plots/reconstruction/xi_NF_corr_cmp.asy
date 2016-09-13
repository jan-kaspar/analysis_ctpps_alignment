import root;
import pad_layout;

string topDir = "../../";

string datasets[] = {
	"run_alignment/10077",
//	"run_alignment/10081",

	"run_physics_margin/fill_4947",
//	"run_physics_margin/fill_4953",
//	"run_physics_margin/fill_4961",
//	"run_physics_margin/fill_4964",
	
//	"run_physics_no_margin/fill_4964",
//	"run_physics_margin/fill_4976",

//	"run_physics_no_margin/fill_4985",
//	"run_physics_no_margin/fill_4988",
//	"run_physics_no_margin/fill_4990",
//	"run_physics_no_margin/fill_5005",
//	"run_physics_no_margin/fill_5013",
//	"run_physics_no_margin/fill_5017",
//	"run_physics_no_margin/fill_5020",
//	"run_physics_no_margin/fill_5021",
//	"run_physics_no_margin/fill_5024",
	"run_physics_no_margin/fill_5026",
//	"run_physics_no_margin/fill_5027",
//	"run_physics_no_margin/fill_5028",
//	"run_physics_no_margin/fill_5029",
//	"run_physics_no_margin/fill_5030",
//	"run_physics_no_margin/fill_5038",
//	"run_physics_no_margin/fill_5043",
//	"run_physics_no_margin/fill_5045",
//	"run_physics_no_margin/fill_5048",
	"run_physics_no_margin/fill_5052",

	"run_physics_no_margin/fill_5261",
//	"run_physics_no_margin/fill_5264",
//	"run_physics_no_margin/fill_5265",
//	"run_physics_no_margin/fill_5266",
//	"run_physics_no_margin/fill_5267",

	"run_physics_no_margin/fill_5274",
	"run_physics_no_margin/fill_5279",
};

//string datasets[] = {
//	"run_physics_margin/fill_4953",
//};

string arm_tags[];
string arm_labels[];

arm_tags.push("L"); arm_labels.push("left arm");
arm_tags.push("R"); arm_labels.push("right arm");


string alignment = "method x";

string cut_option = "with cuts";

xSizeDef = 8cm;
ySizeDef = 8cm;

real xi_min = 0.03;
real xi_max = 0.20;

//----------------------------------------------------------------------------------------------------

NewPad(false);
label(cut_option + ", " + alignment);

NewRow();

NewPad(false);
for (int ai : arm_tags.keys)
{
	NewPad(false);
	label("{\SetFontSizesXX " + replace(arm_labels[ai], "_", "\_") + "}");
}

for (int dsi : datasets.keys)
{
	NewRow();

	NewPad(false);
	label("{\SetFontSizesXX " + replace(datasets[dsi], "_", "\_") + "}");

	for (int ai : arm_tags.keys)
	{
		NewPad("$\xi^{\rm N}$", "$\xi^{\rm F}$");

		string arm = arm_tags[ai];
		string f = topDir + datasets[dsi] + "/reconstruction_test.root";
		RootObject obj = RootGetObject(f, alignment+"/"+cut_option+"/h2_xi_" + arm + "_F_vs_xi_" + arm + "_N");

		TH2_x_min = xi_min; TH2_x_max = xi_max;
		TH2_y_min = xi_min; TH2_y_max = xi_max;

		draw(obj);

		draw((xi_min, xi_min)--(xi_max, xi_max), dashed);
	
		limits((xi_min, xi_min), (xi_max, xi_max), Crop);
	}
}

GShipout(hSkip=1mm, vSkip=1mm, margin=1mm);
