#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>

// --- OYUN AYARLARI ---
const int EKRAN_GENISLIK = 600;
const int EKRAN_YUKSEKLIK = 800;

// Uzay Arka Planı için Yıldız Yapısı
struct Yildiz {
    sf::Vector2f pozisyon;
    float hiz;
    sf::Color renk;
    int yanipSonmeZamanlayicisi;
};

// >>> YENİ: Patlama Parçacığı Yapısı <<<
struct PatlamaParcacigi {
    sf::Vector2f pozisyon;
    sf::Vector2f hiz;
    sf::Color renk;
    float omur;          // Parçacığın kalan ömrü (saniye cinsinden)
    float maksimumOmur;   // Başlangıç ömrü
};

// Mermi Sinifi
class Mermi {
public:
    sf::RectangleShape sekil;
    float hiz;
    bool oyuncuMermisi;

    Mermi(float x, float y, bool oyuncununMu) {
        oyuncuMermisi = oyuncununMu;
        // Oyuncu mermisi yukari (-), dusman mermisi asagi (+) gider
        hiz = oyuncuMermisi ? -7.0f : 4.0f;
        sekil.setSize(sf::Vector2f(4, 12));
        sekil.setFillColor(oyuncuMermisi ? sf::Color::Cyan : sf::Color::Red);
        sekil.setPosition(x, y);
    }

    void guncelle() {
        sekil.move(0, hiz);
    }
};

// Dusman Durumlari (Dalis mekanizmasi icin)
enum class DusmanDurumu { Formasyon, Dalista };

// Dusman Sinifi
class Dusman {
public:
    sf::ConvexShape kanatlar;
    sf::ConvexShape govde;
    sf::ConvexShape cekirdek;

    sf::Vector2f formasyonPozisyonu;
    sf::Vector2f aktifPozisyonu;
    DusmanDurumu durum;
    float dalisHizi;
    float sinusZamani;

    Dusman(float x, float y) {
        formasyonPozisyonu = sf::Vector2f(x, y);
        aktifPozisyonu = formasyonPozisyonu;
        durum = DusmanDurumu::Formasyon;
        dalisHizi = 3.5f;
        sinusZamani = static_cast<float>(std::rand() % 100);

        // Katman 1: Dış Kanat Yapısı (Mavi)
        kanatlar.setPointCount(6);
        kanatlar.setPoint(0, sf::Vector2f(15, 10));
        kanatlar.setPoint(1, sf::Vector2f(32, 5));
        kanatlar.setPoint(2, sf::Vector2f(26, 22));
        kanatlar.setPoint(3, sf::Vector2f(15, 16));
        kanatlar.setPoint(4, sf::Vector2f(6, 22));
        kanatlar.setPoint(5, sf::Vector2f(0, 5));
        kanatlar.setFillColor(sf::Color(40, 110, 255));
        kanatlar.setOrigin(15, 15);
        kanatlar.setPosition(aktifPozisyonu);

        // Katman 2: Ana İç Gövde 
        govde.setPointCount(4);
        govde.setPoint(0, sf::Vector2f(15, 2));
        govde.setPoint(1, sf::Vector2f(22, 14));
        govde.setPoint(2, sf::Vector2f(15, 28));
        govde.setPoint(3, sf::Vector2f(8, 14));
        govde.setFillColor(sf::Color(255, 190, 0));
        govde.setOrigin(15, 15);
        govde.setPosition(aktifPozisyonu);

        // Katman 3: Kokpit 
        cekirdek.setPointCount(4);
        cekirdek.setPoint(0, sf::Vector2f(15, 8));
        cekirdek.setPoint(1, sf::Vector2f(18, 14));
        cekirdek.setPoint(2, sf::Vector2f(15, 18));
        cekirdek.setPoint(3, sf::Vector2f(12, 14));
        cekirdek.setFillColor(sf::Color(255, 40, 40));
        cekirdek.setOrigin(15, 15);
        cekirdek.setPosition(aktifPozisyonu);
    }

    void guncelle(float formasyonOfsetX) {
        if (durum == DusmanDurumu::Formasyon) {
            aktifPozisyonu.x = formasyonPozisyonu.x + formasyonOfsetX;
        }
        else if (durum == DusmanDurumu::Dalista) {
            aktifPozisyonu.y += dalisHizi;
            sinusZamani += 0.05f;
            aktifPozisyonu.x += std::sin(sinusZamani) * 2.0f;

            if (aktifPozisyonu.y > EKRAN_YUKSEKLIK + 20) {
                aktifPozisyonu.y = formasyonPozisyonu.y;
                durum = DusmanDurumu::Formasyon;
            }
        }

        kanatlar.setPosition(aktifPozisyonu);
        govde.setPosition(aktifPozisyonu);
        cekirdek.setPosition(aktifPozisyonu);
    }
};

// Oyuncu (Dost) Sınıfı
class Oyuncu {
public:
    sf::ConvexShape kanatlar;
    sf::ConvexShape govde;
    sf::ConvexShape kokpit;

    float hiz;
    int can;
    sf::Vector2f pozisyon;

    Oyuncu() {
        hiz = 5.5f;
        can = 3;
        pozisyon = sf::Vector2f(EKRAN_GENISLIK / 2.0f, EKRAN_YUKSEKLIK - 60);

        // Katman 1: Geniş Dış Kanatlar ve Zırh (Açık Gümüş/Beyaz)
        kanatlar.setPointCount(5);
        kanatlar.setPoint(0, sf::Vector2f(20, 0));
        kanatlar.setPoint(1, sf::Vector2f(44, 38));
        kanatlar.setPoint(2, sf::Vector2f(29, 30));
        kanatlar.setPoint(3, sf::Vector2f(11, 30));
        kanatlar.setPoint(4, sf::Vector2f(-4, 38));
        kanatlar.setFillColor(sf::Color(230, 235, 245));
        kanatlar.setOrigin(20, 19);
        kanatlar.setPosition(pozisyon);

        // Katman 2: Agresif Ana Gövde Yapısı (Sarı/Turuncu)
        govde.setPointCount(4);
        govde.setPoint(0, sf::Vector2f(20, 4));
        govde.setPoint(1, sf::Vector2f(32, 28));
        govde.setPoint(2, sf::Vector2f(20, 22));
        govde.setPoint(3, sf::Vector2f(8, 28));
        govde.setFillColor(sf::Color(255, 175, 0));
        govde.setOrigin(20, 19);
        govde.setPosition(pozisyon);

        // Katman 3: Enerji Çekirdeği / Cam Kokpit (Neon Kırmızı)
        kokpit.setPointCount(4);
        kokpit.setPoint(0, sf::Vector2f(20, 10));
        kokpit.setPoint(1, sf::Vector2f(24, 18));
        kokpit.setPoint(2, sf::Vector2f(20, 21));
        kokpit.setPoint(3, sf::Vector2f(16, 18));
        kokpit.setFillColor(sf::Color(240, 20, 20));
        kokpit.setOrigin(20, 19);
        kokpit.setPosition(pozisyon);
    }

    void solaHareketEt() {
        if (pozisyon.x > 25) {
            pozisyon.x -= hiz;
            senkronizeEt();
        }
    }

    void sagaHareketEt() {
        if (pozisyon.x < EKRAN_GENISLIK - 25) {
            pozisyon.x += hiz;
            senkronizeEt();
        }
    }

private:
    void senkronizeEt() {
        kanatlar.setPosition(pozisyon);
        govde.setPosition(pozisyon);
        kokpit.setPosition(pozisyon);
    }
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(EKRAN_GENISLIK, EKRAN_YUKSEKLIK), "Galaga Clone");
    window.setFramerateLimit(60);

    // Rastgele Özelliklerde Yıldız Havuzu Oluşturma
    std::vector<Yildiz> yildizlar;
    for (int i = 0; i < 80; ++i) {
        Yildiz y;
        y.pozisyon = sf::Vector2f(std::rand() % EKRAN_GENISLIK, std::rand() % EKRAN_YUKSEKLIK);
        y.hiz = 0.5f + (std::rand() % 20) / 10.0f;
        y.yanipSonmeZamanlayicisi = std::rand() % 60;

        int renkSecimi = std::rand() % 4;
        if (renkSecimi == 0) y.renk = sf::Color::White;
        else if (renkSecimi == 1) y.renk = sf::Color(255, 255, 140);
        else if (renkSecimi == 2) y.renk = sf::Color(140, 200, 255);
        else y.renk = sf::Color(255, 130, 130);

        yildizlar.push_back(y);
    }

    // Font yukleme islemi 
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Hata yonetimi
    }

    // Arayuz Metinleri
    sf::Text txtSkor;
    txtSkor.setFont(font);
    txtSkor.setCharacterSize(18);
    txtSkor.setFillColor(sf::Color::Green);
    txtSkor.setPosition(10, 10);

    sf::Text txtCan;
    txtCan.setFont(font);
    txtCan.setCharacterSize(18);
    txtCan.setFillColor(sf::Color::Yellow);
    txtCan.setPosition(EKRAN_GENISLIK - 100, 10);

    sf::Text txtGameOver;
    txtGameOver.setFont(font);
    txtGameOver.setCharacterSize(45);
    txtGameOver.setFillColor(sf::Color::White);
    txtGameOver.setString("GAME OVER");

    sf::FloatRect metinBoyutu = txtGameOver.getLocalBounds();
    txtGameOver.setOrigin(metinBoyutu.left + metinBoyutu.width / 2.0f, metinBoyutu.top + metinBoyutu.height / 2.0f);
    txtGameOver.setPosition(sf::Vector2f(EKRAN_GENISLIK / 2.0f, EKRAN_YUKSEKLIK / 2.0f));

    // Oyun Dongusu Elemanlari
    Oyuncu oyuncu;
    std::vector<Mermi> mermiler;
    std::vector<Dusman> dusmanlar;
    // >>> YENİ: Aktif patlama parçacıklarını tutacak vektör <<<
    std::vector<PatlamaParcacigi> parcaciklar;

    int mevcutSkor = 0;
    int enYuksekSkor = 236;
    bool oyunBitti = false;

    // Dusman Formasyonunun Olusturulmasi (6 sutun x 4 satir grid yapisi)
    for (int satir = 0; satir < 4; ++satir) {
        for (int sutun = 0; sutun < 6; ++sutun) {
            float xPoz = 100.0f + sutun * 70.0f;
            float yPoz = 100.0f + satir * 50.0f;
            dusmanlar.push_back(Dusman(xPoz, yPoz));
        }
    }

    sf::Clock oyunSaati;
    float atisGecikmesi = 0.0f;
    float dusmanAtisZamani = 0.0f;
    float dalisZamanlayici = 0.0f;

    // --- ANA OYUN DONGUSU ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = 1.0f / 60.0f;
        float toplamZaman = oyunSaati.getElapsedTime().asSeconds();

        // Yıldız Pozisyonlarını Güncelleme ve Kırpıştırma Sistemi
        for (auto& y : yildizlar) {
            y.pozisyon.y += y.hiz;
            if (y.pozisyon.y > EKRAN_YUKSEKLIK) {
                y.pozisyon.y = 0;
                y.pozisyon.x = std::rand() % EKRAN_GENISLIK;
            }
            y.yanipSonmeZamanlayicisi++;
            if (y.yanipSonmeZamanlayicisi % 60 < 15) {
                y.renk.a = 70;
            }
            else {
                y.renk.a = 255;
            }
        }

        // >>> YENİ: Patlama Parçacıklarını Güncelleme ve Ömrü Bitenleri Silme <<<
        for (auto it = parcaciklar.begin(); it != parcaciklar.end();) {
            it->pozisyon += it->hiz;
            it->omur -= dt;

            // Parçacık eskidikçe opaklığını (Alpha değerini) düşürerek sönme efekti veriyoruz
            float oran = it->omur / it->maksimumOmur;
            if (oran < 0) oran = 0;
            it->renk.a = static_cast<sf::Uint8>(oran * 255);

            if (it->omur <= 0) {
                it = parcaciklar.erase(it);
            }
            else {
                ++it;
            }
        }

        if (!oyunBitti) {
            // --- KLAVYE KONTROLLERİ ---
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                oyuncu.solaHareketEt();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                oyuncu.sagaHareketEt();
            }

            // Coklu Atis Mekanizmasi
            if (atisGecikmesi > 0) atisGecikmesi -= dt;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && atisGecikmesi <= 0) {
                mermiler.push_back(Mermi(oyuncu.pozisyon.x, oyuncu.pozisyon.y - 20, true));
                atisGecikmesi = 0.25f;
            }

            // --- GUNCELLEMELER ---
            float formasyonOfsetX = std::sin(toplamZaman * 2.0f) * 40.0f;

            // Belirli araliklarla rastgele bir dusmanin dalisa gecmesi
            dalisZamanlayici += dt;
            if (dalisZamanlayici > 3.0f && !dusmanlar.empty()) {
                int rastgeleIndeks = std::rand() % dusmanlar.size();
                if (dusmanlar[rastgeleIndeks].durum == DusmanDurumu::Formasyon) {
                    dusmanlar[rastgeleIndeks].durum = DusmanDurumu::Dalista;
                }
                dalisZamanlayici = 0.0f;
            }

            // Dusmanlarin belirli periyotlarla atis yapmasi
            dusmanAtisZamani += dt;
            if (dusmanAtisZamani > 1.2f && !dusmanlar.empty()) {
                int atesEdenDusman = std::rand() % dusmanlar.size();
                mermiler.push_back(Mermi(dusmanlar[atesEdenDusman].aktifPozisyonu.x, dusmanlar[atesEdenDusman].aktifPozisyonu.y + 15, false));
                dusmanAtisZamani = 0.0f;
            }

            // Dusman pozisyonlarinin guncellenmesi
            for (size_t i = 0; i < dusmanlar.size(); ++i) {
                dusmanlar[i].guncelle(formasyonOfsetX);

                if (dusmanlar[i].durum == DusmanDurumu::Dalista) {
                    if (dusmanlar[i].kanatlar.getGlobalBounds().intersects(oyuncu.kanatlar.getGlobalBounds())) {

                        // >>> YENİ: Oyuncuya çarpınca da düşman konumunda patlama oluştur <<<
                        for (int p = 0; p < 15; ++p) {
                            PatlamaParcacigi par;
                            par.pozisyon = dusmanlar[i].aktifPozisyonu;
                            float aci = static_cast<float>(std::rand() % 360) * 3.14159f / 180.0f;
                            float h = 1.0f + (std::rand() % 30) / 10.0f;
                            par.hiz = sf::Vector2f(std::cos(aci) * h, std::sin(aci) * h);
                            par.renk = (std::rand() % 2 == 0) ? sf::Color(255, 100, 0) : sf::Color(255, 200, 0);
                            par.maksimumOmur = 0.4f + (std::rand() % 40) / 100.0f;
                            par.omur = par.maksimumOmur;
                            parcaciklar.push_back(par);
                        }

                        oyuncu.can--;
                        dusmanlar[i].aktifPozisyonu.y = dusmanlar[i].formasyonPozisyonu.y;
                        dusmanlar[i].durum = DusmanDurumu::Formasyon;
                        if (oyuncu.can <= 0) {
                            oyunBitti = true;
                        }
                    }
                }
            }

            // Mermilerin guncellenmesi
            for (auto it = mermiler.begin(); it != mermiler.end();) {
                it->guncelle();
                if (it->sekil.getPosition().y < 0 || it->sekil.getPosition().y > EKRAN_YUKSEKLIK) {
                    it = mermiler.erase(it);
                }
                else {
                    ++it;
                }
            }

            // --- CARPISMA TESTLERI (COLLISION DETECTION) ---
            for (auto mermiIt = mermiler.begin(); mermiIt != mermiler.end();) {
                bool mermiSilindi = false;

                if (mermiIt->oyuncuMermisi) {
                    // Oyuncu mermisinin dusmana carpma durumu
                    for (auto dusmanIt = dusmanlar.begin(); dusmanIt != dusmanlar.end();) {
                        if (mermiIt->sekil.getGlobalBounds().intersects(dusmanIt->kanatlar.getGlobalBounds())) {

                            // >>> YENİ: Düşman vurulduğunda patlama parçacıkları üretme mekanizması <<<
                            sf::Vector2f patlamaMerkezi = dusmanIt->aktifPozisyonu;
                            for (int p = 0; p < 25; ++p) { // 25 adet kıvılcım parçacığı fırlatılır
                                PatlamaParcacigi par;
                                par.pozisyon = patlamaMerkezi;

                                // 360 derece rastgele bir açı hesaplama
                                float aci = static_cast<float>(std::rand() % 360) * 3.14159f / 180.0f;
                                float h = 1.5f + (std::rand() % 40) / 10.0f; // Rastgele saçılma hızı
                                par.hiz = sf::Vector2f(std::cos(aci) * h, std::sin(aci) * h);

                                // Düşman renk paletine uygun ateş/patlama renkleri (Turuncu, Sarı, Kırmızı kıvılcımlar)
                                int renkSec = std::rand() % 3;
                                if (renkSec == 0) par.renk = sf::Color(255, 69, 0);       // Turuncu-Kırmızı
                                else if (renkSec == 1) par.renk = sf::Color(255, 165, 0);  // Turuncu
                                else par.renk = sf::Color(255, 215, 0);                   // Altın Sarısı

                                par.maksimumOmur = 0.3f + (std::rand() % 50) / 100.0f; // 0.3 - 0.8 saniye arası ömür
                                par.omur = par.maksimumOmur;

                                parcaciklar.push_back(par);
                            }

                            mevcutSkor += 10;
                            if (mevcutSkor > enYuksekSkor) {
                                enYuksekSkor = mevcutSkor;
                            }
                            dusmanIt = dusmanlar.erase(dusmanIt);
                            mermiIt = mermiler.erase(mermiIt);
                            mermiSilindi = true;
                            break;
                        }
                        else {
                            ++dusmanIt;
                        }
                    }
                }
                else {
                    if (mermiIt->sekil.getGlobalBounds().intersects(oyuncu.kanatlar.getGlobalBounds())) {
                        oyuncu.can--;
                        mermiIt = mermiler.erase(mermiIt);
                        mermiSilindi = true;
                        if (oyuncu.can <= 0) {
                            oyunBitti = true;
                        }
                    }
                }

                if (!mermiSilindi) {
                    ++mermiIt;
                }
            }

            // Tum dusmanlar vurulduysa yeni bir dalga (wave) olusturma
            if (dusmanlar.empty()) {
                for (int satir = 0; satir < 4; ++satir) {
                    for (int sutun = 0; sutun < 6; ++sutun) {
                        dusmanlar.push_back(Dusman(100.0f + sutun * 70.0f, 100.0f + satir * 50.0f));
                    }
                }
            }
        }

        // Metin iceriklerinin anlik guncellenmesi
        txtSkor.setString("SCORE: " + std::to_string(mevcutSkor) + "  high score=" + std::to_string(enYuksekSkor));
        txtCan.setString("LIVES: " + std::to_string(oyuncu.can > 0 ? oyuncu.can : 0));

        // --- EKRANA CIZIM ASAMASI ---
        window.clear(sf::Color(10, 10, 25)); // Koyu Uzay Arka Planı

        // Yıldızları Çiz
        for (const auto& y : yildizlar) {
            sf::RectangleShape yildizSekil(sf::Vector2f(2.0f, 2.0f));
            yildizSekil.setPosition(y.pozisyon);
            yildizSekil.setFillColor(y.renk);
            window.draw(yildizSekil);
        }

        // >>> YENİ: Patlama Parçacıklarını Ekrana Çiz <<<
        for (const auto& par : parcaciklar) {
            sf::RectangleShape parSekil(sf::Vector2f(3.0f, 3.0f)); // Küçük kare kıvılcımlar
            parSekil.setPosition(par.pozisyon);
            parSekil.setFillColor(par.renk);
            window.draw(parSekil);
        }

        if (!oyunBitti) {
            // Oyuncu gemisinin katmanlarını çiz
            window.draw(oyuncu.kanatlar);
            window.draw(oyuncu.govde);
            window.draw(oyuncu.kokpit);

            // Düşman gemilerini çiz
            for (const auto& d : dusmanlar) {
                window.draw(d.kanatlar);
                window.draw(d.govde);
                window.draw(d.cekirdek);
            }

            // Mermileri çiz
            for (const auto& m : mermiler) {
                window.draw(m.sekil);
            }
        }
        else {
            window.draw(txtGameOver);
        }

        // Arayuz elemanlarini ciz
        window.draw(txtSkor);
        window.draw(txtCan);

        window.display();
    }

    return 0;
}