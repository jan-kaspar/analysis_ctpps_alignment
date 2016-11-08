void ApplySettings()
{
	lhc_fill = 5021;

	cut1_a = -1.010; cut1_c = -1.259; cut1_si = 0.20;
	cut2_a = -0.980; cut2_c = +0.576; cut2_si = 0.20;
	cut3_a = -1.166; cut3_c = +0.741; cut3_si = 0.15;
	cut4_a = -1.146; cut4_c = +0.035; cut4_si = 0.15;

	selectionRangesX["L_1_F"] = SelectionRange(11.3, 16.5);
	selectionRangesX["L_1_N"] = SelectionRange(10.0, 14.5);
	selectionRangesX["R_1_N"] = SelectionRange(9.3, 15.5);
	selectionRangesX["R_1_F"] = SelectionRange(8.8, 14.5);
}
