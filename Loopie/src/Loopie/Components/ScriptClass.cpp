#include "ScriptClass.h"

#include "Loopie/Scripting/ScriptingManager.h"
#include "Loopie/Core/Log.h"

#include <mono/metadata/object.h>
#include <mono/metadata/class.h>
#include <mono/metadata/assembly.h>

namespace Loopie
{

	ScriptClass::ScriptClass(std::shared_ptr<ScriptingClass> scriptingClass) : m_scriptingClass(scriptingClass)
	{
		m_instance = m_scriptingClass->Instantiate();

		m_OnCreate = m_scriptingClass->GetMethod("OnCreate", 0);
		m_OnUpdate = m_scriptingClass->GetMethod("OnUpdate", 1);
	}

	ScriptClass::~ScriptClass()
	{
		DestroyInstance();
	}

	void ScriptClass::SetUp()
	{
		for (const auto& [name, fieldData] : m_scriptFields)
			SetFieldValueInternal(name, fieldData.GetBuffer());

		InvokeOnCreate();
	}

	void ScriptClass::InvokeOnCreate()
	{
		if (m_OnCreate)
			m_scriptingClass->InvokeMethod(m_instance, m_OnCreate);
	}

	void ScriptClass::InvokeOnUpdate()
	{
		if (m_OnUpdate)
			m_scriptingClass->InvokeMethod(m_instance, m_OnUpdate);
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
		m_className = fullName;
	}

	void ScriptClass::DestroyInstance()
	{
		m_instance = nullptr;
		m_OnCreate = nullptr;
		m_OnUpdate = nullptr;
	}

	bool ScriptClass::GetFieldValueInternal(const std::string& fieldName, void* buffer)
	{
		const auto& fields = m_scriptingClass->GetFields();
		auto it = fields.find(fieldName);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_get_value(m_instance, field.ClassField, buffer);
		return true;
	}

	bool ScriptClass::SetFieldValueInternal(const std::string& fieldName, const void* value)
	{
		const auto& fields = m_scriptingClass->GetFields();
		auto it = fields.find(fieldName);
		if (it == fields.end())
			return false;

		const ScriptField& field = it->second;
		mono_field_set_value(m_instance, field.ClassField, (void*)value);
		return true;
	}

}