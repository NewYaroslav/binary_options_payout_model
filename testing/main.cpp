#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>

#include "IntradeBar.cpp"

using namespace payout_model;

int main(int argc, char const *argv[])
{
	int currency_code; 								//��� ������ �����: 1 - ������, 2 - �����struct 
	tm test_tm={0};									//��������, ���������� ������� �������� ���� � ����� ��������
	
	char test_time_string[20]; 						//������, � ������� ����������� ���� � ����� ��������
	int year, month, day, hour,minute,second; 		//�������� ��������� ���� � ������� ��������
	
	int durationtime; 								//����������������� ��������        
	int currency_pair_index;						//������ �������� ����
	double amount;									//������ � ��������� ������
	
	IntradeBar *intradeBar;
	
	// ��������� ������� ������ ��� ����������� ����������� ���������
	setlocale(LC_ALL, "rus");
	
	std::cout<<"������� ���� � ����� �������� � ������� dd.mm.yyyy hh:MM:ss..."; 
	
	//��������� ���� � ����� �������� ��������� � ������ ������ � ��������� 
	std::cin.getline(test_time_string,sizeof(test_time_string));
	//�� ������ ���������� ������ ������� �����
	std::istringstream test_string_stream(test_time_string);    
	
	//��������� ���������� ��������� test_tm ���������� ��������� ����(�����, �����, ���) � ������� (���, ������, �������) ��������
	//��������������, ��� ���� �������� � �������
	test_string_stream>>std::get_time(&test_tm,"%d.%m.%Y %T");  
	//��������� ����� �������� �� ������ �����
	test_tm.tm_isdst=1; 
	//���� ��������� ������ �� ����� ���� �������������  � ��������� tm, ��� ���� � ��������� �������
	if(test_string_stream.fail()) 
	{
		std::cout<<"���� ������� �����������"<<std::endl;
		return 0;
	}

	std::cout<<"�������, � ����� ������ ����: 1 - ������, 2 - �����"<<std::endl;
	std::cin>>currency_code;
	
	//���� ��������� ������ � ������ �����, ��������, ������ ������ ����� ����� ������ ��� �������������� �����
	if(!std::cin.good())
	{
		std::cout<<"��� ������ ������ �����������. ��������� ������ 1 ��� 2";
		return 0;
	}
	if(!((currency_code==1)||(currency_code==2)))
	{
		std::cout<<"��� ������ ������ �����������. ��������� ������ 1 ��� 2";
		return 0;
	}
	
	
	std::cout<<std::endl<<"������� ����������������� ���������� � �������� (�� 180 �� 30000)...";
	std::cin>>durationtime;
	if(!std::cin.good())
	{
		std::cout<<"����������������� ���������� ������ �����������";
		return 0;
	}
	
	std::cout<<std::endl<<"������� ������ �������� ���� (�� 0 �� 26)...";
	std::cin>>currency_pair_index;
	if(!std::cin.good())
	{
		std::cout<<"������ �������� ���� ����� �����������";
		return 0;
	}
	
	std::cout<<std::endl<<"������� ����� ��������...";
	std::cin>>amount;
	if(!std::cin.good())
	{
		std::cout<<"����� �������� ������� �������...";
		return 0;
	}
	std::cout<<std::endl<<std::endl;
	//����� ��������, �������� ���� currency_code=1 ��� currency_code=2
	if(currency_code==1)
	{
		intradeBar=new IntradeBar("USD");
	}
	else
	{
		intradeBar=new IntradeBar("RUB");
	}
	//IntradeBar intradeBar("RUB");
	double payout=0;
	time_t test_time_t;
	//�������������� �������, ����������� � ��������� test_tm � ����� ���� time_t � ������ �������� �����
	test_time_t=mktime(&test_tm);
	unsigned long long timestamp=test_time_t;
	//��� ������, ������������ ������� get_payout
	int return_code;
	return_code=intradeBar->get_payout(payout,timestamp,durationtime,currency_pair_index,amount);
	
	//������ ���� ������ � �������������� ������������ �� ������
	switch(return_code)
	{
		case PayoutCancelType::DAYOFF:
			std::cout<<"��������. ������� ������: 0"<<std::endl;
			break;
		case PayoutCancelType::NIGHTHOURS:
			std::cout<<"����"<<std::endl;
			break;
		case PayoutCancelType::BEGINEVENINGHOUR:
			std::cout<<"������ ��������� ����"<<std::endl;
			break;
		case PayoutCancelType::TOOLITTLETIME:
			std::cout<<"������� �������� ����������"<<std::endl;
			break;
		case PayoutCancelType::TOOMUCHTIME:
			std::cout<<"������� ������� ����������"<<std::endl;
			break;
		case PayoutCancelType::CURRENCYPAIRISMISSING:
			std::cout<<"����������� ��������� �������� ����"<<std::endl;
			break;
		case PayoutCancelType::TOOLITTLEMONEY:
			std::cout<<"����������� ������� ����� �����"<<std::endl;
			break;
		default:std::cout<<"������� ������: "<<payout<<std::endl;
	}
	std::string test_string; //������, ���������� ��� �������� ����.
	test_string=intradeBar->get_currecy_pair_name(currency_pair_index);
	if(test_string.empty())//���� ������ � ������ �������� ���� ������
	{
		std::cout<<"������ �������� ���� ������ �� ���������"<<std::endl;
	}
	else
	{
		std::cout<<"�������� ����: "<<test_string<<std::endl;
	}
	std::cout<<"������ �������� ���:"<<std::endl;
	//�������� �������, � ������� ����� ��������� ����� ���� �������� ���
	std::vector<std::string> test_vector=intradeBar->get_currecy_pair_list();
	//������� ��� ���������
	for(auto it=test_vector.begin(); it!=test_vector.end();++it)
	{
		std::cout<<*it<<std::endl;
	}
	return 0;
}