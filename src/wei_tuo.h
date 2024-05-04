#pragma once
#include<ctime>
#include<string>
class WeiTuo
{
private:
	//�µ�ʱ��
	time_t time;
	//ί��id
	int id;
	//�ͻ�id
	int customerID;
	//��Լid
	int contractID;
	//��Լ����
	std::string contractName;
	//��������
	std::string optype;
	//�µ��۸�
	double price;
	//�µ�����  ����Ϊ�������
	int count;
public:
	WeiTuo(time_t t = -1, int id = -1, int customerID = -1, int contractID = -1, std::string contractName = "", std::string optype = "", double price = -1, int count = -1);
	~WeiTuo();
	time_t getTime();
	int getID();
	int getCustomerID();
	int getContractID();
	std::string getContractName();
	std::string getOptype();
	double getPrice();
	int getCount();
	void setTime(time_t t);
	void setID(int id);
	void setCustomerID(int customerID);
	void setContractID(int contractID);
	void setContractName(std::string contractName);
	void setOptype(std::string optype);
	void setPrice(double price);
	void setCount(int count);
};