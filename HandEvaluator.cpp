#include "HandEvaluator.h"
#include <algorithm>
#include <array>

// ─────────────────────────────────────────────────────────────
//  Hàm nội bộ: trích xuất 3 giá trị đã sắp xếp tăng dần
// ─────────────────────────────────────────────────────────────
static std::array<int, 3> sapXepGiaTri(const TayBai& tay) {
    std::array<int, 3> ds = {
        tay.layDanhSach()[0].giaTri(),
        tay.layDanhSach()[1].giaTri(),
        tay.layDanhSach()[2].giaTri()
    };
    std::sort(ds.begin(), ds.end());
    return ds;
}

// ─────────────────────────────────────────────────────────────
//  Kiểm tra 3 lá cùng chất
// ─────────────────────────────────────────────────────────────
bool DanhGiaBai::laCungChat(const TayBai& tay) {
    const auto& ds = tay.layDanhSach();
    return ds[0].chat == ds[1].chat && ds[1].chat == ds[2].chat;
}

// ─────────────────────────────────────────────────────────────
//  Kiểm tra 3 lá liên tiếp (A chỉ đứng đầu: Q-K-A)
// ─────────────────────────────────────────────────────────────
bool DanhGiaBai::laSanh(const TayBai& tay) {
    auto ds = sapXepGiaTri(tay);
    return (ds[1] - ds[0] == 1) && (ds[2] - ds[1] == 1);
}

// ─────────────────────────────────────────────────────────────
//  Sảnh rồng: A-K-Q cùng chất
// ─────────────────────────────────────────────────────────────
bool DanhGiaBai::laSanhRong(const TayBai& tay) {
    auto ds = sapXepGiaTri(tay);
    // Q=12, K=13, A=14
    return laCungChat(tay) && ds[0] == 12 && ds[1] == 13 && ds[2] == 14;
}

// ─────────────────────────────────────────────────────────────
//  Đánh giá hạng bài chính
// ─────────────────────────────────────────────────────────────
HangBai DanhGiaBai::danhGia(const TayBai& tay) {
    auto ds      = sapXepGiaTri(tay);
    bool cungChat = laCungChat(tay);
    bool sanh     = laSanh(tay);

    // Sám cô: 3 con Ách
    if (ds[0] == 14 && ds[1] == 14 && ds[2] == 14)
        return HangBai::SAM_CO;

    // Liêng: 3 lá cùng số (không phải 3 Ách)
    if (ds[0] == ds[1] && ds[1] == ds[2])
        return HangBai::LIENG;

    // Sảnh rồng: A-K-Q cùng chất
    if (laSanhRong(tay))
        return HangBai::SANH_RONG;

    // Sảnh: 3 liên tiếp cùng chất (trừ sảnh rồng)
    if (sanh && cungChat)
        return HangBai::SANH;

    // Thùng: 3 cùng chất, không liên tiếp
    if (cungChat)
        return HangBai::THUNG;

    // Sảnh rác: 3 liên tiếp, khác chất
    if (sanh)
        return HangBai::SANH_RAC;

    // Đôi: 2 lá giống nhau
    if (ds[0] == ds[1] || ds[1] == ds[2])
        return HangBai::DOI;

    // Bài rác
    return HangBai::RAC;
}

// ─────────────────────────────────────────────────────────────
//  Tỉ lệ thắng theo hạng bài
// ─────────────────────────────────────────────────────────────
double DanhGiaBai::tiLeThang(HangBai hang) {
    switch (hang) {
        case HangBai::SAM_CO:    return 0.9999;   // 99.99%
        case HangBai::LIENG:     return 0.9977;   // 99.77%
        case HangBai::SANH_RONG: return 0.9975;   // 99.75%
        case HangBai::SANH:      return 0.9955;   // 99.55%
        case HangBai::THUNG:     return 0.9459;   // 94.59%
        case HangBai::SANH_RAC:  return 0.45;
        case HangBai::DOI:       return 0.35;
        case HangBai::RAC:       return 0.10;
        default:                 return 0.0;
    }
}

// ─────────────────────────────────────────────────────────────
//  So sánh lá cao (từ lá cao nhất xuống thấp nhất)
// ─────────────────────────────────────────────────────────────
int DanhGiaBai::soSanhLaCao(const TayBai& a, const TayBai& b) {
    auto da = sapXepGiaTri(a);
    auto db = sapXepGiaTri(b);
    for (int i = 2; i >= 0; --i) {
        if (da[i] > db[i]) return  1;
        if (da[i] < db[i]) return -1;
    }
    return 0;
}

// ─────────────────────────────────────────────────────────────
//  So sánh Đôi: hạng đôi trước, lá lẻ sau
// ─────────────────────────────────────────────────────────────
int DanhGiaBai::soSanhDoi(const TayBai& a, const TayBai& b) {
    auto da = sapXepGiaTri(a);
    auto db = sapXepGiaTri(b);

    int doiA, leA, doiB, leB;
    if (da[0] == da[1]) { doiA = da[0]; leA = da[2]; }
    else                { doiA = da[2]; leA = da[0]; }
    if (db[0] == db[1]) { doiB = db[0]; leB = db[2]; }
    else                { doiB = db[2]; leB = db[0]; }

    if (doiA != doiB) return doiA > doiB ? 1 : -1;
    if (leA  != leB)  return leA  > leB  ? 1 : -1;
    return 0;
}

// ─────────────────────────────────────────────────────────────
//  So sánh Liêng (3 cùng số)
// ─────────────────────────────────────────────────────────────
int DanhGiaBai::soSanhLieng(const TayBai& a, const TayBai& b) {
    auto da = sapXepGiaTri(a);
    auto db = sapXepGiaTri(b);
    if (da[0] > db[0]) return  1;
    if (da[0] < db[0]) return -1;
    return 0;
}

// ─────────────────────────────────────────────────────────────
//  So sánh tổng quát hai tay bài
//  Trả về: 1 (a thắng), -1 (b thắng), 0 (hoà)
// ─────────────────────────────────────────────────────────────
int DanhGiaBai::soSanh(const TayBai& a, const TayBai& b) {
    HangBai ha = danhGia(a);
    HangBai hb = danhGia(b);

    if (ha != hb)
        return static_cast<int>(ha) > static_cast<int>(hb) ? 1 : -1;

    // Cùng hạng → so sánh chi tiết
    switch (ha) {
        case HangBai::SAM_CO:    return 0;                 // Chỉ có AAA
        case HangBai::LIENG:     return soSanhLieng(a, b);
        case HangBai::SANH_RONG: return 0;                 // AKQ cùng chất luôn hoà
        case HangBai::SANH:
        case HangBai::SANH_RAC:  return soSanhLaCao(a, b);
        case HangBai::THUNG:     return soSanhLaCao(a, b);
        case HangBai::DOI:       return soSanhDoi(a, b);
        default:                 return soSanhLaCao(a, b); // RAC
    }
}

// ─────────────────────────────────────────────────────────────
//  Tên hạng bài
// ─────────────────────────────────────────────────────────────
std::string DanhGiaBai::tenHang(HangBai hang) {
    switch (hang) {
        case HangBai::SAM_CO:    return "Sam Co (3 Ach - BAI THANH!)";
        case HangBai::LIENG:     return "Lieng (3 cung so)";
        case HangBai::SANH_RONG: return "Sanh Rong (A-K-Q cung chat)";
        case HangBai::SANH:      return "Sanh (lien tiep cung chat)";
        case HangBai::THUNG:     return "Thung (cung chat)";
        case HangBai::SANH_RAC:  return "Sanh rac (lien tiep khac chat)";
        case HangBai::DOI:       return "Doi";
        default:                 return "Bai rac / Mau thau";
    }
}
