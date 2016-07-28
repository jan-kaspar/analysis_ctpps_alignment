#include <string>
#include <vector>

//----------------------------------------------------------------------------------------------------

std::vector<std::string> input_files;

std::string input_ntuple_name;

void InitInputFiles()
{
	input_ntuple_name = "TotemNtuple";

	input_files.clear();

	std::string prefix = "root://eostotem.cern.ch//eos/totem/data/ctpps/reduction/version1/274999/";

	//for (int idx = 0; idx <= 0; ++idx)
	for (int idx : { 0, 1, 7, 15, 16 })
	{
		char buf[100];
		sprintf(buf, "%i", idx);

		input_files.push_back(prefix + buf + "/reco.root");
	}
}
