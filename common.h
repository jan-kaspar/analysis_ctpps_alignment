#include <map>
#include <vector>
#include <string>

#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------

struct TrackData
{
	bool valid = false;
	double x = 0.;
	double y = 0.;

	void operator= (const TotemRPLocalTrack &ft)
	{
		valid = ft.isValid();
		x = ft.getX0();
		y = ft.getY0();
	}
};

//----------------------------------------------------------------------------------------------------

struct TrackDataCollection : public map<unsigned int, TrackData>
{
};

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

unsigned int lhc_fill = 0;

bool alignment_run = false;

double n_si = 3.;

bool cut1_apply = true;
bool cut2_apply = true;
bool cut3_apply = false;
bool cut4_apply = false;

double cut1_a = -1., cut1_c = 0., cut1_si = 0.20;
double cut2_a = -1., cut2_c = 0., cut2_si = 0.20;
double cut3_a = -1., cut3_c = 0., cut3_si = 0.20;
double cut4_a = -1., cut4_c = 0., cut4_si = 0.20;

std::map<std::string, SelectionRange> selectionRangesX;
