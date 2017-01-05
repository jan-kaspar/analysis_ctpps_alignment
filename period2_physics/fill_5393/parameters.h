void ApplySettings()
{
	lhc_fill = 5393;

	cut1_a = -0.997; cut1_c = -1.005; cut1_si = 0.20;
	cut2_a = -0.997; cut2_c = -1.005; cut2_si = 0.20;
	cut3_a = -1.177; cut3_c = -0.688; cut3_si = 0.15;
	cut4_a = -1.177; cut4_c = -0.688; cut4_si = 0.15;

	// TODO
	selectionRangesX["L_1_F"] = SelectionRange(9.8, 16.5);
	selectionRangesX["L_1_N"] = SelectionRange(7.1, 14.5);
	selectionRangesX["R_1_N"] = SelectionRange(8.0, 15.5);
	selectionRangesX["R_1_F"] = SelectionRange(7.0, 14.5);
}
