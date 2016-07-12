void ApplySettings()
{
	cut1_a = -1.020; cut1_c = -0.912; cut1_si = 0.20;
	cut2_a = -0.983; cut2_c = +0.638; cut2_si = 0.20;
	cut3_a = -1.164; cut3_c = +0.509; cut3_si = 0.08;
	cut4_a = -1.138; cut4_c = +0.036; cut4_si = 0.08;

	selectionRangesX["L_1_F"] = SelectionRange(12.5, 15.5);
	selectionRangesX["L_1_N"] = SelectionRange(11.5, 13.5);
	selectionRangesX["R_1_N"] = SelectionRange(10.5, 14.5);
	selectionRangesX["R_1_F"] = SelectionRange(10.0, 13.5);
}
