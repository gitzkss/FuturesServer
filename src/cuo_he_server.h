#pragma once
#include<vector>
#include<queue>
#include<iostream>
#include"wei_tuo.h"
#include"customer_info.h"
#include"jiao_yi_data.h"
#include"utils.h"

class CuoHeServer
{
private:
	double newPrice;
	//��һ��
	double sellPrice;
	//��һ����
	int sellCount;
	//��һ��
	double buyPrice;
	//��һ����
	int buyCount;
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
public:
	CuoHeServer(double kaipanPrice, double zhangTingPrice, double dieTingPrice);
	~CuoHeServer();
	void addBuy(WeiTuo wt);
	void addSell(WeiTuo wt);
	int removeBuy(WeiTuo wt);
	int removeSell(WeiTuo wt);
	void updateState(double price);
	bool match(WeiTuo wt);
	// ��ӡ�ɽ�����
	void print(JiaoYiData data);
	// ��ӡ�̿�����
	void printState();
	//ִ��ί�ж�������
	void executeWeiTuoQueue();
	//�����̿ڵ�ί�������۸������
	void updateBS();
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
	int verdict(WeiTuo wt, CustomerInfo* cinfo = nullptr);
	//wtΪ�ɽ�ί��,priceΪ�ɽ��۸�,countΪ�ɽ�����
	WeiTuo accepted(WeiTuo* wt, double price, int count);
	//timeΪ�ɽ�ʱ��,priceΪ�ɽ��۸�,typeΪ�ɽ�����,duokaiΪ�࿪,duopingΪ��ƽ,kongkaiΪ�տ�,kongpingΪ��ƽ����
	JiaoYiData getJiaoYiData(double price, int type, int duokai, int kongkai, int duoping, int kongping, time_t time = 0);
	std::map<std::string, std::string> getPankou();
};