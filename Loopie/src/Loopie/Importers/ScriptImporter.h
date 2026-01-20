#pragma once
#include "Loopie/Resources/MetadataRegistry.h"

#include <memory>
#include <vector>
#include <string>


namespace Loopie {
    class ScriptImporter {
    public:
        static void ImportScript(const std::string& filepath, Metadata& metadata);
        static bool CheckIfIsScript(const char* path);
    private:
        
    };
}