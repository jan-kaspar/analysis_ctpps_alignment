#include <map>
#include <vector>
#include <string>

//----------------------------------------------------------------------------------------------------

struct SelectionRange
{
	double min;
	double max;

	SelectionRange(double _mi=0., double _ma=0.) : min(_mi), max(_ma)
	{
	}
};

//----------------------------------------------------------------------------------------------------

// parameter defaults

bool alignment_run = false;

double n_si = 3.;

bool cut1_apply = true;
bool cut2_apply = true;
bool cut3_apply = true;
bool cut4_apply = true;

double cut1_a = -1., cut1_c = 0., cut1_si = 0.20;
double cut2_a = -1., cut2_c = 0., cut2_si = 0.20;
double cut3_a = -1., cut3_c = 0., cut3_si = 0.20;
double cut4_a = -1., cut4_c = 0., cut4_si = 0.20;

std::map<std::string, SelectionRange> selectionRangesX;
