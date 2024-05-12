#include"cuo_he_server.h"
CuoHeServer::CuoHeServer(double kaipanPrice, double tingban, double baozhengjin, double shouxufei, double pointPrice)
	:kaipanPrice(kaipanPrice), baozhengjin(baozhengjin), shouxufei(shouxufei), pointPrice(pointPrice)
{
	zhangTingPrice = (int)(kaipanPrice + kaipanPrice * tingban);
	dieTingPrice = (int)(kaipanPrice - kaipanPrice * tingban);
	buyPrice = -1;
	buyCount = 0;
	sellPrice = -1;
	sellCount = 0;
	newPrice = kaipanPrice;
	waiPan = 0;
	neiPan = 0;
	zhangDieFu = 0;
	maxPrice = -1;
	minPrice= 0x3f3f3f3f;
	WeiTuo wt;
	wt.setCount(0);
	buy.push_back(wt);
	sell.push_back(wt);
	freopen("src/customer_info.txt", "r", stdin);
	int n;
	std::cin >> n;
	for (int i = 0; i < n; i++)
	{
		CustomerInfo cinfo;
		std::cin >> cinfo.customerID >> cinfo.password >> cinfo.quanyi;
		cinfo.keyong = cinfo.quanyi;
		customerMap[cinfo.customerID] = cinfo;
	}
	fclose(stdin);
};

CuoHeServer::~CuoHeServer() {};

void CuoHeServer::addBuy(WeiTuo wt)
{
	//wt的价格对类的buy vector作二分插入排序，第0个元素不参与排序，按照价格高从到低排序
	//找到第一个价格小于wt的价格的元素的位置,插入到它的前面
	//如果找不到这样的元素，插入到最后
	//vector元素下标越小证明优先级越高，价格越高优先级越高，时间越早优先级越高
	int low = 1;
	int high = buy.size() - 1;
	int mid;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (buy[mid].getPrice() < wt.getPrice())
		{
			high = mid - 1;
		}
		else if (buy[mid].getPrice() > wt.getPrice())
		{
			low = mid + 1;
		}
		else
		{
			if (buy[mid].getTime() < wt.getTime())
			{
				low = mid + 1;
			}
			else
			{
				high = mid - 1;
			}
		}
	}
	buy.insert(buy.begin() + low, wt);
}

void CuoHeServer::addSell(WeiTuo wt)
{
	//wt的价格对类的sell vector作二分插入排序，第0个元素不参与排序，按照价格低从到高排序
	//找到第一个价格大于wt的价格的元素，或者价格相等时间比它早的元素，插入到它的前面
	//如果找不到这样的元素，插入到最后
	int low = 1;
	int high = sell.size() - 1;
	int mid;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (sell[mid].getPrice() > wt.getPrice())
		{
			high = mid - 1;
		}
		else if (sell[mid].getPrice() < wt.getPrice())
		{
			low = mid + 1;
		}
		else
		{
			if (sell[mid].getTime() < wt.getTime())
			{
				low = mid + 1;
			}
			else
			{
				high = mid - 1;
			}
		}
	}
	sell.insert(sell.begin() + low, wt);
}

int CuoHeServer::removeBuy(WeiTuo wt)
{
	//wt的价格对类的buy vector作二分查找，找到和参数所有字段完全相同的元素，删除它。如果找到返回1否则返回0
	int low = 1;
	int high = buy.size() - 1;
	int mid;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (buy[mid].getPrice() < wt.getPrice())
		{
			high = mid - 1;
		}
		else if (buy[mid].getPrice() > wt.getPrice())
		{
			low = mid + 1;
		}
		else
		{
			int i = mid;
			double price = buy[mid].getPrice();
			while (mid >= 1 && price == buy[mid].getPrice())
			{
				if (buy[mid].getPrice() == buy[mid].getPrice() && buy[mid].getOptype() == wt.getOptype() && buy[mid].getCount() == wt.getCount() && buy[mid].getCustomerID() == wt.getCustomerID())
				{
					buy.erase(buy.begin() + mid);
					return 1;
				}
				mid--;
			}
			mid = i + 1;
			while (mid < buy.size() && price == buy[mid].getPrice())
			{
				if (buy[mid].getPrice() == buy[mid].getPrice() && buy[mid].getOptype() == wt.getOptype() && buy[mid].getCount() == wt.getCount() && buy[mid].getCustomerID() == wt.getCustomerID())
				{
					buy.erase(buy.begin() + mid);
					return 1;
				}
				mid++;
			}
			break;
		}
	}
	return 0;
}

bool CuoHeServer::match(WeiTuo wt)
{
	JiaoYiData result;
	int duokai = 0;
	int duoping = 0;
	int kongkai = 0;
	int kongping = 0;
	//type=1主动为多开，type=2为空开，type=3为多平，type=4为空平
	int type;
	if (wt.getOptype() == "duokai")
		type = 1;
	else if (wt.getOptype() == "kongkai")
		type = 2;
	else if (wt.getOptype() == "duoping")
		type = 3;
	else
		type = 4;
	int count = 0;
	double price = -1;
	time_t time = ::time(nullptr);
	bool is_ok = 0;
	while (buy.size() > 1 && sell.size() > 1 && buy[1].getPrice() >= sell[1].getPrice())
	{
		is_ok = 1;
		//price = sell[1].getPrice();
		//成交额 假如新委托在买一，那么成交价为卖一价；反之如果新委托在卖一，那么成交价为买一
		if (wt.getPrice() == buy[1].getPrice())
			price = sell[1].getPrice();
		else
			price = buy[1].getPrice();

		//撮合成功，报告给客户
		int cnt = std::min(buy[1].getCount(), sell[1].getCount());
		if (buy[1].getCount() > sell[1].getCount())
		{
			accepted(&sell[1], price, cnt);
			accepted(&buy[1], price, cnt);
			if (type == 1 || type == 4)
			{
				//如果是主动买入
				if (sell[1].getOptype() == "kongkai")
					kongkai += cnt;
				else
					duoping += cnt;
			}
			else
			{
				//如果是主动卖出
				if (buy[1].getOptype() == "duokai")
					duokai += cnt;
				else
					kongping += cnt;
			}
			sell.erase(sell.begin() + 1);
		}
		else if (buy[1].getCount() < sell[1].getCount())
		{
			accepted(&sell[1], price, cnt);
			accepted(&buy[1], price, cnt);
			if (type == 1 || type == 4)
			{
				//如果是主动买入
				if (sell[1].getOptype() == "kongkai")
					kongkai += cnt;
				else
					duoping += cnt;
			}
			else
			{
				//如果是主动卖出
				if (buy[1].getOptype() == "duokai")
					duokai += cnt;
				else
					kongping += cnt;
			}
			buy.erase(buy.begin() + 1);
		}
		else
		{
			if (type == 1 || type == 4)
			{
				//如果是主动买入
				if (sell[1].getOptype() == "kongkai")
					kongkai += cnt;
				else
					duoping += cnt;
			}
			else
			{
				//如果是主动卖出
				if (buy[1].getOptype() == "duokai")
					duokai += cnt;
				else
					kongping += cnt;
			}
			//撮合成功，报告给客户
			accepted(&sell[1], price, cnt);
			accepted(&buy[1], price, cnt);
			buy.erase(buy.begin() + 1);
			sell.erase(sell.begin() + 1);
		}
		if (buy.size() <= 1 || sell.size() <= 1)
		{
			break;
		}
	}
	if (is_ok)
	{
		updateState(price);
		count = duokai + duoping + kongkai + kongping;
		if (type == 1 || type == 4)
			waiPan += count;
		else
			neiPan += count;
		result = getJiaoYiData(price, type, duokai, kongkai, duoping, kongping,time);
		result.setCount(count);
		print(result);
		if (wt.getCount() > count)
		{
			//如果成交数量小于委托数量，剩余的挂单
			wt.setCount(wt.getCount() - count);
			if (type == 1 || type == 4)
			{
				removeBuy(buy[1]);
				addBuy(wt);
			}
			else
			{
				removeSell(sell[1]);
				addSell(wt);
			}
		}
		return 1;
	}
	return 0;
}

void CuoHeServer::updateBS()
{
	if (buy.size() <= 1)
	{
		buyPrice = -1;
		buyCount = 0;
	}
	else
	{
		buyPrice = buy[1].getPrice();
		buyCount = findBuyCount(buyPrice);
	}
	if (sell.size() <= 1)
	{
		sellPrice = -1;
		sellCount = 0;
	}
	else
	{
		sellPrice = sell[1].getPrice();
		sellCount = findSellCount(sellPrice);
	}
}

void CuoHeServer::print(JiaoYiData data)
{
	jiaoYiDataQueue.push(data);
	if (jiaoYiDataQueue.size() > 10)
		jiaoYiDataQueue.pop();
	//打印成交信息
	std::cout << "time:" <<'\n'
		<< data.getTime() <<'\n'
		<< "price:" << data.getPrice() <<'\n'
		<< "optype:" << data.getOptype()<<'\n'
		<< "count:" << data.getCount() <<'\n'
		<< "color:" << data.getColor() << "\n";
}

void CuoHeServer::executeWeiTuoQueue()
{
	while (!weiTuoQueue.empty())
	{
		WeiTuo wt = weiTuoQueue.front();
		//负数代表撤单
		if (wt.getCount() < 0)
		{
			wt.setCount(-wt.getCount());
			if (wt.getOptype() == "duokai" || wt.getOptype() == "kongping")
				removeBuy(wt);
			else
				removeSell(wt);
		}
		else
		{
			if (wt.getOptype() == "duokai" || wt.getOptype() == "kongping")
			{
				addBuy(wt);
			}
			else
			{
				addSell(wt);
			}
			match(wt);
		}
		updateBS();
		weiTuoQueue.pop();
	}
}

int CuoHeServer::removeSell(WeiTuo wt)
{
	//wt的价格对类的sell vector作二分查找，找到和参数所有字段完全相同的元素，删除它。如果找到返回1否则返回0
	int low = 1;
	int high = sell.size() - 1;
	int mid;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (sell[mid].getPrice() > wt.getPrice())
		{
			high = mid - 1;
		}
		else if (sell[mid].getPrice() < wt.getPrice())
		{
			low = mid + 1;
		}
		else
		{
			int i = mid;
			double price = sell[mid].getPrice();
			while(mid>=1&&price==sell[mid].getPrice())
			{
				if (sell[mid].getPrice() == sell[mid].getPrice() && sell[mid].getOptype() == wt.getOptype()&&sell[mid].getCount() == wt.getCount() && sell[mid].getCustomerID() == wt.getCustomerID())
				{
					sell.erase(sell.begin() + mid);
					return 1;
				}
				mid--;
			}
			mid = i + 1;
			while (mid < sell.size() && price == sell[mid].getPrice())
			{
				if (sell[mid].getPrice() == sell[mid].getPrice() && sell[mid].getOptype() == wt.getOptype() && sell[mid].getCount() == wt.getCount() && sell[mid].getCustomerID() == wt.getCustomerID())
				{
					sell.erase(sell.begin() + mid);
					return 1;
				}
				mid++;
			}
			break;
		}
	}
	return 0;
}

int CuoHeServer::findBuyCount(double price)
{
	//二分找到类的buy vector中价格等于price的所有元素的count的和
	int low = 1;
	int high = buy.size() - 1;
	int mid;
	int count = 0;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (buy[mid].getPrice() < price)
		{
			high = mid - 1;
		}
		else if (buy[mid].getPrice() > price)
		{
			low = mid + 1;
		}
		else
		{
			count += buy[mid].getCount();
			int i = mid - 1;
			while (i >= 1 && buy[i].getPrice() == price)
			{
				count += buy[i].getCount();
				i--;
			}
			i = mid + 1;
			while (i < buy.size() && buy[i].getPrice() == price)
			{
				count += buy[i].getCount();
				i++;
			}
			return count;
		}
	}
	return count;
}

int CuoHeServer::findSellCount(double price)
{
	//二分找到类的sell vector中价格等于price的所有元素的count的和
	int low = 1;
	int high = sell.size() - 1;
	int mid;
	int count = 0;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (sell[mid].getPrice() > price)
		{
			high = mid - 1;
		}
		else if (sell[mid].getPrice() < price)
		{
			low = mid + 1;
		}
		else
		{
			count += sell[mid].getCount();
			int i = mid - 1;
			while (i >= 1 && sell[i].getPrice() == price)
			{
				count += sell[i].getCount();
				i--;
			}
			i = mid + 1;
			while (i < sell.size() && sell[i].getPrice() == price)
			{
				count += sell[i].getCount();
				i++;
			}
			return count;
		}
	}
	return count;
}

void CuoHeServer::updateState(double price)
{
	newPrice = price;
	maxPrice = std::max(maxPrice, price);
	minPrice = std::min(minPrice, price);
	if (buy.size() <= 1)
	{
		buyPrice = -1;
		buyCount = 0;
	}
	else
	{
		buyPrice = buy[1].getPrice();
		buyCount = findBuyCount(buyPrice);
	}
	if (sell.size() <= 1)
	{
		sellPrice = -1;
		sellCount = 0;
	}
	else
	{
		sellPrice = sell[1].getPrice();
		sellCount = findSellCount(sellPrice);
	}
	zhangDieFu = (newPrice - kaipanPrice) / kaipanPrice*100;
	zhangDieFu = (int)(zhangDieFu * 100 + 0.5) / 100.0;
	for (auto it = customerMap.begin(); it != customerMap.end(); it++)
	{
		it->second.update(newPrice, pointPrice);
	}
}

std::string CuoHeServer::verdict(WeiTuo wt, int customerID)
{
	int price = wt.getPrice();
	if (wt.getCount() >= 0 && (price > zhangTingPrice || price < dieTingPrice))
	{
		std::cout << "Forbidden!" << std::endl;
		return "超出涨跌停限制";
	}
	else
	{
		CustomerInfo& c = customerMap[customerID];
		int ok = c.receiveWeiTuo(newPrice, pointPrice, baozhengjin, shouxufei, wt);
		if (ok==1)
		{
			weiTuoQueue.push(wt);
			executeWeiTuoQueue();
			return "ok";
		}
		else
		{
			if (ok == -1)
				return "可用资金不足";
			else if (ok == 0)
				return "超出可平仓位";
			else if (ok == -2)
				return "撤销的委托与实际委托不匹配";
			else if (ok == -3)
				return "委托数量达到上限";
			else
				return "未知错误";
		}
	}
	return "ok";
}

void CuoHeServer::printState()
{
	std::cout<<"----------------------"<<std::endl;
	std::cout << "newPrice:" << newPrice << std::endl;
	std::cout << "sellPrice:" << sellPrice << std::endl;
	std::cout << "sellCount:" << sellCount << std::endl;
	std::cout << "buyPrice:" << buyPrice << std::endl;
	std::cout << "buyCount:" << buyCount << std::endl;
	std::cout << "zhangTingPrice:" << zhangTingPrice << std::endl;
	std::cout << "dieTingPrice:" << dieTingPrice << std::endl;
	std::cout << "maxPrice:" << maxPrice << std::endl;
	std::cout << "minPrice:" << minPrice << std::endl;
	std::cout << "waiPan:" << waiPan << std::endl;
	std::cout << "neiPan:" << neiPan << std::endl;
	std::cout << "zhangDieFu:" << zhangDieFu <<"%"<< std::endl;
	std::cout<<"----------------------"<<std::endl;
	std::cout<<std::endl;
}

double CuoHeServer::getKaiPanPrice()
{
	return kaipanPrice;
}

double CuoHeServer::getNewPrice()
{
	return newPrice;
}

double CuoHeServer::getSellPrice()
{
	return sellPrice;
}

int CuoHeServer::getSellCount()
{
	return sellCount;
}

double CuoHeServer::getBuyPrice()
{
	return buyPrice;
}

int CuoHeServer::getBuyCount()
{
	return buyCount;
}

double CuoHeServer::getZhangTingPrice()
{
	return zhangTingPrice;
}

double CuoHeServer::getDieTingPrice()
{
	return dieTingPrice;
}

double CuoHeServer::getMaxPrice()
{
	return maxPrice;
}

double CuoHeServer::getMinPrice()
{
	return minPrice;
}

int CuoHeServer::getWaiPan()
{
	return waiPan;
}

int CuoHeServer::getNeiPan()
{
	return neiPan;
}

double CuoHeServer::getZhangDieFu()
{
	return zhangDieFu;
}

std::vector<WeiTuo> CuoHeServer::getBuy()
{
	return buy;
}

std::vector<WeiTuo> CuoHeServer::getSell()
{
	return sell;
}

std::queue<WeiTuo> CuoHeServer::getWeiTuoQueue()
{
	return weiTuoQueue;
}

int CuoHeServer::accepted(WeiTuo* wt, double price, int count)
{
	//std::cout << "accepted: " << wt->getCustomerID() << " price:" << price << " count::" << count << std::endl;
	CustomerInfo& c = customerMap[wt->getCustomerID()];
	c.acceptWeiTuo(price, pointPrice, baozhengjin, shouxufei, *wt, price, count);
	int d = wt->getCount() - count;
	if (d < 0)
		wt->setCount(0);
	else
		wt->setCount(d);
	return wt->getContractID();
}


//type=1主动为多开，type=2为空开，type=3为多平，type=4为空平
//匹配成功的主动多开和挂单的空开数量相等则result为红色双开
//匹配成功的主动空开和挂单的多开数量相等则result为绿色双开
// 
//匹配成功的主动多开和挂单的多平数量相等则result为红色多换
//匹配成功的主动空开和挂单的空平数量相等则result为绿色空换
// 
//匹配成功的主动多开数量为挂单的空开+多平则result为红色多开
//匹配成功的主动空开数量为挂单的多开+空平则result为绿色空开
// 
//匹配成功的主动多平和挂单的空平数量相等则result为绿色双平
//匹配成功的主动空平和挂单的多平数量相等则result为红色双平
// 
//匹配成功的主动多平数量为挂单的空平+多开则result为绿色多平
//匹配成功的主动空平数量为挂单的多平+空开则result为红色空平
JiaoYiData CuoHeServer::getJiaoYiData(double price, int type, int duokai, int kongkai, int duoping, int kongping, time_t time)
{
	JiaoYiData result;
	int cnt = duokai + kongkai + duoping + kongping;
	result.setCount(cnt);
	result.setTime(time);
	result.setPrice(price);
	if (type == 1)//多开
	{
		if (cnt == kongkai)	//红色双开
		{
			result.setOptype("shuangkai");
			result.setColor(1);
		}
		else if (cnt == duoping)//红色多换
		{
			result.setOptype("duohuan");
			result.setColor(1);
		}
		else//红色多开
		{
			result.setOptype("duokai");
			result.setColor(1);
		}
	}
	if (type == 2)//空开
	{
		if (cnt == duokai)//绿色双开
		{
			result.setOptype("shuangkai");
			result.setColor(0);
		}
		else if (cnt == kongping)//绿色空换
		{
			result.setOptype("konghuan");
			result.setColor(0);
		}
		else//绿色空开
		{
			result.setOptype("kongkai");
			result.setColor(0);
		}
	}
	if (type == 3)//多平
	{
		if (cnt == kongping)//绿色双平
		{
			result.setOptype("shuangping");
			result.setColor(0);
		}
		else//绿色多平
		{
			result.setOptype("duoping");
			result.setColor(0);
		}
	}
	if (type == 4)//空平
	{
		if (cnt == duoping)//红色双平
		{
			result.setOptype("shuangping");
			result.setColor(1);
		}
		else//红色空平
		{
			result.setOptype("kongping");
			result.setColor(1);
		}
	}
	return result;
}

std::map<std::string, std::string> CuoHeServer::getPankou(int customerID)
{
	std::map<std::string, std::string>res;
	res["type"] = "pankou";
	res["newPrice"] = std::to_string(newPrice);
	res["sellPrice"] = std::to_string(sellPrice);
	res["sellCount"] = std::to_string(sellCount);
	res["buyPrice"] = std::to_string(buyPrice);
	res["buyCount"] = std::to_string(buyCount);
	res["zhangTingPrice"] = std::to_string(zhangTingPrice);
	res["dieTingPrice"] = std::to_string(dieTingPrice);
	res["maxPrice"] = std::to_string(maxPrice);
	res["minPrice"] = std::to_string(minPrice);
	res["waiPan"] = std::to_string(waiPan);
	res["neiPan"] = std::to_string(neiPan);
	res["zhangDieFu"] = std::to_string(zhangDieFu);
	res["shouxufei"] = std::to_string((int)shouxufei);
	res["ganggan"] = std::to_string((int)pointPrice);
	getCustomerInfo(res, customerID);
	std::queue<JiaoYiData> temp = jiaoYiDataQueue;
	int i = 1;
	while (!temp.empty())
	{
		JiaoYiData data = temp.front();
		res["time" + std::to_string(i)] = std::to_string(data.getTime());
		res["price" + std::to_string(i)] = std::to_string((int)data.getPrice());
		res["optype" + std::to_string(i)] = data.getOptype();
		res["count" + std::to_string(i)] = std::to_string(data.getCount());
		res["color" + std::to_string(i)] = std::to_string(data.getColor());
		temp.pop();
		i++;
	}
	return res;
}

std::queue<JiaoYiData> CuoHeServer::getJiaoYiDataQueue()
{
	return jiaoYiDataQueue;
}

void CuoHeServer::getCustomerInfo(std::map<std::string, std::string>& res,int customerID)
{
	CustomerInfo c = customerMap[customerID];
	res["quanyi"] = std::to_string(c.quanyi);
	res["keyong"] = std::to_string(c.keyong);
	std::vector<WeiTuo> temp = c.weituo;
	for (int i = 0; i < temp.size(); i++)
	{
		res["_time" + std::to_string(i+1)] = std::to_string(temp[i].getTime());
		res["_optype" + std::to_string(i+1)] = temp[i].getOptype();
		res["_price" + std::to_string(i+1)] = std::to_string(temp[i].getPrice());
		res["_count" + std::to_string(i+1)] = std::to_string(temp[i].getCount());
	}
	res["duo"] = std::to_string(c.chicang.getDuoCount());
	res["kong"] = std::to_string(c.chicang.getKongCount());
	res["avgduo"] = std::to_string(c.chicang.getAvgDuo());
	res["avgkong"] = std::to_string(c.chicang.getAvgKong());
	res["fengxianlv"] = std::to_string(c.fengxianlv);
}

