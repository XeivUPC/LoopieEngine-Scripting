#include "ScriptingManager.h"

#include "Loopie/Scripting/ScriptGlue.h"

#include "Loopie/Core/Log.h"
#include "Loopie/Core/Application.h"
#include "Loopie/Events/Event.h"

#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/attrdefs.h>

namespace Loopie {
	ScriptingContext ScriptingManager::s_Data;
	bool ScriptingManager::s_IsRunning;

	static ScriptFieldType MonoTypeToScriptFieldType(_MonoType* monoType)
	{
		std::string typeName = mono_type_get_name(monoType);

		auto it = s_ScriptFieldTypeMap.find(typeName);
		if (it == s_ScriptFieldTypeMap.end())
		{
			return ScriptFieldType::None;
		}

		return it->second;
	}
	MonoString* ScriptingManager::CreateString(const char* string)
	{
		return mono_string_new(s_Data.AppDomain, string);
	}
	void ScriptingManager::Init() {
		mono_set_dirs("mono/lib", "mono/etc");
		mono_config_parse(NULL);

		s_Data.RootDomain = mono_jit_init("LoopieScriptingRuntime");
		if (!s_Data.RootDomain) {
			Log::Error("Failed to initialize Mono");
			return;
		}

		s_Data.AppDomain = mono_domain_create_appdomain("LoopieAppDomain", nullptr);
		mono_domain_set(s_Data.AppDomain, true);


		s_Data.CoreAssemblyFilepath = "Scripting/Loopie.Core.dll";
		s_Data.AppAssemblyFilepath = "Scripting/Game.dll";

		LoadCoreAssembly();
		LoadAppAssembly(); 

		ScriptGlue::RegisterFunctions();

		LoadScriptingClasses(s_Data.CoreImage); // ChangeToGame
	}

	void ScriptingManager::Shutdown() {

		mono_domain_set(mono_get_root_domain(), false);

		if (s_Data.AppDomain) {
			mono_domain_unload(s_Data.AppDomain);
			s_Data.AppDomain = nullptr;
		}
		if (s_Data.RootDomain) {
			mono_jit_cleanup(s_Data.RootDomain);
			s_Data.RootDomain = nullptr;
		}	
	}

	void ScriptingManager::Reload() { /// Fix Errro, crash when reload and start runtime

		Log::Info("Reloading scripting domain");

		mono_domain_set(s_Data.RootDomain, false);
		mono_domain_unload(s_Data.AppDomain);

		s_Data.AppDomain = mono_domain_create_appdomain((char*)"LoopieAppDomain", nullptr);
		mono_domain_set(s_Data.AppDomain, true);

		// Reload assemblies
		LoadCoreAssembly();
		LoadAppAssembly();

		LoadScriptingClasses(s_Data.CoreImage); // ChangeToGame

		s_Data.Dirty = false;
	}

	void ScriptingManager::LoadCoreAssembly()
	{
		s_Data.CoreAssembly = LoadAssembly(s_Data.CoreAssemblyFilepath.c_str());
		if(s_Data.CoreAssembly)
			s_Data.CoreImage = mono_assembly_get_image(s_Data.CoreAssembly);
	}

	void ScriptingManager::LoadAppAssembly()
	{
		s_Data.AppAssembly = LoadAssembly(s_Data.AppAssemblyFilepath.c_str());
		if(s_Data.AppAssembly)
			s_Data.AppImage = mono_assembly_get_image(s_Data.AppAssembly);
	}

	void ScriptingManager::RuntimeStart()
	{
		s_IsRunning = true;
		Application::GetInstance().m_notifier.Notify(EngineNotification::OnRuntimeStart);
	}

	void ScriptingManager::RuntimeStop()
	{
		s_IsRunning = false;
		Application::GetInstance().m_notifier.Notify(EngineNotification::OnRuntimeStop);
	}

	void ScriptingManager::AttachCurrentThread()
	{
		if (!mono_thread_current())
			mono_thread_attach(mono_domain_get());
	}

	void ScriptingManager::LoadScriptingClasses(_MonoImage* monoImage)
	{
		s_Data.ScriptingClasses.clear();

		//// Load Scripting Classes
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(monoImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(monoImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(monoImage, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, className);
			else
				fullName = className;

			MonoClass* monoClass = mono_class_from_name(monoImage, nameSpace, className);

			std::shared_ptr<ScriptingClass> scriptClass = std::make_shared<ScriptingClass>(nameSpace, className, true);
			s_Data.ScriptingClasses[fullName] = scriptClass;

			int fieldCount = mono_class_num_fields(monoClass);
			Log::Warn("{} has {} fields:", className, fieldCount);
			void* iterator = nullptr;
			while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
			{
				const char* fieldName = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);
				if (flags & MONO_FIELD_ATTR_PUBLIC)
				{
					MonoType* type = mono_field_get_type(field);
					ScriptFieldType fieldType = MonoTypeToScriptFieldType(type);
					scriptClass->GetFields()[fieldName] = { fieldType, fieldName, field };

					Log::Warn("   {}", fieldName);
				}
			}
		}
	}

	_MonoObject* ScriptingManager::InstantiateScriptingClass(_MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data.AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	bool ScriptingManager::ExistsScriptingClass(const std::string& monoClassName)
	{
		return s_Data.ScriptingClasses.find(monoClassName) != s_Data.ScriptingClasses.end();
	}

	std::shared_ptr<ScriptingClass> ScriptingManager::GetScriptingClass(const std::string& monoClassName)
	{
		if (!ExistsScriptingClass(monoClassName))
			return nullptr;

		return s_Data.ScriptingClasses.at(monoClassName);
	}

	_MonoAssembly* ScriptingManager::LoadAssembly(const char* path)
	{
		_MonoAssembly* assembly = mono_domain_assembly_open(s_Data.AppDomain, path);
		if (!assembly)
			Log::Error("Failed to load assembly: {}", path);
		return assembly;
	}
}