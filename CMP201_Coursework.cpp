#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <complex>
#include <fstream>
#include "CMP201_Coursework.h"

using std::string;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::complex;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istreambuf_iterator;
typedef std::chrono::steady_clock the_clock;
/*
	This program tests the performance of the KMP and brute force string searching algorithms,
	both algorithms need to match a 14 character pattern that only occurs at the end of text files containing,
	DNA sequences of different lengths ( 1 million, 10 million and 20 million),
	the results are saved to a svg file
*/
int bruteForce(string keyword, string text)
{
	int keywordLen = keyword.length();
	int textLen = text.length();
	int i, j;

	for (i = 0, j = 0; i < textLen && j < keywordLen; i++)
	{
		if (keyword[j] == text[i])
			j++;
		
		else 
		{
			i -= j;
			j = 0;
		}
	}

	if (j == keywordLen)
		return i - j;
	else
		return -1;
}

int knuthMorrisPratt(string keyword, string text)
{
	int keywordLen = keyword.length();
	int textLen = text.length();

	// CREATING PREFIX/SUFFIX TABLE FOR PATTERN
	
	std::vector<int> fTable (keyword.length());
	for (int i = 0; i < keywordLen; i++)
		fTable[i] = 0;

	int i = 0; // text iterator
	int j = 1; // keyword iterator

	while (j < keywordLen)
	{
		if (keyword[i] == keyword[j])
		{
			fTable[j] = i + 1;
			i++;
			j++;
		}
		else
		{
			if (i > 0)
				i = fTable[i - 1];
			else
			{
				fTable[j] = 0;
				j++;
			}
		}
	}

	// SEARCH 
	i, j = 0;
	while (i < textLen)
	{
		if (keyword[j] == text[i])
		{
			i++;
			j++;

			if (j == keywordLen) // match found
			{
				return i - j;
			}
		}

		else
		{
			if (j > 0) // if we have previously matched characters
			{
				j = fTable[j - 1];
			}

			else
			{
				i++;
			}
		}
	}
	return -1;
}

void Benchmark(string pattern, string &contents, std::stringstream &fileToWrite, string currentFile)
{
	// runs both algorithms against 1m, 10m and 20m text length
	for (int i = 0; i < 10; i++) // changed to 10, originally ran 100 times for sample collection
	{
		the_clock::time_point start = the_clock::now();
		bruteForce(pattern, contents);
		the_clock::time_point end = the_clock::now();

		auto time_taken = duration_cast<milliseconds>(end - start).count();
		cout << "\nBrute Force took: " << time_taken << " ms." << endl;
		fileToWrite << "kmp" << "," << time_taken << "," << currentFile << endl;

		start = the_clock::now();
		knuthMorrisPratt(pattern, contents);
		end = the_clock::now();

		time_taken = duration_cast<milliseconds>(end - start).count();
		cout << "\nKMP took: " << time_taken << " ms." << endl;
		fileToWrite << "bruteforce" << "," << time_taken << "," << currentFile << endl;
	}
}

void LoadFile1(string &contents)
{
	std::ifstream in("sequence1m.txt", std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
	}
}

void LoadFile2(string &contents)
{
	std::ifstream in("sequence10m.txt", std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
	}
}

void LoadFile3(string &contents)
{
	std::ifstream in("sequence20m.txt", std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
	}
}

int main()
{
		string contents;

		std::stringstream fileToWrite;
		fileToWrite << "algorithm, time (ms), file" << endl;

		LoadFile1(contents);
		Benchmark("tgttaaatgatttt", contents, fileToWrite, "1"); // 1 million

		LoadFile2(contents);
		Benchmark("tgttaaatgatttt", contents, fileToWrite, "2"); // 10 million

		LoadFile3(contents);
		Benchmark("tgttaaatgatttt", contents, fileToWrite, "3"); // 20 million

		ofstream file("benchmark.csv");
		file << fileToWrite.str();
		file.close();
}
