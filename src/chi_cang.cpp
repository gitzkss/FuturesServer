#include "chi_cang.h"

ChiCang::ChiCang(int id, int contractID, std::string contractName) :id(id), contractID(contractID), contractName(contractName) 
{
	duoCount = 0;
	kongCount = 0;
	duoSum = 0;
	kongSum = 0;
	kepingduoCount = 0;
	kepingkongCount = 0;
};
ChiCang::~ChiCang() {};

int ChiCang::getKepingDuoCount()
{
	return kepingduoCount;
}
int ChiCang::getKepingKongCount()
{
	return kepingkongCount;
}

void ChiCang::removeKePingDuo(int cnt)
{
	kepingduoCount -= cnt;
}
void ChiCang::removeKePingKong(int cnt)
{
	kepingkongCount -= cnt;
}
void ChiCang::addKePingDuo(int cnt)
{
	kepingduoCount += cnt;
}
void ChiCang::addKePingKong(int cnt)
{
	kepingkongCount += cnt;
}

int ChiCang::getDuoCount()
{
	return duoCount;
}
int ChiCang::getKongCount()
{
	return kongCount;
}
std::stack<std::pair<double, int> > ChiCang::getDuo()
{
	return duo;
}
std::stack<std::pair<double, int> > ChiCang::getKong()
{
	return kong;
}
void ChiCang::addDuo(double price,int cnt)
{
	this->duo.push(std::make_pair(price, cnt));
	this->duoSum += price * cnt;
	duoCount += cnt;
	kepingduoCount += cnt;
}
void ChiCang::addKong(double price,int cnt)
{
	this->kong.push(std::make_pair(price, cnt));
	this->kongSum += price * cnt;
	kongCount += cnt;
	kepingkongCount += cnt;
}
void ChiCang::removeDuo(int cnt)
{
	//duo元素是一个pair类型，first是价格,second是数量
	//减少cnt手多头仓位，栈顶元素的数量减少cnt，如果栈顶元素数量为0，则弹出栈顶元素，否则修改栈顶元素的数量
	while (!duo.empty()&&cnt>0)
	{
		if (duo.top().second > cnt)
		{
			duoSum -= duo.top().first * cnt;
			duo.top().second -= cnt;
			duoCount -= cnt;
			cnt = 0;
		}
		else
		{
			duoCount -= duo.top().second;
			cnt -= duo.top().second;
			duoSum -= duo.top().first * duo.top().second;
			duo.pop();
		}
	}

}
void ChiCang::removeKong(int cnt)
{
	//kong元素是一个pair类型，first是价格,second是数量
	//减少cnt手空头仓位，栈顶元素的数量减少cnt，如果栈顶元素数量为0，则弹出栈顶元素，否则修改栈顶元素的数量
	while (!kong.empty() && cnt > 0)
	{
		if (kong.top().second > cnt)
		{
			kongSum -= kong.top().first * cnt;
			kong.top().second -= cnt;
			kongCount -= cnt;
			cnt = 0;
		}
		else
		{
			kongCount -= kong.top().second;
			cnt -= kong.top().second;
			kongSum -= kong.top().first * kong.top().second;
			kong.pop();
		}
	}
}
double ChiCang::getAvgDuo()
{
	if (duo.size() == 0)
	{
		return 0;
	}
	else
	{
		return duoSum / duoCount;
	}
}
double ChiCang::getAvgKong()
{
	if (kong.size() == 0)
	{
		return 0;
	}
	else
	{
		return kongSum / kongCount;
	}
}

