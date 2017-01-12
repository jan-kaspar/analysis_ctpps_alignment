import root;
import pad_layout;
include "../io_alignment_format.asy";

string topDir = "../../";

struct FillData
{
	int fill;
	string datasets[];
};

FillData fill_data[];

void AddDataSet(string p)
{
	int fill = (int) substr(p, find(p, "fill_")+5, 4);

	bool found = false;
	for (FillData fd : fill_data)
	{
		if (fd.fill == fill)
		{
			found = true;
			fd.datasets.push(p);
		}
	}

	if (!found)
	{
		FillData fd;
		fd.fill = fill;
		fd.datasets.push(p);

		fill_data.push(fd);
	}
}
	
// primary sample
AddDataSet("period1_physics_margin/fill_4947");
AddDataSet("period1_physics_margin/fill_4953");
AddDataSet("period1_physics_margin/fill_4961");
AddDataSet("period1_physics_margin/fill_4964");

AddDataSet("period1_physics/fill_4964");
AddDataSet("period1_physics_margin/fill_4976");

AddDataSet("period1_physics/fill_4985");
AddDataSet("period1_physics/fill_4988");
AddDataSet("period1_physics/fill_4990");
AddDataSet("period1_physics/fill_5005");
AddDataSet("period1_physics/fill_5013");
AddDataSet("period1_physics/fill_5017");
AddDataSet("period1_physics/fill_5020");
AddDataSet("period1_physics/fill_5021");
AddDataSet("period1_physics/fill_5024");
AddDataSet("period1_physics/fill_5026");
AddDataSet("period1_physics/fill_5027");
AddDataSet("period1_physics/fill_5028");
AddDataSet("period1_physics/fill_5029");
AddDataSet("period1_physics/fill_5030");
AddDataSet("period1_physics/fill_5038");
AddDataSet("period1_physics/fill_5043");
AddDataSet("period1_physics/fill_5045");
AddDataSet("period1_physics/fill_5048");
AddDataSet("period1_physics/fill_5052");

AddDataSet("period1_physics/fill_5261");
AddDataSet("period1_physics/fill_5264");
AddDataSet("period1_physics/fill_5265");
AddDataSet("period1_physics/fill_5266");
AddDataSet("period1_physics/fill_5267");
AddDataSet("period1_physics/fill_5274");
AddDataSet("period1_physics/fill_5275");
AddDataSet("period1_physics/fill_5276");
AddDataSet("period1_physics/fill_5277");
AddDataSet("period1_physics/fill_5279");
AddDataSet("period1_physics/fill_5287");
AddDataSet("period1_physics/fill_5288");

// secondary sample
if (false)
{
	AddDataSet("period1_physics_margin_ss/fill_4947");
	AddDataSet("period1_physics_margin_ss/fill_4961");
	AddDataSet("period1_physics_margin_ss/fill_4976");
	
	AddDataSet("period1_physics_ss/fill_4985");
	AddDataSet("period1_physics_ss/fill_4988");
	AddDataSet("period1_physics_ss/fill_4990");
	AddDataSet("period1_physics_ss/fill_5005");
	AddDataSet("period1_physics_ss/fill_5013");
	AddDataSet("period1_physics_ss/fill_5017");
	AddDataSet("period1_physics_ss/fill_5020");
	AddDataSet("period1_physics_ss/fill_5021");
	AddDataSet("period1_physics_ss/fill_5024");
	AddDataSet("period1_physics_ss/fill_5026");
	AddDataSet("period1_physics_ss/fill_5027");
	AddDataSet("period1_physics_ss/fill_5028");
	AddDataSet("period1_physics_ss/fill_5029");
	AddDataSet("period1_physics_ss/fill_5030");
	AddDataSet("period1_physics_ss/fill_5038");
	AddDataSet("period1_physics_ss/fill_5043");
	AddDataSet("period1_physics_ss/fill_5045");
	AddDataSet("period1_physics_ss/fill_5048");
	AddDataSet("period1_physics_ss/fill_5052");
	AddDataSet("period1_physics_ss/fill_5261");
	AddDataSet("period1_physics_ss/fill_5267");
	AddDataSet("period1_physics_ss/fill_5277");
	AddDataSet("period1_physics_ss/fill_5279");
	AddDataSet("period1_physics_ss/fill_5287");
	AddDataSet("period1_physics_ss/fill_5288");
}

//----------------------------------------------------------------------------------------------------

string methods[];
pen method_pens[];

methods.push("method y"); method_pens.push(blue);
methods.push("method x"); method_pens.push(red);

int rp_ids[];
string rps[], rp_labels[];
real rp_shift_m[], rp_shift_no_m[];
rp_ids.push(3); rps.push("L_1_F"); rp_labels.push("L-210-fr-hr"); rp_shift_m.push(-3.65); rp_shift_no_m.push(-4.17);
rp_ids.push(2); rps.push("L_1_N"); rp_labels.push("L-210-nr-hr"); rp_shift_m.push(-1.15); rp_shift_no_m.push(-0.00);
rp_ids.push(102); rps.push("R_1_N"); rp_labels.push("R-210-nr-hr"); rp_shift_m.push(-3.32); rp_shift_no_m.push(-3.93);
rp_ids.push(103); rps.push("R_1_F"); rp_labels.push("R-210-fr-hr"); rp_shift_m.push(-2.96); rp_shift_no_m.push(-3.57);

yTicksDef = RightTicks(0.2, 0.1);

xSizeDef = 15cm;

//----------------------------------------------------------------------------------------------------

string TickLabels(real x)
{
	if (x >=0 && x < fill_data.length)
	{
		return format("%i", fill_data[(int) x].fill);
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

	NewPad("fill", "horizontal shift$\ung{mm}$");

	if (rp_shift_m[rpi] != 0)
	{
		real sh = rp_shift_m[rpi], unc = 0.15;
		real fill_min = -1, fill_max = 6;
		draw((fill_min, sh+unc)--(fill_max, sh+unc), black+dashed);
		draw((fill_min, sh)--(fill_max, sh), black+1pt);
		draw((fill_min, sh-unc)--(fill_max, sh-unc), black+dashed);
		draw((fill_max, sh-2*unc), invisible);
		draw((fill_max, sh+2*unc), invisible);
	}
	
	if (rp_shift_no_m[rpi] != 0)
	{
		real sh = rp_shift_no_m[rpi], unc = 0.15;
		real fill_min = 3, fill_max = fill_data.length;
		draw((fill_min, sh+unc)--(fill_max, sh+unc), black+dashed);
		draw((fill_min, sh)--(fill_max, sh), black+1pt);
		draw((fill_min, sh-unc)--(fill_max, sh-unc), black+dashed);
		//filldraw((fill_min, sh-unc)--(fill_max, sh-unc)--(fill_max, sh+unc)--(fill_min, sh+unc)--cycle, black+opacity(0.1), nullpen);
		draw((fill_max, sh-2*unc), invisible);
		draw((fill_max, sh+2*unc), invisible);
	}

	for (int fdi : fill_data.keys)
	{
		write(format("    %i", fill_data[fdi].fill));

		for (int dsi : fill_data[fdi].datasets.keys)
		{
			string dataset = fill_data[fdi].datasets[dsi];

			write("        " + dataset);
	
			mark m = (find(dataset, "margin") != -1) ? mSq+4pt+false : mCi+3pt;

			AlignmentResults arc[];
			int ret = LoadAlignmentResults(topDir + dataset+"/process_alignments.out", arc);
			if (ret != 0)
				continue;
	
			for (int mi : methods.keys)
			{
				bool method_found = false;
				AlignmentResults ar;
				for (AlignmentResults ari : arc)
				{
					if (ari.label == methods[mi])
					{
						method_found = true;
						ar = ari;
						break;
					}
				}

				if (!method_found)
					continue;

				if (!ar.results.initialized(rp_ids[rpi]))
					continue;

				real v = ar.results[rp_ids[rpi]].sh_x;

				bool point_valid = (fabs(v) > 1);

				if (point_valid)
				{
					pen p = method_pens[mi];
					draw((fdi, v), m + p);
				}
			}
		}
	}

	xlimits(-1, fill_data.length, Crop);

	//yaxis(XEquals(23.5, false), heavygreen);

	AttachLegend("{\SetFontSizesXX " + rp_labels[rpi] + "}");
}

//----------------------------------------------------------------------------------------------------

NewPad(false);

for (int mi : methods.keys)
{
	pen p = method_pens[mi];
	AddToLegend(methods[mi], p);
}

AttachLegend();

//----------------------------------------------------------------------------------------------------

GShipout(hSkip=5mm, vSkip=1mm);
