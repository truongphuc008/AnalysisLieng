#pragma once
#include <string>

// =============================================================
//  Bộ bài Tây – Các giá trị cơ bản
// =============================================================

// Chất bài
enum class ChatBai {
    CHUON    = 0,   // Chuồn (♣)
    RO       = 1,   // Rô    (♦)
    CO       = 2,   // Cơ    (♥)
    BICH     = 3    // Bích  (♠)
};

// Số bài
enum class SoBai {
    HAI     = 2,
    BA      = 3,
    BON     = 4,
    NAM     = 5,
    SAU     = 6,
    BAY     = 7,
    TAM     = 8,
    CHIN    = 9,
    MUOI    = 10,
    BAO     = 11,   // J (Bao)
    DAM     = 12,   // Q (Đầm)
    VIET    = 13,   // K (Việt)
    ACH     = 14    // A (Ách)
};

// =============================================================
//  Class La – Một lá bài
// =============================================================
class La {
public:
    ChatBai chat;
    SoBai   so;

    La(ChatBai c, SoBai s) : chat(c), so(s) {}

    // Giá trị số của lá bài (2-14)
    int giaTri() const { return static_cast<int>(so); }

    // Ký hiệu chất bài
    std::string kyHieuChat() const;

    // Tên số bài
    std::string tenSo() const;

    // Biểu diễn dạng chuỗi, ví dụ: "A[Co]", "K[Bich]"
    std::string chuoi() const;
};
