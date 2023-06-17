#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Webpage
{
public:
	string name;
	double pagerank;
	int index;
	static int count;
	static double max;
	static double min;
	int impressions;
	int clicks;
	double ctr;
	double score;
	double NormPR;
	
	Webpage(string x)
	{
		name = x;
		count++;
		index = count;
	}
	Webpage()
	{
		index = ++count;
	}
	void SetName(string x)
	{
		name = x;
	}
	void SetCTR()
	{
		ctr = double(clicks / impressions);
	}
	void SetNormPR()
	{
		NormPR = (pagerank - min) / (max - min);
	}
	void setScore()
	{
		this->SetCTR();
		this->SetNormPR();
		double calc = (0.1 * impressions) / (1 + 0.1 * impressions);
		score = 0.4 * NormPR + (calc * NormPR + calc * ctr) * 0.6;
	}
};

