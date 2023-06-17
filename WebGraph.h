#pragma once
#include "Webpage.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;
const int Count = 30;
class WebGraph
{
	public:
		double Graph[Count][Count];
		vector<double> pagerank;
		vector <Webpage> webpages;
		vector <Webpage> results;
		WebGraph()
		{
			pagerank.resize(Count);
			for (int i = 0; i < Count; i++)
				for (int j = 0; j < Count; j++)
					Graph[i][j] = 0;
			for (int i = 0; i < Count; i++)
			{
				pagerank[i] = 1.0 / Count;
			}
		}
		void SetGraphImpressionsClicks(vector <Webpage> websites)
		{
			webpages = websites;
			ifstream  ctrFile, cFile, graph;
			ctrFile.open("Number of Impressions file.csv");
			graph.open("Web Graph file.csv");
			cFile.open("Number of Clicks file.csv");
			string website, comma, imp, c_num;
			string x, y;
			while (graph >> x)
			{
				graph >> comma;
				graph >> y;
				for (int i = 0; i < webpages.size(); i++)
				{
					if (webpages[i].name == x)
					{
						for (int j = 0; j < webpages.size(); j++)
							if (webpages[j].name == y)
							{
								Graph[webpages[j].index][webpages[i].index] = 1;
							}
					}
				}
			}
			int j = 0;
			while (cFile >> website)
			{
				cFile >> comma >> c_num;
				webpages[j].clicks = stoi(c_num);
				j++;
			}
			int k = 0;
			while (ctrFile >> website && k < Count)
			{
				ctrFile >> comma;
				ctrFile >> imp;
				webpages[k].impressions = stoi(imp);
				k++;
			}
			graph.close();
			ctrFile.close();
			cFile.close();
		}

		vector<vector<double>> MatrixH()
		{
			vector<double> counter(Count);
			vector<vector<double>> H(Count, vector<double>(Count));
			for (int j = 0; j < Count; j++)
				for (int i = 0; i < Count; i++)
					if (Graph[i][j] == 1)
						counter[j]++;
			for (int j = 0; j < Count; j++)
				for (int i = 0; i < Count; i++)
				{
					if (Graph[i][j] == 1)
						H[i][j] = 1.0 / counter[j];
				}
			return H;
		}
		void PageRank()
		{
			vector<vector<double>> result(Count, vector<double>(1));
			vector<vector<double>> power = PowMatrix(MatrixH(), 2);
			vector<vector<double>> pagerankMatrix(Count, vector<double>(1));
			for (int i = 0; i < Count; i++)
				pagerankMatrix[i][0] = pagerank[i];
			result = MatrixMult(power, pagerankMatrix, Count, 1, Count);
			for (int i = 0; i < Count; i++)
			{
				if (webpages[i].clicks != 0)
					pagerank[i] = result[i][0];
				webpages[i].pagerank = pagerank[i];
			}
		}
		vector<vector<double>> PowMatrix(vector<vector<double>> matrix, int n)
		{
			vector<vector<double>> result(Count, vector<double>(Count));
			result = matrix;
			while (n > 0)
			{
				if (n % 2 == 1)
				{
					result = MatrixMult(result, matrix, Count, Count, Count);
					n = n - 1;
				}
				n /= 2;
				matrix = MatrixMult(matrix, matrix, Count, Count, Count);
			}
			return result;
		}
		vector<vector<double>> MatrixMult(vector<vector<double>> X, vector<vector<double>> Y, int rx, int cy, int common)
		{	
			vector<vector<double>> product(Count, vector<double>(Count));
			for (int i = 0; i < rx; i++)
				for (int j = 0; j < cy; j++)
				{
					product[i][j] = 0;
					for (int k = 0; k < common; k++)
						product[i][j] += X[i][k] * Y[k][j];
				}
			return product;
		}
		void FindMaxMinPR()
		{
			for (int i = 0; i < Count; i++)
				webpages[i].setScore();
			for (int i = 0; i < Count; i++)
			{
				if (webpages[0].max < webpages[i].pagerank)
					webpages[0].max = webpages[i].pagerank;
				if (webpages[0].min > webpages[i].pagerank)
					webpages[0].min = webpages[i].pagerank;
			}
		}
		void SortScores()
		{
			sort(results.begin(), results.end(), [](Webpage a, Webpage b) {return a.score > b.score; });
		}
		void FilterResults(vector<string> words)
		{
			FindMaxMinPR();
			int counter = 0;
			results = webpages;
			for (int i = 0; i < results.size(); i++)
			{
				counter = 0;
				for (int j = 0; j < words.size(); j++)
				{
					if (results[i].name != words[j])
						counter++;
					if (counter == words.size())
					{
						results.erase(results.begin() + i);
						i--;
					}
				}
			}
			SortScores();
			
		}
		vector <Webpage> GetWebPages()
		{
			return results;
		}
		vector<Webpage> UpdateImpressions()
		{
			fstream impressFile("Number of Impressions file.csv");
			for (int i = 0; i < webpages.size(); i++)
				for (int j = 0; j < results.size(); j++)
				{
					if (webpages[i].name == results[j].name)
					{
						(webpages[i].impressions)++;
						webpages[i].setScore();
						break;
					}
				}
			SortScores();
			for (int i = 0; i < webpages.size(); i++)
				impressFile << webpages[i].name << " , " << webpages[i].impressions << endl;

			impressFile.close();
			return results;
		}
};