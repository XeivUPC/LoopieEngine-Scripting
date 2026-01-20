#pragma once
#include <map>
#include <unordered_map>
#include <string> 

struct _MonoClass;
struct _MonoObject;
struct _MonoMethod;
struct _MonoClassField;
struct _MonoType;

namespace Loopie {


	enum class ScriptFieldType
	{
		None,
		Float,
		Double,
		Bool,
		Char,
		Byte,
		Short,
		Int,
		Long,
		UByte,
		UShort,
		UInt,
		ULong,
	};

	static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
	{
		{ "System.Single", ScriptFieldType::Float },
		{ "System.Double", ScriptFieldType::Double },
		{ "System.Boolean", ScriptFieldType::Bool },
		{ "System.Char", ScriptFieldType::Char },
		{ "System.Int16", ScriptFieldType::Short },
		{ "System.Int32", ScriptFieldType::Int },
		{ "System.Int64", ScriptFieldType::Long },
		{ "System.Byte", ScriptFieldType::Byte },
		{ "System.UInt16", ScriptFieldType::UShort },
		{ "System.UInt32", ScriptFieldType::UInt },
		{ "System.UInt64", ScriptFieldType::ULong },
	};

	

	struct ScriptField
	{
		ScriptFieldType Type;
		std::string Name;

		_MonoClassField* ClassField;
	};

	class ScriptingClass {
	public:
		ScriptingClass() = default;
		ScriptingClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

		_MonoObject* Instantiate();
		_MonoMethod* GetMethod(const std::string& name, int parameterCount);
		_MonoObject* InvokeMethod(_MonoObject* instance, _MonoMethod* method, void** params = nullptr);
		const std::map<std::string, ScriptField>& GetFields() const { return m_fields; }
		std::map<std::string, ScriptField>& GetFields() { return m_fields; }
	private:
		std::string m_classNamespace;
		std::string m_className;
		_MonoClass* m_monoClass = nullptr;

		std::map<std::string, ScriptField> m_fields;
	};
	
}