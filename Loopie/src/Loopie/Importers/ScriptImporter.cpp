#include "ScriptImporter.h"

namespace Loopie
{

	void ScriptImporter::ImportScript(const std::string& filepath, Metadata& metadata)
	{
		metadata.Type = ResourceType::SCRIPT;
		MetadataRegistry::SaveMetadata(filepath, metadata);
	}

	bool ScriptImporter::CheckIfIsScript(const char* path)
	{
		std::string extension = std::filesystem::path(path).extension().string();
		for (char& c : extension)
		{
			c = std::tolower(static_cast<unsigned char>(c));
		}

		if (!extension.empty() && extension[0] == '.')
			extension = extension.substr(1);

		return extension == "cs";
	}
}
