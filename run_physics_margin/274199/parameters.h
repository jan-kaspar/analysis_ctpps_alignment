void ApplySettings()
{
	cut1_a = -1.007; cut1_c = -2.354; cut1_si = 0.20;
	cut2_a = -0.975; cut2_c = +0.499; cut2_si = 0.20;
	cut3_a = -1.154; cut3_c = +1.243; cut3_si = 0.15;
	cut4_a = -1.137; cut4_c = +0.111; cut4_si = 0.15;

	selectionRangesX["L_1_F"] = SelectionRange(9.6, 15.2);
	selectionRangesX["L_1_N"] = SelectionRange(7.0, 12.7);
	selectionRangesX["R_1_N"] = SelectionRange(8.0, 13.5);
	selectionRangesX["R_1_F"] = SelectionRange(7.0, 12.5);
}
