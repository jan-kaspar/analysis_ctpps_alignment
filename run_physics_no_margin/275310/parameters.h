void ApplySettings()
{
	cut1_a = -1.012; cut1_c = -1.154; cut1_si = 0.20;
	cut2_a = -0.977; cut2_c = +0.541; cut2_si = 0.20;
	cut3_a = -1.164; cut3_c = +0.673; cut3_si = 0.08;
	cut4_a = -1.138; cut4_c = +0.043; cut4_si = 0.08;

	selectionRangesX["L_1_F"] = SelectionRange(11.5, 16.5);
	selectionRangesX["L_1_N"] = SelectionRange(10.0, 14.5);
	selectionRangesX["R_1_N"] = SelectionRange( 9.5, 15.5);
	selectionRangesX["R_1_F"] = SelectionRange( 9.0, 14.5);
}
