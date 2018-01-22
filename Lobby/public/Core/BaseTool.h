
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

	//日期计算类型
	enum DayCountType
	{
		DATE = 0,		//按日期计算
		WEEK,			//按星期计算
		DATE_OR_WEEK,	//日期星期都有效
		ERVERY_DAY,		//日期星期都无效,即每天

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

	//判断当前时间是否在指定时间范围内 nDayCountType为计算类型: 按周计算/按日期计算; 传入的时间小时与分是以百进制计算
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
	void				LocalTime( time_t in, tm& out ); // 秒转换为tm [xingzhishan]
	void				LocalTime( tm& in, uint32 &out); // tm转换为秒 [xingzhishan]
	uint32				GetNextGameLogicDay(uint32 inTime);	// 以当前时间为基准获取游戏逻辑上的下一次夸天时间 [xingzhishan]	
	uint32				GetCurrGameLogicDay(uint32 inTime);	
	int32				GetWeekNumOfYear();					//得到当前是今年的第几周[1-52]
	int32				GetWeekNumFrom2011();				//获取从2011年开始到当前一共是第几周
	int32               GetYearAndWeekNum(); //得到一个6位数，如201001(2010年第一周)
	int32				GetTodaySecondTime();
	uint32				GetMondayGameLogicTime(uint32 uTime);// 获得本周周一凌晨6点时间 [5/9/2016 TianShuai]
	uint32				GetNextMondayGameLogicTime(uint32 uTime);// 获得下周周一凌晨6点时间 [5/9/2016 TianShuai]
	uint32				GetWeekDay24Time();// 获得本周日24点，及下周一零点[3/24/2017 TianShuai]
	uint32				GetMuchWeeksLogicTime(int32 muchWeek, int32 week, int32 hour, int32 minute); // 获得每n周的时间 [9/18/2016 scw]
	
	int32				GetWeekNumFrom2011(int32 iDays);  //传入一个从2011年1月1日开始累积的天数，计算该时间是第几周（从2011年1月1日开始计算）
	int32				GetDaysNumEplised(); //计算从2011年1月1日开始，到今天结束，总共过去了多少天
	int32				GetDaysNumEplised(uint32 uTime); //计算从2011年1月1日开始，到今天结束，总共过去了多少天

	uint32				GetDayTimeBeginTime(uint32 uTime); //获取当前日期的一天的起始时间

	void				FormatDataTime(char *pStr,uint32 nTime);
	void				FormatDataTime(char *pStr,tm &t);
	void				FormatDataTime(char *pStr);
	void				FormatDataTimeWithoutSeparate(char *pStr,uint32 nTime);

	uint32				GetCurrHourTime(); // 获得当前整点时间 [9/7/2016 scw]
	uint32				GetThisDayTime(int32 nHour, int32 nMin);
	uint32				GetThisWeekTime(int32 nWeekDay, int32 nHour, int32 nMin); //获得指定周几的指定小时分  周0-6
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
	QUICK_LOG_LOGIN		= 0,	// 用户登陆流程错误 [yangzhen]
	QUICK_LOG_DEBUG		= 1,	// 普通调试信息 [yangzhen]
	QUICK_LOG_ERROR		= 2,	// 服务器系统内部严重错误 [yangzhen]
	QUICK_LOG_LUA_ERROR	= 3,	// 服务器lua脚本错误 [yangzhen]
	QUICK_LOG_FUNCTION	= 4,	// lua脚本报错 [yangzhen]
	QUICK_LOG_WORLD		= 5,	// 主线程中统计信息 [yangzhen]
	QUICK_LOG_ITEMCHANGE	= 6,	// 物品运维数据统计 [yangzhen]
	QUICK_LOG_MONEYCHANGE	= 7,	// 物品运维数据统计 [yangzhen]
	QUICK_LOG_CHAT		= 8,	// 聊天信息记录 [yangzhen]
	QUICK_LOG_GM		= 9,	// GameMaster管理日志记录 [yangzhen]
	QUICK_LOG_IBMONEY	= 10,
	QUICK_LOG_IBCOST	= 11,
	QUICK_LOG_ENTERMAP  = 12,
	QUICK_LOG_TICK_TIMER	= 13,	// Tick计时器,距离map中player和monster的数量与tick耗时
	QUICK_LOG_DBDEBUG       = 14,    //数据库
	QUICK_LOG_TICK          = 15,    //线程tick
	QUICK_LOG_PLAYERHERO    = 16,    //英雄养成与切换相关
	QUICK_LOG_POSTMAIL    = 17,    //发送邮件
	QUICK_LOG_AWARDPOST   = 18,    //发送邮件
	QUICK_LOG_BATTLE	= 19,	// 战役日志 [yangzhen]
	QUICK_LOG_ARENA		= 20,	// 竞技场日志 [yangzhen]
	QUICK_LOG_ITEM_OPERATE = 21,	// 所有的物品操作日志 [yangzhen]
	QUICK_LOG_MAPLAWLESSOP = 22,	// 记录玩家非法操作 [yangzhen]
	QUICK_LOG_PLAYERUSESKILL = 23,	// 玩家使用技能的统计 [yangzhen]
	QUICK_LOG_SKILL = 24,	// 记录玩家技能养成日志 [yangzhen]
	QUICK_LOG_PLAYERLOGINOUT = 25,	// 玩家登陆登出运维数据统计 [wangyiran]
	QUICK_LOG_ORDER_DEBUG = 26,	// 订单系统正常日志 [wangxiaoyong]
	QUICK_LOG_ORDER_ERROR = 27,	// 订单系统错误日志 [wangxiaoyong]
	QUICK_LOG_ORDER_BILLING = 28, // 只写billing过来的订单 [wangxiaoyong]
	QUICK_LOG_EQUIP = 29, // 装备日志 [xingzhishan]
	QUICK_LOG_DB_HEROGROW = 30, // 只写英雄卡牌养成 [wangxiaoyong]
	QUICK_LOG_AWARDPOST_DB = 31, // 奖励邮件日志 [xingzhishan]
	QUICK_LOG_POST_DB = 32, // 邮件日志 [xingzhishan]
	QUICK_LOG_OBJ_INFO	= 33,	// obj信息 [yangzhen]
	QUICK_LOG_CDKEY	= 34,	// CDKey [xingzhishan]
	QUICK_LOG_DEBUGDUMP	= 35,	// 通过调试的GM命令打印特殊的调试日志 [yangzhen]
	QUICK_LOG_CONNECTOR	= 36,	// 对Connector连接对象的调试日志 [yangzhen]
	QUICK_LOG_WECHATRED	= 37,	// 微信抢红包日志 [yangzhen]
	QUICK_LOG_TOPLIST_RANK,
	QUICK_LOG_ARENA_AWARD,
	QUICK_LOG_ZUOBI,
	QUICK_LOG_ACTIVITY_STATUS,
	QUICK_LOG_PLAYER_RENAME,
	QUICK_LOG_GUILD_RENAME,
	QUICK_LOG_PLAYER_PHYSICALPOWER_OVERFLOW,
	QUICK_LOG_ARENA_RANK_UP_AWARD, //竞技场排名提升奖励 [xingzhishan]
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
	SLOW_LOG_PLAYER_ARTIFACT,//神器
	SLOW_LOG_PLAYER_FIND_TREASURE, // 寻宝日志 [4/22/2016 tianshuai]
	SLOW_LOG_PLAYER_HERO_TRANSFER, // 英雄转移日志 [4/22/2016 tianshuai]
	SLOW_LOG_PLAYER_INDENTURE, // 契约养成 [11/23/2016 TianShuai]
	SLOW_LOG_SHOP_REPEAT_BUY_LIMIT_COUNT_ITEM, // 重复购买有次数限制的订单商品超过了上限 [6/15/2017 "tianshuai"]
	SLOW_LOG_PLAYER_EFFECT_UP_DATA, // 特殊效果 [7/6/2017 "tianshuai"]
	SLOW_LOG_ABATTOIR, // 战神榜日志 [8/2/2017 Chief]
	ALL_LOG_NUMBER,
};

enum eLOG_BACKUP_TYPE
{
	eLOG_BACKUP_BY_TIME	= 0,	// 跨天备份日志
	eLOG_BACKUP_SERVER_START,			// 开服时检测备份日志
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
		// 在跨天的时候缓存日志 [7/29/2015 yangzhen]
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
		BOOL			m_isUseHoral[ALL_LOG_NUMBER]; // 使用(年月日时)每小时后缀日志文件 [11/3/2016 scw]
		char			*m_pBackupCacheLog[ALL_LOG_NUMBER];
		int32			m_BackupLogWritePos[ALL_LOG_NUMBER];
		char			*m_pSaveLog;
		int32			m_nLastBackUpTime;

		BOOL			m_isUseBackUPBuff; // 是否备份日志 [11/3/2016 scw]
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
	int32	m_Pos;			// 在线程池中的索引位置
	BOOL	m_Init;         //判断是否开
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
// printf最大字符数
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

	// 模拟标准printf输出
	static String printf(const char* format, ...);

	// 模拟标准sprintf输出，没有目标字符串数组长度，所以使用时请确保目标字符串数组长度 >= MAX_SPRINTF ！！
	static int sprintf(char* inDest, const char* inFmt, ...);

	// 模拟标准sprintf输出，支持传入目标字符串长度（包含终止符的长度）
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

/** 开始定义字符串相关函数
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

// 初始化CRC算法字典 [12/7/2017 yz]
void kwInitStrCrc();

// 计算字符串CRC码
uint32 kwStrCrc(const String& Data);

// 将数字转换为字符
char* kwItoa(int inNum, char* inOutStr, int radix = 10);

inline bool kwIsAlpha(char c)	//是否为字符
{
	return (c >= 'A' && c <= 'Z')
		|| (c >= 192 && c <= 255)
		|| (c >= 'a' && c <= 'z')
		|| (c == 159) || (c == 140) || (c == 156);
}

// 是否为字符或数字
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