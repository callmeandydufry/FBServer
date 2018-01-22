
#ifndef __BASETOOL_H__
#define __BASETOOL_H__

////////////////////////////////////////////////////////////////////////////////
// Time Module
////////////////////////////////////////////////////////////////////////////////
#include "TypeDefine.h"

#ifdef __LINUX__
#include <sys/utsname.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#ifdef __WINDOWS__
#include "windows.h"
#include <io.h>
#include <direct.h>
#endif

class	TimeSystem
{
	TimeSystem(void);
	~TimeSystem(void);

public :

	//���ڼ�������
	enum DayCountType
	{
		DATE = 0,		//�����ڼ���
		WEEK,			//�����ڼ���
		DATE_OR_WEEK,	//�������ڶ���Ч
		ERVERY_DAY,		//�������ڶ���Ч,��ÿ��

		DAY_COUNT_TYPE_NUM,
	};

public:
	static TimeSystem	&instance(void);

	BOOL				Init();

public:
	uint32				TickCount();
	uint32				NowSavedTime() const { return m_Now; }
	uint32				OrigineTime() const { return m_Begin; }
	void				Update() const;
	uint32				GetCTime() const;
	uint32				RefixANSITimeByTimeZone(uint32 uTime) const;

	//�жϵ�ǰʱ���Ƿ���ָ��ʱ�䷶Χ�� nDayCountTypeΪ��������: ���ܼ���/�����ڼ���; �����ʱ��Сʱ������԰ٽ��Ƽ���
	BOOL				IsTimeInRange(DayCountType nDayCountType, int32 nDay, ulong uBeginTime, ulong uEndTime);

public:
	int32				year() const { return m_TM.tm_year + 1900; }
	int32				month() const { return m_TM.tm_mon + 1; }
	int32				day() const { return m_TM.tm_mday; }
	int32				hour() const { return m_TM.tm_hour; }
	int32				minute() const { return m_TM.tm_min; }
	int32				second() const { return m_TM.tm_sec; }
	uint32				week() const { return m_TM.tm_wday; }
	int32				yday() const { return m_TM.tm_yday; }
	

	uint32				Time2DWORD() const;
	uint32				CurrentDate() const;
	uint32				DiffTime(uint32 Date1, uint32 Date2) const;

	void				ConvertUT(uint32 Date, tm *TM) const;
	void				ConvertTU(tm *TM, uint32 &Date) const;
	uint32				GetDayTime();
	void				LocalTime( time_t in, tm& out ); // ��ת��Ϊtm [xingzhishan]
	void				LocalTime( tm& in, uint32 &out); // tmת��Ϊ�� [xingzhishan]
	uint32				GetNextGameLogicDay(uint32 inTime);	// �Ե�ǰʱ��Ϊ��׼��ȡ��Ϸ�߼��ϵ���һ�ο���ʱ�� [xingzhishan]	
	uint32				GetCurrGameLogicDay(uint32 inTime);	
	int32				GetWeekNumOfYear();					//�õ���ǰ�ǽ���ĵڼ���[1-52]
	int32				GetWeekNumFrom2011();				//��ȡ��2011�꿪ʼ����ǰһ���ǵڼ���
	int32               GetYearAndWeekNum(); //�õ�һ��6λ������201001(2010���һ��)
	int32				GetTodaySecondTime();
	uint32				GetMondayGameLogicTime(uint32 uTime);// ��ñ�����һ�賿6��ʱ�� [5/9/2016 TianShuai]
	uint32				GetNextMondayGameLogicTime(uint32 uTime);// ���������һ�賿6��ʱ�� [5/9/2016 TianShuai]
	uint32				GetWeekDay24Time();// ��ñ�����24�㣬������һ���[3/24/2017 TianShuai]
	uint32				GetMuchWeeksLogicTime(int32 muchWeek, int32 week, int32 hour, int32 minute); // ���ÿn�ܵ�ʱ�� [9/18/2016 scw]
	
	int32				GetWeekNumFrom2011(int32 iDays);  //����һ����2011��1��1�տ�ʼ�ۻ��������������ʱ���ǵڼ��ܣ���2011��1��1�տ�ʼ���㣩
	int32				GetDaysNumEplised(); //�����2011��1��1�տ�ʼ��������������ܹ���ȥ�˶�����
	int32				GetDaysNumEplised(uint32 uTime); //�����2011��1��1�տ�ʼ��������������ܹ���ȥ�˶�����

	uint32				GetDayTimeBeginTime(uint32 uTime); //��ȡ��ǰ���ڵ�һ�����ʼʱ��

	void				FormatDataTime(char *pStr,uint32 nTime);
	void				FormatDataTime(char *pStr,tm &t);
	void				FormatDataTime(char *pStr);
	void				FormatDataTimeWithoutSeparate(char *pStr,uint32 nTime);

	uint32				GetCurrHourTime(); // ��õ�ǰ����ʱ�� [9/7/2016 scw]
	uint32				GetThisDayTime(int32 nHour, int32 nMin);
	uint32				GetThisWeekTime(int32 nWeekDay, int32 nHour, int32 nMin); //���ָ���ܼ���ָ��Сʱ��  ��0-6
private:
	uint32				m_Begin;
	mutable uint32		m_Now;
	mutable time_t		m_TimeT;
	mutable tm			m_TM;

#ifdef __LINUX__
	struct timeval		_tstart;
	struct timezone		tz;
#endif

	void				_CurrentTime();
};

#define INIT_TIMESYSTEM()	TimeSystem::instance().Init()
#define GET_TIME()			TimeSystem::instance()

////////////////////////////////////////////////////////////////////////////////
// Log Module
////////////////////////////////////////////////////////////////////////////////
enum
{
	QUICK_LOG_LOGIN		= 0,	// �û���½���̴��� [yangzhen]
	QUICK_LOG_DEBUG		= 1,	// ��ͨ������Ϣ [yangzhen]
	QUICK_LOG_ERROR		= 2,	// ������ϵͳ�ڲ����ش��� [yangzhen]
	QUICK_LOG_LUA_ERROR	= 3,	// ������lua�ű����� [yangzhen]
	QUICK_LOG_FUNCTION	= 4,	// lua�ű����� [yangzhen]
	QUICK_LOG_WORLD		= 5,	// ���߳���ͳ����Ϣ [yangzhen]
	QUICK_LOG_ITEMCHANGE	= 6,	// ��Ʒ��ά����ͳ�� [yangzhen]
	QUICK_LOG_MONEYCHANGE	= 7,	// ��Ʒ��ά����ͳ�� [yangzhen]
	QUICK_LOG_CHAT		= 8,	// ������Ϣ��¼ [yangzhen]
	QUICK_LOG_GM		= 9,	// GameMaster������־��¼ [yangzhen]
	QUICK_LOG_IBMONEY	= 10,
	QUICK_LOG_IBCOST	= 11,
	QUICK_LOG_ENTERMAP  = 12,
	QUICK_LOG_TICK_TIMER	= 13,	// Tick��ʱ��,����map��player��monster��������tick��ʱ
	QUICK_LOG_DBDEBUG       = 14,    //���ݿ�
	QUICK_LOG_TICK          = 15,    //�߳�tick
	QUICK_LOG_PLAYERHERO    = 16,    //Ӣ���������л����
	QUICK_LOG_POSTMAIL    = 17,    //�����ʼ�
	QUICK_LOG_AWARDPOST   = 18,    //�����ʼ�
	QUICK_LOG_BATTLE	= 19,	// ս����־ [yangzhen]
	QUICK_LOG_ARENA		= 20,	// ��������־ [yangzhen]
	QUICK_LOG_ITEM_OPERATE = 21,	// ���е���Ʒ������־ [yangzhen]
	QUICK_LOG_MAPLAWLESSOP = 22,	// ��¼��ҷǷ����� [yangzhen]
	QUICK_LOG_PLAYERUSESKILL = 23,	// ���ʹ�ü��ܵ�ͳ�� [yangzhen]
	QUICK_LOG_SKILL = 24,	// ��¼��Ҽ���������־ [yangzhen]
	QUICK_LOG_PLAYERLOGINOUT = 25,	// ��ҵ�½�ǳ���ά����ͳ�� [wangyiran]
	QUICK_LOG_ORDER_DEBUG = 26,	// ����ϵͳ������־ [wangxiaoyong]
	QUICK_LOG_ORDER_ERROR = 27,	// ����ϵͳ������־ [wangxiaoyong]
	QUICK_LOG_ORDER_BILLING = 28, // ֻдbilling�����Ķ��� [wangxiaoyong]
	QUICK_LOG_EQUIP = 29, // װ����־ [xingzhishan]
	QUICK_LOG_DB_HEROGROW = 30, // ֻдӢ�ۿ������� [wangxiaoyong]
	QUICK_LOG_AWARDPOST_DB = 31, // �����ʼ���־ [xingzhishan]
	QUICK_LOG_POST_DB = 32, // �ʼ���־ [xingzhishan]
	QUICK_LOG_OBJ_INFO	= 33,	// obj��Ϣ [yangzhen]
	QUICK_LOG_CDKEY	= 34,	// CDKey [xingzhishan]
	QUICK_LOG_DEBUGDUMP	= 35,	// ͨ�����Ե�GM�����ӡ����ĵ�����־ [yangzhen]
	QUICK_LOG_CONNECTOR	= 36,	// ��Connector���Ӷ���ĵ�����־ [yangzhen]
	QUICK_LOG_WECHATRED	= 37,	// ΢���������־ [yangzhen]
	QUICK_LOG_TOPLIST_RANK,
	QUICK_LOG_ARENA_AWARD,
	QUICK_LOG_ZUOBI,
	QUICK_LOG_ACTIVITY_STATUS,
	QUICK_LOG_PLAYER_RENAME,
	QUICK_LOG_GUILD_RENAME,
	QUICK_LOG_PLAYER_PHYSICALPOWER_OVERFLOW,
	QUICK_LOG_ARENA_RANK_UP_AWARD, //������������������ [xingzhishan]
	QUICK_LOG_CITY_BUILDING_MAP,
	SLOW_LOG_AIDEBUG,
	SLOW_LOG_SERVER,           
	SLOW_LOG_RECONNECT,          
	SLOW_LOG_MSGERROR,           
	SLOW_LOG_SERVERERROR,        
	SLOW_LOG_WORLD,              
	SLOW_LOG_RECYCLEPLAYER,      
	SLOW_LOG_MAPSERVER_SHAREMEM, 
	SLOW_LOG_GL_DBCENTER,        
	SLOW_LOG_DEAMON,             
	SLOW_LOG_CLIENT_ERROR_REPORT,
	SLOW_LOG_MAPSERVER_USAGE,    
	SLOW_LOG_MSG_RECORD,         
	SLOW_LOG_RAW_DBSHARE,        
	SLOW_LOG_RAW_BILLING,        
	SLOW_LOG_RAW_MAPSERVER,	     
	SLOW_LOG_RAW_PLATFORM,       
	SLOW_LOG_NET_DEBUG,
	SLOW_LOG_SMERROR,
	SLOW_LOG_REMOTE_LOG_SERVER,
	SLOW_LOG_GAME_LOGIC_ITEMFLOW,
	SLOW_LOG_GAME_LOGIC_MONEYFLOW,
	SLOW_LOG_GAME_LOGIC_USERCHARGE,
	SLOW_LOG_GAME_LOGIC_USERLOGIN,
	SLOW_LOG_GAME_LOGIC_USERLOGOUT,
	SLOW_LOG_GAME_LOGIC_USERREGISTER,
	SLOW_LOG_GAME_LOGIC_BATTLEINFO,
	SLOW_LOG_MERGE_SERVER,
	SLOW_LOG_PLAYER_ARTIFACT,//����
	SLOW_LOG_PLAYER_FIND_TREASURE, // Ѱ����־ [4/22/2016 tianshuai]
	SLOW_LOG_PLAYER_HERO_TRANSFER, // Ӣ��ת����־ [4/22/2016 tianshuai]
	SLOW_LOG_PLAYER_INDENTURE, // ��Լ���� [11/23/2016 TianShuai]
	SLOW_LOG_SHOP_REPEAT_BUY_LIMIT_COUNT_ITEM, // �ظ������д������ƵĶ�����Ʒ���������� [6/15/2017 "tianshuai"]
	SLOW_LOG_PLAYER_EFFECT_UP_DATA, // ����Ч�� [7/6/2017 "tianshuai"]
	SLOW_LOG_ABATTOIR, // ս�����־ [8/2/2017 Chief]
	ALL_LOG_NUMBER,
};

enum eLOG_BACKUP_TYPE
{
	eLOG_BACKUP_BY_TIME	= 0,	// ���챸����־
	eLOG_BACKUP_SERVER_START,			// ����ʱ��ⱸ����־
};

namespace QLogSystem
{

	void	QUICK_LOG(int32 eChannel, const char *cszFormat, ...);
	void	SLOW_AUDIT_LOG(int32 eChannel, const char *cszFormat, ...);

	class	nonecopy
	{
	public:
		nonecopy() {}
		~nonecopy() {}

	private:

		nonecopy(const nonecopy &) {}
		const nonecopy &operator=(const nonecopy &) {}
	};

	class QLogger :
		nonecopy
	{
	enum
	{
		eQUICK_LOG,
		eSLOW_LOG,
	};
	private:
		QLogger(void);
		~QLogger(void);
	
	public:
		static QLogger	&instance(void);

		BOOL			Init(int32 cachesize);
		void			RegisterAllLogPath();
		void			RegisterDir();
		void			RegisterLogPath(int32 eChannel, char* szPath, int8 LogType, BOOL isPrintToConsole = FALSE, BOOL isUseHoral = FALSE);
		void			UpdateAll(void);

		void UploadingLog(int32 eChannel, const char * buff );
		BOOL isUploadingLog(int32 eChannel);
		int8 GetLogType(int32 eChannel);
		BOOL			IsSlowLog(int32 eChannel);
		BOOL			IsPrintToConsole(int32 eChannel);

		void			write_log_realtime(int32 channel, char *msg, int32 elapsedDay);
		void			cache_log_notime(int32 channel, char *msg, int32 elapsedDay);
		void			UpdateNoTime(int32 logid);

		void			setIsUseBackUpBuff(BOOL isUse)
		{
			m_isUseBackUPBuff = isUse;
		}

		void GetChannelNameLocal(int32 channel, char *szName);
#if __MAPSERVER__
		void GetChannelNameRemote(int32 channel, char *szName);
#endif
		void			BackUpLog(eLOG_BACKUP_TYPE eType);
		BOOL			GetFileData(char* szFileName, int32 &nYear, int32 &nMonth, int32 &nDay);
		void			CreateBackUpFolder(int32 nYear, int32 nMonth, int32 nDay);
		void			GetNewPathFileName(char* szOldName, char *szName, int32 &nYear, int32 &nMonth, int32 &nDay);
		void			MoveLog(int32 eChannel,int32 nCurYear, int32 nCurMonth, int32 nCurDay, eLOG_BACKUP_TYPE eType);
		void			MoveLog(char* szOldName, int32 nCurYear, int32 nCurMonth, int32 nCurDay, eLOG_BACKUP_TYPE eType);
	protected:
		void			showmsg(const char *cszMsg);
		// �ڿ����ʱ�򻺴���־ [7/29/2015 yangzhen]
		void			cacheBackupLog(int32 eChannel, char *szMsg);
		void			flushCacheBackupLog(int32 eChannel);

	private:
		char			*m_pMemoryLog[ALL_LOG_NUMBER];
		int32			m_MemorySize;
		int32			m_WritePos[ALL_LOG_NUMBER];
		FastLock		m_channelLock[ALL_LOG_NUMBER];
		char			*m_szLogPath[ALL_LOG_NUMBER];
		BOOL			m_isUploading[ALL_LOG_NUMBER];
		int8			m_LogType[ALL_LOG_NUMBER];
		BOOL			m_isPrintToConsole[ALL_LOG_NUMBER];
		BOOL			m_isUseHoral[ALL_LOG_NUMBER]; // ʹ��(������ʱ)ÿСʱ��׺��־�ļ� [11/3/2016 scw]
		char			*m_pBackupCacheLog[ALL_LOG_NUMBER];
		int32			m_BackupLogWritePos[ALL_LOG_NUMBER];
		char			*m_pSaveLog;
		int32			m_nLastBackUpTime;

		BOOL			m_isUseBackUPBuff; // �Ƿ񱸷���־ [11/3/2016 scw]
	};

//#ifdef __WINDOWS__
#ifndef __RELEASE_SERVER__
	static const int32	ciDefault_Log_Cache_Size = 1024 * 16;
#else
	static const int32	ciDefault_Log_Cache_Size = 1024 * 1024 * 8;
#endif
	static const int32	ciLog_Temp_Buff = 4096;
	static const int32	ciLog_Temp_Name = 128;

	BOOL				__get_logger_guard_state(void);
	void				__open_print_log(BOOL bIsOpen);
	BOOL				__is_open_print_log(void);
	void				__open_allinone(BOOL bIsOpen);
	BOOL				__is_open_allinone(void);

}

#if defined(__LINUX__)
#define QLOGGER_CODEHOLDER() \
	(QLogSystem::QUICK_LOG(SLOW_LOG_SERVERERROR, "%s %d %s", __FILE__, __LINE__, __PRETTY_FUNCTION__))
#define QLOGGER_MESSAGECODEHOLDER(x) \
	( \
	QLogSystem::QUICK_LOG \
	( \
	SLOW_LOG_SERVERERROR, \
	"f:%s l:%d p:%d %s", \
	__FILE__, \
	__LINE__, \
	x, \
	__PRETTY_FUNCTION__ \
	) \
	)
#else
#define QLOGGER_CODEHOLDER()	(QLogSystem::QUICK_LOG(SLOW_LOG_SERVERERROR, "%s %d %s", __FILE__, __LINE__, __FUNCTION__))
#define QLOGGER_MESSAGECODEHOLDER(x) \
	(QLogSystem::QUICK_LOG(SLOW_LOG_SERVERERROR, "f:%s l:%d p:%d %s", __FILE__, __LINE__, x, __FUNCTION__))
#endif

#define QLOGGER_GUARD_STATE		QLogSystem::__get_logger_guard_state()
#define QLOGGER_INITIALIZE()		QLogSystem::QLogger::instance().Init(QLogSystem::ciDefault_Log_Cache_Size)
#define QLOGGER_UPDATE()		QLogSystem::QLogger::instance().UpdateAll()
#define QLOGGER_PRINTLOG_OPEN()		QLogSystem::__open_print_log(TRUE)
#define QLOGGER_PRINTLOG_CLOSE()	QLogSystem::__open_print_log(FALSE)
#define QLOGGER_PRINTLOG_ISOPEN()	QLogSystem::__is_open_print_log()
#define QLOGGER_ALLINONE_OPEN()		QLogSystem::__open_allinone(TRUE)
#define QLOGGER_ALLINONE_CLOSE()	QLogSystem::__open_allinone(FALSE)
#define QLOGGER_ALLINONE_ISOPEN()	QLogSystem::__is_open_allinone()
#define QLOGGER_SYSTEM_MOVELOG(type)	QLogSystem::QLogger::instance().BackUpLog(type);
//#define QLOGGER_SYSTEM(year, month, day)			QLogSystem::QLogger::instance().GetFileData(1, year, month, day);

////////////////////////////////////////////////////////////////////////////////
// Thread Module
////////////////////////////////////////////////////////////////////////////////
enum ThreadStep { ts_0, ts_1, ts_2, ts_3, ts_4, ts_5, ts_6, ts_7, ts_8, ts_9, };
class KThread
{
public:
	enum RunState { THREAD_READY, THREAD_RUNNING, THREAD_EXITING, THREAD_FINISH };

public:
	KThread();
	virtual ~KThread();

public:
	void			start();
	virtual void	stop();
	void			exit(void *retval = NULL);
	virtual void	run();

public:

	TID				getTID() { return m_TID; }
	RunState		getState() { return m_Status; }
	void			setState(RunState status) { m_Status = status; }
	int32			getTickCount() { return m_TickCount; }
	int32			getSteps() { return m_Steps; }
	int32			getContainerPos() { return m_Pos; } 
	void			setContainerPos(int32 nPos) { m_Pos = nPos; }
	
	BOOL			getInit() {return m_Init;}

private:
	TID		m_TID;
	RunState	m_Status;

#if defined(__WINDOWS__)
	HANDLE		m_hThread;
#endif

protected:
	int32	m_TickCount;
	int32	m_Steps;
	int32	m_Pos;			// ���̳߳��е�����λ��
	BOOL	m_Init;         //�ж��Ƿ�
};

extern uint32	g_ThreadCount;
extern uint32	g_WaitQuitThreadCount;

#if defined(__LINUX__)
void		*ThreadEntry(void *derivedThread);
#elif defined(__WINDOWS__)
ulong WINAPI	ThreadEntry(void *derivedThread);
#endif


////////////////////////////////////////////////////////////////////////////////
// Configer Module
////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <fstream>
#include <map>

typedef std::map<std::string,int>							ConfigerValueHashMap;
typedef std::map<std::string,ConfigerValueHashMap* >		ConfigerSectionHashMap;

#define CONFIGER_BUF_SIZE		1024
#define CONFIGER_NOTE			(';')

class Configer
{
public:
	Configer();
	Configer(const char* filename);
	virtual ~Configer();

	BOOL	Open(const char* filename);
	void	Close();

	int32	Get_Property_Int(const char *section, const char *key, int32 nDefault=0 );
	BOOL	GetIntIfExist(const char *section, const char *key, int32& nResult );
	float	GetFloat(const char *section, const char *key, float fDefault=0.0f );
	BOOL	GetFloatIfExist(const char *section, const char *key, float& fResult );
	BOOL	GetText(const char *section, const char *key, char *str, int32 size, const char* strDefault="" );
	const char*	GetText(const char *section, const char *key, const char* strDefault = "");
	BOOL	GetTextIfExist(const char *section, const char *key, char *str, int32 size );
	BOOL	HasSection(const char* section);

private:
	BOOL	_GetInt(const char *section, const char *key, int32& nResult );
	BOOL	_GetFloat(const char *section, const char *key, float& fResult );
	BOOL	_GetText(const char *section, const char *key, char *str, int32 size );
	void	_TrimString( char* buf );
	BOOL	_ParseSection( char* buf, char** pSection );
	BOOL	_ParseKey( char* buf, char** sKey, char** sValue );
	void	_ResizeBuf();
	char*	_GetBufString(int32 nPos);
	int32	_AddBufString(char* str);

private:
	std::ifstream				m_ifile;
	char						m_szFileName[MAX_FILE_PATH];
	ConfigerSectionHashMap		m_SectionData;
	ConfigerValueHashMap*		m_pCurSection;
	char*						m_pBuf;
	int32						m_nBufLen;
	int32						m_nBufMaxLen;
};

////////////////////////////////////////////////////////////////////////////////
// String Tools
////////////////////////////////////////////////////////////////////////////////
// printf����ַ���
#define MAX_SPRINTF 1024

/** Utility class for manipulating Strings.  */
class  StringUtil
{
public:
	typedef std::ostringstream StrStreamType;

	/** Removes any whitespace characters, be it standard space or
	TABs and so on.
	@remarks
	The user may specify wether they want to trim only the
	beginning or the end of the String ( the default action is
	to trim both).
	*/
	static const String& trim(String& str, bool left = true, bool right = true);

	/** Returns a StringVector that contains all the substrings delimited
	by the characters in the passed <code>delims</code> argument.
	@param
	delims A list of delimiter characters to split by
	@param
	maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
	parameters is > 0, the splitting process will stop after this many splits, left to right.
	*/
	static std::vector< String > split(const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0);

	/** Upper-cases all the characters in the string.
	*/
	static void toLowerCase(String& str);

	/** Lower-cases all the characters in the string.
	*/
	static void toUpperCase(String& str);


	/** Returns whether the string begins with the pattern passed in.
	@param pattern The pattern to compare with.
	@param lowerCase If true, the end of the string will be lower cased before
	comparison, pattern should also be in lower case.
	*/
	static bool startsWith(const String& str, const String& pattern, bool lowerCase = true);

	/** Returns whether the string ends with the pattern passed in.
	@param pattern The pattern to compare with.
	@param lowerCase If true, the end of the string will be lower cased before
	comparison, pattern should also be in lower case.
	*/
	static bool endsWith(const String& str, const String& pattern, bool lowerCase = true);

	/** Method for standardising paths - use forward slashes only, end with slash.
	*/
	static String standardisePath(const String &init);

	/** Method for splitting a fully qualified filename into the base name
	and path.
	@remarks
	Path is standardised as in standardisePath
	*/
	static void splitFilename(const String& qualifiedName,
		String& outBasename, String& outPath);

	/** Method for splitting a fully qualified filename into the base name,
	extension and path.
	@remarks
	Path is standardised as in standardisePath
	*/
	static void splitFullFilename(const String& qualifiedName,
		String& outBasename, String& outExtention,
		String& outPath);

	/** Method for splitting a filename into the base name
	and extension.
	*/
	static void splitBaseFilename(const String& fullName,
		String& outBasename, String& outExtention);


	/** Simple pattern-matching routine allowing a wildcard pattern.
	@param str String to test
	@param pattern Pattern to match against; can include simple '*' wildcards
	@param caseSensitive Whether the match is case sensitive or not
	*/
	static bool match(const String& str, const String& pattern, bool caseSensitive = true);

	// ģ���׼printf���
	static String printf(const char* format, ...);

	// ģ���׼sprintf�����û��Ŀ���ַ������鳤�ȣ�����ʹ��ʱ��ȷ��Ŀ���ַ������鳤�� >= MAX_SPRINTF ����
	static int sprintf(char* inDest, const char* inFmt, ...);

	// ģ���׼sprintf�����֧�ִ���Ŀ���ַ������ȣ�������ֹ���ĳ��ȣ�
	static int snprintf(char* inDest, uint32 inSize, const char* inFmt, ...);

	static float parseFloat(const String& val);

	static double parseDouble(const String& val);
	/** Converts a String to a whole number.
	@returns
	0.0 if the value could not be parsed, otherwise the numeric version of the String.
	*/
	static int parseInt(const String& val);

	/** Converts a String to a whole number.
	@returns
	0.0 if the value could not be parsed, otherwise the numeric version of the String.
	*/
	static unsigned int parseUnsignedInt(const String& val);

	/** Converts a String to a whole number.
	@returns
	0.0 if the value could not be parsed, otherwise the numeric version of the String.
	*/
	static long parseLong(const String& val);

	static int64 parseInt64(const String& val);

	static uint64 parseUnsignedInt64(const String& val);

	/** Converts a String to a boolean.
	@remarks
	Returns true if case-insensitive match of the start of the string
	matches "true", "yes" or "1", false otherwise.
	*/
	static bool parseBool(const String& val);

	static const String BLANK;
};

/** ��ʼ�����ַ�����غ���
*/
FORCEINLINE ulong kwStringHash(const String& str)
{
	ulong hash = 0;
	for (size_t i = 0; i < str.size(); i++)
		hash = 5 * hash + str[i];
	return hash;
}

inline char* kwStrcpy(char* Dest, size_t DestCount, const char* Src)
{
#if USE_SECURE_CRT
	_tcscpy_s(Dest, DestCount, Src);
	return Dest;
#else
	return (char*)_tcscpy(Dest, Src);
#endif
}

inline char* kwStrncpy(char* Dest, const char* Src, int MaxLen)
{
	//check(MaxLen>0);
#if USE_SECURE_CRT
	// length of string must be strictly < total buffer length so use (MaxLen-1)
	_tcsncpy_s(Dest, MaxLen, Src, MaxLen - 1);
#else
	_tcsncpy(Dest, Src, MaxLen);
	Dest[MaxLen - 1] = 0;
#endif
	return Dest;
}

inline char* kwStrcat(char* Dest, ulong DestCount, const char* Src)
{
#if USE_SECURE_CRT
	_tcscat_s(Dest, DestCount, Src);
	return Dest;
#else
	return (char*)_tcscat(Dest, Src);
#endif
}

inline int kwStrlen(const char* String) { return (int)_tcslen(String); }
inline char* kwStrstr(const char* String, const char* Find) { return (char*)_tcsstr(String, Find); }
inline char* kwStrchr(const char* String, int c) { return (char*)_tcschr(String, c); }
inline char* kwStrrchr(const char* String, int c) { return (char*)_tcsrchr(String, c); }
inline int kwStrcmp(const char* String1, const char* String2) { return _tcscmp(String1, String2); }
inline int kwStricmp(const char* String1, const char* String2) { return _tcsicmp(String1, String2); }
inline int kwStrncmp(const char* String1, const char* String2, int Count) { return _tcsncmp(String1, String2, Count); }
inline int kwStrnicmp(const char* A, const char* B, int Count) { return _tcsnicmp(A, B, Count); }
inline int kwAtoi(const char* String) { return _tstoi(String); }
inline float kwAtof(const char* String) { return _tstof(String); }
inline double kwAtod(const char* String) { return _tcstod(String, NULL); }
inline int kwStrtoi(const char* Start, char** End, int Base) { return _tcstoul(Start, End, Base); }
inline bool kwIsWhitespace(char c) { return c == ' ' || c == '\t'; }

// ��ʼ��CRC�㷨�ֵ� [12/7/2017 yz]
void kwInitStrCrc();

// �����ַ���CRC��
uint32 kwStrCrc(const String& Data);

// ������ת��Ϊ�ַ�
char* kwItoa(int inNum, char* inOutStr, int radix = 10);

inline bool kwIsAlpha(char c)	//�Ƿ�Ϊ�ַ�
{
	return (c >= 'A' && c <= 'Z')
		|| (c >= 192 && c <= 255)
		|| (c >= 'a' && c <= 'z')
		|| (c == 159) || (c == 140) || (c == 156);
}

// �Ƿ�Ϊ�ַ�������
inline bool kwIsAlphaNum(char cc)
{
	return kwIsAlpha(cc) || (cc >= TEXT('0') && cc <= TEXT('9'));
}

#define UPPER_LOWER_DIFF	32
inline char kwToUpper(char c)
{
	if ((c >= TEXT('a') && c <= TEXT('z')) || (c > 223 && c < 255))
	{
		return c - UPPER_LOWER_DIFF;
	}

	return c;
}

inline char kwToLower(char c)
{
	if ((c >= 192 && c < 223) || (c >= TEXT('A') && c <= TEXT('Z')))
	{
		return c + UPPER_LOWER_DIFF;
	}
	return c;
}

#define ABS(val) ((val >= 0) ? val : 0 - val)

#endif