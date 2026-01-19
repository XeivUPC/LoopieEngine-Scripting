#include "ScriptingManager.h"

#include "Loopie/Core/Log.h"

#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>

namespace Loopie {
	ScriptingContext ScriptingManager::s_Data;

	void ScriptingManager::Init() {
		mono_set_dirs(
			"mono/lib",
			"mono/etc"
		);
		mono_config_parse(NULL);

		s_Data.RootDomain = mono_jit_init("LoopieScriptingRuntime");

		if (!s_Data.RootDomain) {
			Log::Error("Failed to initialize Mono");
		}

		s_Data.AppDomain = mono_domain_create_appdomain((char*)"LoopieAppDomain", nullptr);
		mono_domain_set(s_Data.AppDomain, true);

		LoadCoreAssembly("Scripting/Loopie.Core.dll");
		LoadAppAssembly("Scripting/Game.dll");

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

	void ScriptingManager::Reload() {
		Log::Info("Reloading scripting domain");

		mono_domain_set(s_Data.RootDomain, false);
		mono_domain_unload(s_Data.AppDomain);

		s_Data.AppDomain = mono_domain_create_appdomain((char*)"LoopieAppDomain", nullptr);
		mono_domain_set(s_Data.AppDomain, true);

		// Reload assemblies
		LoadCoreAssembly("Loopie.Core.dll");
		LoadAppAssembly("Game.dll");

		s_Data.Dirty = false;
	}

	void ScriptingManager::LoadCoreAssembly(const char* path)
	{
		s_Data.CoreAssembly = LoadAssembly(path);
		if(s_Data.CoreAssembly)
			s_Data.CoreImage = mono_assembly_get_image(s_Data.CoreAssembly);
	}

	void ScriptingManager::LoadAppAssembly(const char* path)
	{
		s_Data.AppAssembly = LoadAssembly(path);
		if(s_Data.AppAssembly)
			s_Data.AppImage = mono_assembly_get_image(s_Data.AppAssembly);
	}

	_MonoAssembly* ScriptingManager::LoadAssembly(const char* path)
	{
		_MonoAssembly* assembly = mono_domain_assembly_open(s_Data.AppDomain, path);
		if (!assembly)
			Log::Error("Failed to load assembly: {}", path);
		return assembly;
	}
}