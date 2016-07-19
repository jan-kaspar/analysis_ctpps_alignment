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

ref_label.push("10077"); ref_pen.push(blue); ref_offset.push(-0.2);
ref_label.push("10079"); ref_pen.push(red); ref_offset.push(-0.0);
ref_label.push("10081"); ref_pen.push(heavygreen); ref_offset.push(+0.2);

string methods[];
mark method_markers[];

methods.push("method y"); method_markers.push(mCi);
methods.push("method x"); method_markers.push(mCr);

string rps[] = {
	"L_1_F",
	"L_1_N",
	"R_1_N",
	"R_1_F",
};

yTicksDef = RightTicks(0.2, 0.1);

xSizeDef = 8cm;

//----------------------------------------------------------------------------------------------------

string TickLabels(real x)
{
	if (x >=0 && x < datasets.length)
	{
		string ds = datasets[(int)x];
		string bits[] = split(ds, "/");
		return bits[1];
	} else {
		return "";
	}
}

xTicksDef = LeftTicks(rotate(90)*Label(""), TickLabels, Step=1, step=0);

//----------------------------------------------------------------------------------------------------

for (int rpi : rps.keys)
{
	NewPad(false);
	label("{\SetFontSizesXX " + replace(rps[rpi], "_", "\_") + "}");
}

NewRow();

for (int rpi : rps.keys)
{
	write(rps[rpi]);

	NewPad("dataset", "shift$\ung{mm}$");

	for (int dsi : datasets.keys)
	{
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
			
				draw((x, bsh), method_markers[mi]+2pt+p);
				draw((x, bsh-bsh_unc)--(x, bsh+bsh_unc), p);
			}
		}
	}

	xlimits(-1, datasets.length);
}

//----------------------------------------------------------------------------------------------------

NewPad(false);
for (int ri : ref_label.keys)
	AddToLegend(ref_label[ri], ref_pen[ri], mCi+2pt+ref_pen[ri]);
AttachLegend();

GShipout(hSkip=5mm, vSkip=1mm);
