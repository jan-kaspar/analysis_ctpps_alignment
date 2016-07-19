void ApplySettings()
{
	cut1_a = -0.998; cut1_c = -1.554; cut1_si = 0.20;
	cut2_a = -0.962; cut2_c = +0.369; cut2_si = 0.20;
	cut3_a = -1.159; cut3_c = +0.986; cut3_si = 0.08;
	cut4_a = -1.142; cut4_c = +0.040; cut4_si = 0.08;

	selectionRangesX["L_1_F"] = SelectionRange(9.8, 16.5);
	selectionRangesX["L_1_N"] = SelectionRange(7.1, 14.5);
	selectionRangesX["R_1_N"] = SelectionRange(8.0, 15.5);
	selectionRangesX["R_1_F"] = SelectionRange(7.0, 14.5);
}
