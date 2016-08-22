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

string ref_label[];
pen ref_pen[];
real ref_offset[];

ref_label.push("10077"); ref_pen.push(blue); ref_offset.push(-0.2);
ref_label.push("10079"); ref_pen.push(red); ref_offset.push(-0.0);
ref_label.push("10081"); ref_pen.push(heavygreen); ref_offset.push(+0.2);

string methods[];
mark method_markers[];

methods.push("method y"); method_markers.push(mCi+2pt);
methods.push("method x"); method_markers.push(mCr+4pt);

string rps[] = {
	"L_1_F",
	"L_1_N",
	"R_1_N",
	"R_1_F",
};

yTicksDef = RightTicks(0.2, 0.1);

xSizeDef = 14cm;

//----------------------------------------------------------------------------------------------------

string TickLabels(real x)
{
	if (x >=0 && x < datasets.length)
	{
		string ds = datasets[(int)x];
		string bits[] = split(ds, "/");
		return replace(bits[1], "fill_", "");
	} else {
		return "";
	}
}

xTicksDef = LeftTicks(rotate(90)*Label(""), TickLabels, Step=1, step=0);

//----------------------------------------------------------------------------------------------------

/*
for (int rpi : rps.keys)
{
	NewPad(false);
	label("{\SetFontSizesXX " + replace(rps[rpi], "_", "\_") + "}");
}

NewRow();
*/

for (int rpi : rps.keys)
{
	write(rps[rpi]);

	if (rpi == 2)
		NewRow();

	NewPad("fill", "shift$\ung{mm}$");

	for (int dsi : datasets.keys)
	{
		write("    " + datasets[dsi]);

		for (int ri : ref_label.keys)
		{
			for (int mi : methods.keys)
			{
				RootGetObject(topDir + datasets[dsi]+"/match.root", rps[rpi] + "/" + ref_label[ri] + "/" + methods[mi] + "/g_results");
				real ax[] = { 0. };
				real ay[] = { 0. };
				robj.vExec("GetPoint", 0, ax, ay); real bsh = ay[0];
				robj.vExec("GetPoint", 1, ax, ay); real bsh_unc = ay[0];
	
				real x = dsi + ref_offset[ri];
	
				pen p = ref_pen[ri];
			
				draw((x, bsh), method_markers[mi]+p);
				draw((x, bsh-bsh_unc)--(x, bsh+bsh_unc), p);
			}
		}
	}

	xlimits(-1, datasets.length);

	AttachLegend(replace(rps[rpi], "_", "\_"));
}

//----------------------------------------------------------------------------------------------------

NewPad(false);
for (int ri : ref_label.keys)
	AddToLegend(ref_label[ri], ref_pen[ri], mCi+2pt+ref_pen[ri]);
AttachLegend();

GShipout(hSkip=5mm, vSkip=1mm);
