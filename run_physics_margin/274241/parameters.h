void ApplySettings()
{
	cut1_a = -1.008; cut1_c = -2.322; cut1_si = 0.20;
	cut2_a = -0.971; cut2_c = +0.462; cut2_si = 0.20;
	cut3_a = -1.151; cut3_c = +1.229; cut3_si = 0.15;
	cut4_a = -1.144; cut4_c = +0.094; cut4_si = 0.15;

	selectionRangesX["L_1_F"] = SelectionRange(9.6, 15.5);
	selectionRangesX["L_1_N"] = SelectionRange(7.0, 13.0);
	selectionRangesX["R_1_N"] = SelectionRange(8.0, 13.5);
	selectionRangesX["R_1_F"] = SelectionRange(7.0, 13.0);
}
