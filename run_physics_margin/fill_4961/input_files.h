#include <string>
#include <vector>

//----------------------------------------------------------------------------------------------------

std::vector<std::string> input_files;

void InitInputFiles()
{
	input_files.clear();

	std::string prefix = "root://eostotem.cern.ch//eos/totem/data/ctpps/reduction/version1/274199/";

	//for (int idx = 0; idx <= 0; ++idx)
	for (int idx : { 0, 2, 4, 5, 6, 7 })
	{
		char buf[100];
		sprintf(buf, "%i", idx);

		input_files.push_back(prefix + buf + "/reco.root");
	}
}
