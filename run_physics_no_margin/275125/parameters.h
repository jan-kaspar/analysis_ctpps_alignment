void ApplySettings()
{
	cut1_a = -1.012; cut1_c = -1.238; cut1_si = 0.20;
	cut2_a = -0.974; cut2_c = +0.512; cut2_si = 0.20;
	cut3_a = -1.169; cut3_c = +0.739; cut3_si = 0.08;
	cut4_a = -1.141; cut4_c = +0.034; cut4_si = 0.08;

	selectionRangesX["L_1_F"] = SelectionRange(11.0, 15.5);
	selectionRangesX["L_1_N"] = SelectionRange(9.5, 13.5);
	selectionRangesX["R_1_N"] = SelectionRange(9.1, 14.5);
	selectionRangesX["R_1_F"] = SelectionRange(8.5, 13.5);
}
