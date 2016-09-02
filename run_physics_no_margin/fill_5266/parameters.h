void ApplySettings()
{
	lhc_fill = 279841;

	cut1_a = -0.993; cut1_c = -0.830; cut1_si = 0.20;
	cut2_a = -0.970; cut2_c = +0.433; cut2_si = 0.20;
	cut3_a = -1.154; cut3_c = +0.071; cut3_si = 0.15;
	cut4_a = -1.146; cut4_c = +0.026; cut4_si = 0.15;

	selectionRangesX["L_1_F"] = SelectionRange(9.0, 16.5);
	selectionRangesX["L_1_N"] = SelectionRange(8.3, 15.7);
	selectionRangesX["R_1_N"] = SelectionRange(7.5, 15.5);
	selectionRangesX["R_1_F"] = SelectionRange(7.1, 15.0);
}