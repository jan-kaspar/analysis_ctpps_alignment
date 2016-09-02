void ApplySettings()
{
	lhc_fill = 279794;

	cut1_a = -0.994; cut1_c = -0.819; cut1_si = 0.20;
	cut2_a = -0.994; cut2_c = -0.819; cut2_si = 0.20;
	cut3_a = -1.145; cut3_c = +0.087; cut3_si = 0.15;
	cut4_a = -1.145; cut4_c = +0.087; cut4_si = 0.15;

	selectionRangesX["L_1_F"] = SelectionRange(8.7, 16.5);
	selectionRangesX["L_1_N"] = SelectionRange(8.3, 15.7);
	selectionRangesX["R_1_N"] = SelectionRange(7.5, 15.5);
	selectionRangesX["R_1_F"] = SelectionRange(7.1, 15.0);
}
