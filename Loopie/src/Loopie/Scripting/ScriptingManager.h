#pragma once

struct _MonoDomain;
struct _MonoAssembly;
struct _MonoImage;

namespace Loopie {
	struct ScriptingContext {
		_MonoDomain* RootDomain = nullptr;
		_MonoDomain* AppDomain = nullptr;

		_MonoAssembly* CoreAssembly = nullptr;
		_MonoImage* CoreImage = nullptr;

		_MonoAssembly* AppAssembly = nullptr;
		_MonoImage* AppImage = nullptr;


		bool Dirty = false;
	};	

	class ScriptingManager {
	public:
		static void Init();
		static void Shutdown();

		static void Reload();

		static void LoadCoreAssembly(const char* path);
		static void LoadAppAssembly(const char* path);

		static ScriptingContext s_Data;

	private:
		static _MonoAssembly* LoadAssembly(const char* path);
	};
}