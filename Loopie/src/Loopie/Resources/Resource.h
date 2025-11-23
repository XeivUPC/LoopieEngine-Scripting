#pragma once

#include "Loopie/Core/UUID.h"
#include "Loopie/Core/Identificable.h"

namespace Loopie {
	class Resource : public  Identificable {
	public:
		Resource(UUID uuid) : m_uuid(uuid) {}
		virtual ~Resource();

		const UUID& GetUUID() { return m_uuid; }
		//void SetUUID(const std::string uuid) { m_uuid = UUID(uuid); };

		virtual void LoadFromFile(const std::string path) = 0;
		virtual void Reload() = 0;

		const std::string& GetPath() { return m_path; }
	protected:
		UUID m_uuid;
		std::string m_path;
	};
}