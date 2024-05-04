#include"jiao_yi_data.h"

JiaoYiData::JiaoYiData(time_t time, double price, std::string optype, int count,int color)
	:time(time), price(price), optype(optype), count(count) ,color(color)
{
	if(this->time==0)
		this->time = ::time(nullptr);
}

JiaoYiData::~JiaoYiData() {}

double JiaoYiData::getPrice()
{
	return price;
}

time_t JiaoYiData::getTime()
{
	return time;
}

void JiaoYiData::setTime(time_t t)
{
	time = t;
}

std::string JiaoYiData::getOptype()
{
	return optype;
}

int JiaoYiData::getCount()
{
	return count;
}

void JiaoYiData::setPrice(double price)
{
	this->price = price;
}

void JiaoYiData::setOptype(std::string optype)
{
	this->optype = optype;
}

void JiaoYiData::setCount(int count)
{
	this->count = count;
}

int JiaoYiData::getColor()
{
	return color;
}

void JiaoYiData::setColor(int color)
{
	this->color = color;
}
