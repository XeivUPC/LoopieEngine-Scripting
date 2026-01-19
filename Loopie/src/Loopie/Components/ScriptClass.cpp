#include "ScriptClass.h"

#include "Loopie/Scripting/ScriptingManager.h"
#include "Loopie/Core/Log.h"

#include <mono/metadata/object.h>
#include <mono/metadata/class.h>
#include <mono/metadata/assembly.h>

namespace Loopie
{
	ScriptClass::ScriptClass()
	{

	}

	ScriptClass::~ScriptClass()
	{
		DestroyInstance();
	}

	void ScriptClass::Init()
	{

	}

	JsonNode ScriptClass::Serialize(JsonNode& parent) const
	{
		JsonNode scriptObj = scriptObj.CreateObjectField("script");
		
		return scriptObj;
	}

	void ScriptClass::Deserialize(const JsonNode& data)
	{
	}
	void ScriptClass::SetClass(const std::string& fullName)
	{
		m_ClassName = fullName;
	}
	void ScriptClass::CreateInstance()
	{
	}
	void ScriptClass::DestroyInstance()
	{
		m_Instance = nullptr;
		m_Class = nullptr;
		m_OnCreate = nullptr;
		m_OnUpdate = nullptr;
		m_Initialized = false;
	}
	void ScriptClass::Invoke(const char* methodName)
	{

	}
}