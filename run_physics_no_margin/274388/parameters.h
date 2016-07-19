void ApplySettings()
{
	cut1_a = -1.007; cut1_c = -2.254; cut1_si = 0.20;
	cut2_a = -0.964; cut2_c = +0.383; cut2_si = 0.20;
	cut3_a = -1.169; cut3_c = +1.154; cut3_si = 0.08;
	cut4_a = -1.144; cut4_c = +0.030; cut4_si = 0.08;

	selectionRangesX["L_1_F"] = SelectionRange(9.8, 16.5);
	selectionRangesX["L_1_N"] = SelectionRange(7.1, 14.5);
	selectionRangesX["R_1_N"] = SelectionRange(8.0, 15.5);
	selectionRangesX["R_1_F"] = SelectionRange(7.0, 14.5);
}
