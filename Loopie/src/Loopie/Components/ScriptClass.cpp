#include "ScriptClass.h"

#include "Loopie/Scripting/ScriptingManager.h"
#include "Loopie/Core/Log.h"

#include <mono/metadata/object.h>
#include <mono/metadata/class.h>
#include <mono/metadata/assembly.h>

namespace Loopie
{

	ScriptClass::ScriptClass(const std::string& className) : m_className(className)
	{
		m_scriptingClass = ScriptingManager::GetScriptingClass(m_className);
	}

	ScriptClass::~ScriptClass()
	{
		DestroyInstance();
	}

	void ScriptClass::SetUp()
	{
		m_instance = m_scriptingClass->Instantiate();

		m_OnCreate = m_scriptingClass->GetMethod("OnCreate", 0);
		m_OnUpdate = m_scriptingClass->GetMethod("OnUpdate", 1);

		for (const auto& [name, fieldData] : m_scriptFields)
			SetFieldValueInternal(name, fieldData.GetBuffer());
	}

	void ScriptClass::DestroyInstance()
	{
		m_instance = nullptr;
		m_OnCreate = nullptr;
		m_OnUpdate = nullptr;
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
		JsonNode scriptObj = parent.CreateObjectField("script");

		scriptObj.CreateField("class_id", GetClassName());

		const auto& fields = m_scriptingClass->GetFields();

		JsonNode node = scriptObj.CreateObjectField("fields");

		for (const auto& [name, field] : fields)
		{
			switch (field.Type)
			{
			case ScriptFieldType::Int:
				node.CreateField(name, GetFieldValue<int>(name));
				break;
			case ScriptFieldType::Float:
				node.CreateField(name, GetFieldValue<float>(name));
				break;
			case ScriptFieldType::Bool:
				node.CreateField(name, GetFieldValue<bool>(name));
				break;
			// more types ...
			}
		}

		return scriptObj;
	}

	void ScriptClass::Deserialize(const JsonNode& data)
	{

		std::string classID = data.GetValue<std::string>("class_id", "").Result;		
		m_className = classID;
		m_scriptingClass = ScriptingManager::GetScriptingClass(m_className);


		JsonNode node = data.Child("fields");
		const auto& fields = m_scriptingClass->GetFields();
		for (const auto& [name, scriptField] : fields)
		{
			if (!node.HasKey(name))
				continue;

			ScriptFieldData& fieldData = m_scriptFields[name];

			switch (scriptField.Type)
			{
			case ScriptFieldType::Int:
				fieldData.SetValue<int>(node.GetValue<int>(name,0).Result);
				break;
			case ScriptFieldType::Float:
				fieldData.SetValue<float>(node.GetValue<float>(name, 0.f).Result);
				break;
			case ScriptFieldType::Bool:
				fieldData.SetValue<bool>(node.GetValue<bool>(name, false).Result);
				break;

			// more types...
			}
		}
	}
	void ScriptClass::SetClass(const std::string& fullName)
	{
		m_className = fullName;
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