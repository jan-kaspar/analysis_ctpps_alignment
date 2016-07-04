void ApplySettings()
{
	n_si = 3.;

	cut1_a = -1.003; cut1_c = +0.07; cut1_si = 0.18;
	cut2_a = -0.974; cut2_c = +0.15; cut2_si = 0.18;

	selectionRangesX["L_1_F"] = SelectionRange(3.5, 11.0);
	selectionRangesX["L_1_N"] = SelectionRange(3.7, 10.5);
	selectionRangesX["R_1_N"] = SelectionRange(2.0, 10.0);
	selectionRangesX["R_1_F"] = SelectionRange(2.0, 10.0);
}
