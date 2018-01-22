#ifndef __STRUCT_PLATFORM_H__
#define __STRUCT_PLATFORM_H__


#include "TypeDefine.h"
// 
// const int32 MAX_DCN_URL_SIZE			= 128;		//���� url��󳤶�
// const int32 MAX_DCN_APP_KEY_SIZE		= 32;		//���� appkey��󳤶�
// const int32 MAX_DCN_PAYMENT_KEY_SIZE	= 32;		//���� appkey��󳤶�
// const int32 MAX_TOKEN_SIZE				= 64;
// const int32 MAX_SIG_SIZE				= 64;
// const int32 MAX_DCN_USERNAME_SIZE		= 32;
// const int32 MAX_DCN_NICKNAME_SIZE		= 32;
// const int32 MAX_DCN_GENDER_SIZE			= 4;
// const int32 MAX_DCN_AVATAR_URL_SIZE		= 128;
// 
// const char * const STR_MEMBERID			= "memberId";
// const char * const STR_USERNAME			= "username";
// const char * const STR_NICKNAME			= "nickname";
// const char * const STR_GENDER			= "gender";
// const char * const STR_DCN_LEVEL		= "level";
// const char * const STR_AVATAR_URL		= "avatar_url";
// const char * const STR_CREATED_DATE		= "created_date";
// const char * const STR_TOKEN			= "token";
// const char * const STR_ERROR_CODE		= "error_code";
// const char * const STR_ERROR_MSG		= "error_msg";
// 
// const char * const STR_DCN_ACCOUNT_HEAD = "DCN_";
// const char * const STR_DCN_ACCOUNT_END = "_DCN";
// 
// 
// //---------------------------------------------------------------------------------
// //��֤���
// enum PLATFORM_AUTH_RESULT
// {
// 	PLATFORM_AUTH_RESULT_OK = 0,
// 	PLATFORM_AUTH_RESULT_ERROR,			//ͨ�ô���
// 	PLATFORM_AUTH_RESULT_ERROR_CURL,	//CURL����
// 	PLATFORM_AUTH_RESULT_DCN_GET_ERROR,	//��֤δͨ�� 
// 	PLATFORM_AUTH_RESULT_ERROR_JSON,	//��֤ͨ��,JSON�������� 
// 
// 	PLATFORM_AUTH_RESULT_MAX,
// };
// 
// //---------------------------------------------------------------------------------
// //
// enum PLAYTFORM_AUTH_STATUS
// {
// 	PLAYTFORM_AUTH_STATUS_NULL = 0,
// 
// 	PLAYTFORM_AUTH_STATUS_WAIT = 10,
// 	PLAYTFORM_AUTH_STATUS_RECV_ERROR,
// 	//PLAYTFORM_AUTH_STATUS_RECV_OK,
// 	PLAYTFORM_AUTH_STATUS_AUTH_OK,
// 	//PLAYTFORM_AUTH_STATUS_AUTH_ERROR,
// 
// 	PLAYTFORM_AUTH_STATUS_LOGOUT = 100,			//�û�ȡ����¼
// 	PLAYTFORM_AUTH_STATUS_SYS_ERROR = 101,		//ϵͳ����
// 	PLAYTFORM_AUTH_STATUS_APPID_ERROR = 201,	//app_id����
// 	PLAYTFORM_AUTH_STATUS_SIG_NULL = 210,		//sigΪ��
// 	PLAYTFORM_AUTH_STATUS_APPKEY_ERROR = 211,	//app_key����
// 	PLAYTFORM_AUTH_STATUS_TOKEN_NULL = 220,		//tokenΪ��
// 	PLAYTFORM_AUTH_STATUS_TOKEN_ERROR = 221,	//token����
// 	PLAYTFORM_AUTH_STATUS_TOKEN_TIMEOUT = 222,	//token��ʱ����ʾ�û���¼�Ự�ѳ�ʱ���������û����µ�¼�������½ӿڷ������ơ���ע���������Ƶ���Чʱ����24Сʱ��
// 	PLAYTFORM_AUTH_STATUS_NO_PERMIT = 300,		//û�нӿڷ���Ȩ�ޣ�ע������ʱ����ϵ���ּ�����Աȷ��Ȩ���Ƿ�ͨ��
// 
// 	PLAYTFORM_AUTH_STATUS_MAX,
// };
// 
// //---------------------------------------------------------------------------------
// //С��ƽ̨���
// const int32 MAX_MI_URL_SIZE			= 128;		//С�� url��󳤶�
// const int32 MAX_MI_APP_KEY_SIZE		= 64;		//С�� appkey��󳤶�
// const int32 MAX_MI_SESSION_SIZE		= 32;
// const int32 MAX_MI_UID_SIZE			= 32;
// 
// const char * const STR_MI_ACCOUNT_HEAD = "MI_";
// const char * const STR_MI_ACCOUNT_END = "_MI";
// 
// 
// //��֤session���
// enum PLAYTFORM_MI_VERIFY_SESSION_RESULT
// {
// 	PLAYTFORM_MI_VERIFY_SESSION_RESULT_SUCCESS = 200,
// 
// 	PLAYTFORM_MI_VERIFY_SESSION_RESULT_FAILED_APPID = 1515,
// 	PLAYTFORM_MI_VERIFY_SESSION_RESULT_FAILED_UID = 1516,
// 	PLAYTFORM_MI_VERIFY_SESSION_RESULT_FAILED_SESSION = 1520,
// 	PLAYTFORM_MI_VERIFY_SESSION_RESULT_FAILED_SIGNATURE = 1525,
// };
// 
// struct PlatAuthReqData
// {
// 	int  m_nServerID;		// ������id���ɿͻ��˴������� �п��ܺͷ����������serverId ���ã���Ϊ�Ϸ�ԭ�� [xingzhishan]
// 	int	 m_MemberIDSize;
// 	char m_MemberID[MAX_ACCOUNT + 1];	
// 	int	 m_PlatAccountIDSize;
// 	char m_PlatAccountID[ACCOUNT_ID_LEN + 1];//UIN
// 	int  m_TokenSize;
// 	char m_Token[MAX_TOKEN_LEN + 1];			//token
// 	int  m_MacAddressSize;
// 	char m_MacAddress[MAX_MAC_ADDRESS + 1];		//mac
// 	int	 m_IpSize;
// 	char m_Ip[IP_SIZE + 1];						//ip
// 	int	 m_DeviceTypeSize;
// 	char m_DeviceType[MAX_UNIQUECODE_LEN + 1];	//�豸����
// 	int	 m_Version;								//�汾
// 	int	 m_DeviceOSType;						//�豸���� 1:IOS, 2:Android 3:WP
// 	int  m_UserType;							//�û�����(Ĭ��0)
// 	int  m_AppId;								//Ӧ��Id appId
// 	int	 m_ChannelId;							//����Id
// 	int  m_ChildChannel;						//С����Id
// 	int	 m_GameId;								//��ϷId
// 	int	 m_GameServerId;						//��Ϸ������Id
// 	int	 m_ClientId;							//Ӧ��ID��locojoy AppId��
// 	int  m_LogonType;							//��¼����
// 	int64	 m_UserId;							//�˺�id
// 	int	 m_nDeviceIDFALen;
// 	char m_szDeviceIDFA[MAX_DEVICE_IDFA_LENGTH]; // IOS�豸ר�õ����к� [yangzhen]
// 	int  m_nDeviceIDFVLen;
// 	char m_szDeviceIDFV[MAX_DEVICE_IDFV_LENGTH]; // IOS�豸ר�õ����к� [yangzhen]
// 	int	 m_DeviceIDSize;
// 	char m_DeviceID[MAX_DEVICE_IDFA_LENGTH + 1];	//�豸id��ios����idfa��android����mac������ʹ��IDFA�ĳ�������Ϊ���
// 
// 	char mNetwork[GAME_LOGIC_LOG_NETWORK_LEN + 1]; //�û���½ʹ�õ����� WIFI/3G/2G
// 	char mIMEI[MACHINE_UNIQUE_CODE + 1];  // �豸��Ψһ��ʾ��
// 	char mTelecomOper[USER_NET_BUSINESS + 1];//�û�������Ӫ��
// 	char mDeviceHardware[PHONE_MODEL + 1]; //�ֻ��ͺţ��硱iPhone 6 (A1549/A1586)������iPad Air (A1474)���ȵ�
// 	char mDeviceSoftware[PHONE_OS_MODEL + 1];//�ֻ�����ϵͳ����IOS 7.0 ,ANDROID 3.0�ȵ�
// 	int32 mScreenWidth;//��ʾ�����
// 	int32 mScreenHight;//��ʾ���߶�
// 	float mPixelDensity; // �����ܶ�[xingzhishan]
// 	char mCpuHardware[PHONE_MACHINE_INFO + 1];//�ַ����洢��ʽΪ��|�ָ��cpu����|Ƶ��|����
// 	int32 mDeviceMemory;//�ڴ���������λG��
// 	char mGLRender[PHONE_GL_INFO + 1];//opengl render��Ϣ
// 	char mGLVersion[PHONE_GL_INFO + 1];//opengl�汾��Ϣ
// 	char mAppBundleName[MAX_ACCOUNT + 1];//�ͻ�����Ϸ�İ���
// 	char mAgeStr[CHILD_PROTECTION_AGE_STRING_LENGTH + 1];
// 	int	 m_FacebookIDSize;
// 	char m_FacebookID[ACCOUNT_ID_LEN + 1];
// 	void Clear()
// 	{
// 		memset(this, 0, sizeof(PlatAuthReqData));
// 		m_nServerID = INVALID_ID;
// 	}
// 
// 	void		SetNetwork(const char* param)
// 	{
// 		KCheck(param);
// 		strncpy(mNetwork, param, GAME_LOGIC_LOG_NETWORK_LEN);
// 		mNetwork[GAME_LOGIC_LOG_NETWORK_LEN] = '\0';
// 		CheckLawLessChar(mNetwork, GAME_LOGIC_LOG_NETWORK_LEN);
// 	}
// 
// 	const char* GetNetwork() const
// 	{
// 		return mNetwork;
// 	}
// 
// 	void		SetPlatAccountID(const char* param)
// 	{
// 		m_PlatAccountIDSize = 0;
// 		memset(m_PlatAccountID, 0, sizeof(m_PlatAccountID));
// 
// 		if (param)
// 		{
// 			m_PlatAccountIDSize = std::min((int)strlen(param), MAX_ACCOUNT);
// 			memcpy(m_PlatAccountID, param, m_PlatAccountIDSize);
// 		}
// 	}
// 
// 	const char* GetPlatAccountID() const
// 	{
// 		return m_PlatAccountID;
// 	}
// 
// 	void		SetIMEI(const char * szIMEI)
// 	{
// 		KCheck(szIMEI);
// 		strncpy(mIMEI, szIMEI, MACHINE_UNIQUE_CODE);
// 		mIMEI[MACHINE_UNIQUE_CODE] = '\0';
// 		CheckLawLessChar(mIMEI, MACHINE_UNIQUE_CODE);
// 	}
// 
// 	const char* GetIMEI() const
// 	{
// 		return mIMEI;
// 	}
// 
// 	void		SetTelecomOper(const char* param)
// 	{
// 		KCheck(param);
// 		strncpy(mTelecomOper, param, USER_NET_BUSINESS);
// 		mTelecomOper[USER_NET_BUSINESS] = '\0';	
// 		CheckLawLessChar(mTelecomOper, USER_NET_BUSINESS);
// 	}
// 
// 	const char* GetTelecomOper() const
// 	{
// 		return mTelecomOper;
// 	}
// 
// 
// 	void		SetDeviceHardware(const char* param)
// 	{
// 		KCheck(param);
// 		strncpy(mDeviceHardware, param, PHONE_MODEL);
// 		mDeviceHardware[PHONE_MODEL] = '\0';
// 		CheckLawLessChar(mDeviceHardware, PHONE_MODEL);
// 	}
// 
// 	const char* GetDeviceHardware() const
// 	{
// 		return mDeviceHardware;
// 	}
// 
// 	void		SetDeviceSoftware(const char* param)
// 	{
// 		KCheck(param);
// 		strncpy(mDeviceSoftware, param, PHONE_OS_MODEL);
// 		mDeviceSoftware[PHONE_OS_MODEL] = '\0';	
// 		CheckLawLessChar(mDeviceSoftware, PHONE_OS_MODEL);
// 	}
// 
// 	const char* GetDeviceSoftware() const
// 	{
// 		return mDeviceSoftware;
// 	}
// 
// 	void		SetScreenWidth(int32 value){mScreenWidth = value;}
// 	int32		GetScreenWidth() const
// 	{
// 		return mScreenWidth;
// 	} 
// 	void		SetScreenHight(int32 value){mScreenHight = value;}
// 	int32		GetScreenHight() const
// 	{
// 		return mScreenHight;
// 	}
// 	void		SetPixelDensity(float value){mPixelDensity = value;}
// 	float		GetPixelDensity() const
// 	{
// 		return mPixelDensity;
// 	}
// 	void		SetCpuHardware(const char* param)
// 	{
// 		KCheck(param);
// 		strncpy(mCpuHardware, param, PHONE_MACHINE_INFO);
// 		mCpuHardware[PHONE_MACHINE_INFO] = '\0';
// 		CheckLawLessChar(mCpuHardware, PHONE_MACHINE_INFO);
// 	}
// 
// 	const char* GetCpuHardware() const
// 	{
// 		return mCpuHardware;
// 	}
// 
// 	void		SetAgeStr(const char* param)
// 	{
// 		KCheck(param);
// 		strncpy(mAgeStr, param, CHILD_PROTECTION_AGE_STRING_LENGTH);
// 		mAgeStr[CHILD_PROTECTION_AGE_STRING_LENGTH] = '\0';
// 		CheckLawLessChar(mAgeStr, CHILD_PROTECTION_AGE_STRING_LENGTH);
// 	}
// 
// 	const char*		GetAgeStr() const
// 	{
// 		return mAgeStr;
// 	}
// 
// 	void		SetDeviceMemory(int32 value) {mDeviceMemory = value;}
// 	int32		GetDeviceMemory() const
// 	{
// 		return mDeviceMemory;
// 	}
// 	void		SetGLRender(const char* param)
// 	{
// 		KCheck(param);
// 		strncpy(mGLRender, param, PHONE_GL_INFO);
// 		mGLRender[PHONE_GL_INFO] = '\0';	
// 		CheckLawLessChar(mGLRender, PHONE_GL_INFO);
// 	}
// 
// 	const char* GetGLRender() const
// 	{
// 		return mGLRender;
// 	}
// 
// 	void		SetGLVersion(const char* param)
// 	{
// 		KCheck(param);
// 		strncpy(mGLVersion, param, PHONE_GL_INFO);
// 		mGLVersion[PHONE_GL_INFO] = '\0';
// 		CheckLawLessChar(mGLVersion, PHONE_GL_INFO);
// 	}
// 
// 	const char* GetGLVersion() const
// 	{
// 		return mGLVersion;
// 	}
// 
// 	void SetAppBundleName(const char* bundleName)
// 	{
// 		KCheck(bundleName);
// 		strncpy(mAppBundleName, bundleName, MAX_ACCOUNT);
// 		mAppBundleName[MAX_ACCOUNT] = '\0';
// 		CheckLawLessChar(mAppBundleName, MAX_ACCOUNT);
// 	}
// 
// 	const char* GetAppBundleName() const
// 	{
// 		return mAppBundleName;
// 	}
// 
// 	void SetFacebookID(const char* param)
// 	{
// 		m_FacebookIDSize = 0;
// 		memset(m_FacebookID, 0, sizeof(m_FacebookID));
// 
// 		if (param)
// 		{
// 			m_FacebookIDSize = std::min((int)strlen(param), ACCOUNT_ID_LEN);
// 			memcpy(m_FacebookID, param, m_FacebookIDSize);
// 		}
// 	}
// 
// 	const char* GetFacebookID() const
// 	{
// 		return m_FacebookID;
// 	}
// 
// 	void	SetMemberID(const char* key);
// 	void	SetToken(const char* key);
// 	void	SetMacAddress(const char* key);
// 	void	SetIp(const char* key);
// 	void	SetDeviceType(const char* key);
// 	void	SetDeviceIDFA(const char* key);
// 	void	SetDeviceIDFV(const char* key);
// 	void	SetDeviceID(const char* key);
// 	BOOL	Send(SendStream &oStream) const;
// 	BOOL	Reci(RecieveStream &iStream) const;
// 	uint32	GetSize() const;
// };
// 
// struct PlatAuthReq
// {
// 	char			m_plat;
// 	ID_t			m_nPlayerID;
// 	char			m_szPlatPrefix[MAX_PLATFORM_NAME_PREFIX];
// 	// �ⲿ��������ƽ̨��������ݣ����Էŵ��������� [4/23/2015 yangzhen]
// 	PlatAuthReqData	m_authReqData;
// 
// 	BOOL	Send(SendStream &oStream) const;
// 	BOOL	Recieve(RecieveStream &iStream) const;
// 	uint32	GetSize() const;
// 	void	SetIdentity(const char* id);
// 
// 	PlatAuthReq()
// 	{
// 		Clear();
// 	}
// 
// 	void Clear()
// 	{
// 		memset(this, 0, sizeof(*this));
// 	}
// };
// 
// struct PlatAuthRet
// {
// 	char			m_plat;
// 	ID_t			m_nPlayerID;
// 	char			m_Result;
// 	int				m_MemberIDSize;
// 	char			m_MemberID[MAX_ACCOUNT + 1];//�������˺�ID UIN
// 
// 	//union
// 	//{
// 	//	PlatAuthRetLoco	m_authRetLoco;
// 	//};
// 
// 	BOOL	Send(SendStream &oStream) const;
// 	BOOL	Recieve(RecieveStream &iStream);
// 	uint32	GetSize() const;
// 
// 	PlatAuthRet()
// 	{
// 		memset(this, 0, sizeof(*this));
// 	}
// 
// 	void Clear()
// 	{
// 		memset(this, 0, sizeof(*this));
// 	}
// };
// 
// // ���ӶϿ��󽫱�Ҫ�����ݻ����������Ա��ڶ��������Ľ�����֤ [yangzhen]
// struct PlatCacheReconnectData
// {
// 	int32 m_nChannelId;
// 	int32 m_nDeviceOSType;
// 	int32  m_TokenOrSessionSize;
// 	char m_szTokenOrSession[MAX_TOKEN_LEN + 1];	//token or session
// 
// 
// 
// 	PlatCacheReconnectData()
// 	{
// 		Clear();
// 	}
// 
// 	void Clear()
// 	{
// 		m_nDeviceOSType = INVALID_KHAN_ID;
// 		m_nChannelId = INVALID_KHAN_ID;
// 		m_TokenOrSessionSize = 0;
// 		memset(m_szTokenOrSession, 0, sizeof(m_szTokenOrSession));
// 	}
// 
// 	void SetTokenOrSession(const char* szTokenOrSession)
// 	{
// 		if(NULL != szTokenOrSession)
// 		{
// 			memset(m_szTokenOrSession, 0, sizeof(m_szTokenOrSession));
// 			m_TokenOrSessionSize = strnlen(szTokenOrSession, MAX_TOKEN_LEN);
// 			strncpy(m_szTokenOrSession, szTokenOrSession, MAX_TOKEN_LEN);
// 		}
// 	}
// 
// 	void SetChannelId(int32 nChannelId)
// 	{
// 		m_nChannelId = nChannelId;
// 	}
// 
// 	void SetOSId(int32 mOsId)
// 	{
// 		m_nDeviceOSType = mOsId;
// 	}
// };
// 
// struct PlatOrderRetData
// {
// 	DECLARE_NET_MESSAGE_STRING_VAR(int32, MAX_ACCOUNT, ServerID, FALSE);
// 	DECLARE_NET_MESSAGE_STRING_VAR(int32, MAX_ACCOUNT, UID, FALSE);
// 	DECLARE_NET_MESSAGE_ATOM_VAR(int32, ProductIndex);
// 	DECLARE_NET_MESSAGE_ATOM_VAR(int32, ChargeDiamond);
// 	DECLARE_NET_MESSAGE_ATOM_VAR(int32, DonateDiamond);
// 	DECLARE_NET_MESSAGE_ATOM_VAR(int32, ProductPrice);
// 	DECLARE_NET_MESSAGE_STRING_VAR(int32, MAX_ORDER_ID_SIZE + 1, BillNo, FALSE);
// public:
// 	void	Clear();
// 	BOOL	Recieve(RecieveStream &iStream);
// };
// 
// struct PlatOrderRet
// {
// 	char			m_plat;
// 	SNID_t			m_nPlayerSNID;
// 	char			m_Result;
// 	PlatOrderRetData	m_OrderRetData;
// 
// 	PlatOrderRet()
// 	{
// 		Clear();
// 	}
// 	void Clear()
// 	{
// 		memset(this, 0, sizeof(*this));
// 	}
// 	BOOL	Recieve(RecieveStream &iStream);
// };

#endif //__STRUCT_PLATFORM_H__
