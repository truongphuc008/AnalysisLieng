#pragma once
#include "Card.h"
#include <vector>
#include <string>

// =============================================================
//  Class TayBai – 3 lá bài của 1 người chơi
// =============================================================
class TayBai {
private:
    std::vector<La> cacLa;

public:
    TayBai() = default;

    void them(const La& la);
    void xoa();

    const std::vector<La>& layDanhSach() const { return cacLa; }
    int  soLa()   const { return static_cast<int>(cacLa.size()); }
    bool dayDu()  const { return cacLa.size() == 3; }

    // Biểu diễn bộ 3 lá dạng chuỗi
    std::string chuoi() const;
};
