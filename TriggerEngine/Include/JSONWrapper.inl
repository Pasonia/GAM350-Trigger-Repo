#include <cstdio>
#include <typeinfo>
#include "rapidjson/prettywriter.h"	// for stringify JSON
#include "rapidjson/filestream.h"	// wrapper of C stream for prettywriter as output

namespace Scripter
{
	enum VAR_TYPES
	{
		TYPE_BOOL = 0,
		TYPE_INT,
		TYPE_UNSIGNED_INT,
		TYPE_LONG,
		TYPE_UNSIGNED_LONG,
		TYPE_FLOAT,
		TYPE_DOUBLE,
		TYPE_STRING,
		TYPE_TOTAL
	};

	union universal_var
	{
		bool mVar_bool;
		int mVar_int;
		unsigned mVar_uint;
		float mVar_float;
		double mVar_double;
		long mVar_long;
		unsigned long mVar_ulong;
		char mVar_string[32];
	};

	struct universal_type
	{
		unsigned char id;
		universal_var data;
		template <typename T>
		universal_type(const T& badtype) : id(TYPE_TOTAL + 1) {}
		universal_type(const bool& b) : id(TYPE_BOOL)					{ data.mVar_bool = b; }
		universal_type(const int& i) : id(TYPE_INT)					{ data.mVar_int = i; }
		universal_type(const unsigned int& i) : id(TYPE_UNSIGNED_INT)		{ data.mVar_uint = i; }
		universal_type(const float& f) : id(TYPE_FLOAT)				{ data.mVar_float = f; }
		universal_type(const double& d) : id(TYPE_DOUBLE)				{ data.mVar_double = d; }
		universal_type(const long& l) : id(TYPE_LONG)					{ data.mVar_long = l; }
		universal_type(const unsigned long& l) : id(TYPE_UNSIGNED_LONG)	{ data.mVar_ulong = l; }
		universal_type(const char * c) : id(TYPE_STRING)				{ strcpy_s(data.mVar_string, c); }

		bool Valid()
		{
			return id < TYPE_TOTAL;
		}
	};

	template<typename T>
	void Pushback_To_Array(rapidjson::Value& Val, T obj,
		const char * string, Document * doc)
	{
		universal_type in_value(obj);
		if (in_value.Valid() && Val.IsArray())
		{
			switch (in_value.id)
			{
			case TYPE_BOOL:
			{
							  Val.PushBack(in_value.data.mVar_bool, doc->GetAllocator());
			}
				break;
			case TYPE_INT:
			{
							 Val.PushBack(in_value.data.mVar_int, doc->GetAllocator());
			}
				break;
			case TYPE_UNSIGNED_INT:
			{
									  Val.PushBack(in_value.data.mVar_uint, doc->GetAllocator());
			}
				break;
			case TYPE_LONG:
			{
							  Val.PushBack(in_value.data.mVar_long, doc->GetAllocator());
			}
				break;
			case TYPE_UNSIGNED_LONG:
			{
									   Val.PushBack(in_value.data.mVar_ulong, doc->GetAllocator());
			}
				break;
			case TYPE_FLOAT:
			{
							   Val.PushBack(static_cast<double>(in_value.data.mVar_float), doc->GetAllocator());
			}
				break;
			case TYPE_DOUBLE:
			{
								Val.PushBack(in_value.data.mVar_double, doc->GetAllocator());
			}
				break;
			case TYPE_STRING:
			{
								Val.PushBack(in_value.data.mVar_string, doc->GetAllocator());
			}
				break;
			default: break;
			}
		}
	}

	template<typename T>
	void SetValue(Value& Val, T variable)
	{
		universal_type in_value(variable);
		switch (in_value.id)
		{
		case TYPE_BOOL:
		{
						  Val.SetBool(in_value.data.mVar_bool);
		}
			break;
		case TYPE_INT:
		{
						 Val.SetInt(in_value.data.mVar_int);
		}
			break;
		case TYPE_UNSIGNED_INT:
		{
								  Val.SetUint(in_value.data.mVar_uint);
		}
			break;
		case TYPE_LONG:
		{
						  Val.SetInt64(in_value.data.mVar_long);
		}
			break;
		case TYPE_UNSIGNED_LONG:
		{
								   Val.SetUint64(in_value.data.mVar_ulong);
		}
			break;
		case TYPE_FLOAT:
		{
						   Val.SetDouble(static_cast<double>(in_value.data.mVar_float));
		}
			break;
		case TYPE_DOUBLE:
		{
							Val.SetDouble(in_value.data.mVar_double);
		}
			break;
		case TYPE_STRING:
		{
							Val.SetString(in_value.data.mVar_string);
		}
			break;
		default: break;
		}
	}

	template<typename T>
	void AppendToJSONObject(rapidjson::Value& obj, const T& variable,
		const char * string, Document * doc)
	{
		universal_type in_value(variable);
		Value Val;
		SetValue(Val, variable);
		obj.AddMember(string, Val, doc->GetAllocator());
	}

	inline void InitializeObject(Value & val)
	{
		val.SetObject();
	}

	inline void InitializeDocument(rapidjson::Document * doc)
	{
		InitializeObject(*doc);
	}

	inline void InitializeArray(rapidjson::Value& val)
	{
		val.SetArray();
	}

	inline void AppendJSONObjectToJSONObject(Value& src_obj, Value& dest_obj, const char * name, Document * doc)
	{
		dest_obj.AddMember(name, src_obj, doc->GetAllocator());
	}

	inline void AppendStringToJSONObject(Value& Val, const char * tag, const char * str, Document * doc)
	{
		Value objtype;
		objtype.SetString(str, doc->GetAllocator());
		Val.AddMember(tag, objtype, doc->GetAllocator());
	}
	inline void GetInt(Document * doc, char * var_name, int * dest)
	{
		if (dest)
		{
			*dest = 0;
			if ((*doc)[var_name].IsInt())
				*dest = (*doc)[var_name].GetInt();
		}
	}
	inline void GetDouble(Document * doc, char * var_name, double * dest)
	{
		if (dest)
		{
			*dest = 0.0;
			if ((*doc)[var_name].IsDouble())
				*dest = (*doc)[var_name].GetDouble();
		}
	}
	inline void GetString(Document * doc, char * var_name, std::string * dest)
	{
		if (dest)
		{
			*dest = "";
			if ((*doc)[var_name].IsString())
				*dest = (*doc)[var_name].GetString();
		}
	}
	inline void GetBool(Document * doc, char * var_name, bool * dest)
	{
		if (dest)
		{
			*dest = false;
			if ((*doc)[var_name].IsBool())
				*dest = (*doc)[var_name].GetBool();
		}
	}/*
	inline void GetInt(const Value * doc, char * var_name, int * dest)
	{
		if (dest)
		{
			*dest = 0;
			if ((*doc)[var_name].IsInt())
				*dest = (*doc)[var_name].GetInt();
		}
	}
	inline void GetDouble(const Value * doc, char * var_name, double * dest)
	{
		if (dest)
		{
			*dest = 0.0;
			if ((*doc)[var_name].IsDouble())
				*dest = (*doc)[var_name].GetDouble();
		}
	}
	inline void GetFloat(const Value * doc, char * var_name, float * dest)
	{
		double d;
		GetDouble(doc, var_name, &d);
		*dest = static_cast<float>(d);
	}
	inline void GetString(const Value * doc, char * var_name, std::string * dest)
	{
		if (dest)
		{
			*dest = "";
			if ((*doc)[var_name].IsString())
				*dest = (*doc)[var_name].GetString();
		}
	}
	inline void GetBool(const Value * doc, char * var_name, bool * dest)
	{
		if (dest)
		{
			*dest = false;
			if ((*doc)[var_name].IsBool())
				*dest = (*doc)[var_name].GetBool();
		}
	}*/
}