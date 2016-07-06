void ApplySettings()
{
	n_si = 3.;

	cut1_a = -1.007; cut1_c = -2.259; cut1_si = 0.20;
	cut2_a = -0.970; cut2_c = +0.434; cut2_si = 0.21;

	selectionRangesX["L_1_F"] = SelectionRange(9.8, 15.5);
	selectionRangesX["L_1_N"] = SelectionRange(7.1, 13.5);
	selectionRangesX["R_1_N"] = SelectionRange(8.0, 14.5);
	selectionRangesX["R_1_F"] = SelectionRange(7.0, 13.5);
}
