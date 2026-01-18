

struct _MonoDomain;
struct _MonoAssembly;
struct _MonoImage;

namespace Loopie {



	struct ScriptingContext {
		_MonoDomain* RootDomain = nullptr;
		_MonoDomain* AppDomain = nullptr;
		_MonoAssembly* CoreAssembly = nullptr;
		_MonoImage* CoreImage = nullptr;
	};	

	class ScriptingManager {
	public:
		static void Init();
		static void Shutdown();

		static ScriptingContext s_Data;
	};
}