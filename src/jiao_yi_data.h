#pragma once
#include<string>
#include<ctime>
class JiaoYiData
{
private:
	time_t time;
	double price;
	std::string optype;
	int count;
	//0:ÂÌÉ« 1:ºìÉ«
	int color;
public:
	JiaoYiData(time_t time = 0, double price = -1, std::string optype = "", int count = -1,int color = 1);
	~JiaoYiData();
	time_t getTime();
	void setTime(time_t t);
	double getPrice();
	std::string getOptype();
	int getCount();
	int getColor();
	void setPrice(double price);
	void setOptype(std::string optype);
	void setCount(int count);
	void setColor(int color);
};