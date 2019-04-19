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
	int varc;
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
	ofstream myfile;
	myfile.open("example.txt");
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
	cout << "area:" << area << "sqrt" << sqrt(area);
	initialW = sqrt(area);
	/*int pps = (pin_count / 4);
	if ( (initialW / pps) >= (min (pin_height, pin_width) ) )
		core_const = 1;
	else
		die_cosnt = 1;

	if (die_cosnt)
		W = 4 * min(pin_height, pin_width);
	else
		W = initialW;

	cout << "w:" << W;
	*/
	//W = initialW;
	W = sqrt(area);
	cout << "W:" << W;

	IloEnv env;
	
		IloModel model(env);
		IloNumVarArray vars(env);
		//IloNumVarArray x(env);
		//IloNumVarArray y(env);
		//IloNumVarArray z(env);
		//IloNumVarArray xc(env);
		//IloNumVarArray yc(env);
		//IloNumVarArray res(env);
		IloNumVar a(env);
		//res.add(IloNumVar(env));
		vars.add(IloNumVar(env));
		model.add(IloMinimize(env, vars[0]));
		IloRangeArray c;
		for (int i = 1; i <= hard_count; i++)
		{
			//x.add(IloNumVar(env, 0, 40));
			//x.add(IloNumVar(env));

			//y.add(IloNumVar(env, 0, 40));
			//y.add(IloNumVar(env));
			//z.add(IloNumVar(env));
			/*vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			model.add(vars[i] >= 0);
			myfile << "x" << i << " >= 0" << endl;
			model.add(vars[i+1] >= 0);
			myfile << "y" << i << " >=0" << endl;
			model.add(vars[i] + vars[i+2] * (hmp[i-1].height) + (1 - vars[i+2])*(hmp[i-1].width) <= W);
			myfile << "x" << i << " + "<< hmp[i-1].height << " z"<< i << " + " << hmp[i-1].width <<" - " << hmp[i-1].width << " z" << i << " <= " << W<<endl ;
			model.add(vars[i+1] + vars[i+2] * (hmp[i-1].width) + (1 - vars[i+2])*(hmp[i-1].height) <= vars[0]);
			myfile << "y" << i << " + " << hmp[i - 1].width << " z" << i << " + " << hmp[i - 1].height << " - " << hmp[i - 1].height << " z" << i << " <= " << "Y" << endl;
			cout << i << endl;*/
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			model.add(vars[i] >= 0);
			myfile << "x" << i << " >= 0" <<";"<< endl;
			model.add(vars[i + 1] >= 0);
			myfile << "y" << i << " >= 0" << ";" << endl;
			model.add(vars[i] + hmp[i - 1].width <= W);
			myfile << "x" << i << " + "  << hmp[i - 1].width <<" <= " << W << ";" << endl;
			model.add(vars[i + 1] + hmp[i - 1].height <= vars[0]);
			myfile << "y" << i << " + " << hmp[i - 1].height <<  " <= " << "Y" << ";" << endl;
			cout << i << endl; 
		}
		int ct = hard_count;
		/*for (auto it1 = hmp.begin(); it1 != hmp.end(); ++it1)
			for (auto it2 = hmp.begin(); it2 != hmp.end(); ++it2)*/
		for (int i = 1; i <= hard_count - 1; i++)
			for (int j = i+1; j <= hard_count; j++)
			{
				vars.add(IloNumVar(env));
				vars.add(IloNumVar(env));
				//xc.add(IloNumVar(env));
				//yc.add(IloNumVar(env));
				/*model.add(x[i] + z[i] * (hmp[i].height) + (1 - z[i])*(hmp[i].width) <= x[j] + W * (xc[ct] + yc[ct]));
				model.add(y[i] + z[i] * (hmp[i].width) + (1 - z[i])*(hmp[i].height) <= y[j] + W * (1 + xc[ct] - yc[ct]));
				model.add(x[j] + z[j] * (hmp[j].height) + (1 - z[j])*(hmp[j].width) <= x[i] + W * (1 - xc[ct] + yc[ct]));
				model.add(y[j] + z[j] * (hmp[j].width) + (1 - z[j])*(hmp[j].height) <= y[i] + W * (2 - xc[ct] - yc[ct]));*/
			/*	model.add(vars[i] +vars[i+2] * (hmp[i].height) + (1 - vars[i+2])*(hmp[i].width) <= vars[j] + W * (vars[ct] + vars[ct+1]));
				myfile << "x" << i << " + " << hmp[i - 1].height << " z" << i << " + " << hmp[i - 1].width << " - " << hmp[i - 1].width << " z" << i << " <= " << "x" <<j << " + " << W << " x" <<i<<j << " + " << W << " y" << i << j << endl;
				model.add(vars[i+1] + vars[i+2] * (hmp[i].width) + (1 - vars[i+2])*(hmp[i].height) <= vars[j+1] + W * (1 + vars[ct] - vars[ct+1]));
				myfile << "y" << i << " + " << hmp[i - 1].width << " z" << i << " + " << hmp[i - 1].height << " - " << hmp[i - 1].height << " z" << i << " <= " << "y" << j << " + "<< W <<" + " << W << " x" << i << j << " - "<< W << " y" << i << j << endl;
				model.add(vars[j] + vars[j+2] * (hmp[j].height) + (1 - vars[j+2])*(hmp[j].width) <= vars[i] + W * (1 - vars[ct] + vars[ct+1]));
				myfile << "x" << j << " + " << hmp[j - 1].height << " z" << j << " + " << hmp[j - 1].width << " - " << hmp[j - 1].width << " z" << j << " <= " << "x" << i << " + " << W << " - "<< W << " x" << i << j << " + " << W << " y" << i << j << endl;
				model.add(vars[j+1] + vars[j+2] * (hmp[j].width) + (1 - vars[j+2])*(hmp[j].height) <= vars[i+1] + W * (2 - vars[ct] - vars[ct+1]));
				myfile << "y" << j << " + " << hmp[j - 1].width << " z" << j << " + " << hmp[j - 1].height << " - " << hmp[j - 1].height << " z" << i << " <= " << "y" << i << " + " << "2 "<< W << " - " << W << " x" << i << j << " - " << W << " y" << i << j << endl;
				ct++;
*/
				model.add(vars[i] + hmp[i-1].width <= vars[j] + W * (vars[ct] + vars[ct + 1]));
				myfile << "x" << i << " + " << hmp[i - 1].width << " <= " << "x" << j << " + " << W << " x" << i << j << " + " << W << " y" << i << j << ";" << endl;
				model.add(vars[i+1] + hmp[i-1].height<= vars[j + 1] + W * (1 + vars[ct] - vars[ct + 1]));
				myfile << "y" << i << " + " << hmp[i - 1].height <<  " <= " << "y" << j << " + " << W << " + " << W << " x" << i << j << " - " << W << " y" << i << j << ";" << endl;
				model.add(vars[j] + hmp[j-1].width <= vars[i] + W * (1 - vars[ct] + vars[ct + 1]));
				myfile << "x" << j << " + " << hmp[j - 1].width << " <= " << "x" << i << " + " << W << " - " << W << " x" << i << j << " + " << W << " y" << i << j << ";" << endl;
				model.add(vars[j + 1] + hmp[j-1].height<= vars[i + 1] + W * (2 - vars[ct] - vars[ct + 1]));
				myfile << "y" << j << " + " << hmp[j - 1].height << " <= " << "y" << i << " + " << "2 " << W << " - " << W << " x" << i << j << " - " << W << " y" << i << j << ";" << endl;
				ct++;
			}

		
		IloCplex cplex(model);
		cplex.solve();
		//cout << " min =" << cplex.getObjValue() << endl;
		cout << cplex.getStatus();
		if (cplex.getStatus() == IloAlgorithm::Optimal) {
			IloNumArray vals(env);
			//cplex.getValues(vals, vars);
			//env.out() <<"value: "<< vars[0];
			//env.out() << "Values = " << vals << endl;
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
	/*string file_name = "modules.txt";
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
	return 0;*/
	float utilization = 0.6;
	float aspect_min = 0.5;
	float aspect_max = 2;
	int WafterU;
	int pin_max;
	int pins_side;
	int pins_min_width;
	int init_W;
	string file_name = "modules.txt";
	unordered_map < int, HardR> HM;
	unordered_map < int, SoftR> SM;
	int area = 0;
	int totalw = 0;
	int W;
	int hard_count = 0, soft_count = 0;
	int pin_count = 0, pin_height = 0, pin_width = 0, inner_dim = 0, outer_dim = 0;
	parser(file_name, HM, SM, pin_count, pin_width, pin_height, hard_count, soft_count);
	cout << "hard:" << hard_count << endl;
	cout << "soft:" << soft_count << endl;

	for (auto it = HM.begin(); it != HM.end(); ++it)
	{
		int h = it->second.height;
		int w = it->second.width;
		area += (h*w);
		totalw += w;
	}
	for (auto it = SM.begin(); it != SM.end(); ++it)
	{
		
		area += it->second.area;
	}
	
	init_W = ceil(sqrt(area));
	WafterU = init_W / utilization;
	pin_max = max(pin_height, pin_width);
	pins_side = ceil(pin_count / 4);
	pins_min_width = pins_side * pin_max;
	W = max(WafterU, pins_min_width);
	//W = totalh;
	cout << "W = " << W;
	IloEnv env;
	stringstream logfile;

	try {
		IloModel model(env);
		IloNumVarArray vars(env);
		IloRangeArray con(env);
		IloCplex cplex(model);
		ofstream LogFile("LogFile.txt");
		ofstream Results("Results.txt");
		cplex.setOut(LogFile);
		// Set options of IP solver
		cplex.setParam(IloCplex::TiLim, 100.000);
		cplex.setParam(IloCplex::Threads, 1);
		cplex.setParam(IloCplex::EpGap, 0.0);
		cplex.setParam(IloCplex::EpAGap, 0.0);
		vars.add(IloIntVar(env));
		IloRangeArray c(env);
		
		// Variables
		for (int i = 1; i <= hard_count*2; i+=2)
		{
			//cout << "ii=" << i << endl;
			//cout << "h=" << HM[i / 2].height<<endl;
			//cout << "w=" << HM[i / 2].width<<endl;
			vars.add(IloIntVar(env));
			vars.add(IloIntVar(env));
			c.add(vars[i] >= 0);
			c.add(vars[i + 1] >= 0);
			c.add(vars[i] + HM[i/2].width <= W);
			c.add(vars[i + 1] + HM[i/2].height - vars[0]<= 0);
		}
		
		int ct = hard_count*2+1;
		for (int i = 1; i <= hard_count*2 - 2; i+=2)
			for (int j = i + 2; j <= hard_count * 2; j += 2)
			{
				vars.add(IloIntVar(env));
				vars.add(IloIntVar(env));
				//cout << "i=" << i<<endl;
				//cout << "j=" << j<<endl;
				c.add(vars[i] + HM[i/2].width - (vars[j] + W * (vars[ct] + vars[ct + 1]))  <= 0);
				c.add(vars[i + 1] + HM[i/2].height - (vars[j + 1] + W * (1 + vars[ct] - vars[ct + 1])) <= 0 );
				c.add(vars[j] + HM[j/2].width - (vars[i] + W * (1 - vars[ct] + vars[ct + 1])) <= 0 );
				c.add(vars[j + 1] + HM[j/2].height - (vars[i + 1] + W * (2 - vars[ct] - vars[ct + 1])) <= 0 );
				c.add(((vars[ct] == 1) + (vars[ct] == 0)) == 1);
				c.add(((vars[ct+1] == 1) + (vars[ct+1] == 0)) == 1);
				ct+=2;
				cout << "ct=" << ct << endl;
			}
		cout << "final_ct" << ct;
		// x -> i , y -> i+1 , w -> i+2 , delta -> i+3 , c -> i+4
		for (int i = ct; i < (ct+ soft_count * 4); i += 4)
		{
			int iarea;
			float wmin_i, wmax_i, hmin_i, hmax_i, deltai , ci;
			int wmin, wmax, hmin, hmax;
			cout << "ii=" << i << endl;
			iarea = SM[(i - ct)/4].area;
			wmin_i = sqrt(iarea*aspect_min);
			wmax_i = sqrt(iarea*aspect_max);
			hmax_i = iarea / wmax_i + (wmax_i - wmin_i) * (iarea / (wmax_i*wmax_i));
			hmin_i = iarea / wmax_i + (wmax_i - wmax_i) * (iarea / (wmax_i*wmax_i));
			wmin = ceil(wmin_i);
			wmax = ceil(wmax_i);
			hmin = ceil(hmin_i);
			hmax = ceil(hmax_i);
			deltai = (hmin_i - hmax_i) / (wmax_i - wmin_i);
			ci = hmax_i - deltai * wmin_i;
			vars.add(IloIntVar(env));
			vars.add(IloIntVar(env));
			vars.add(IloIntVar(env));
			vars.add(IloIntVar(env));
			c.add(vars[i] >= 0);
			c.add(vars[i + 1] >= 0);
			c.add(vars[i] + vars[i+2] <= W);
			c.add(vars[i + 1] +vars[i+3] - vars[0] <= 0);
			c.add(vars[i + 2] >= wmin_i);
			c.add(vars[i + 2] <= wmax_i);
			c.add(vars[i + 3] >= hmin_i);
			c.add(vars[i + 3] <= hmax_i);
			c.add((vars[i + 3] - vars[i + 2] * deltai - ci) == 0);
		}
		
		int cts = ct + soft_count*4 ;
		cout << "cts:" << cts;
		for (int i = ct; i < (ct + soft_count*4 - 4); i += 4)
			for (int j = i + 4; j < (ct + soft_count * 4); j += 4)
			{
				int iarea;
				float wmin_i, wmax_i, hmin_i, hmax_i, deltai, ci;
				int wmin, wmax, hmin, hmax;
				iarea = SM[(i - ct)/4].area;
				wmin_i = sqrt(iarea*aspect_min);
				wmax_i = sqrt(iarea*aspect_max);
				hmax_i = iarea / wmax_i + (wmax_i - wmin_i) * ( iarea/ (wmax_i*wmax_i) );
				hmin_i = iarea / wmax_i + (wmax_i - wmax_i) * (iarea / (wmax_i*wmax_i));
				deltai = (hmin_i - hmax_i) / (wmax_i - wmin_i);
				ci = hmax_i - deltai * wmin_i;

				int jarea;
				float wmin_j, wmax_j, hmin_j, hmax_j, deltaj, cj;
				iarea = SM[(j - ct)/4].area;
				wmin_j = sqrt(jarea*aspect_min);
				wmax_j = sqrt(jarea*aspect_max);
				hmax_j = jarea / wmax_j + (wmax_j - wmin_j) * (jarea / (wmax_j*wmax_j));
				hmin_j = jarea / wmax_j + (wmax_j - wmax_j) * (iarea / (wmax_j*wmax_j));
				


				vars.add(IloIntVar(env));
				vars.add(IloIntVar(env));
				cout << "i=" << i << endl;
				cout << "j=" << j << endl;
				cout << "ct=" << ct << endl;
				c.add(vars[i] + vars[i+2] - (vars[j] + W * (vars[cts] + vars[cts + 1])) <= 0);
				c.add(vars[i + 1] + vars[i+3] - (vars[j + 1] + W * (1 + vars[cts] - vars[cts + 1])) <= 0);
				c.add(vars[j] + vars[j+2] - (vars[i] + W * (1 - vars[cts] + vars[cts + 1])) <= 0);
				c.add(vars[j + 1] + vars[j+3] - (vars[i + 1] + W * (2 - vars[cts] - vars[cts + 1])) <= 0);
				c.add(((vars[cts] == 1) + (vars[cts] == 0)) == 1);
				c.add(((vars[cts + 1] == 1) + (vars[cts + 1] == 0)) == 1);
				cts += 2;
			}

		model.add(c);
		model.add(IloMinimize(env, vars[0]));

		// Solve
		cplex.solve();
		if (cplex.getStatus() == IloAlgorithm::Optimal) {
			IloNumArray vals(env);
			cplex.getValues(vals, vars);
			Results << "Values = " << vals << endl;
		}
		else
		{
			cout << "couldn't find a solution, please change the utilization factor"<<endl;
		}
	}
	catch (IloException& e) {
		cerr << "C-Exp: " << e << endl;
	}
	catch (...) {
		cerr << "Unknown Exception" << endl;
	}


	const string str = logfile.str();
	cout << str << endl;
	env.end();
	system("pause");
	return 0;
}

