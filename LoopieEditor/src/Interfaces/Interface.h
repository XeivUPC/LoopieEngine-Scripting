#pragma once

namespace Loopie {
	class Interface {
	public:
		Interface() = default;
		~Interface() = default;
		virtual void Render() {};
	};
}