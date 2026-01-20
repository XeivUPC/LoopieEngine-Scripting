#pragma once
#include "Loopie/Scripting/ScriptingClass.h"
#include <string>
#include <memory>
#include <unordered_map>

struct _MonoDomain;
struct _MonoAssembly;
struct _MonoImage;
struct _MonoObject;
struct _MonoClass;

namespace Loopie {
	struct ScriptingContext {
		_MonoDomain* RootDomain = nullptr;
		_MonoDomain* AppDomain = nullptr;

		_MonoAssembly* CoreAssembly = nullptr;
		_MonoImage* CoreImage = nullptr;

		_MonoAssembly* AppAssembly = nullptr;
		_MonoImage* AppImage = nullptr;

		std::string CoreAssemblyFilepath;
		std::string AppAssemblyFilepath;

		std::unordered_map<std::string, std::shared_ptr<ScriptingClass>> ScriptingClasses;

		bool Dirty = false;
	};	


	class ScriptingManager {
	public:
		static void Init();
		static void Shutdown();

		static void Reload();

		static void LoadCoreAssembly();
		static void LoadAppAssembly();

		static void LoadScriptingClasses(_MonoImage* monoImage);
		static _MonoObject* InstantiateScriptingClass(_MonoClass* monoClass);
		static bool ExistsScriptingClass(const std::string& monoClassName);
		std::shared_ptr<ScriptingClass> GetScriptingClass(const std::string& monoClassName);
		const std::unordered_map<std::string, std::shared_ptr<ScriptingClass>>& GetScriptingClasses() { return s_Data.ScriptingClasses; }

		static ScriptingContext s_Data;

	private:
		static _MonoAssembly* LoadAssembly(const char* path);
	};
	
}