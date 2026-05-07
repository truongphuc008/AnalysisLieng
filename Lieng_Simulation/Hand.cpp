#include "Hand.h"
#include <stdexcept>

void TayBai::them(const La& la) {
    if (cacLa.size() >= 3)
        throw std::runtime_error("Tay bai da du 3 la!");
    cacLa.push_back(la);
}

void TayBai::xoa() {
    cacLa.clear();
}

std::string TayBai::chuoi() const {
    std::string s;
    for (const auto& la : cacLa) {
        if (!s.empty()) s += "  ";
        s += la.chuoi();
    }
    return s;
}
