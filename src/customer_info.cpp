#include "customer_info.h"

void  CustomerInfo::update(double newPrice,int pointPrice)
{
	int duoCount = chicang.getDuoCount();
	int kongCount = chicang.getKongCount();
	if (chicang.nowprice == 0)
		chicang.nowprice = newPrice;
	else
	{
		int discnt = duoCount - kongCount;
		double disp = (newPrice - chicang.nowprice) * discnt * pointPrice;
		quanyi += disp;
		keyong += disp;
		chicang.nowprice = newPrice;
	}
	fengxianlv = 1.0 - keyong / quanyi;
}
int CustomerInfo::addWeiTuo(WeiTuo wt)
{
	if (weituo.size() >= 10)
	{
		return 0;
	}
	weituo.push_back(wt);
	return 1;
}
int CustomerInfo::verifyKaiCang(double baozhengjin, double shouxufei, WeiTuo wt)
{
	if ((baozhengjin+shouxufei) * wt.getCount() >= keyong*0.85)
	{
		return 0;
	}
	if (weituo.size() >= 10)
	{
		return -1;
	}
	return 1;
}
int CustomerInfo::verifyPingCang(WeiTuo wt)
{
	if (weituo.size() >= 10)
	{
		return -1;
	}
	if (wt.getOptype() == "duoping")
	{
		if (wt.getCount() > chicang.getKepingDuoCount())
		{
			return 0;
		}
	}
	else
	{
		if (wt.getCount() > chicang.getKepingKongCount())
		{
			return 0;
		}
	}
	return 1;
}
int CustomerInfo::receiveWeiTuo(double newPrice,int pointPrice, double baozhengjin, double shouxufei, WeiTuo wt)
{
	update(newPrice, pointPrice);
	if (wt.getCount() < 0)
	{
		if (removeWeiTuo(wt))
		{
			if (wt.getOptype() == "duokai" || wt.getOptype() == "kongkai")
				keyong += baozhengjin * abs(wt.getCount());
			if (wt.getOptype() == "duoping")
				chicang.addKePingDuo(-wt.getCount());
			else if(wt.getOptype() == "kongping")
				chicang.addKePingKong(-wt.getCount());
			return 1;
		}
		else
			return -2;
	}
	if (wt.getOptype() == "duokai" || wt.getOptype() == "kongkai")
	{

		int ok = verifyKaiCang(baozhengjin, shouxufei, wt);
		//-1 可用资金不足
		if (ok == 0)
			return -1;
		//-3 委托队列达到上限
		else if (ok == -1)
			return -3;
		keyong -= baozhengjin*wt.getCount();
	}
	else
	{
		int ok = verifyPingCang(wt);
		//0 超出可平仓位
		if (ok == 0)
			return 0;
		else if (ok == -1)
			return -3;
		if (wt.getOptype() == "duoping")
			chicang.removeKePingDuo(wt.getCount());
		else
			chicang.removeKePingKong(wt.getCount());
	}
	addWeiTuo(wt);
	update(newPrice, pointPrice);
	return 1;
}
int CustomerInfo::removeWeiTuo(WeiTuo wt)
{
	for (int i = 0; i < weituo.size(); i++)
	{
		if (wt.getOptype() == weituo[i].getOptype() && wt.getPrice() == weituo[i].getPrice() && abs(wt.getCount()) == weituo[i].getCount())
		{
			weituo.erase(weituo.begin() + i);
			
			return 1;
		}
	}
	return 0;

}
void CustomerInfo::updateWeiTuo(WeiTuo wt, int cnt)
{
	for (int i = 0; i < weituo.size(); i++)
	{
		if (wt.getOptype() == weituo[i].getOptype() && wt.getPrice() == weituo[i].getPrice()&&wt.getCount()==weituo[i].getCount())
		{
			weituo[i].setCount(weituo[i].getCount() - cnt);
			if (weituo[i].getCount() == 0)
			{
				weituo.erase(weituo.begin() + i);
			}
			break;
		}
	}
}
void CustomerInfo::acceptWeiTuo(double newPrice,double pointPrice,double baozhengjin, int shouxufei, WeiTuo wt, double price, double cnt)
{
	update(newPrice, pointPrice);
	quanyi -= shouxufei * cnt;
	keyong -= shouxufei * cnt;
	//处理保证金 更新持仓
	if (wt.getOptype() == "duoping" || wt.getOptype() == "kongping")
	{
		keyong += baozhengjin * cnt;
		if(wt.getOptype() == "duoping")
			chicang.removeDuo(cnt);
		else
			chicang.removeKong(cnt);
	}
	else
	{
		if (wt.getOptype() == "duokai")
			chicang.addDuo(price, cnt);
		else
			chicang.addKong(price, cnt);
	}
	//更新委托队列
	updateWeiTuo(wt, cnt);
	//更新资金
	update(newPrice, pointPrice);
}