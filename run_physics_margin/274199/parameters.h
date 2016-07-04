void ApplySettings()
{
	n_si = 3.;

	cut1_a = -1.007; cut1_c = -2.354; cut1_si = 0.20;
	cut2_a = -0.975; cut2_c = +0.498; cut2_si = 0.21;

	selectionRangesX["L_1_F"] = SelectionRange(9.2, 15.5);
	selectionRangesX["L_1_N"] = SelectionRange(7.2, 13.4);
	selectionRangesX["R_1_N"] = SelectionRange(7.4, 14.0);
	selectionRangesX["R_1_F"] = SelectionRange(7.0, 14.0);
}
