void ApplySettings()
{
	lhc_fill = 5029;

	cut1_a = -1.015; cut1_c = -1.007; cut1_si = 0.20;
	cut2_a = -0.987; cut2_c = +0.657; cut2_si = 0.20;
	cut3_a = -1.163; cut3_c = +0.550; cut3_si = 0.15;
	cut4_a = -1.142; cut4_c = +0.039; cut4_si = 0.15;

	selectionRangesX["L_1_F"] = SelectionRange(12.2, 16.5);
	selectionRangesX["L_1_N"] = SelectionRange(11.1, 14.5);
	selectionRangesX["R_1_N"] = SelectionRange(10.2, 15.5);
	selectionRangesX["R_1_F"] = SelectionRange(9.6, 14.5);

	yAlignmentRangeX["L_1_F"] = SelectionRange(8.0, 12.0);
	yAlignmentRangeX["L_1_N"] = SelectionRange(8.0, 12.0);
	yAlignmentRangeX["R_1_N"] = SelectionRange(5.0, 10.5);
	yAlignmentRangeX["R_1_F"] = SelectionRange(4.0, 11.0);

	RelAlignmentRangeX["L_1_F"] = SelectionRange(8.0, -1.);
	RelAlignmentRangeX["L_1_N"] = SelectionRange(8.0, -1.);
	RelAlignmentRangeX["R_1_N"] = SelectionRange(6.0, 6.8);
	RelAlignmentRangeX["R_1_F"] = SelectionRange(6.0, 6.8);
}
