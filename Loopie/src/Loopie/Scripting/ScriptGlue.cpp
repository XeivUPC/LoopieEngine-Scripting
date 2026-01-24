#include "ScriptGlue.h"
#include "Loopie/Scripting/ScriptingManager.h"
#include "Loopie/Components/ScriptClass.h"

#include "Loopie/Components/Transform.h"

#include "Loopie/Core/UUID.h"
#include "Loopie/Core/InputEventManager.h"

#include "Loopie/Core/Application.h"
#include "Loopie/Scene/Scene.h"
#include "Loopie/Scene/Entity.h"
#include "Loopie/Core/Log.h"
#include "Loopie/Core/Time.h"
#include "Loopie/Core/Assert.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>

namespace Loopie
{
	namespace Utils {
		std::string MonoStringToString(MonoString* string)
		{
			char* cStr = mono_string_to_utf8(string);
			std::string str(cStr);
			mono_free(cStr);
			return str;
		}
	}

	#define ADD_INTERNAL_CALL(Name) mono_add_internal_call("Loopie.InternalCalls::" #Name, Name)

#pragma region Log
	static void NativeLog(MonoString* parameter)
	{
		std::string str = Utils::MonoStringToString(parameter);
		Log::Warn("{0}", str);
	}

	static void NativeLog_Int(int parameter)
	{
		Log::Warn("{0}", parameter);
	}

	static void NativeLog_Float(float parameter)
	{
		Log::Warn("{0}", parameter);
	}

	static void NativeLog_Vector2(glm::vec2* parameter)
	{
		Log::Warn("{0} {1}", parameter->x, parameter->y);
	}

	static void NativeLog_Vector3(glm::vec3* parameter)
	{
		Log::Warn("{0} {1} {2}", parameter->x, parameter->y, parameter->z);
	}

	static void NativeLog_Vector4(glm::vec4* parameter)
	{
		Log::Warn("{0} {1} {2} {3}", parameter->x, parameter->y, parameter->z, parameter->w);
	}
#pragma endregion

#pragma region Components
	/*static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = &Application::GetInstance().GetScene();
		ASSERT(scene == nullptr, "Scene not found");
		std::shared_ptr<Entity> entity = scene->GetEntity(entityID);
		ASSERT(entity == nullptr, "Entity not found");

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		ASSERT(entity->HasComponent<ScriptClass>(), "{0} has no {1} component", entity->GetName(), managedType);
		std::vector<ScriptClass*> components = entity->GetComponents<ScriptClass>();
		for (size_t i = 0; i < components.size(); i++)
		{

		}
	}*/

	static MonoString* Entity_FindEntityByName(MonoString* name)
	{
		char* nameCStr = mono_string_to_utf8(name);

		Scene* scene = &Application::GetInstance().GetScene();
		ASSERT(scene == nullptr, "Scene not found");
		std::shared_ptr<Entity> entity = scene->GetEntity(nameCStr);
		mono_free(nameCStr);

		if (!entity)
			return ScriptingManager::CreateString("");

		return ScriptingManager::CreateString(entity->GetUUID().Get().c_str());
	}
#pragma endregion

#pragma region Transform
	static void Transform_GetPosition(UUID id, vec3* position)
	{
		Scene* scene = &Application::GetInstance().GetScene();
		std::shared_ptr<Entity> entity = scene->GetEntity(id);
		*position = entity->GetTransform()->GetPosition();
	}

	static void Transform_SetPosition(UUID id, vec3* position)
	{
		Scene* scene = &Application::GetInstance().GetScene();
		std::shared_ptr<Entity> entity = scene->GetEntity(id);
		entity->GetTransform()->SetPosition(*position);
	}
#pragma endregion

#pragma region Input
	static MonoBoolean Input_IsKeyDown(int keycode)
	{
		return Application::GetInstance().GetInputEvent().GetKeyStatus((SDL_Scancode)keycode) == KeyState::DOWN;
	}

	static MonoBoolean Input_IsKeyUp(int keycode)
	{
		return Application::GetInstance().GetInputEvent().GetKeyStatus((SDL_Scancode)keycode) == KeyState::UP;
	}

	static MonoBoolean Input_IsKeyPressed(int keycode)
	{
		return Application::GetInstance().GetInputEvent().GetKeyStatus((SDL_Scancode)keycode) == KeyState::REPEAT;
	}

	static MonoBoolean Input_IsMouseButtonDown(int mouseButton)
	{
		return Application::GetInstance().GetInputEvent().GetMouseButtonStatus(mouseButton) == KeyState::DOWN;
	}

	static MonoBoolean Input_IsMouseButtonUp(int mouseButton)
	{
		return Application::GetInstance().GetInputEvent().GetMouseButtonStatus(mouseButton) == KeyState::UP;
	}

	static MonoBoolean Input_IsMouseButtonPressed(int mouseButton)
	{
		return Application::GetInstance().GetInputEvent().GetMouseButtonStatus(mouseButton) == KeyState::REPEAT;
	}
	static MonoBoolean Input_IsGamepadButtonDown(int gamepadButton)
	{
		return Application::GetInstance().GetInputEvent().GetGamepadButtonStatus((SDL_GamepadButton)gamepadButton) == KeyState::DOWN;
	}

	static MonoBoolean Input_IsGamepadButtonUp(int gamepadButton)
	{
		return Application::GetInstance().GetInputEvent().GetGamepadButtonStatus((SDL_GamepadButton)gamepadButton) == KeyState::UP;
	}

	static MonoBoolean Input_IsGamepadButtonPressed(int gamepadButton)
	{
		return Application::GetInstance().GetInputEvent().GetGamepadButtonStatus((SDL_GamepadButton)gamepadButton) == KeyState::REPEAT;
	}
#pragma endregion

#pragma region Time
	static float Time_GetDeltaTime()
	{
		return Time::GetDeltaTime();
	}
#pragma endregion

	/*void ScriptGlue::RegisterComponents()
	{

	}*/

	void ScriptGlue::RegisterFunctions()
	{
		ADD_INTERNAL_CALL(NativeLog);
		ADD_INTERNAL_CALL(NativeLog_Int);
		ADD_INTERNAL_CALL(NativeLog_Float);
		ADD_INTERNAL_CALL(NativeLog_Vector2);
		ADD_INTERNAL_CALL(NativeLog_Vector3);

		ADD_INTERNAL_CALL(Entity_FindEntityByName);

		ADD_INTERNAL_CALL(Transform_GetPosition);
		ADD_INTERNAL_CALL(Transform_SetPosition);

		ADD_INTERNAL_CALL(Input_IsKeyDown);
		ADD_INTERNAL_CALL(Input_IsKeyUp);
		ADD_INTERNAL_CALL(Input_IsKeyPressed);
		ADD_INTERNAL_CALL(Input_IsMouseButtonDown);
		ADD_INTERNAL_CALL(Input_IsMouseButtonUp);
		ADD_INTERNAL_CALL(Input_IsMouseButtonPressed);
		ADD_INTERNAL_CALL(Input_IsGamepadButtonDown);
		ADD_INTERNAL_CALL(Input_IsGamepadButtonUp);
		ADD_INTERNAL_CALL(Input_IsGamepadButtonPressed);

		ADD_INTERNAL_CALL(Time_GetDeltaTime);
	}
}