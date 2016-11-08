void ApplySettings()
{
	lhc_fill = 5393;

	// TODO
	cut1_a = -1.005; cut1_c = -2.357; cut1_si = 0.20;
	cut2_a = -0.970; cut2_c = +0.434; cut2_si = 0.20;
	cut3_a = -1.160; cut3_c = +1.155; cut3_si = 0.15;
	cut4_a = -1.145; cut4_c = +0.034; cut4_si = 0.15;

	// TODO
	selectionRangesX["L_1_F"] = SelectionRange(9.8, 16.5);
	selectionRangesX["L_1_N"] = SelectionRange(7.1, 14.5);
	selectionRangesX["R_1_N"] = SelectionRange(8.0, 15.5);
	selectionRangesX["R_1_F"] = SelectionRange(7.0, 14.5);
}
