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

double n_si = 3.;

double cut1_a = -1., cut1_c = 0., cut1_si = 0.20;
double cut2_a = -1., cut2_c = 0., cut2_si = 0.20;

std::map<std::string, SelectionRange> selectionRangesX;
