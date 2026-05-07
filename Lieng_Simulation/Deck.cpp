#include "Deck.h"
#include <algorithm>
#include <random>
#include <stdexcept>

// ── Xây dựng bộ bài 52 lá ───────────────────────────────────
BoBai::BoBai() : viTriDinh(0) {
    const ChatBai danhSachChat[] = {
        ChatBai::CHUON, ChatBai::RO, ChatBai::CO, ChatBai::BICH
    };
    const SoBai danhSachSo[] = {
        SoBai::HAI,  SoBai::BA,   SoBai::BON,  SoBai::NAM,
        SoBai::SAU,  SoBai::BAY,  SoBai::TAM,  SoBai::CHIN,
        SoBai::MUOI, SoBai::BAO,  SoBai::DAM,  SoBai::VIET, SoBai::ACH
    };
    for (ChatBai c : danhSachChat)
        for (SoBai s : danhSachSo)
            cacLa.emplace_back(c, s);
}

// ── Xáo bài (Fisher-Yates) ───────────────────────────────────
void BoBai::xao() {
    static std::mt19937 boSinh{ std::random_device{}() };
    std::shuffle(cacLa.begin(), cacLa.end(), boSinh);
    viTriDinh = 0;
}

// ── Rút 1 lá ─────────────────────────────────────────────────
La BoBai::chia() {
    if (viTriDinh >= static_cast<int>(cacLa.size()))
        throw std::runtime_error("Bo bai da het la!");
    return cacLa[viTriDinh++];
}

// ── Đặt lại và xáo ──────────────────────────────────────────
void BoBai::datLai() {
    viTriDinh = 0;
    xao();
}
