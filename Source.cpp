﻿#include <ilcplex/ilocplex.h>
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

		if (components.at(0) == ".hard_module") //hard_modules
		{
			HardR temp;
			temp.name = components.at(1);
			temp.width = stoi(components.at(2));
			temp.height = stoi(components.at(3));
			hard_map[hard_counter] = temp;
			hard_counter++;
		}
		else if (components.at(0) == ".soft_module")  //soft_modules
		{
			SoftR temp;
			temp.name = components.at(1);
			temp.area = stoi(components.at(2));
			soft_map[soft_counter] = temp;
			soft_counter++;
		}
		else if (components.at(0) == ".pins")  //I/O pads
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


//ILOSTLBEGIN

int main()
{
	
	float utilization = 0.9;
	float suti = 0.72;
	float aspect_min = 0.5;
	float aspect_max = 1/0.5;
	int WafterU;
	int pin_max, pin_min;
	int pins_side;
	int pins_min_width;
	int init_W;
	bool io_del = 0;
	string file_name = "test8.txt";
	unordered_map < int, HardR> HM;
	unordered_map < int, SoftR> SM;
	double area = 0;
	float totalw = 0;
	float totalh = 0;
	int totalmax = 0;
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
		totalh += h;
	}
	for (auto it = SM.begin(); it != SM.end(); ++it)
	{
		float wmin_i, wmax_i, hmin_i, hmax_i;
		int iarea;
		area += it->second.area;
		iarea = it->second.area;
		wmin_i = sqrt(iarea*aspect_min);
		wmax_i = ceil(sqrt(iarea*aspect_max));
		hmax_i = iarea / wmax_i + (wmax_i - wmin_i) * (iarea / (wmax_i*wmax_i));
		hmin_i = iarea / wmax_i + (wmax_i - wmax_i) * (iarea / (wmax_i*wmax_i));
		totalw += wmax_i;
		totalh += hmax_i;
	}
	totalmax = max(totalh, totalw);
	init_W = ceil(sqrt(area));
	WafterU = init_W / utilization;
	pin_max = max(pin_height, pin_width);
	pin_min = min(pin_height, pin_width);
	pins_side = ceil(pin_count / 4.0);
	pins_min_width = pins_side * pin_max;
	if (pins_min_width >= WafterU)
		io_del = 1;
	W = max(WafterU, pins_min_width);

	cout << "W = " << W;
	int wid = W;
	IloEnv env;
	stringstream logfile;
	cout << "total" << totalw << " , from area:" << wid;
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
		vars.add(IloNumVar(env));
		IloRangeArray c(env);
		
		// Hard Modules Constrains 
		for (int i = 1; i <= hard_count*3; i+=3)
		{
			
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));

			c.add(vars[i] >= 0);
			c.add(vars[i + 1] >= 0);
			//c.add(vars[i] + HM[i/2].width <= W);
			c.add(vars[i] + HM[i / 3].width * (1- vars[i+2]) + HM[i / 3].height*vars[i+2] <= W);
			c.add(vars[i + 1] + HM[i/3].height*(1 - vars[i + 2]) + HM[i / 3].width*vars[i + 2] - vars[0]<= 0);
			c.add(((vars[i+2] == 1) + (vars[i+2] == 0)) == 1);

		}
		
		// Hard Modules Overlapping Constrains 
		int ct = hard_count*3+1;
		for (int i = 1; i <= hard_count*3 - 3; i+=3)
			for (int j = i + 3; j <= hard_count * 3; j += 3)
			{
				vars.add(IloNumVar(env));
				vars.add(IloNumVar(env));

			
				c.add(vars[i] + HM[i / 3].width*(1-vars[i+2]) + HM[i / 3].height*vars[i+2] - (vars[j] + W * (vars[ct] + vars[ct + 1])) <= 0);
				c.add(vars[i + 1] + HM[i / 3].height* (1-vars[i+2]) + HM[i / 3].width*vars[i+2] - (vars[j + 1] + W * (1 + vars[ct] - vars[ct + 1])) <= 0);
				c.add(vars[j] + HM[j / 3].width*(1-vars[j+2]) + HM[j / 3].height*vars[j+2] - (vars[i] + W * (1 - vars[ct] + vars[ct + 1])) <= 0);
				c.add(vars[j + 1] + HM[j / 3].height*(1-vars[j+2]) + HM[j / 3].width * vars[j+2] - (vars[i + 1] + W * (2 - vars[ct] - vars[ct + 1])) <= 0);
				c.add(((vars[ct] == 1) + (vars[ct] == 0)) == 1);
				c.add(((vars[ct+1] == 1) + (vars[ct+1] == 0)) == 1);
				ct+=2;
				cout << "ct=" << ct << endl;
			}
		cout << "final_ct" << ct;

		// Soft Modules Cosntrains
		for (int i = ct; i < (ct+ soft_count * 4); i += 4)
		{
			int iarea;
			float wmin_i, wmax_i, hmin_i, hmax_i, deltai , ci;
			cout << "ii=" << i << endl;
			iarea = SM[(i - ct)/4].area/suti;
			wmin_i = sqrt(iarea*aspect_min);
			wmax_i = ceil(sqrt(iarea*aspect_max));
			hmax_i = ceil(iarea / wmax_i + (wmax_i - wmin_i) * (iarea / (wmax_i*wmax_i)));
			hmin_i = iarea / wmax_i + (wmax_i - wmax_i) * (iarea / (wmax_i*wmax_i));
			deltai = ceil( (hmin_i - hmax_i) / (wmax_i - wmin_i));
			ci = ceil( hmax_i - deltai * wmin_i);
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			vars.add(IloNumVar(env));
			c.add(vars[i] >= 0);
			c.add(vars[i + 1] >= 0);
			c.add(vars[i] + vars[i+2] - W <= 0 );
			c.add(vars[i + 1] + vars[i+3] - vars[0] <= 0);
			c.add((vars[i + 3] - vars[i + 2] * deltai - ci) == 0);
			c.add(vars[i + 2] >= wmin_i);
			c.add(vars[i + 2] <= wmax_i);
		
		}

		// Soft Modules Non Overlapping Constrains
		
		int cts = ct + soft_count*4 ;
		cout << "cts:" << cts;
		for (int i = ct; i < (ct + soft_count*4 - 4); i += 4)
			for (int j = i + 4; j < (ct + soft_count * 4); j += 4)
			{
			

				vars.add(IloNumVar(env));
				vars.add(IloNumVar(env));
				cout << "i=" << i << endl;
				cout << "j=" << j << endl;
				cout << "ct=" << ct << endl;
				c.add(vars[i] + vars[i+2] - (vars[j] + wid * (vars[cts] + vars[cts + 1])) <= 0);
				c.add(vars[i + 1] + vars[i+3] - (vars[j + 1] + wid * (1 + vars[cts] - vars[cts + 1])) <= 0);
				c.add(vars[j] + vars[j + 2] - (vars[i] + wid * ( 1 - vars[cts] + vars[cts + 1])) <= 0);
				c.add(vars[j + 1] + vars[j+3] - (vars[i + 1] + wid * (2 - vars[cts] - vars[cts + 1])) <= 0);
				c.add(((vars[cts] == 1) + (vars[cts] == 0)) == 1);
				c.add(((vars[cts + 1] == 1) + (vars[cts + 1] == 0)) == 1);
				cts += 2;
			}

		// Hard Modules with soft modules non overlapping constrains

		for (int i = 1; i <= hard_count * 3; i += 3)

			for (int j = ct; j < (ct + soft_count * 4); j += 4) {


				int jarea;
				float wmin_j, wmax_j, hmin_j, hmax_j, deltaj, cj;
				jarea = SM[(j - ct) / 4].area / suti;
				wmin_j = sqrt(jarea*aspect_min);
				wmax_j = sqrt(jarea*aspect_max);
				hmax_j = jarea / wmax_j + (wmax_j - wmin_j) * (jarea / (wmax_j*wmax_j));
				hmin_j = jarea / wmax_j + (wmax_j - wmax_j) * (jarea / (wmax_j*wmax_j));
				deltaj = (hmin_j - hmax_j) / (wmax_j - wmin_j);
				cj = hmax_j - deltaj * wmin_j;


				vars.add(IloNumVar(env));
				vars.add(IloNumVar(env));
				cout << "i=" << i << endl;
				cout << "j=" << j << endl;
				cout << "ct=" << ct << endl;
				c.add(vars[i] + vars[i + 2] * HM[i / 3].height + (1 - vars[i + 2])*HM[i / 3].width - (vars[j] + W * (vars[cts] + vars[cts + 1]) ) <= 0 );
				c.add( vars[i] - vars[j+2] - vars[j] + W*(1-vars[cts] + vars[cts+1]) >= 0  );
				c.add(vars[i + 1] + vars[i + 2] * HM[i / 3].width + (1 - vars[i + 2])* HM[i / 3].height - vars[j + 1] - W * (1 + vars[cts] - vars[cts + 1]) <= 0);
				c.add(vars[i+1] - vars[j+3] - vars[j+1] + W*(2 - vars[cts] - vars[cts + 1]) >= 0);
				c.add(((vars[cts] == 1) + (vars[cts] == 0)) == 1);
				c.add(((vars[cts + 1] == 1) + (vars[cts + 1] == 0)) == 1);
				cts += 2;
			}

		model.add(c);
		model.add(IloMinimize(env, vars[0]));

		// Solve
		cplex.solve();

		// Put Results in output file
		if (cplex.getStatus() == IloAlgorithm::Optimal) {
			IloNumArray vals(env);
			cplex.getValues(vals, vars);
			Results << "W:" << W + 2*pin_max << endl;
			if (io_del)
			{
				Results << "H:" << pins_min_width + 2 * pin_max << endl;
				Results << "Total Area:" << W * pins_min_width << endl;
				Results << "Final Utilization:" << area / (W * pins_min_width) << endl;
			}
			else
			{
				Results << "H:" << vals[0] + 2 * pin_max << endl;
				Results << "Total Area:" << W * vals[0] << endl;
				Results << "Final Utilization:" << area / (W * vals[0]) << endl;
			}
			
			Results << "I/O pads:" << endl;
			for (int i = 1; i <= pins_side; i++)
			{
				Results << "0 , " << i * pin_min<<endl;
			}

			for (int i = 1; i <= pins_side; i++)
			{
				Results << i * pin_min << " , 0" << endl;
			}

			for (int i = 1; i <= pins_side; i++)
			{
				Results << W << " , " << i * pin_min << endl;
			}
			for (int i = 1; i <= pins_side; i++)
			{
				Results << i * pin_min << " , "<< vals[0] << endl;
			}

			Results << "Hard Modules:" << endl;
			for (int i = 1; i <= hard_count * 3; i += 3) 
			{
				Results<< HM[i/3].name<<":		" << vals[i] + pin_max << "," <<vals[i+1]+pin_max<<"		";
				if (vals[i + 2])
					Results << "rotated";
				Results << endl;
			}

			Results << "Soft Modules:" << endl;
			for (int i = ct; i < (ct + soft_count * 4); i += 4)
			{
				Results <<SM[i/4 - hard_count - 1].name<<":		"<< vals[i]+pin_max << "		" << vals[i + 1]+pin_max <<"	" << vals[i+2] << "		" << vals[i+3]<<endl;
			}
			cout << "Values = " << vals << endl;
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

