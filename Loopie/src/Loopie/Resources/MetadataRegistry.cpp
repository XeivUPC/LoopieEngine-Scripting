#include "MetadataRegistry.h"

#include "Loopie/Core/Log.h"
#include "Loopie/Core/Application.h"
#include "Loopie/Files/Json.h"
#include "Loopie/Files/DirectoryManager.h"

namespace Loopie {
	Metadata MetadataRegistry::GetMetadataAsset(const std::string& assetPath)
	{
		std::filesystem::path metadataPath = assetPath + ".meta";

        if (!std::filesystem::exists(metadataPath)) {
            Metadata metadata;
            SaveMetadata(assetPath, metadata);
            return metadata;
        }
        else {
            Metadata metadata;

            JsonData data = Json::ReadFromFile(metadataPath);
            metadata.UUID = UUID(data.GetValue<std::string>("Id").Result);
            metadata.HasCache = data.GetValue<bool>("HasCache").Result;
            if (metadata.HasCache) {
                JsonNode cacheNode = data.Child("Caches");
                int entries = cacheNode.Size();
                Project project = Application::GetInstance().m_activeProject;
                for (size_t i = 0; i < entries; i++)
                {
                    std::string cachePath = cacheNode.GetArrayElement<std::string>(i).Result;
                    if (!std::filesystem::exists(project.GetChachePath() / cachePath))
                    {
                        metadata.CachesPath.clear();
                        metadata.HasCache = false;
                        break;
                    }
                    metadata.CachesPath.push_back(cachePath);
                }
            }
            return metadata;
        }
	}

	void MetadataRegistry::SaveMetadata(const std::filesystem::path& assetPath, const Metadata& metadata)
	{
        std::filesystem::path metadataPath = assetPath.string() + ".meta";
        JsonData data;
        data.CreateField("Id",metadata.UUID.Get());
        data.CreateField("HasCache", metadata.HasCache);

        if (metadata.HasCache) {
            data.CreateArrayField("Caches");
            for (const auto& paths : metadata.CachesPath)
            {
                data.AddArrayElement("Caches", paths);
            }
        }
       
        data.ToFile(metadataPath);
	}
    bool MetadataRegistry::IsMetadataFile(const std::filesystem::path& assetPath)
    {
        return assetPath.extension() == ".meta";
    }
}