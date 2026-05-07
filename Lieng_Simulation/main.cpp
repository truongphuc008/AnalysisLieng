#include <iostream>
#include <iomanip>
#include <vector>
#include "Player.h"
#include "Simulation.h"
#include "Visualizer.h"

// ================================================================
//  Mini-Project: Mo phong bai Lien (OOP C++)
//  Hoc phan: Lap trinh huong doi tuong (OOP)
//
//  Mo ta:
//    6 nguoi choi voi tam ly khac nhau tham gia 100,000 van bai
//    Lieng. Chuong trinh khao sat so tien cua moi nguoi theo tung
//    luot va xuat file CSV + bieu do HTML.
//
//  Mo hinh (theo Lieng.docx + Danhsach.xlsx):
//    Chi so Tiep tuc choi  C = TamLy * TiLeThang * 100
//    C <= 500              -> Bo bai
//    C > 500               -> Tiep tuc: cuoc C * 0.01 * Vong
//
//  Bang gioi han cuoc toi da (Nguong chiu dung rui ro):
//    C 0-500   -> 0d        C 501-600  -> 50d
//    C 601-700 -> 150d      C 701-800  -> 300d
//    C 801-850 -> 500d      C 851-880  -> 650d
//    C 881-910 -> 750d      C 911-940  -> 850d
//    C 941-970 -> 920d      C 971-990  -> 960d
//    C 990-1000-> 1000d (Toi da)
//
//  Ti le thang tu Danhsach.xlsx:
//    Sam Co Ach: 99.99%   Lieng:       99.77%
//    Sanh Rong:  99.75%   Sanh:        99.55%
//    Thung:      94.59%   Sanh Rac:    45%
//    Doi:        35%      Bai Rac:     10%
//
//  Bai dac biet (Sam Co / Lieng / Sanh Rong):
//    -> Bo qua tam ly, luon To toi da 1000d
//
//  Hoa: Nhieu nguoi cung hang cao nhat -> chia deu pot
// ================================================================


int main() {
    std::cout << "================================================\n";
    std::cout << "   DU AN: MO PHONG BAI LIENG (LTHDDT C++)\n";
    std::cout << "================================================\n\n";

    // ── Cấu hình 6 người chơi ──
    //  { Ten, Tien ban dau (d), Tam ly (1-10) }
    //
    //  Tam ly anh huong den:
    //    - Muc cuoc toi da
    //    - Quyet dinh To khi chiSoTiepTuc > 500
    // ──────────────────────────────────────────────
    std::vector<NguoiChoi> danhSachNguoiChoi = {
        NguoiChoi("An",    10000.0,  8.0),   // Hung hang
        NguoiChoi("Binh",  10000.0,  5.0),   // Trung binh
        NguoiChoi("Chi",   10000.0,  3.0),   // Than trong
        NguoiChoi("Dung",  10000.0,  9.0),   // Lieu linh
        NguoiChoi("Em",    10000.0,  6.0),   // Hoi mao hiem
        NguoiChoi("Phong", 10000.0,  2.0),   // Rat than trong
    };

    std::cout << "NGUOI CHOI:\n";
    std::cout << "+----------+----------+-----+-------------------------+\n";
    std::cout << "| Ten      | Tien(d)  | TL  | Phong cach              |\n";
    std::cout << "+----------+----------+-----+-------------------------+\n";
    const char* phongCach[] = {
        "Hung hang         ",
        "Trung binh        ",
        "Than trong        ",
        "Lieu linh         ",
        "Hoi mao hiem      ",
        "Rat than trong    "
    };
    for (int i = 0; i < static_cast<int>(danhSachNguoiChoi.size()); ++i) {
        std::cout << "| " << std::left  << std::setw(9) << danhSachNguoiChoi[i].ten
                  << "| " << std::right << std::setw(8) << danhSachNguoiChoi[i].tien
                  << " | " << std::setw(3) << danhSachNguoiChoi[i].tamLy
                  << " | " << phongCach[i] << "|\n";
    }
    std::cout << "+----------+----------+-----+-------------------------+\n";

    std::cout << "\nGIAI THICH MO HINH (theo Lieng.docx + Danhsach.xlsx):\n";
    std::cout << "  C = TamLy x TiLeThang x 100\n";
    std::cout << "  C <= 500  -> Bo bai\n";
    std::cout << "  C > 500   -> Tiep tuc: cuoc C*0.01*Vong dong\n";
    std::cout << "  Bang gioi han cuoc toi da (bang nguong):\n";
    std::cout << "    C 501-600: 50d    C 601-700: 150d   C 701-800: 300d\n";
    std::cout << "    C 801-850: 500d   C 851-880: 650d   C 881-910: 750d\n";
    std::cout << "    C 911-940: 850d   C 941-970: 920d   C 971-990: 960d\n";
    std::cout << "    C 990+  : 1000d (Toi da)\n";
    std::cout << "  Bai dac biet (Sam Co / Lieng / Sanh Rong) -> To toi da 1000d\n";
    std::cout << "  Hoa: Nhieu nguoi cung hang bai -> chia deu chung cuoc\n\n";

    // ── Giai đoạn 1: 100,000 ván (Biểu đồ đường) ────────────────
    std::cout << "\n[ Giai doan 1 ] Chay 100,000 van de lay du lieu bieu do duong...\n";
    CauHinh cauHinh1;
    cauHinh1.tongSoVan   = 100000;
    cauHinh1.luuMoi      = 100;
    cauHinh1.duongDanCSV  = "results.csv";
    cauHinh1.duongDanHTML = "results.html";

    std::vector<NguoiChoi> ds1 = danhSachNguoiChoi;
    MoPhong moPhong1(ds1, cauHinh1);
    moPhong1.chay(true); 

    auto lichSu100k = moPhong1.layLichSu();
    moPhong1.xuatCSV(); // Xuất CSV cho 100k ván
    
    // ── Giai đoạn 2: 200 ván (Biểu đồ cột) ──────────────────────
    std::cout << "\n[ Giai doan 2 ] Chay 200 van doc lap de lay du lieu bieu do cot...\n";
    CauHinh cauHinh2;
    cauHinh2.tongSoVan   = 200;
    cauHinh2.luuMoi      = 1;
    
    std::vector<NguoiChoi> ds2 = danhSachNguoiChoi;
    MoPhong moPhong2(ds2, cauHinh2);
    moPhong2.chay(false); // Chạy ngầm, không in ra màn hình

    auto soVanThang200 = moPhong2.laySoVanThang();

    // ── Xuất Kết quả HTML gộp chung ──────────────────────────────
    std::vector<std::string> tenNguoiChoi;
    for(const auto& ng: danhSachNguoiChoi) tenNguoiChoi.push_back(ng.ten);
    
    HienThi::xuatHTML(lichSu100k, soVanThang200, tenNguoiChoi, cauHinh1.luuMoi, cauHinh1.duongDanHTML);
    HienThi::inBangTongKet(ds1, 10000.0);

    std::cout << "\n[+] Da xuat file CSV : " << cauHinh1.duongDanCSV << "\n";
    std::cout << "[+] Da xuat bieu do  : " << cauHinh1.duongDanHTML << "\n";
    std::cout << "\n[*] Dang mo bieu do trong trinh duyet...\n";

    std::string lenh = "start \"\" \"" + cauHinh1.duongDanHTML + "\"";
    system(lenh.c_str());

    // Giữ cửa sổ khi chạy bằng double-click .exe
    std::cout << "Nhan Enter de thoat...";
    std::cin.get();

    return 0;
}
