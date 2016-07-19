void ApplySettings()
{
	cut1_a = -1.015; cut1_c = -0.852; cut1_si = 0.20;
	cut2_a = -0.977; cut2_c = +0.565; cut2_si = 0.20;
	cut3_a = -1.157; cut3_c = +0.350; cut3_si = 0.08;
	cut4_a = -1.142; cut4_c = +0.037; cut4_si = 0.08;

	selectionRangesX["L_1_F"] = SelectionRange(12.5, 16.5);
	selectionRangesX["L_1_N"] = SelectionRange(11.5, 14.5);
	selectionRangesX["R_1_N"] = SelectionRange(10.0, 15.5);
	selectionRangesX["R_1_F"] = SelectionRange( 9.0, 14.5);
}
