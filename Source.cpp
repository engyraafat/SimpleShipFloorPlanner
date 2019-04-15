#include <iostream>
#include<string>
#include<vector>
#include<istream>
#include<unordered_map>
using namespace std;
#include<fstream>
struct HWpair {
	int height;
	int width;
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
void parser(string filename, unordered_map<string, HWpair> & mod_map)
{
	string mod_text;
	ifstream input;
	vector<string> components;
	input.open(filename);

	if (!input.is_open()) {
		throw invalid_argument{ "error in opening file\n" };
	}
	while (!input.eof())
	{
		getline(input, mod_text);
		split(mod_text, components);
		HWpair temp;
		temp.height = stoi(components.at(3));
		temp.width = stoi(components.at(4));
		mod_map[components.at(2)] = temp;
	}
}
int main()
{
	string file_name;
	unordered_map < string, HWpair> module_map;
	parser(file_name, module_map);

}