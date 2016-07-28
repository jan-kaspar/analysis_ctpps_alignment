void ApplySettings()
{
	lhc_fill = 4961;

	cut1_a = -1.010; cut1_c = -2.335; cut1_si = 0.20;
	cut2_a = -0.973; cut2_c = +0.482; cut2_si = 0.20;
	cut3_a = -1.156; cut3_c = +1.250; cut3_si = 0.15;
	cut4_a = -1.141; cut4_c = +0.111; cut4_si = 0.15;

	selectionRangesX["L_1_F"] = SelectionRange(9.6, 16.5);
	selectionRangesX["L_1_N"] = SelectionRange(7.0, 14.5);
	selectionRangesX["R_1_N"] = SelectionRange(8.0, 15.5);
	selectionRangesX["R_1_F"] = SelectionRange(7.0, 14.5);
}
