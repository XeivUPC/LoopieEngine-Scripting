#include "Loopie/Core/UUID.h"
#include "Loopie/Core/Random.h"

namespace Loopie {

    UUID::UUID() : m_id(Generate()) {
    }

    UUID::UUID(const std::string& id)
        : m_id(id) {
    }

    const std::string& UUID::Get() const {
        return m_id;
    }

    std::string UUID::Generate() {
        const char* v = "0123456789abcdef";
        const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

        std::string res;
        res.reserve(36);

        for (int i = 0; i < 16; i++) {
            if (dash[i]) res += '-';
            res += v[Random::Get(0, 15)];
            res += v[Random::Get(0, 15)];
        }
        return res;
    }

    bool UUID::operator==(const UUID& other) const {
        return m_id == other.m_id;
    }

}
