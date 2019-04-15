#include <iostream>
#include<string>
#include<vector>
#include<istream>
#include<unordered_map>
#include<math.h>
#include <algorithm>
//#include "lp_lib.h"
using namespace std;
#include<fstream>
struct HardR {
	string name;
	int height;
	int width;
};
struct SoftR {
	string name;
	int area;
	
};
void split(string str, vector<string> & vec)
{
	string word = "";
	for (auto x : str)
	{
		if (x == ' ')
		{
			vec.push_back(word);
			word = "";
		}
		else
		{
			word = word + x;
		}
	}
	vec.push_back(word);
}

void parser(string filename, unordered_map<int, HardR> & hard_map, unordered_map<int, SoftR> & soft_map , int & pin_count, int & pin_width, int & pin_height)
{
	string mod_text;
	ifstream input;
	int hard_counter = 0;
	int soft_counter = 0;
	input.open(filename);

	if (!input.is_open()) {
		cout<< "error in opening file\n" ;
	}
	while (!input.eof())
	{
		vector<string> components;
		getline(input, mod_text);
		split(mod_text, components);
		
		if (components.at(0) == ".hard_module")
		{
			HardR temp;
			temp.name = components.at(1);
			temp.height = stoi(components.at(2));
			temp.width = stoi(components.at(3));
			hard_map[hard_counter] = temp;
			hard_counter++;
		}
		else if (components.at(0) == ".soft_module")
		{
			SoftR temp;
			temp.name = components.at(1);
			temp.area = stoi(components.at(2));
			soft_map[soft_counter] = temp;
			soft_counter++;
		}
		else if (components.at(0) == ".pins")
		{
			pin_count = stoi(components.at(1));
			pin_width = stoi(components.at(2));
			pin_height = stoi(components.at(3));

		}
		else
			cout << "invalid input";
	}
}
void diePerimeter(int count, int width, int height, int & outer_dim, int & inner_dim)
{
	int rectPerside = ceil(count / 4.0);	// the number or rectangles per side rounded up
	int max_dim = max(width, height);		//the maximim dimension to get a square shape
	inner_dim = max_dim * rectPerside;
	outer_dim = inner_dim + 2 * max_dim;
	
}
void HardRectangles(unordered_map < int, HardR> mp) {
	int area = 0;
	float W;
	for (auto it = mp.begin(); it != mp.end(); ++it)
	{
		int h = it->second.height;
		int w = it->second.width;
		area += (h*w);
	}
	W = sqrt(area);
}
int main()
{
	string file_name = "modules.txt";
	unordered_map < int, HardR> HM;
	unordered_map < int, SoftR> SM;
	int pin_count = 0, pin_height = 0, pin_width = 0, inner_dim = 0, outer_dim = 0;
	parser(file_name, HM, SM ,pin_count , pin_width , pin_height );
	for (auto it = HM.begin(); it != HM.end(); it++)
		cout <<  it->first << "," << it->second.name << "," << it->second.height << "," << it->second.width << endl;
	for (auto it = SM.begin(); it != SM.end(); it++)
		cout << it->first << "," << it->second.name <<","<< it->second.area << endl;
	cout << "pins:" << pin_count << "," << pin_height << "," << pin_width<<endl;
	HardRectangles(HM);
	diePerimeter(pin_count, pin_width, pin_height, outer_dim, inner_dim);
	cout << outer_dim << "," << inner_dim;
	system("pause");
	return 0;
}