#pragma once
#include<vector>
#include<queue>
#include<iostream>
#include"wei_tuo.h"
#include"customer_info.h"
#include"jiao_yi_data.h"
#include"utils.h"
#include <unistd.h>

class CuoHeServer
{
private:
	double newPrice;
	//卖一价
	double sellPrice;
	//卖一数量
	int sellCount;
	//买一价
	double buyPrice;
	//买一数量
	int buyCount;
	double shouxufei;
	double baozhengjin;
	double pointPrice;
	double zhangTingPrice;
	double dieTingPrice;
	double maxPrice;
	double minPrice;
	int waiPan;
	int neiPan;
	double zhangDieFu;
	double kaipanPrice;
	std::vector<WeiTuo> buy;
	std::vector<WeiTuo> sell;
	std::queue<WeiTuo> weiTuoQueue;
	std::queue<JiaoYiData> jiaoYiDataQueue;
public:
	CuoHeServer(double kaipanPrice, double tingban,double baozhengjin,double shouxufei,double pointPrice);
	~CuoHeServer();
	std::map<int, CustomerInfo> customerMap;
	std::map<int, int> idMap;
	std::map<int, int> socketMap;
	void addBuy(WeiTuo wt);
	void addSell(WeiTuo wt);
	int removeBuy(WeiTuo wt);
	int removeSell(WeiTuo wt);
	void updateState(double price);
	bool match(WeiTuo wt);
	// 打印成交数据
	void print(JiaoYiData data);
	// 打印盘口数据
	void printState();
	//执行委托队列内容
	void executeWeiTuoQueue();
	//更新盘口的委托买卖价格和数量
	void updateBS();
	//获取客户委托队列以及个人信息
	void getCustomerInfo(std::map<std::string, std::string>& res, int customerID);
	double getKaiPanPrice();
	double getNewPrice();
	double getSellPrice();
	int getSellCount();
	double getBuyPrice();
	int getBuyCount();
	int findBuyCount(double price);
	int findSellCount(double price);
	double getZhangTingPrice();
	double getDieTingPrice();
	double getMaxPrice();
	double getMinPrice();
	int getWaiPan();
	int getNeiPan();
	double getZhangDieFu();
	std::vector<WeiTuo> getBuy();
	std::vector<WeiTuo> getSell();
	std::queue<WeiTuo> getWeiTuoQueue();
	std::string verdict(WeiTuo wt, int customerID);
	//wt为成交委托,price为成交价格,count为成交数量
	int accepted(WeiTuo* wt, double price, int count);
	//time为成交时间,price为成交价格,type为成交类型,duokai为多开,duoping为多平,kongkai为空开,kongping为空平数量
	JiaoYiData getJiaoYiData(double price, int type, int duokai, int kongkai, int duoping, int kongping, time_t time = 0);
	std::map<std::string, std::string> getPankou(int customerID);
	std::queue<JiaoYiData> getJiaoYiDataQueue();
};