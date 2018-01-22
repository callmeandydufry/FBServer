#include "stdafx.h"
#include "JsonSerialization.h"
#include "RapidJson/prettywriter.h"

//--------------------------------------------------------------------------
JsonReadSerializer::JsonReadSerializer()
	: mIsValid(false)
{

}

bool JsonReadSerializer::fromString(const char* jsonString)
{
	if (mJsonDocument.Parse(jsonString).HasParseError())
	{
		mIsValid = false;
	}
	else
	{
		mIsValid = true;
	}
	return mIsValid;
}

void JsonReadSerializer::serializeInt8(const char* key, char& val)
{
	if (mParentNode->IsInt())
	{
		val = (char)mParentNode->GetInt();
	}
}

void JsonReadSerializer::serializeUint8(const char* key, uchar& val)
{
	if (mParentNode->IsUint())
	{
		val = (uchar)mParentNode->GetUint();
	}
}

void JsonReadSerializer::serializeInt16(const char* key, short& val)
{
	if (mParentNode->IsInt())
	{
		val = (short)mParentNode->GetInt();
	}
}

void JsonReadSerializer::serializeUint16(const char* key, uint16& val)
{
	if (mParentNode->IsUint())
	{
		val = (uint16)mParentNode->GetUint();
	}
}

void JsonReadSerializer::serializeInt32(const char* key, int32& val)
{
	if (mParentNode->IsInt())
	{
		val = (int32)mParentNode->GetInt();
	}
}

void JsonReadSerializer::serializeUint32(const char* key, uint32& val)
{
	if (mParentNode->IsUint())
	{
		val = (uint32)mParentNode->GetUint();
	}
}

void JsonReadSerializer::serializeInt64(const char* key, int64& val)
{
	if (mParentNode->IsInt64())
	{
		val = (int64)mParentNode->GetInt64();
	}
}

void JsonReadSerializer::serializeUint64(const char* key, uint64& val)
{
	if (mParentNode->IsUint64())
	{
		val = (uint64)mParentNode->GetUint64();
	}
}

void JsonReadSerializer::serializeFloat(const char* key, float& val)
{
	if (mParentNode->IsFloat())
	{
		val = (float)mParentNode->GetFloat();
	}
}

void JsonReadSerializer::serializeDouble(const char* key, double& val)
{
	if (mParentNode->IsDouble())
	{
		val = (double)mParentNode->GetDouble();
	}
}

void JsonReadSerializer::serializeCharArray(const char* key, char*& val, int32 valSize)
{
	if (mParentNode->IsString())
	{
		int strLen = (valSize - 1 < mParentNode->GetStringLength() ? valSize - 1 : mParentNode->GetStringLength());
		memcpy(val, mParentNode->GetString(), strLen);
		val[strLen] = 0;
	}
}

void JsonReadSerializer::serializeCharArrayConst(const char* key, const char*& val, int32 valSize)
{
	// 不能修改const char* [10/20/2016 yz]
}

void JsonReadSerializer::serializeString(const char* key, std::string& val)
{
	if (mParentNode->IsString())
	{
		val = mParentNode->GetString();
	}
}


bool JsonReadSerializer::enterJsonNode(const char* key)
{
	rapidjson::Value* parentNode = mParentNode != NULL ? mParentNode : &mJsonDocument;
	rapidjson::Value::MemberIterator childIt = parentNode->FindMember(key);
	if (childIt != parentNode->MemberEnd())
	{
		mNodeStack.push_back(parentNode);
		mParentNode = &childIt->value;
		return true;
	}
	return false;
}

void JsonReadSerializer::leaveJsonNode(const char* key)
{
	mParentNode = NULL;
	if (mNodeStack.size() > 0)
	{
		mParentNode = mNodeStack.back();
		mNodeStack.pop_back();
	}
}

bool JsonReadSerializer::enterArrayNode(const char* key, int32 i)
{
	if (mParentNode->IsArray() && mParentNode->Size() > i)
	{
		mNodeStack.push_back(mParentNode);
		mParentNode = &(*mParentNode)[i];
		return true;
	}
	return false;
}

void JsonReadSerializer::leaveArrayNode(const char* key)
{
	mParentNode = NULL;
	if (mNodeStack.size() > 0)
	{
		mParentNode = mNodeStack.back();
		mNodeStack.pop_back();
	}
}

//--------------------------------------------------------------------------
JsonWriteSerializer::JsonWriteSerializer()
{
	// 设置文档对象的类型 [10/20/2016 yz]
	mJsonDocument.SetObject();
}

bool JsonWriteSerializer::toString(std::string& json)
{
	rapidjson::StringBuffer sb;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
	writer.SetFormatOptions(rapidjson::kFormatNoFormatSpace);
	if (mJsonDocument.Accept(writer))
	{
		json = sb.GetString();
		return true;
	}
	return false;
}

void JsonWriteSerializer::serializeInt8(const char* key, char& val)
{
	mParentNode->SetInt(val);
}

void JsonWriteSerializer::serializeUint8(const char* key, uchar& val)
{
	mParentNode->SetUint(val);
}

void JsonWriteSerializer::serializeInt16(const char* key, short& val)
{
	mParentNode->SetInt(val);
}

void JsonWriteSerializer::serializeUint16(const char* key, uint16& val)
{
	mParentNode->SetUint(val);
}

void JsonWriteSerializer::serializeInt32(const char* key, int32& val)
{
	mParentNode->SetInt(val);
}

void JsonWriteSerializer::serializeUint32(const char* key, uint32& val)
{
	mParentNode->SetUint(val);
}

void JsonWriteSerializer::serializeInt64(const char* key, int64& val)
{
	mParentNode->SetInt64(val);
}

void JsonWriteSerializer::serializeUint64(const char* key, uint64& val)
{
	mParentNode->SetUint64(val);
}

void JsonWriteSerializer::serializeFloat(const char* key, float& val)
{
	mParentNode->SetFloat(val);
}

void JsonWriteSerializer::serializeDouble(const char* key, double& val)
{
	mParentNode->SetDouble(val);
}

void JsonWriteSerializer::serializeCharArray(const char* key, char*& val, int32 valSize)
{
	mParentNode->SetString(val, mJsonDocument.GetAllocator());
}

void JsonWriteSerializer::serializeCharArrayConst(const char* key, const char*& val, int32 valSize)
{
	mParentNode->SetString(val, mJsonDocument.GetAllocator());
}

void JsonWriteSerializer::serializeString(const char* key, std::string& val)
{
	mParentNode->SetString(val.c_str(), mJsonDocument.GetAllocator());
}

bool JsonWriteSerializer::enterJsonNode(const char* key)
{
	if (mParentNode)
	{
		mNodeStack.push_back(mParentNode);
	}
	mParentNode = new rapidjson::Value();
	return true;
}

void JsonWriteSerializer::leaveJsonNode(const char* key)
{
	rapidjson::Value* value = mParentNode;
	if (mNodeStack.size() > 0)
	{
		mParentNode = mNodeStack.back();
		mNodeStack.pop_back();
		if (!mParentNode->IsObject())
		{
			mParentNode->SetObject();
		}
		mParentNode->AddMember(rapidjson::GenericStringRef<char>(key), *value, mJsonDocument.GetAllocator());
	}
	else
	{
		mParentNode = NULL;
		mJsonDocument.AddMember(rapidjson::GenericStringRef<char>(key), *value, mJsonDocument.GetAllocator());
	}
	delete value;
}

bool JsonWriteSerializer::enterArrayNode(const char* key, int32 i)
{
	if (!mParentNode->IsArray())
	{
		mParentNode->SetArray();
	}
	
	mNodeStack.push_back(mParentNode);
	mParentNode = new rapidjson::Value();
	return true;
}

void JsonWriteSerializer::leaveArrayNode(const char* key)
{
	rapidjson::Value* value = mParentNode;
	if (mNodeStack.size() > 0)
	{
		mParentNode = mNodeStack.back();
		mNodeStack.pop_back();
		mParentNode->PushBack(*value, mJsonDocument.GetAllocator());
	}
	else
	{
		mParentNode = NULL;
		mJsonDocument.PushBack(*value, mJsonDocument.GetAllocator());
	}
	delete value;
}
