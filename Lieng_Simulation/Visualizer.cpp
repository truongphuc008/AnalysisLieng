#include "Visualizer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <sstream>

// ─────────────────────────────────────────────────────────────
//  Ký tự đại diện mỗi người chơi trên biểu đồ
// ─────────────────────────────────────────────────────────────
static const char KY_TU_NGUOI_CHOI[] = { '1', '2', '3', '4', '5', '6' };

// ─────────────────────────────────────────────────────────────
//  Bảng tổng kết cuối cùng
// ─────────────────────────────────────────────────────────────
void HienThi::inBangTongKet(
    const std::vector<NguoiChoi>& danhSach,
    double tienBanDau)
{
    std::cout << "\n";
    std::cout << "+--------------------------------------------------------+\n";
    std::cout << "|               BANG TONG KET MO PHONG                  |\n";
    std::cout << "+----------------+----------+----------+------------------+\n";
    std::cout << "| Ten            |  Tien(d) | Tam ly   | Loi/Lo (d)      |\n";
    std::cout << "+----------------+----------+----------+------------------+\n";
    for (const auto& ng : danhSach) {
        double chenh = ng.tien - tienBanDau;
        std::string dau = chenh >= 0 ? "+" : "";
        std::cout << "| " << std::left  << std::setw(15) << ng.ten
                  << "| " << std::right << std::setw(8)  << std::fixed
                  << std::setprecision(0) << ng.tien
                  << " | " << std::setw(7) << std::setprecision(2)
                  << ng.tamLy
                  << " | " << dau
                  << std::setw(14) << std::setprecision(0) << chenh << " |\n";
    }
    std::cout << "+----------------+----------+----------+------------------+\n\n";
}

// ─────────────────────────────────────────────────────────────
//  Xuất CSV
// ─────────────────────────────────────────────────────────────
void HienThi::xuatCSV(
    const std::vector<std::vector<double>>& lichSu,
    const std::vector<std::string>&         tenNguoiChoi,
    int buocLuu,
    const std::string& duongDan)
{
    std::ofstream f(duongDan);
    if (!f.is_open()) {
        std::cerr << "[!] Khong mo duoc file: " << duongDan << "\n";
        return;
    }

    // Header
    f << "Van";
    for (const auto& ten : tenNguoiChoi)
        f << "," << ten;
    f << "\n";

    // Dữ liệu
    for (int i = 0; i < static_cast<int>(lichSu.size()); ++i) {
        f << i * buocLuu;
        for (double v : lichSu[i])
            f << "," << std::fixed << std::setprecision(2) << v;
        f << "\n";
    }
}

// ─────────────────────────────────────────────────────────────
//  Vẽ biểu đồ ASCII trong terminal
// ─────────────────────────────────────────────────────────────
void HienThi::inBieuDoASCII(
    const std::vector<std::vector<double>>& lichSu,
    const std::vector<std::string>&         tenNguoiChoi,
    int buocLuu,
    int rong,
    int cao)
{
    if (lichSu.empty()) return;
    int soNguoi    = static_cast<int>(tenNguoiChoi.size());
    int soSnapshot = static_cast<int>(lichSu.size());

    // ── Tìm min/max tiền ─────────────────────────────────────
    double tienMin =  1e18, tienMax = -1e18;
    for (const auto& hang : lichSu)
        for (double v : hang) {
            tienMin = std::min(tienMin, v);
            tienMax = std::max(tienMax, v);
        }
    if (tienMax == tienMin) tienMax = tienMin + 1.0;

    // ── Tạo lưới cao x rong, điền ' ' ────────────────────────
    std::vector<std::string> luoi(cao, std::string(rong, ' '));

    auto toaDoX = [&](int snap) -> int {
        return static_cast<int>(
            std::round(static_cast<double>(snap) / (soSnapshot - 1) * (rong - 1))
        );
    };
    auto toaDoY = [&](double tien) -> int {
        int y = static_cast<int>(
            std::round((tien - tienMin) / (tienMax - tienMin) * (cao - 1))
        );
        return std::max(0, std::min(cao - 1, y));
    };

    // ── Vẽ đường cho từng người chơi ─────────────────────────
    for (int pi = 0; pi < soNguoi; ++pi) {
        char c = KY_TU_NGUOI_CHOI[pi % 6];
        for (int si = 0; si < soSnapshot; ++si) {
            int x   = toaDoX(si);
            int y   = toaDoY(lichSu[si][pi]);
            int hang = (cao - 1) - y;
            if (x >= 0 && x < rong && hang >= 0 && hang < cao)
                luoi[hang][x] = c;
        }
    }

    // ── In biểu đồ ───────────────────────────────────────────
    std::cout << "\n";
    std::cout << "=== BIEU DO SO TIEN THEO LUOT CHOI ===\n\n";

    for (int r = 0; r < cao; ++r) {
        double giaTriY = tienMax - (tienMax - tienMin) * r / (cao - 1);
        if (r == 0 || r == cao / 2 || r == cao - 1)
            std::cout << std::setw(10) << std::fixed << std::setprecision(0)
                      << giaTriY << " |";
        else
            std::cout << "           |";
        std::cout << luoi[r] << "\n";
    }

    // Trục X
    std::cout << "           +";
    for (int i = 0; i < rong; ++i) std::cout << "-";
    std::cout << "\n";
    std::cout << "           0";
    std::string giua = std::to_string((soSnapshot / 2) * buocLuu) + " van";
    std::string cuoi = std::to_string((soSnapshot - 1) * buocLuu) + " van";
    int viTriGiua = rong / 2 - static_cast<int>(giua.size()) / 2;
    std::string trucX(rong, ' ');
    for (int i = 0; i < static_cast<int>(giua.size()) && viTriGiua + i < rong; ++i)
        trucX[viTriGiua + i] = giua[i];
    for (int i = 0; i < static_cast<int>(cuoi.size()); ++i)
        trucX[rong - static_cast<int>(cuoi.size()) + i] = cuoi[i];
    std::cout << trucX << "\n\n";

    // Chú giải
    std::cout << "  CHU GIAI:\n";
    for (int pi = 0; pi < soNguoi; ++pi) {
        double tienCuoi = lichSu.back()[pi];
        std::cout << "    [" << KY_TU_NGUOI_CHOI[pi] << "] " << tenNguoiChoi[pi]
                  << " - " << std::fixed << std::setprecision(0)
                  << tienCuoi << " dong\n";
    }
    std::cout << "\n";
}

// ─────────────────────────────────────────────────────────────
//  Xuất file HTML với biểu đồ Chart.js
// ─────────────────────────────────────────────────────────────
void HienThi::xuatHTML(
    const std::vector<std::vector<double>>& lichSu,
    const std::vector<std::string>&         tenNguoiChoi,
    int buocLuu,
    const std::string& duongDan)
{
    std::ofstream f(duongDan);
    if (!f.is_open()) {
        std::cerr << "[!] Khong mo duoc file HTML: " << duongDan << "\n";
        return;
    }

    int soNguoi    = static_cast<int>(tenNguoiChoi.size());
    int soSnapshot = static_cast<int>(lichSu.size());

    // Màu biểu đồ cho từng người chơi
    const char* MAU[] = {
        "#FF6B6B",  // Đỏ – An
        "#4ECDC4",  // Xanh ngọc – Binh
        "#45B7D1",  // Xanh dương – Chi
        "#FFA07A",  // Cam – Dung
        "#98D8C8",  // Xanh lá nhạt – Em
        "#DDA0DD"   // Tím nhạt – Phong
    };

    // ── Xây dựng mảng nhãn (số ván) ──────────────────────────
    std::ostringstream nhanJS;
    nhanJS << "[";
    for (int i = 0; i < soSnapshot; ++i) {
        if (i > 0) nhanJS << ",";
        nhanJS << i * buocLuu;
    }
    nhanJS << "]";

    // Lấy snapshot mỗi N điểm để chart không quá nặng
    int buoc = std::max(1, soSnapshot / 500);

    // ── Xây dựng datasets JS ─────────────────────────────────
    std::ostringstream tapDuLieuJS;
    tapDuLieuJS << "[";
    for (int pi = 0; pi < soNguoi; ++pi) {
        if (pi > 0) tapDuLieuJS << ",\n";
        tapDuLieuJS << "{\n";
        tapDuLieuJS << "  label: '" << tenNguoiChoi[pi] << "',\n";
        tapDuLieuJS << "  borderColor: '" << MAU[pi % 6] << "',\n";
        tapDuLieuJS << "  backgroundColor: '" << MAU[pi % 6] << "22',\n";
        tapDuLieuJS << "  borderWidth: 2,\n";
        tapDuLieuJS << "  pointRadius: 0,\n";
        tapDuLieuJS << "  tension: 0.3,\n";
        tapDuLieuJS << "  fill: false,\n";
        tapDuLieuJS << "  data: [";

        bool dauTien = true;
        for (int si = 0; si < soSnapshot; si += buoc) {
            if (!dauTien) tapDuLieuJS << ",";
            tapDuLieuJS << std::fixed << std::setprecision(0) << lichSu[si][pi];
            dauTien = false;
        }
        tapDuLieuJS << "]\n}";
    }
    tapDuLieuJS << "]";

    // Nhãn đã lấy mẫu
    std::ostringstream nhanMauJS;
    nhanMauJS << "[";
    bool dauTien = true;
    for (int si = 0; si < soSnapshot; si += buoc) {
        if (!dauTien) nhanMauJS << ",";
        nhanMauJS << si * buocLuu;
        dauTien = false;
    }
    nhanMauJS << "]";

    // Bảng tổng kết
    std::ostringstream bangTongKet;
    for (int pi = 0; pi < soNguoi; ++pi) {
        double tienCuoi  = lichSu.back()[pi];
        double tienDau   = lichSu.front()[pi];
        double chenh     = tienCuoi - tienDau;
        std::string dau  = (chenh >= 0 ? "+" : "") +
            std::to_string(static_cast<long long>(chenh));
        bangTongKet
            << "<tr>"
            << "<td><span class='cham' style='background:" << MAU[pi%6] << "'></span>"
            << tenNguoiChoi[pi] << "</td>"
            << "<td style='color:" << MAU[pi%6] << ";font-weight:700'>"
            << std::fixed << std::setprecision(0) << tienCuoi << "</td>"
            << "<td style='color:" << (chenh >= 0 ? "#4ECDC4" : "#FF6B6B") << "'>"
            << dau << "</td>"
            << "</tr>\n";
    }

    // ── Ghi HTML ─────────────────────────────────────────────
    f << R"(<!DOCTYPE html>
<html lang="vi">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Mo phong Bai Lieng – Bieu do So tien</title>
<script src="https://cdn.jsdelivr.net/npm/chart.js@4.4.0/dist/chart.umd.min.js"></script>
<style>
  * { margin: 0; padding: 0; box-sizing: border-box; }
  body {
    background: #0f0f1a;
    color: #e0e0f0;
    font-family: 'Segoe UI', sans-serif;
    min-height: 100vh;
    padding: 24px;
  }
  h1 {
    text-align: center;
    font-size: 1.8rem;
    margin-bottom: 6px;
    background: linear-gradient(90deg, #FF6B6B, #4ECDC4, #DDA0DD);
    -webkit-background-clip: text;
    background-clip: text;
    -webkit-text-fill-color: transparent;
    color: transparent;
  }
  .phu-de {
    text-align: center;
    color: #888;
    font-size: 0.9rem;
    margin-bottom: 28px;
  }
  .the {
    background: #1a1a2e;
    border: 1px solid #2a2a4a;
    border-radius: 16px;
    padding: 24px;
    margin-bottom: 24px;
    box-shadow: 0 4px 24px rgba(0,0,0,0.4);
  }
  .the h2 {
    font-size: 1rem;
    color: #aaa;
    text-transform: uppercase;
    letter-spacing: 2px;
    margin-bottom: 20px;
  }
  .khung-bieu-do { position: relative; height: 420px; }
  table {
    width: 100%;
    border-collapse: collapse;
    font-size: 0.95rem;
  }
  th {
    background: #252540;
    padding: 10px 16px;
    text-align: left;
    color: #aaa;
    font-weight: 600;
    text-transform: uppercase;
    font-size: 0.78rem;
    letter-spacing: 1px;
  }
  td {
    padding: 10px 16px;
    border-bottom: 1px solid #2a2a4a;
  }
  tr:last-child td { border-bottom: none; }
  tr:hover td { background: #1f1f3a; }
  .cham {
    display: inline-block;
    width: 10px; height: 10px;
    border-radius: 50%;
    margin-right: 8px;
    vertical-align: middle;
  }
  .luoi { display: grid; grid-template-columns: 2fr 1fr; gap: 24px; }
  @media(max-width:700px){ .luoi{ grid-template-columns:1fr; } }
</style>
</head>
<body>
<h1>Khao Sat Bai Lieng – LTHDDT C++</h1>
<p class="phu-de">Mo phong 100,000 luot choi | 6 nguoi choi voi tam ly khac nhau</p>

<div class="the">
  <h2>Bieu Do So Tien Theo Luot Choi</h2>
  <div class="khung-bieu-do">
    <canvas id="bieuDo"></canvas>
  </div>
</div>

<div class="luoi">
  <div class="the">
    <h2>Bang Tong Ket</h2>
    <table>
      <thead>
        <tr>
          <th>Nguoi Choi</th>
          <th>Tien Cuoi (dong)</th>
          <th>Loi / Lo (dong)</th>
        </tr>
      </thead>
      <tbody>
)";

    f << bangTongKet.str();

    f << R"(      </tbody>
    </table>
  </div>

  <div class="the">
    <h2>Giai Thich Mo Hinh</h2>
    <p style="color:#aaa;line-height:1.7;font-size:0.9rem">
      <b style="color:#4ECDC4">Chi So Tiep Tuc</b> = Tam Ly &times; Ti Le Thang &times; 100
      <br><br>
      &gt; 500 &amp;&amp; bai &ge; Sanh Rac &rarr; <b style="color:#FF6B6B">To (nang cuoc)</b>
      <br>
      &le; 500, du tien &rarr; <b style="color:#45B7D1">Theo (goi)</b>
      <br>
      Con lai &rarr; <b style="color:#888">Bo bai</b>
      <br><br>
      <b style="color:#FFD700">Bai dac biet</b>: Sam Co / Lieng / Sanh Rong
      <br>
      &rarr; Luon To toi da 1000d (bo qua tam ly)
      <br><br>
      Thang: Tam Ly +1.0 &nbsp;|&nbsp; Thua: Tam Ly &minus;0.2
      <br>
      Gioi han Tam Ly: [1.0, 10.0]
    </p>
  </div>
</div>

<script>
const nhan = )";
    f << nhanMauJS.str() << ";\n";
    f << "const tapDuLieu = " << tapDuLieuJS.str() << ";\n";
    f << R"(
const ctx = document.getElementById('bieuDo').getContext('2d');
new Chart(ctx, {
  type: 'line',
  data: { labels: nhan, datasets: tapDuLieu },
  options: {
    responsive: true,
    maintainAspectRatio: false,
    interaction: { mode: 'index', intersect: false },
    plugins: {
      legend: {
        labels: { color: '#ccc', font: { size: 13 }, padding: 16 }
      },
      tooltip: {
        backgroundColor: '#1a1a2e',
        borderColor: '#333',
        borderWidth: 1,
        titleColor: '#fff',
        bodyColor: '#ccc',
        callbacks: {
          label: ctx => ctx.dataset.label + ': ' +
            ctx.parsed.y.toLocaleString('vi-VN') + ' dong'
        }
      }
    },
    scales: {
      x: {
        ticks: { color: '#666', maxTicksLimit: 10 },
        grid:  { color: '#1f1f3a' },
        title: { display: true, text: 'So luot choi', color: '#888' }
      },
      y: {
        ticks: {
          color: '#666',
          callback: v => v.toLocaleString('vi-VN') + ' d'
        },
        grid:  { color: '#1f1f3a' },
        title: { display: true, text: 'So tien (dong)', color: '#888' }
      }
    }
  }
});
</script>
</body>
</html>
)";

    std::cout << "[+] Da xuat bieu do HTML: " << duongDan << "\n";
}
