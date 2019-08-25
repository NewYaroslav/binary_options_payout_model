#include <vector>
#include <string>
#include <map>
//------------------------------------------------------------------------------------------------------
namespace payout_model
{
	///������ ����� ������ ���������� ������
	enum PayoutCancelType{
			OK=0,							///< ������ �����������
			DAYOFF=1,						///< �������� ���� ��� ��������
			NIGHTHOURS=2, 					///< ������ �����, ����� ������ �� �������� ������
			BEGINEVENINGHOUR=3, 			///< ������ ������ � ������ ���� ������� (���� ��������� ������ � ����� ����)
			TOOLITTLETIME=4, 				///< ������� �������� ����� ����������
			TOOMUCHTIME=5, 					///< ������� ������� ����� ����������
			CURRENCYPAIRISMISSING=6, 		///< ����������� �������� ���� � ��������� ��������
			TOOLITTLEMONEY=7				///< ������� ������ ������
	};
//------------------------------------------------------------------------------------------------------
	class IntradeBar
	{
		private:
			std::string currency_name;								///< ������������ ������ �����. ��� �������, USD ��� RUB
			static std::map<int, std::string> currency_pairs;       ///< ������ ��������� �������� ��� ������� IntradeBar � ������� <����, ��������>
		
		public:
		
	
			/** \brief �������� ������� ������

			* �������� ������ ����������� ������ �� 0 �� 1.0, ��� 1.0 ������������� 100% ������� �������
			* \param[out] payout ������� ������
			* \param[in] timestamp ��������� ����� unix ������� (GMT)
			* \param[in] duration ������������ ������� � ��������
			* \param[in] currency_pair_indx  ����� �������� ���� �� ������ �������� ��� �������
			* \param[in] amount ������ ������ ��������� �������
			* \return ��������� ������� (0 � ������ ������, ����� ��. PayoutCancelType)
			*/
			int get_payout(double &payout, 
							const unsigned long long timestamp, 
							const int duration, 
							const int currency_pair_indx, 
							const double amount);
			
			
			/** \brief �������� ��� �������� ���� �� �� ������

			* \param[in] currency_pair_indx  ����� �������� ���� �� ������ �������� ��� �������
			* \return ��� �������� ���� ���� ������ ������, ���� ��������� ������ ����������� � ������ �������� ���
			*/
			std::string get_currecy_pair_name(const int currency_pair_indx);
			
			
			///�������� ������ ���� �������� ��� �������
			std::vector<std::string> get_currecy_pair_list();
			
			/** 
			  * � ������������ ������ IntradeBar ������������ ������ �����.
			  * � �������� ������ ����� �� ��������� ������������ RUB
			*/
			IntradeBar()
			{
				currency_name="RUB";
			}
		
			IntradeBar(std::string _currency_name)
			{
				currency_name=_currency_name;
			}
				
			~IntradeBar()
			{
			
			}
		
	};
	
	std::map<int, std::string> IntradeBar::currency_pairs={{0,"EURUSD"},{1,"USDJPY"},{2,"GBPUSD"},{3,"USDCHF"},
																		{4,"USDCAD"},{5,"EURJPY"},{6,"AUDUSD"},{7,"NZDUSD"},
																		{8,"EURGBP"},{9,"EURCHF"},{10,"AUDJPY"},{11,"GBPJPY"},
																		{12,"CHFJPY"},{13,"EURCAD"},{14,"AUDCAD"},{15,"CADJPY"},
																		{16,"NZDJPY"},{17,"AUDNZD"},{18,"GBPAUD"},{19,"EURAUD"},
																		{20,"GBPCHF"},{21,"EURNZD"},{22,"AUDCHF"},{23,"GBPNZD"},
																		{24,"USDRUB"},{25,"GBPCAD"},{26,"GC"}};
	
	/** \brief �������� ������� ������

			* �������� ������ ����������� ������ �� 0 �� 1.0, ��� 1.0 ������������� 100% ������� �������
			* \param[out] payout ������� ������
			* \param[in] timestamp ��������� ����� unix ������� (GMT)
			* \param[in] duration ������������ ������� � ��������
			* \param[in] currency_pair_indx  ����� �������� ���� �� ������ �������� ��� �������
			* \param[in] amount ������ ������ ��������� �������
			* \return ��������� ������� (0 � ������ ������, ����� ��. PayoutCancelType)
	*/
	inline int IntradeBar::get_payout(double &payout, 
										const unsigned long long timestamp, 
										const int duration, 
										const int currency_pair_indx, 
										const double amount)
	{
		struct tm *operation_time_in_tm;						///< ����� ������ �������� �� ��������, �������������� � ������� ��������� ��������� tm

		time_t operation_time_in_time_t=timestamp;				///< ����� ������ �������� � ������� time_t, ��������������� ����� ��������� ����� unix ������� (GMT)
		
		/*
		 *������� gmtime ��������� �������� ��������� �� ��������� tm, ���������� ����������� ����� � ����������� �� ���������� �����. 
		 * � �������� ��������� � ������� ���������� ��������� �� ��������� ����� ������� GMT, ��� ������� ���������� ����������� ��������������
		 */
		operation_time_in_tm=gmtime(&operation_time_in_time_t);
		
		/*
		 *year - ��� ���������� ��������
		 *tm_year - ��������� ���������� ��� �� 1900 ���� �� ����, ���������� � ��������
		 */
		int year=operation_time_in_tm->tm_year+1900;	
		
		/*
		 * month - ����� ���������� ��������
		 * tm_month - ��������� ���������� ������� � 1 ������ ���� tm_year �� ����, ���������� � ��������
		 */
		int month=operation_time_in_tm->tm_mon+1;
		
		// day - ����� ������
		int day=operation_time_in_tm->tm_mday;
		
		// hour - ���������� ����� �� 0 �� 23
		int hour=operation_time_in_tm->tm_hour;
		int minute=operation_time_in_tm->tm_min;
		int second=operation_time_in_tm->tm_sec;
		
		// weekday - ���� ������ �� 0 �� 6, ������� � ����������� 
		int weekday=operation_time_in_tm->tm_wday;
		
		std::string currency_pair_name; //������������ �������� ���
				
		// ���� �������� ��������� � �������, �����������, 1 ������ ��� 25 �������
		// ������� ����������� payout=0
		// ������� ���� ������� ���������� ������� PayoutCancelType::DAYOFF 
		if(weekday==6||weekday==0||(day==1&&month==1)||(day==25&&month==12))
		{
			payout=0;
			return PayoutCancelType::DAYOFF;
		}//if
		
		// ���� �������� ��������� ����� 21:00 �� ��������
		// ������� ����������� payout=0
		// ������� ���� ������� ���������� ������� PayoutCancelType::NIGHTHOURS 
		if(hour>=21)
		{
			payout=0;
			return PayoutCancelType::NIGHTHOURS;
		}//if
		
		// ���� �������� ��������� ����� � 14:00 �� 20:00 �� �������� � ��� ������ ��� ��������� ������ ����
		// ������� ����������� payout=0
		// ������� ���� ������� ���������� ������� PayoutCancelType::BEGINEVENINGHOUR 
		if(((hour>=14&&hour<=19)&&(minute==59||minute==0||minute==1))||
			(hour==13&&minute==59)||(hour==21&&(minute==0||minute==1)))
		{
			payout=0;
			return PayoutCancelType::BEGINEVENINGHOUR;
		}//if
		
		// ���� ����������������� ���������� ������ 3 ����� (180 ������)
		// ������� ����������� payout=0
		// ������� ���� ������� ���������� ������� PayoutCancelType::BEGINEVENINGHOUR 
		if(duration<180)
		{
			payout=0;
			return PayoutCancelType::TOOLITTLETIME;
		}//if
		
		// ���� ����������������� ���������� ������ 500 ����� (30000 ������)
		// ������� ����������� payout=0
		// ������� ���� ������� ���������� ������� PayoutCancelType::TOOMUCHTIME 
		if(duration>30000)
		{
			payout=0;
			return PayoutCancelType::TOOMUCHTIME;
		}//if
		
		// ��������� ����� �������� ����, ����������� � �������� �� �� �������
		// ����������� �������� �������� ��� � �� ���� ���������� ���������� currency_pairs
		
		currency_pair_name=get_currecy_pair_name(currency_pair_indx);
		
		// ���� ����� get_currecy_pair_name ������ ������ ������, �.�. � ������ ����������� �������� ���� � �������� currency_pair_indx
		// ������� ����������� payout=0
		// ������� ���� ������� ���������� ������� PayoutCancelType::CURRENCYPAIRISMISSING 
		if(currency_pair_name.empty())
		{
			payout=0;
			return PayoutCancelType::CURRENCYPAIRISMISSING;
		}//if

		// ���� ������� ����� ������� ���� ������ 0
		// ������� ����������� payout=0
		// ������� ���� ������� ���������� ������� PayoutCancelType::TOOLITTLEMONEY 
		if(amount<=0)
		{
			payout=0;
			return PayoutCancelType::TOOLITTLEMONEY;
		}
		
		// ���� �������� ��������� ����� 9 ������ 2019 ����
		if(year>=2019&&month>=1&&day>=9)
		{
			// ���� ���� � �������� � ������ ������ 80 �������� ��� ���� � ������ � ������ ������ 5000 ������
			if((currency_name=="USD"&&amount>=80)||(currency_name=="RUB"&&amount>=5000))
			{
				payout=0.85;// ������� ������ �������� 85 (0,85)
			}
			// ���� ���� � �������� � ������ ������ 80 �������� ��� ���� � ������ � ������ ������ 5000 ������
			else
			{
				// ���� ����������������� ���������� �� 3 �� 4 �����
				if(duration>=180&&duration<240)
				{
					payout=0.82; // ������� ������ �������� 82 (0,82)
				}
				else
				{
					// ���� ����������������� ���������� �� 4 �� 500 �����
					if(duration>=240&&duration<=30000)
					{
						payout=0.79; // ������� ������ �������� 79 (0,79)
					}
				}
			}
		}// if
		// ���� �������� ��������� ����� 9 ������ 2019 ����
		else
		{
			// ���� ���� � �������� � ������ ������ 80 �������� ��� ���� � ������ � ������ ������ 5000 ������
			if((currency_name=="USD"&&amount>=80)||(currency_name=="RUB"&&amount>=5000))
			{
				payout=0.84; // ������� ������ �������� 84 (0,84)
			}
			// ���� ���� � �������� � ������ ������ 80 �������� ��� ���� � ������ � ������ ������ 5000 ������
			else
			{
				// ���� ����������������� ���������� �� 3 �� 4 �����
				if(duration>=180&&duration<240)
				{
					payout=0.8; // ������� ������ �������� 80 (0,8)
				}
				else
				{
				    // ���� ����������������� ���������� �� 4 �� 500 �����
					if(duration>=240&&duration<=30000)
					{
						payout=0.77; // ������� ������ �������� 77 (0,77)
					}
				}
			}
		}
		
		return 0;
	};
	
	
	/** \brief �������� ��� �������� ���� �� �� ������

	 * \param[in] currency_pair_indx  ����� �������� ���� �� ������ �������� ��� �������
	 * \return ��� �������� ���� ���� ������ ������, ���� ��������� ������ ����������� � ������ �������� ���
	*/
	inline std::string IntradeBar::get_currecy_pair_name(const int currency_pair_indx)
	{
		// ������� � ����� ���� ��������� ��������� currency_pairs ��� ������ ���������
		// currency_pairs.begin() - ��������� ��������� �� ������ ���������
		// currency_pair_index!=currency_pairs.end() - �������� ��������� �� �������� �� ��������� �������� ���������
		// ++currency_pair_index - ������� ��������� �� ��������� ������� ���������
		for(auto currency_pair_index=currency_pairs.begin(); currency_pair_index!=currency_pairs.end(); ++currency_pair_index)
		{
			if((*currency_pair_index).first==currency_pair_indx) //���� ������ ��������, �� ������� ��������� �������� ����� ��������
				// ������� ����� �������� ����, ��������������� ����� �������
				return (*currency_pair_index).second;
		}
		// � ��� ����� ���� ��������, ���� � ��������� currency_pairs ����������� �������� ���� � �������� currency_pair_index
		return std::string();//������� ������ ������
	};
	
	///�������� ������ ���� �������� ��� �������
	inline std::vector<std::string> IntradeBar::get_currecy_pair_list()
	{
		std::vector<std::string> currency_pair_list; // ������, � ������� ���������� ����� ���� �������� ���
		currency_pair_list.clear(); //������� ������� �� ������
		
		// ������� � ����� ���� ��������� ��������� currency_pairs ��� ������ ���������
		// currency_pairs.begin() - ��������� ��������� �� ������ ���������
		// currency_pair_index!=currency_pairs.end() - �������� ��������� �� �������� �� ��������� �������� ���������
		// ++currency_pair_index - ������� ��������� �� ��������� ������� ���������
		for(auto currency_pair_index=currency_pairs.begin(); currency_pair_index!=currency_pairs.end(); ++currency_pair_index)
		{
			// � ����� ������� currency_pair_list ����������� ��� �������� ����, �� ������� ��������� ��������
			currency_pair_list.push_back((*currency_pair_index).second);
		}
		return currency_pair_list; //������� �������, ����������� ����� ���� �������� ���
	};
	
	
	
}