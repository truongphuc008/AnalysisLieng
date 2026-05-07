#include "Card.h"

// ── Ký hiệu chất bài ────────────────────────────────────────
std::string La::kyHieuChat() const {
    switch (chat) {
        case ChatBai::CHUON: return "Chuon";
        case ChatBai::RO:    return "Ro";
        case ChatBai::CO:    return "Co";
        case ChatBai::BICH:  return "Bich";
        default:             return "?";
    }
}

// ── Tên số bài ──────────────────────────────────────────────
std::string La::tenSo() const {
    switch (so) {
        case SoBai::HAI:  return "2";
        case SoBai::BA:   return "3";
        case SoBai::BON:  return "4";
        case SoBai::NAM:  return "5";
        case SoBai::SAU:  return "6";
        case SoBai::BAY:  return "7";
        case SoBai::TAM:  return "8";
        case SoBai::CHIN: return "9";
        case SoBai::MUOI: return "10";
        case SoBai::BAO:  return "J";
        case SoBai::DAM:  return "Q";
        case SoBai::VIET: return "K";
        case SoBai::ACH:  return "A";
        default:          return "?";
    }
}

// ── Chuỗi hiển thị ──────────────────────────────────────────
std::string La::chuoi() const {
    return tenSo() + "[" + kyHieuChat() + "]";
}
