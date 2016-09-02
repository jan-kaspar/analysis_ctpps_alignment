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

	"run_physics_no_margin/fill_5261",
	"run_physics_no_margin/fill_5264",
	"run_physics_no_margin/fill_5265",
	"run_physics_no_margin/fill_5266",
	"run_physics_no_margin/fill_5267",
};

string ref_label[];

ref_label.push("10077");
ref_label.push("10079");
ref_label.push("10081");

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

real rp_shift_m[] =    { -3.65, -1.15, -3.32, -2.96 };
real rp_shift_no_m[] = { -4.17,    0., -3.93, -3.57 };

yTicksDef = RightTicks(0.2, 0.1);

xSizeDef = 12cm;

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
	//scale(Linear, Linear(true));

	if (rp_shift_m[rpi] != 0)
	{
		real sh = rp_shift_m[rpi], unc = 0.1;
		real fill_min = -1, fill_max = 6;
		draw((fill_min, sh+unc)--(fill_max, sh+unc), black+dashed);
		draw((fill_min, sh)--(fill_max, sh), black+1pt);
		draw((fill_min, sh-unc)--(fill_max, sh-unc), black+dashed);
		draw((fill_max, sh-2*unc), invisible);
		draw((fill_max, sh+2*unc), invisible);
	}
	
	if (rp_shift_no_m[rpi] != 0)
	{
		real sh = rp_shift_no_m[rpi], unc = 0.1;
		real fill_min = 3, fill_max = datasets.length;
		draw((fill_min, sh+unc)--(fill_max, sh+unc), black+dashed);
		draw((fill_min, sh)--(fill_max, sh), black+1pt);
		draw((fill_min, sh-unc)--(fill_max, sh-unc), black+dashed);
		//filldraw((fill_min, sh-unc)--(fill_max, sh-unc)--(fill_max, sh+unc)--(fill_min, sh+unc)--cycle, black+opacity(0.1), nullpen);
		draw((fill_max, sh-2*unc), invisible);
		draw((fill_max, sh+2*unc), invisible);
	}

	for (int dsi : datasets.keys)
	{
		write("    " + datasets[dsi]);

		pen p = (find(datasets[dsi], "no_margin") == -1) ? blue : red;

		for (int mi : methods.keys)
		{
			real S1=0, Ss=0, Su=0;

			for (int ri : ref_label.keys)
			{
				RootGetObject(topDir + datasets[dsi]+"/match.root", rps[rpi] + "/" + ref_label[ri] + "/" + methods[mi] + "/g_results");
				real ax[] = { 0. };
				real ay[] = { 0. };
				robj.vExec("GetPoint", 0, ax, ay); real bsh = ay[0];
				robj.vExec("GetPoint", 1, ax, ay); real bsh_unc = ay[0];
	
				S1 += 1;
				Ss += bsh;		
				Su += bsh_unc;		
			}
			
			real x = dsi;

			real m_sh = Ss / S1;
			real u_sh = Su / S1;

			bool pointValid = (fabs(m_sh) > 0.01);

			if (pointValid)
			{
				draw((x, m_sh), method_markers[mi]+p);
				draw((x, m_sh-u_sh)--(x, m_sh+u_sh), p);
			}
		}
	}

	xlimits(-1, datasets.length, Crop);

	AttachLegend(replace(rps[rpi], "_", "\_"));
}

GShipout(hSkip=5mm, vSkip=1mm);
