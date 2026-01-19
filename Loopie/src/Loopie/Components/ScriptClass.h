#pragma once
#include "Loopie/Components/Component.h"


struct _MonoObject;
struct _MonoClass;
struct _MonoMethod;

namespace Loopie {
	class ScriptClass : public Component
	{
	public:
		DEFINE_TYPE(ScriptClass)

		ScriptClass();
		~ScriptClass();
		void Init() override; //// From Component

		JsonNode Serialize(JsonNode& parent) const override;
		void Deserialize(const JsonNode& data) override;

		void SetClass(const std::string& fullName);
		const std::string& GetClass() const { return m_ClassName; }

	private:
		void CreateInstance();
		void DestroyInstance();

		void Invoke(const char* methodName);

	private:
		std::string m_ClassName;

		_MonoObject* m_Instance = nullptr;
		_MonoClass* m_Class = nullptr;

		_MonoMethod* m_OnCreate = nullptr;
		_MonoMethod* m_OnUpdate = nullptr;

		bool m_Initialized = false;
	};
}