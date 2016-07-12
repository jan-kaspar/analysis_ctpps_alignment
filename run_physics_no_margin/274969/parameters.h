void ApplySettings()
{
	cut1_a = -1.000; cut1_c = -1.548; cut1_si = 0.20;
	cut2_a = -0.962; cut2_c = +0.364; cut2_si = 0.20;
	cut3_a = -1.168; cut3_c = +0.896; cut3_si = 0.08;
	cut4_a = -1.146; cut4_c = +0.038; cut4_si = 0.08;

	selectionRangesX["L_1_F"] = SelectionRange(9.8, 15.5);
	selectionRangesX["L_1_N"] = SelectionRange(7.1, 13.5);
	selectionRangesX["R_1_N"] = SelectionRange(8.0, 14.5);
	selectionRangesX["R_1_F"] = SelectionRange(7.0, 13.5);
}
