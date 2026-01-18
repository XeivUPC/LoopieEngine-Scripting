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
	}

	void ScriptingManager::Shutdown() {
		if (s_Data.RootDomain) {
			mono_jit_cleanup(s_Data.RootDomain);
		}
		
	}
}