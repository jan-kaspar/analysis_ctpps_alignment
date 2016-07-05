void ApplySettings()
{
	n_si = 3.;

	cut1_a = -1.006; cut1_c = -2.343; cut1_si = 0.20;
	cut2_a = -0.975; cut2_c = +0.487; cut2_si = 0.21;

	selectionRangesX["L_1_F"] = SelectionRange(9.8, 15.8);
	selectionRangesX["L_1_N"] = SelectionRange(7.1, 13.0);
	selectionRangesX["R_1_N"] = SelectionRange(8.0, 14.0);
	selectionRangesX["R_1_F"] = SelectionRange(7.4, 13.5);
}
