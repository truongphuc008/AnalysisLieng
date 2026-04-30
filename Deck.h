#pragma once
#include "Card.h"
#include <vector>

// =============================================================
//  Class BoBai – Bộ bài 52 lá
// =============================================================
class BoBai {
private:
    std::vector<La> cacLa;
    int viTriDinh;   // Chỉ số lá tiếp theo sẽ được rút

public:
    BoBai();

    // Xáo bài (Fisher-Yates với mt19937)
    void xao();

    // Rút 1 lá từ đỉnh bộ bài
    La chia();

    // Đặt lại bộ bài về 52 lá và xáo lại
    void datLai();

    // Số lá còn lại
    int conLai() const { return static_cast<int>(cacLa.size()) - viTriDinh; }
};
