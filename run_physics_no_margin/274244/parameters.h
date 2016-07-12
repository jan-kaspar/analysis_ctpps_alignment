void ApplySettings()
{
	cut1_a = -1.007; cut1_c = -2.327; cut1_si = 0.20;
	cut2_a = -0.964; cut2_c = +0.386; cut2_si = 0.20;
	cut3_a = -1.147; cut3_c = +1.149; cut3_si = 0.08;
	cut4_a = -1.142; cut4_c = +0.039; cut4_si = 0.08;

	selectionRangesX["L_1_F"] = SelectionRange(9.8, 15.8);
	selectionRangesX["L_1_N"] = SelectionRange(7.1, 13.0);
	selectionRangesX["R_1_N"] = SelectionRange(8.0, 14.0);
	selectionRangesX["R_1_F"] = SelectionRange(7.4, 13.5);
}
