#include <ilcplex/ilocplex.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include<string>
#include<vector>
#include<istream>
#include<unordered_map>
#include<math.h>
#include <algorithm>
#include <ctime>
#include<fstream>



//#include "lp_lib.h"
//#include <stdio.h>
using namespace std;
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

void parser(string filename, unordered_map<int, HardR> & hard_map, unordered_map<int, SoftR> & soft_map, int & pin_count, int & pin_width, int & pin_height, int &hard_counter , int &soft_counter)
{
	string mod_text;
	ifstream input;
	 hard_counter = 0;
	 soft_counter = 0;
	int counter = 0;
	input.open(filename);

	if (!input.is_open()) {
		cout << "error in opening file\n ";
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
			temp.width = stoi(components.at(2));
			temp.height = stoi(components.at(3));
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
void HardRectangles(unordered_map < int, HardR> hmp , unordered_map < int, SoftR> smp , int pin_count , int pin_height ,int pin_width , int hard_count , int soft_ount) {
	int area = 0;
	int W;
	bool core_const = false;
	bool die_cosnt = false;
	float initialW;
	for (auto it = hmp.begin(); it != hmp.end(); ++it)
	{
		int h = it->second.height;
		int w = it->second.width;
		area += (h*w);
	}
	for (auto it = smp.begin(); it != smp.end(); ++it)
	{
		int a = it->second.area;
		area += a;
	}

	initialW = sqrt(area);
	int pps = (pin_count / 4);
	if ( (initialW / pps) >= (min (pin_height, pin_width) ) )
		core_const = 1;
	else
		die_cosnt = 1;

	if (die_cosnt)
		W = 4 * min(pin_height, pin_width);
	else
		W = initialW;


	IloEnv env;
	
		IloModel model(env);
		IloNumVarArray x(env);
		IloNumVarArray y(env);
		IloNumVarArray z(env);
		IloNumVarArray xc(env);
		IloNumVarArray yc(env);
		IloNumVarArray res(env);
		IloNumVar a(env);
		res.add(IloNumVar(env));
		for (int i = 0; i < hard_count; i++)
		{
			//x.add(IloNumVar(env, 0, 40));
			x.add(IloNumVar(env));

			//y.add(IloNumVar(env, 0, 40));
			y.add(IloNumVar(env));
			z.add(IloNumVar(env));

			model.add(x[i] >= 0);
			model.add(y[i] >= 0);
			model.add(x[i] + z[i] * (hmp[i].height) + (1 - z[i])*(hmp[i].width) <= W);
			model.add(y[i] + z[i] * (hmp[i].width) + (1 - z[i])*(hmp[i].height) <= a);

			cout << i << endl;
		}
		int ct = 0;
		/*for (auto it1 = hmp.begin(); it1 != hmp.end(); ++it1)
			for (auto it2 = hmp.begin(); it2 != hmp.end(); ++it2)*/
		for (int i = 0; i < hard_count - 1; i++)
			for (int j = i; j < hard_count; j++)
			{
				xc.add(IloNumVar(env));
				yc.add(IloNumVar(env));
				model.add(x[i] + z[i] * (hmp[i].height) + (1 - z[i])*(hmp[i].width) <= x[j] + W * (xc[ct] + yc[ct]));
				model.add(y[i] + z[i] * (hmp[i].width) + (1 - z[i])*(hmp[i].height) <= y[j] + W * (1 + xc[ct] - yc[ct]));
				model.add(x[j] + z[j] * (hmp[j].height) + (1 - z[j])*(hmp[j].width) <= x[i] + W * (1 - xc[ct] + yc[ct]));
				model.add(y[j] + z[j] * (hmp[j].width) + (1 - z[j])*(hmp[j].height) <= y[i] + W * (2 - xc[ct] - yc[ct]));

				ct++;

			}

		model.add(IloMinimize(env, a));
		IloCplex cplex(model);
		cplex.solve();
		cout << " min =" << cplex.getObjValue() << endl;
		if (cplex.getStatus() == IloAlgorithm::Optimal) {
			IloNumArray vals(env);
			cplex.getValues(vals, x);
			env.out() << "Values = " << vals << endl;
		}
	
	env.end();
}

//ILOSTLBEGIN

int main()
{
	//IloEnv env;
	//IloModel model(env);
	//IloNumVarArray x(env);
	//x.add(IloNumVar(env, 0, 40));
	//x.add(IloNumVar(env)); // default : between 0 and +∞
	//x.add(IloNumVar(env));
	//model.add(-x[0] + x[1] + x[2] <= 20);
	//model.add(x[0] - 3 * x[1] + x[2] <= 30);
	//model.add(IloMaximize(env, x[0] + 2 * x[1] + 3 * x[2]));
	//IloCplex cplex(model);
	//cplex.solve();
	//cout << " Max =" << cplex.getObjValue() << endl;
	//if (cplex.getStatus() == IloAlgorithm::Optimal) {
	//	IloNumArray vals(env);
	//	cplex.getValues(vals, x);
	//	env.out() << "Values = " << vals << endl;
	//	env.out() << x;
	//}
	//env.end();
	//system("pause");
	string file_name = "modules.txt";
	unordered_map < int, HardR> HM;
	unordered_map < int, SoftR> SM;
	int hard_count = 0, soft_count = 0;
	int pin_count = 0, pin_height = 0, pin_width = 0, inner_dim = 0, outer_dim = 0;
	parser(file_name, HM, SM, pin_count, pin_width, pin_height, hard_count, soft_count);
	for (auto it = HM.begin(); it != HM.end(); it++)
		cout << it->first << "," << it->second.name << "," << it->second.height << "," << it->second.width << endl;
	for (auto it = SM.begin(); it != SM.end(); it++)
		cout << it->first << "," << it->second.name << "," << it->second.area << endl;
	cout << "pins:" << pin_count << "," << pin_height << "," << pin_width << endl;
	HardRectangles(HM, SM, pin_count, pin_height , pin_width, hard_count , soft_count);
	//diePerimeter(pin_count, pin_width, pin_height, outer_dim, inner_dim);
	//cout << outer_dim << "," << inner_dim;
	system("pause");
	return 0;
}

