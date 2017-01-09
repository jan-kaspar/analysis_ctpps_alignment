void ApplySettings()
{
	lhc_fill = 5267;

	cut1_a = -0.999; cut1_c = -0.721; cut1_si = 0.20;
	cut2_a = -0.976; cut2_c = +0.481; cut2_si = 0.20;
	cut3_a = -1.126; cut3_c = +0.109; cut3_si = 0.15;
	cut4_a = -1.125; cut4_c = +0.044; cut4_si = 0.15;

	selectionRangesX["L_1_F"] = SelectionRange(9.2, 16.5);
	selectionRangesX["L_1_N"] = SelectionRange(8.3, 15.7);
	selectionRangesX["R_1_N"] = SelectionRange(12.5, 16.0);
	selectionRangesX["R_1_F"] = SelectionRange(12.0, 15.1);
}
