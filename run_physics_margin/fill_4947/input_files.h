#include <string>
#include <vector>

//----------------------------------------------------------------------------------------------------

std::vector<std::string> input_files;

void InitInputFiles()
{
	input_files.clear();

	std::string prefix = "root://eostotem.cern.ch//eos/totem/data/ctpps/reduction/version2/273730/";

	//for (int idx = 0; idx <= 0; ++idx)
	for (int idx : { 3, 4, 5, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 })
	{
		char buf[100];
		sprintf(buf, "%i", idx);

		input_files.push_back(prefix + buf + "/reco.root");
	}
}
