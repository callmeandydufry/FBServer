

//CREATEENUM---------------------------------
#define CREATEENUM_DECLARE_METHOD_NO_PARAM(CLS, funcName) \
	ExportMethod_##funcName,

#define CREATEENUM_DECLARE_METHOD_PARAM_WITHTYPE(CLS, funcName, R, ...)
#define CREATEENUM_DECLARE_METHOD_PARAM_NOTYPE(CLS, funcName, ...)
	
//CREATEVIRTUALFUNC---------------------------------
#define CREATEVIRTUALFUNC_DECLARE_METHOD_NO_PARAM(CLS, funcName)
#define CREATEVIRTUALFUNC_DECLARE_METHOD_PARAM_WITHTYPE(CLS, funcName, R, ...) \
	virtual R funcName(__VA_ARGS__) = 0;
#define CREATEVIRTUALFUNC_DECLARE_METHOD_PARAM_NOTYPE(CLS, funcName, ...)

//CREATEEXPORTFUNC---------------------------------
#define CREATEEXPORTFUNC_DECLARE_METHOD_NO_PARAM(CLS, funcName)
#define CREATEEXPORTFUNC_DECLARE_METHOD_PARAM_WITHTYPE(CLS, funcName, R, ...) \
	virtual PacketReturnCallableInstance<CALLBACK_CLS, typename RemoveReferenceTypeUtil<R>::Type>* funcName(__VA_ARGS__)	\
	{	\
		typedef typename RemoveReferenceTypeUtil<R>::Type ReturnType;\
		DataPacket* packet = getDataPacket();	\
		packet->mPackMethodId = ExportMethod_##funcName;	\
		PacketReturnCallableInstance<CALLBACK_CLS, ReturnType>* returnCallback = createReturnCallback<CALLBACK_CLS, ReturnType>();	\
		
#define CREATEEXPORTFUNC_DECLARE_METHOD_PARAM_NOTYPE(CLS, funcName, ...)	\
		packet->mData.clear();	\
		serializeParams(packet->mData,##__VA_ARGS__);	\
		return returnCallback;	\
	}

	//REGISTERFUNC---------------------------------
#define REGISTERFUNC_DECLARE_METHOD_NO_PARAM(CLS, funcName) \
    registerExportMethod(ExportMethod_##funcName, &CLS::funcName);
#define REGISTERFUNC_DECLARE_METHOD_PARAM_WITHTYPE(CLS, funcName, R, ...)
#define REGISTERFUNC_DECLARE_METHOD_PARAM_NOTYPE(CLS, funcName, ...)

#define DECLARE_EXPORT_METHOD_PARAM0(CLS, OPERATION, name, R) \
	OPERATION##_DECLARE_METHOD_NO_PARAM(CLS, name)	\
	OPERATION##_DECLARE_METHOD_PARAM_WITHTYPE(CLS, name, R)	\
	OPERATION##_DECLARE_METHOD_PARAM_NOTYPE(CLS, name)	\

#define DECLARE_EXPORT_METHOD_PARAM1(CLS, OPERATION, name, R, T1, P1) \
	OPERATION##_DECLARE_METHOD_NO_PARAM(CLS, name)	\
	OPERATION##_DECLARE_METHOD_PARAM_WITHTYPE(CLS, name, R, T1 P1)	\
	OPERATION##_DECLARE_METHOD_PARAM_NOTYPE(CLS, name, P1)	\

#define DECLARE_EXPORT_METHOD_PARAM2(CLS, OPERATION, name, R, T1, P1, T2, P2) \
	OPERATION##_DECLARE_METHOD_NO_PARAM(CLS, name)	\
	OPERATION##_DECLARE_METHOD_PARAM_WITHTYPE(CLS, name, R, T1 P1, T2 P2)	\
	OPERATION##_DECLARE_METHOD_PARAM_NOTYPE(CLS, name, P1, P2)	\

#define DECLARE_EXPORT_METHOD_PARAM3(CLS, OPERATION, name, R, T1, P1, T2, P2, T3, P3) \
	OPERATION##_DECLARE_METHOD_NO_PARAM(CLS, name)	\
	OPERATION##_DECLARE_METHOD_PARAM_WITHTYPE(CLS, name, R, T1 P1, T2 P2, T3 P3)	\
	OPERATION##_DECLARE_METHOD_PARAM_NOTYPE(CLS, name, P1, P2, P3)	\

#define DECLARE_EXPORT_METHOD_PARAM4(CLS, OPERATION, name, R, T1, P1, T2, P2, T3, P3, T4, P4) \
	OPERATION##_DECLARE_METHOD_NO_PARAM(CLS, name)	\
	OPERATION##_DECLARE_METHOD_PARAM_WITHTYPE(CLS, name, R, T1 P1, T2 P2, T3 P3, T4 P4)	\
	OPERATION##_DECLARE_METHOD_PARAM_NOTYPE(CLS, name, P1, P2, P3, P4)	\

#define DECLARE_EXPORT_METHOD_PARAM5(CLS, OPERATION, name, R, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5) \
	OPERATION##_DECLARE_METHOD_NO_PARAM(CLS, name)	\
	OPERATION##_DECLARE_METHOD_PARAM_WITHTYPE(CLS, name, R, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5)	\
	OPERATION##_DECLARE_METHOD_PARAM_NOTYPE(CLS, name, P1, P2, P3, P4, P5)	\

#define DECLARE_EXPORT_METHOD_PARAM6(CLS, OPERATION, name, R, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6) \
	OPERATION##_DECLARE_METHOD_NO_PARAM(CLS, name)	\
	OPERATION##_DECLARE_METHOD_PARAM_WITHTYPE(CLS, name, R, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6)	\
	OPERATION##_DECLARE_METHOD_PARAM_NOTYPE(CLS, name, P1, P2, P3, P4, P5, P6)	\

#define DECLARE_EXPORT_METHOD_PARAM7(CLS, OPERATION, name, R, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6, T7, P7) \
	OPERATION##_DECLARE_METHOD_NO_PARAM(CLS, name)	\
	OPERATION##_DECLARE_METHOD_PARAM_WITHTYPE(CLS, name, R, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7)	\
	OPERATION##_DECLARE_METHOD_PARAM_NOTYPE(CLS, name, P1, P2, P3, P4, P5, P6, P7)	\

#define DECLARE_EXPORT_METHOD_PARAM8(CLS, OPERATION, name, R, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6, T7, P7, T8, P8) \
	OPERATION##_DECLARE_METHOD_NO_PARAM(CLS, name)	\
	OPERATION##_DECLARE_METHOD_PARAM_WITHTYPE(CLS, name, R, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8)	\
	OPERATION##_DECLARE_METHOD_PARAM_NOTYPE(CLS, name, P1, P2, P3, P4, P5, P6, P7, P8)	\

#define CREATE_CLASS_INTERFACE(clsName, MethodList) \
	class clsName : public ExportMethodExecutor<clsName>	\
	{	\
	public: \
		enum ExportMethodID	\
		{	\
			MethodList(clsName, CREATEENUM) \
			ExportMethod_PLACE_HOLDER,	\
			ExportMethod_MAX	\
		};	\
		MethodList(clsName, CREATEVIRTUALFUNC) \
		static void registerFunctions() \
		{	\
			MethodList(clsName, REGISTERFUNC) \
		}	\
	};	\
	\
	template<typename CALLBACKCLASS>	\
	class clsName##ExportCallable : public PacketCallable	\
	{	\
		public: \
		typedef clsName THISCLASS;	\
		typedef CALLBACKCLASS CALLBACK_CLS;	\
		enum ExportMethodID	\
		{	\
			MethodList(clsName, CREATEENUM) \
			ExportMethod_PLACE_HOLDER,	\
			ExportMethod_MAX	\
		};	\
		MethodList(clsName, CREATEEXPORTFUNC); \
	};


