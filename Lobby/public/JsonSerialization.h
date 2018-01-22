#ifndef __JSONSERIALIZATION_H__
#define __JSONSERIALIZATION_H__

#include "TypeDefine.h"
#include <string.h>

#include "RapidJson/document.h"

#define MAX_UNSERIALIZE_JSON_LEN (65535)

class JsonSerializer
{
public:
	JsonSerializer()
		: mParentNode(NULL)
	{

	}
	
	template<typename T>
	void serialize(const char* key, T& val, int32 valSize)
	{
		if (enterJsonNode(key))
		{
			serializeImpl(key, val, valSize);
			leaveJsonNode(key);
		}
	}

	template<typename T>
	void serializeArray(const char* key, T* val, int32 valSize, int32 arraySize)
	{
		if (enterJsonNode(key))
		{
			for (int i = 0; i < arraySize; ++i)
			{
				if (enterArrayNode(key, i))
				{
					serializeImpl(key, val[i], valSize);
					leaveArrayNode(key);
				}
				else
				{
					break;
				}
			}
			leaveJsonNode(key);
		}
	}

	template<typename T>
	void serializeImpl(const char* key, T& val, int32 valSize)
	{
		// 非C++支持预定义基本类型都作为自定义类型，而自定类型必须要定义自定的serializeJson接口 [10/19/2016 yz]
		val.serializeJson(this);
	}

	void serializeImpl(const char* key, char& val, int32 valSize)
	{
		serializeInt8(key, val);
	}

	void serializeImpl(const char* key, uchar& val, int32 valSize)
	{
		serializeUint8(key, val);
	}

	void serializeImpl(const char* key, short& val, int32 valSize)
	{
		serializeInt16(key, val);
	}

	void serializeImpl(const char* key, uint16& val, int32 valSize)
	{
		serializeUint16(key, val);
	}

	void serializeImpl(const char* key, int32& val, int32 valSize)
	{
		serializeInt32(key, val);
	}

	void serializeImpl(const char* key, uint32& val, int32 valSize)
	{
		serializeUint32(key, val);
	}

	void serializeImpl(const char* key, int64& val, int32 valSize)
	{
		serializeInt64(key, val);
	}

	void serializeImpl(const char* key, uint64& val, int32 valSize)
	{
		serializeUint64(key, val);
	}

	void serializeImpl(const char* key, float& val, int32 valSize)
	{
		serializeFloat(key, val);
	}

	void serializeImpl(const char* key, double& val, int32 valSize)
	{
		serializeDouble(key, val);
	}

	void serializeImpl(const char* key, char*& val, int32 valSize)
	{
		serializeCharArray(key, val, valSize);
	}

	void serializeImpl(const char* key, const char*& val, int32 valSize)
	{
		serializeCharArrayConst(key, val, valSize);
	}

	void serializeImpl(const char* key, std::string& val, int32 valSize)
	{
		serializeString(key, val);
	}

	template<int TMAXLEN, typename TSIZE_TYPE>
	void serializeImpl(const char* key, FixedString<TMAXLEN, TSIZE_TYPE>& val, int32 valSize)
	{
		char* content = val.data();
		serializeCharArray(key, content, val.maxlen());
		val.reCalcLength();
	}

	template<typename T>
	void serializeImpl(const char* key, T val[], int32 allSize)
	{
		int arraySize = allSize / sizeof(T);
		for (int i = 0; i < arraySize; ++i)
		{
			if (enterArrayNode(key, i))
			{
				serializeImpl(key, val[i], sizeof(val[i]));
				leaveArrayNode(key);
			}
			else
			{
				break;
			}
		}
	}

	void serializeImpl(const char* key, char val[], int32 valSize)
	{
		serializeCharArray(key, val, valSize);
	}

	void serializeImpl(const char* key, const char val[], int32 valSize)
	{
		serializeCharArrayConst(key, val, valSize);
	}

protected:
	virtual void serializeInt8(const char* key, char& val) = 0;
	virtual void serializeUint8(const char* key, uchar& val) = 0;
	virtual void serializeInt16(const char* key, short& val) = 0;
	virtual void serializeUint16(const char* key, uint16& val) = 0;
	virtual void serializeInt32(const char* key, int32& val) = 0;
	virtual void serializeUint32(const char* key, uint32& val) = 0;
	virtual void serializeInt64(const char* key, int64& val) = 0;
	virtual void serializeUint64(const char* key, uint64& val) = 0;
	virtual void serializeFloat(const char* key, float& val) = 0;
	virtual void serializeDouble(const char* key, double& val) = 0;
	virtual void serializeCharArray(const char* key, char*& val, int32 valSize) = 0;
	virtual void serializeCharArrayConst(const char* key, const char*& val, int32 valSize) = 0;
	virtual void serializeString(const char* key, std::string& val) = 0;

	virtual bool enterJsonNode(const char* key) = 0;
	virtual void leaveJsonNode(const char* key) = 0;

	virtual bool enterArrayNode(const char* key, int32 i) = 0;
	virtual void leaveArrayNode(const char* key) = 0;

protected:
	rapidjson::Document mJsonDocument;
	rapidjson::Value* mParentNode;
	std::vector<rapidjson::Value*> mNodeStack;
};

class JsonReadSerializer : public JsonSerializer
{
public:
	JsonReadSerializer();
	bool fromString(const char* jsonString);

protected:
	virtual void serializeInt8(const char* key, char& val);
	virtual void serializeUint8(const char* key, uchar& val);
	virtual void serializeInt16(const char* key, short& val);
	virtual void serializeUint16(const char* key, uint16& val);
	virtual void serializeInt32(const char* key, int32& val);
	virtual void serializeUint32(const char* key, uint32& val);
	virtual void serializeInt64(const char* key, int64& val);
	virtual void serializeUint64(const char* key, uint64& val);
	virtual void serializeFloat(const char* key, float& val);
	virtual void serializeDouble(const char* key, double& val);
	virtual void serializeCharArray(const char* key, char*& val, int32 valSize);
	virtual void serializeCharArrayConst(const char* key, const char*& val, int32 valSize);
	virtual void serializeString(const char* key, std::string& val);

	virtual bool enterJsonNode(const char* key);
	virtual void leaveJsonNode(const char* key);

	virtual bool enterArrayNode(const char* key, int32 i);
	virtual void leaveArrayNode(const char* key);

protected:
	bool mIsValid;
};

class JsonWriteSerializer : public JsonSerializer
{
public:
	JsonWriteSerializer();
	bool toString(std::string& json);

protected:
	virtual void serializeInt8(const char* key, char& val);
	virtual void serializeUint8(const char* key, uchar& val);
	virtual void serializeInt16(const char* key, short& val);
	virtual void serializeUint16(const char* key, uint16& val);
	virtual void serializeInt32(const char* key, int32& val);
	virtual void serializeUint32(const char* key, uint32& val);
	virtual void serializeInt64(const char* key, int64& val);
	virtual void serializeUint64(const char* key, uint64& val);
	virtual void serializeFloat(const char* key, float& val);
	virtual void serializeDouble(const char* key, double& val);
	virtual void serializeCharArray(const char* key, char*& val, int32 valSize);
	virtual void serializeCharArrayConst(const char* key, const char*& val, int32 valSize);
	virtual void serializeString(const char* key, std::string& val);

	virtual bool enterJsonNode(const char* key);
	virtual void leaveJsonNode(const char* key);

	virtual bool enterArrayNode(const char* key, int32 i);
	virtual void leaveArrayNode(const char* key);
};

template<typename T>
bool convertJsonToStruct(const char* json, T& structData)
{
	JsonReadSerializer jsonReader;
	if (jsonReader.fromString(json))
	{
		structData.serializeJson(&jsonReader);
		return true;
	}
	return false;
}

template<typename T>
bool convertStructToJson(std::string& json, T& structData)
{
	JsonWriteSerializer jsonWritter;
	structData.serializeJson(&jsonWritter);
	if (jsonWritter.toString(json))
	{
		return true;
	}
	return false;
}

#define JSON_ATTRIBUTE(NAME, ATTR) \
	serializer->serialize(NAME, ATTR, (int32)sizeof(ATTR));

#define JSON_ATTRIBUTE_ARRAY(NAME, ATTR) \
	serializer->serializeArray(NAME, ATTR, (int32)sizeof(ATTR[0]), (int32)(sizeof(ATTR) / sizeof(ATTR[0])));

#define JSON_SERIALIZE_METHODS()	\
void serialize(std::string& json)	\
{	\
	convertStructToJson(json, *this);	\
}	\
void unserialize(const char* json)	\
{	\
	convertJsonToStruct(json, *this);	\
}

#endif