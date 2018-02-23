import root;
import pad_layout;

string topDir = "../../";

string datasets[] = {
	"period1_physics_margin/fill_4947",
//	"period1_physics_margin/fill_4953",
//	"period1_physics_margin/fill_4961",
	"period1_physics_margin/fill_4964",
	
//	"period1_physics/fill_4964",
//	"period1_physics_margin/fill_4976",

//	"period1_physics/fill_4985",
	"period1_physics/fill_4988",
//	"period1_physics/fill_4990",
//	"period1_physics/fill_5005",
//	"period1_physics/fill_5013",
//	"period1_physics/fill_5017",
//	"period1_physics/fill_5020",
//	"period1_physics/fill_5021",
//	"period1_physics/fill_5024",
	"period1_physics/fill_5026",
//	"period1_physics/fill_5027",
//	"period1_physics/fill_5028",
//	"period1_physics/fill_5029",
//	"period1_physics/fill_5030",
//	"period1_physics/fill_5038",
//	"period1_physics/fill_5043",
//	"period1_physics/fill_5045",
	"period1_physics/fill_5048",
//	"period1_physics/fill_5052",

	"period1_physics/fill_5261",
//	"period1_physics/fill_5264",
	"period1_physics/fill_5265",
	"period1_physics/fill_5266",
	"period1_physics/fill_5267",

//	"period1_alignment/10077",
//	"period1_alignment/10081",
};

//----------------------------------------------------------------------------------------------------

string arms[], arm_labels[];
arms.push("L"); arm_labels.push("sector 45");
arms.push("R"); arm_labels.push("sector 56");

//----------------------------------------------------------------------------------------------------

string alignment = "method x";

string cut_option = "with cuts";

xSizeDef = 8cm;
xTicksDef = LeftTicks(0.05, 0.01);

//----------------------------------------------------------------------------------------------------

for (int ai : arms.keys)
{
	NewPad(false);
	label(arm_labels[ai]);
}

NewRow();

for (string arm : arms)
{
	NewPad("$\xi$ (FN mean)", "std.~dev.~of $(\xi_{\rm F} - \xi_{\rm N})$");

	if (arm == "L")
	{
		TGraph_x_min = 0.05;
		TGraph_x_max = 0.12;
	} else {
		TGraph_x_min = 0.05;
		TGraph_x_max = 0.15;
	}

	for (int dsi : datasets.keys)
	{
		string f = topDir + datasets[dsi] + "/reconstruction_test.root";
		RootObject obj = RootGetObject(f, alignment+"/"+cut_option+"/g_RMS_xi_" + arm + "_FNdiff_vs_xi_" + arm + "_FNmean");

		string label = replace(datasets[dsi], "_", "\_");

		draw(obj, "l", StdPen(dsi), label);
	}

	limits((0.05, 0), (0.15, 0.003), Crop);
}

frame f_legend = BuildLegend();

NewPad(false);
add(shift(0, 50) * f_legend);

GShipout(vSkip=1mm);
