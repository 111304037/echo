//#pragma once
///*
//	1 Ŀǰֻ��׷��F8��FF15��ʽ�ĵ��ã�������ռ�������
//	2 Ŀǰÿ��׷������Ϊ65536����Ҫ����Tick����������ã�������������ΪЧ��ԭ��û�ж���������ƣ�Ҫ�Լ�ע��.
//	3 ����ԭ���ǵ���TrackFunctions֮�󣬻�ɨ�����Σ�Ȼ���¼���еĵ��ñ��ã�Ȼ���û�����׷��ĳ������ʱ�����ڼ�¼�в���ָ���ĺ������õ㣬�޸�Ϊ��ת��CallTracker_CallSheel,CallTracker_CallSheel���м�¼���ٵ��øú���.
//	4 ����ʾ��
//		Echo::CallTrackManager::instance().Initialize();
//		Echo::CallTrackManager::instance().Tick();
//		Echo::CallTrackManager::instance().Release();
//	5 Ŀǰֻ��WINDOW��׷�ٿ����ã��Ժ���������ƽ̨��׷��.
//	6 CallTracker_CallSheel�м������������첽���ò��������⣬������CallTracker_CallSheel�м���Ҳ���Ǹ�׷�ٺ������������ܻ����������⣬Ҫ�Լ����������
//*/
//
//#include "Foundation/Base/EchoDef.h"
//#include <map>
//#include <vector>
//
//namespace Echo
//{
//#ifdef ECHO_PLATFORM_WINDOWS
//	Dword __stdcall GetOpCodeSize(unsigned char* iptr0);
//#endif
//
//	class ECHO_EXPORT_FOUNDATION CallTrackModule
//	{
//		friend 	void libEvent_Read(struct bufferevent*,void*);
//	public:
//		CallTrackModule(const char* pathName,const char* fileName);
//		~CallTrackModule(void);
//		
//		// ���׷����Ϣ
//		void OutputTrackerInfo(FILE* fp);
//		// ��ȡ����
//		const std::string& GetPDBPathName(){ return m_sPDBPathName; }
//		// ��ȡ����
//		const std::string& GetModuleName(){ return m_sFileName; }
//		// ��ȡȫ�����ŵ�����
//		const std::map<std::string,void*>& GetAllSymload(){ return m_SymloadList; };
//		// ��ȡ��ѯ��ַ
//		void* ToSymloadAddress(void* address);
//		// ����
//		bool Load(const std::vector<unsigned int>& e8Call,const std::vector<unsigned int>& ff15Call);
//		// ж��
//		void Unload();
//		// ׷��
//		int Track(char* funName);
//		// ȡ��׷��
//		int CancelTrack(char* funName);
//
//	private:
//		void TrackCall(const std::string& callName,unsigned int codeAddress,unsigned int jumpToAddress);
//		void* GetFunctionAddress(const char* sName);
//		bool CheckAddress(void* address);
//		size_t GetPageSize(){ return 4096; }
//		bool EnablePageReadWrite(void* address,size_t size);
//		bool EnablePageReadWriteByte(void* address);
//		const std::map<std::string,void*> GetFunctionDist(const std::vector<std::string>& vsFunction);
//
//	private:
//		unsigned long	m_BaseOfDll;
//		void*			m_BaseAddress;
//		std::string		m_sPDBPathName;
//		std::string		m_sFileName;
//		//std::map<std::string,std::vector<void*>> m_TrackRecordList;
//
//		std::map<std::string,void*>	m_SymloadList;//���еķ���
//		std::map<void*,std::string>	m_QueryList;//ͨ�����������ҵ���ǰλ�ö�Ӧ�ĺ�������
//		std::map<void*,void*> m_AddrToSymAddrList; //ʵ�ʴ����ַ��ģ���ļ���ַ֮���ת����
//		typedef struct
//		{
//			unsigned int address;
//			unsigned int jumpTo;
//			unsigned int queryPos;
//		}CallPos;
//		std::multimap<std::string,CallPos> m_e8CallPos;//����λ��
//		std::multimap<std::string,CallPos> m_ff15CallPos;//����λ��
//		typedef struct 
//		{
//			unsigned int  callAddress;//�����е�λ��
//			unsigned int  allocAddress;//�����ַ
//			unsigned char oldCode[6];//ԭʼ����
//		}TrackRecord;
//		std::multimap<std::string,TrackRecord> m_callRecord;//׷�ټ�¼�����ڻָ�
//	};
//
//	class ECHO_EXPORT_FOUNDATION CallTrackManager
//	{
//		friend struct bufferevent*& getClientEvent();
//		friend void libEvent_Read(struct bufferevent* bev,void* ctx);
//		friend void libEvent_Event(struct bufferevent* bev,short events,void* ctx);
//
//		std::vector<CallTrackModule*> ma_Modules;
//		struct event_base* m_base;
//		std::vector<unsigned int> ma_e8Call;
//		std::vector<unsigned int> ma_ff15Call;
//	public:
//		CallTrackManager();
//		~CallTrackManager();
//		static CallTrackManager& instance();				// ����
//		bool Initialize();									// ��ʼ��
//		void Tick();										// ÿ֡���д���
//		void Release();										// �ر�
//		static std::vector<std::string> GetModuleList();	// ��ȡ����ģ�������
//	private:
//		int ListenCall(void* callAddress);
//		int ListenCall(const char* callList);				// ����ĳ������
//		int CancelListenCall(void* callAddress);			// ȡ������
//		int CancelListenCall(const char* callList);			// ȡ������
//		std::vector<std::string> ToVectorString(const char* str);	//ת����Ϊ���鴮
//		bool CheckAddress(void* address);					// ���һ����ַ�Ƿ���Ч
//	private:
//		struct evconnlistener*		m_listener_event;
//		struct bufferevent*			m_client_event;
//	public:
//		void OnMsg(struct bufferevent* bev,void* ctx);	//������Ϣ
//		void OnMsg_GetModuleList(struct bufferevent* bev,void* ctx);//����ģ���
//		void OnMsg_LoadModule(struct bufferevent* bev,void* ctx);//����ģ���
//		void OnMsg_UnloadModule(struct bufferevent* bev,void* ctx);//�ͷ�ģ���
//		void OnMsg_GetSymloadList(struct bufferevent* bev,void* ctx);
//		void OnMsg_Track(struct bufferevent* bev,void* ctx); //׷��
//		void OnMsg_CancelTrack(struct bufferevent* bev,void* ctx); //ȡ��׷��
//	};
//}
