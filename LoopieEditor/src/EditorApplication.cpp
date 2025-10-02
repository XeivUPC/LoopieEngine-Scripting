#include "ProjectSetupModule.h"


#include "Loopie/Core/Application.h"
#include "Loopie/Core/Log.h"
#include "Loopie/Files/Json.h"

#include <vector>

namespace Loopie {

	class EditorApplication : public Application {
	public:
		EditorApplication() : Application() {
			//// Open first the module -> Project CreationSetUp
			//// Once the Porject is selected, remove the module and add the EditorModule

			AddModule(new ProjectSetupModule());

			std::string json_str = R"({
										"player": {
											"name": "Alice",
											"stats": { "hp": 100, "mana": 50 },
											"inventory": ["sword", "shield"]
										}
									})";

			JsonData data = Json::ReadFromString(json_str);

			JsonNode root = data.Root();
			JsonNode playerNode = root.Child("player");
			JsonNode nameNode = root.Child("player.name");



			Log::Info("{0}",data.Get<int>("player.stats.mana").Result);
			Log::Info("{0}", root.Get<std::string>("player.name").Result);
			Log::Info("{0}", playerNode.Get<std::string>("name").Result);

			//playerNode.Set<std::string>("name","Lucia");
			root.Set("player.name", "Carla");

			Log::Info("{0}", playerNode.Get<std::string>("name").Result);
			Log::Info("{0}", root.Get<std::string>("player.name").Result);

			nameNode.SetSelf("Manuel");

			Log::Info("{0}", playerNode.Get<std::string>("name").Result);
			Log::Info("{0}", root.Get<std::string>("player.name").Result);
			Log::Info("{0}", nameNode.GetSelf<std::string>().Result);

			Log::Info("{0}", playerNode.AddArrayElement("inventory", "potion"));
			Log::Info("{0}", playerNode.Get<std::vector<std::string>>("inventory").Result[0]);

			playerNode.AddField("testStats", 100);

			JsonNode test = playerNode.AddObjectField("backpack");
			Log::Info("{0}", test.AddField("potions", 3).Exists());

			JsonNode test2 = playerNode.AddArrayField("titles");
			Log::Info("{0}", test2.AddArrayElementSelf("baron"));

			root.Child("player.inventory").ModifyArrayElementSelf(0, "camion");

			Json::WriteToFile("test.json", data);
		}
	};
}

int main() {
	Loopie::Application* app = new Loopie::EditorApplication();
	app->Run();
	delete app;
}