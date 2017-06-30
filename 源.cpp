#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<set>
#include<map>
using namespace std;

set<int> read_holiday()
{
	ifstream in("C:\\Users\\VULCNAVSeries\\Documents\\Tencent Files\\275868482\\FileRecv\\Holiday.csv");
	vector<string> holiday = {"January","February","March","April","May","June","July","August","September","October","November","December"};
	vector<int> days_of_mouth_in_2017 = { 31,28,31,30,31,30,31,31,30,31,30,31 };//2017年各月的日期
	set<int> holiday_day;
	if (!in)
	{
		cout << "the file has been renamed or moved." << endl;
		return holiday_day;
	}
	while (!in.eof())
	{
		int day = 0;
		string c;
		getline(in, c);
		while (c.find(',') != string::npos)
		{
			c.replace(c.find(','), 1, " ");
		}
		istringstream cc(c);
		vector<string> vec;
		string inin;
		while (cc >> inin)
		{
			vec.push_back(inin);
	    }
		if (vec.size() >= 4)//出去标题
		{
			for (int i = 0; i < holiday.size(); i++)
			{
				if (vec[vec.size() - 2] == holiday[i])
				{
					for (int j = 0; j < i; j++)
					{
						day = day + days_of_mouth_in_2017[j];
					}
				}
			}
			day = day + atoi(vec[vec.size() - 1].c_str());
			holiday_day.insert(day);
		}
	}
	return holiday_day;
}

vector<string> read_future(string order_number)
{
	ifstream in("C:\\Users\\VULCNAVSeries\\Documents\\Tencent Files\\275868482\\FileRecv\\Future Trade Sample_Commodities.csv");
	vector<string> vec;
	if (!in)
	{
		cout << "the file has been renamed or moved." << endl;
	}
	while (!in.eof())
	{
		string c;
		getline(in,c);
		if (c.find(order_number) != string::npos && order_number.size() == 7)
		{
			while (c.find(',') != string::npos)
			{
				c.replace(c.find(','), 1, " ");
			}
			istringstream cc(c);
			string node;
			while (cc >> node)
			{
				vec.push_back(node);
			}
			return vec;
		}
	}
	return vec;
}

vector<string> read_swap(string order_number)
{
	ifstream in("C:\\Users\\VULCNAVSeries\\Documents\\Tencent Files\\275868482\\FileRecv\\Swap Trade Sample_Commodities.csv");
	vector<string> vec;
	if (!in)
	{
		cout << "the file has been renamed or moved." << endl;
	}
	while (!in.eof())
	{
		string c;
		getline(in, c);
		if (c.find(order_number) != string::npos && order_number.size() == 7)
		{
			while (c.find(',') != string::npos)
			{
				c.replace(c.find(','), 1, " ");
			}
			istringstream cc(c);
			string node;
			while (cc >> node)
			{
				vec.push_back(node);
			}
			return vec;
		}
	}
	return vec;
}

map<string, double> get_swap_imformation(string date, string swap_product)
{
	map<string, double> futue_imformation;
	ifstream in("C:\\Users\\VULCNAVSeries\\Documents\\Tencent Files\\275868482\\FileRecv\\Marks.json");
	string c;
	string include;
	ostringstream buf;
	vector<int> days_of_mouth_in_2017 = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	int pos_of_product;
	int pos_of_end = 0;
	char ch;
	while (buf&&in.get(ch))
		buf.put(ch);
	c = buf.str();
	int pos_of_date = c.find("\"As-of-day\": \"" + date);
	if (pos_of_date == -1)
	{
		return futue_imformation;
	}
	else
	{
		pos_of_product = c.find(swap_product, pos_of_date);
		for (int i = c.find("[", pos_of_product); i <= c.find("]", pos_of_product); i++)
		{
			include = include + c[i];
		}
		while (true)
		{
			int pos_of_type1 = include.find("SettlementPrice", pos_of_end);
			int pos_of_type2 = include.find("Forward", pos_of_end);
			pos_of_end = include.find("}", pos_of_end + 1);
			if (pos_of_end == -1)
			{
				break;
			}
			if (pos_of_type1 != -1 && pos_of_type1 < pos_of_end)
			{
				string date_of_future;
				string mark_of_future;
				for (int i = include.find("Date", pos_of_type1) + 7; i <= include.find("Date", pos_of_type1) + 14; i++)
				{
					date_of_future = date_of_future + include[i];
				}
				for (int i = include.find("Mark", pos_of_type1) + 7; i <= include.find("Mark", pos_of_type1) + 11; i++)
				{
					mark_of_future = mark_of_future + include[i];
				}
				int month = (date_of_future[4] - 48) * 10 + (date_of_future[5] - 48);
				int day = (date_of_future[6] - 48) * 10 + (date_of_future[7] - 48);
				int days = day;
				for (int i = 0; i < month - 1; i++)
				{
					days = days + days_of_mouth_in_2017[i];
				}
				futue_imformation[to_string(days)] = atof(mark_of_future.c_str());
			}
			if (pos_of_type2 != -1 && pos_of_type2 < pos_of_end)
			{
				string ContractMonth_of_future;
				string mark_of_future;
				for (int i = include.find("ContractMonth", pos_of_type2) + 16; i <= include.find("ContractMonth", pos_of_type2) + 21; i++)
				{
					ContractMonth_of_future = ContractMonth_of_future + include[i];
				}
				for (int i = include.find("Mark", pos_of_type2) + 7; i <= include.find("Mark", pos_of_type2) + 11; i++)
				{
					mark_of_future = mark_of_future + include[i];
				}
				futue_imformation[ContractMonth_of_future] = atof(mark_of_future.c_str());
			}
		}
		return futue_imformation;
	}
}

vector<int> get_workday_and_allworkdays(string start, string end)
{
	vector<int> days;
	int start_of_days = 0;
	int end_of_days = 0;
	int workday = 0;//期权工作日
	int allworkday;//期权总历时
	vector<int> days_of_mouth_in_2017 = { 31,28,31,30,31,30,31,31,30,31,30,31 };//2017年各月的日期
	while (start.find('/') != string::npos)
	{
		start.replace(start.find('/'), 1, " ");
	}
	while (end.find('/') != string::npos)
	{
		end.replace(end.find('/'), 1, " ");
	}
	istringstream ss(start);
	istringstream ee(end);
	string mouth_start;
	string day_start;
	string year_start;
	string mouth_end;
	string day_end;
	string year_end;
	set<int> holiday = read_holiday();
	if (holiday.size() == 0)
	{
		return days;
	}
	ss >> mouth_start;
	ss >> day_start;
	ss >> year_start;
	for (int i = 0; i < atoi(mouth_start.c_str()) - 1; i++)
	{
		start_of_days = start_of_days + days_of_mouth_in_2017[i];
	}
	start_of_days = start_of_days + atoi(day_start.c_str());
	ee >> mouth_end;
	ee >> day_end;
	ee >> year_end;
	for (int i = 0; i < atoi(mouth_end.c_str()) - 1; i++)
	{
		end_of_days = end_of_days + days_of_mouth_in_2017[i];
	}
	end_of_days = end_of_days + atoi(day_end.c_str());
	allworkday = end_of_days - start_of_days;
	for (int i = start_of_days; i <= end_of_days + 1; i++)
	{
		if (i % 7 != 0 && i % 7 != 1 && holiday.find(i) == holiday.end())
		{
			days.push_back(i);
		}
	}
	days.push_back(allworkday);
	return days;
}

double get_mark(string date,string future_product,string period)
{
	ifstream in("C:\\Users\\VULCNAVSeries\\Documents\\Tencent Files\\275868482\\FileRecv\\Marks.json");
	string c;
	ostringstream buf;
	int pos_of_product;
	int pos_of_Type;
	int pos_of_period;
	int pos_of_price;
	char ch;
	while (buf&&in.get(ch))
		buf.put(ch);  
	c = buf.str();
	int pos_of_date = c.find("\"As-of-day\": \"" + date);
	if (pos_of_date == -1)
	{
		return -1;
	}
	else
	{
		pos_of_product = c.find(future_product, pos_of_date);
		pos_of_Type = c.find("Forward", pos_of_product);
		pos_of_period = c.find(period, pos_of_Type);
		pos_of_price = c.find("Mark", pos_of_period) + 7;
	}
	string price;
	for (int i = 0; i < 5; i++)
	{
		price = price + c[pos_of_price + i];
	}
	double price_of_product = atof(price.c_str());
	return price_of_product;
}

int main()
{
	int type;
	string order_number;//订单序号（用户输入）
	string date;//当天的日期（用户输入）
	string buy_sell;//买或卖
	string future_product;//期货商品名
	string swap_product;//商品名
	string period;//期货的日期
	string start;//掉期开始日期
	string end;//掉期结束日期
	vector<int> days_of_mouth_in_2017 = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	double price;
	double pv0;
	int workday;//掉期工作日
	int allworkday;//掉期总历时
	int lots;
	cout << "future or swap?" << endl;
	cout << "you choose('1' for future while '2' for swap):";
	cin >> type;
	cout << "type in today's date(e.x.:20170515):";
	cin >> date;
	if (date.size() != 8)
	{
		cout << "wrong date!" << endl;
		system("pause");
		return 0;
	}
	if (type == 1)
	{
		cout << "type in the order number:";
		cin >> order_number;
		vector<string> vec = read_future(order_number);
		if (vec.size() == 0)
		{
			cout << "wrong order number!" << endl;
			system("pause");
			return 0;
		}
		else
		{
			buy_sell = vec[1];
			future_product = vec[2];
			period = vec[3];
			price = atof(vec[4].c_str());
			lots = atoi(vec[5].c_str());
		}
		double mark = get_mark(date, future_product, period);
		if (mark == -1)
		{
			cout << "wrong date!" << endl;
			system("pause");
			return 0;
		}
		if (buy_sell == "S")
		{
			pv0 = (price - mark) * lots * 2500;
		}
		if (buy_sell == "B")
		{
			pv0 = (mark - price) * lots * 2500;
		}
		cout << "RiskExposure:" << lots << endl;
		cout << "pv0:" << pv0 << endl;
		system("pause");
		return 0;
	}
	if (type == 2)
	{
		cout << "type in the order number:";
		cin >> order_number;
		vector<string> vec = read_swap(order_number);
		if (vec.size() == 0)
		{
			cout << "wrong order number!" << endl;
			system("pause");
			return 0;
		}
		else
		{
			buy_sell = vec[3];
			price = atof(vec[4].c_str());
			swap_product = vec[5];
			if (vec[5].find("WTI") != string::npos)
			{
				swap_product = "WTI";
			}
			if (vec[5].find("BRENT") != string::npos)
			{
				swap_product = "BRENT";
			}
			lots = atoi(vec[6].c_str());
			start = vec[8];
			end = vec[9];
			int month_now = (date[4] - 48) * 10 + (date[5] - 48);
			int day_now = (date[6] - 48) * 10 + (date[7] - 48);
			int days_now = day_now;
			double floating_price = 0;
			for (int i = 0; i < month_now - 1; i++)
			{
				days_now = days_now + days_of_mouth_in_2017[i];
			}
			map<string, double> futue_imformation = get_swap_imformation(date, swap_product);
			vector<int> days = get_workday_and_allworkdays(start, end);
			for (int i = 0; i < days.size() - 2; i++)
			{
				if (days[i] <= days_now)
				{
					floating_price = floating_price + futue_imformation[to_string(days[i])];
				}
				else
				{
					if (days[i] < 142)
					{
						floating_price = floating_price + futue_imformation["Jun-17"];
					}
					else if (days[i] >= 142 && days[i] < 173)
					{
						floating_price = floating_price + futue_imformation["Jul-17"];
					}
					else if (days[i] >= 173 && days[i] < 203)
					{
						floating_price = floating_price + futue_imformation["Aug-17"];
					}
					else if (days[i] >= 203 && days[i] < 234)
					{
						floating_price = floating_price + futue_imformation["Sep-17"];
					}
					else if (days[i] >= 234 && days[i] < 265)
					{
						floating_price = floating_price + futue_imformation["Oct-17"];
					}
					else if (days[i] >= 265 && days[i] < 295)
					{
						floating_price = floating_price + futue_imformation["Nov-17"];
					}
					else if (days[i] >= 295 && days[i] < 326)
					{
						floating_price = floating_price + futue_imformation["Dec-17"];
					}
				}
			}
			workday = days.size() - 1;
			allworkday = days[workday];
			double RiskExposure = workday * lots / allworkday;
			if (buy_sell == "S")
			{
				pv0 = (price - floating_price / workday) * lots ;
			}
			if (buy_sell == "B")
			{
				pv0 = (floating_price / workday - price ) * lots;
			}
			cout << "RiskExposure:" << RiskExposure << endl;
			cout << "pv0:" << pv0 << endl;
			system("pause");
			return 0;
		}
	}
	else
	{
		cout << "wrong choose!" << endl;
		system("pause");
		return 0;
	}
}