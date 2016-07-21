struct AlignmentResult
{
	double sh_x = 0.;		// mm

	AlignmentResult(double _sh_x=0.) : sh_x(_sh_x)
	{
	}

	void Write(FILE *f)
	{
		fprintf(f, "sh_x=%.3f\n", sh_x);
	}
};

//----------------------------------------------------------------------------------------------------

struct AlignmentResults : public map<unsigned int, AlignmentResult>
{
	void Write(FILE *f)
	{
		for (auto &p : *this)
		{
			fprintf(f, "id=%u,", p.first);
			p.second.Write(f);
		}
	}
};

//----------------------------------------------------------------------------------------------------

struct AlignmentResultsSet : public map<string, AlignmentResults>
{
	void Write(FILE *f)
	{
		for (auto &p : *this)
		{
			fprintf(f, "[%s]\n", p.first.c_str());
			p.second.Write(f);
		}
	}

	int Load(const string &fn)
	{
		FILE *f = fopen(fn.c_str(), "r");
		if (!f)
			return -1;

		return Load(f);

		fclose(f);
	}

	int Load(FILE *f)
	{
		// TODO
		return 0;
	}
};
