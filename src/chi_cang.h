#pragma once
#include <string>
#include <stack>
class ChiCang
{
private:
	int id;
	int contractID;
	std::string contractName;
	double duoSum;
	double kongSum;
	int duoCount;
	int kongCount;
	int kepingduoCount;
	int kepingkongCount;
	std::stack<std::pair<double,int> >duo;
	std::stack<std::pair<double,int> >kong;
public:
	ChiCang(int id = -1, int contractID = -1, std::string contractName="");
	~ChiCang();
	std::stack<std::pair<double, int> > getDuo();
	std::stack<std::pair<double, int> > getKong();
	double getAvgDuo();
	double getAvgKong();
	int getDuoCount();
	int getKongCount();
	int getKepingDuoCount();
	int getKepingKongCount();
	void removeKePingDuo(int cnt);
	void removeKePingKong(int cnt);
	void addKePingDuo(int cnt);
	void addKePingKong(int cnt);
	void addDuo(double price,int cnt);
	void addKong(double price,int cnt);
	void removeDuo(int cnt);
	void removeKong(int cnt);
};
