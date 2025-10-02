#include "Json.h"



namespace Loopie {

#pragma region Json

	JsonData Json::ReadFromString(const std::string& data)
	{
		JsonData jsonData;

		jsonData.m_data = json::parse(data);

		jsonData.m_empty = jsonData.m_data.is_discarded();

		return jsonData;
	}

	JsonData Json::ReadFromFile(const std::filesystem::path& filePath)
	{
		JsonData jsonData;

		std::ifstream ifs(filePath);
		jsonData.m_data = json::parse(ifs);
		jsonData.m_empty = jsonData.m_data.is_discarded();

		return jsonData;
	}

	bool Json::WriteToFile(const std::filesystem::path& filePath, const JsonData& jsonData, int indent)
	{
		std::ofstream ofs(filePath);
		if (!ofs.is_open()) return false;
		ofs << jsonData.m_data.dump(indent);
		return true;
	}

#pragma endregion

#pragma region JsonNode

	JsonNode::JsonNode(json* node) : m_node(node) {}
	JsonNode::JsonNode(json* node, json* parentNode) : m_node(node), m_parentNode(parentNode) {}

	JsonNode JsonNode::Child(const std::string& keyPath) const
	{
		if (!Exists())
			return JsonNode();

		json* node = m_node;
		json* parentNode = m_parentNode;

		std::stringstream ss(keyPath);
		std::string token;
		while (std::getline(ss, token, '.')) {
			if (!node->contains(token))
				return JsonNode();
			parentNode = node;
			node = &((*node)[token]);
		}

		return JsonNode(node,parentNode);
	}

	void JsonNode::Clear() {
		m_node = nullptr;
		m_parentNode = nullptr;
	}

	bool JsonNode::RemoveField(const std::string& keyPath) {
		if (keyPath.empty())
			return false;

		auto pos = keyPath.find_last_of('.');
		std::string parentPath = (pos == std::string::npos) ? "" : keyPath.substr(0, pos);
		std::string key = (pos == std::string::npos) ? keyPath : keyPath.substr(pos + 1);

		JsonNode node = Child(parentPath);
		if (!node.Exists())
			return false;

		if (!node.m_node->contains(key))
			return false;

		node.m_node->erase(key);
		return true;
	}

	JsonNode JsonNode::AddObjectField(const std::string& keyPath) {
		return AddField(keyPath, json::object());
	}

	JsonNode JsonNode::AddArrayField(const std::string& keyPath) {
		return AddField(keyPath, json::array());
	}

	bool JsonNode::RemoveArrayElement(const std::string& keyPath, unsigned int index) {
		if (keyPath.empty())
			return RemoveArrayElementSelf(index);

		JsonNode node = Child(keyPath);
		if (!node.Exists() || !node.m_node->is_array())
			return false;

		if (node.m_node->empty())
			return false;

		if (node.m_node->size() <= index)
			return false;
		node.m_node->erase(node.m_node->begin() + index);
		return true;

	}

	bool JsonNode::RemoveArrayElementSelf(unsigned int index) {
		if (!Exists() || !m_node->is_array())
			return false;

		if (m_node->empty())
			return false;

		if (m_node->size() <= index)
			return false;
		m_node->erase(m_node->begin() + index);
		return true;
	}

	bool JsonNode::ClearArrayField(const std::string& keyPath) {
		if (keyPath.empty())
			return ClearArrayFieldSelf();

		JsonNode node = Child(keyPath);
		if (!node.Exists() || !node.m_node->is_array())
			return false;

		node.m_node->clear();
		return true;
	}

	bool JsonNode::ClearArrayFieldSelf() {
		if (!Exists() || !m_node->is_array())
			return false;

		m_node->clear();
		return true;
	}

#pragma endregion

	
}