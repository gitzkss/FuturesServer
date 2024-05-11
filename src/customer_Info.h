#pragma once
#include<string>
#include<vector>
#include"chi_cang.h"
#include"wei_tuo.h"
class CustomerInfo
{
public:
	int customerID;
	std::string password;
	std::string customerName;
	double quanyi;
	double keyong;
	double fengxianlv;
	ChiCang chicang;
	std::vector<WeiTuo>weituo;
public:
	void update(double newPrice, int pointPrice);
	int addWeiTuo(WeiTuo wt);
	int verifyKaiCang(double baozhengjin,double shouxufei,WeiTuo wt);
	int verifyPingCang(WeiTuo wt);
	//接收委托
	int receiveWeiTuo(double newPrice, int pointPrice, double baozhengjin, double shouxufei, WeiTuo wt);
	//撤销委托
	int removeWeiTuo(WeiTuo wt);
	void updateWeiTuo(WeiTuo wt,int cnt);
	//委托成交
	void acceptWeiTuo(double newPrice, double pointPrice, double baozhengjin, int shouxufei, WeiTuo wt, double price, double cnt);
};