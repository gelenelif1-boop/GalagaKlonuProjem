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

// Patlama Parçacığı Yapısı
struct PatlamaParcacigi {
    sf::Vector2f pozisyon;
    sf::Vector2f hiz;
    sf::Color renk;
    float omur;          // Parçacığın kalan ömrü (saniye cinsinden)
    float maksimumOmur;   // Başlangıç ömrü
    float boyut;         // >>> YENİ: Game Over patlaması için değişken boyut desteği <<<
};

// Lazer İzi Parçacığı Yapısı
struct LazerIzi {
    sf::Vector2f pozisyon;
    sf::Color renk;
    float omur;
    float maksimumOmur;
};

// Mermi Sinifi
class Mermi {
public:
    sf::RectangleShape sekil;
    float hiz;
    bool oyuncuMermisi;

    std::vector<LazerIzi> izler;
    float izBrakmaZamanlayicisi;

    Mermi(float x, float y, bool oyuncununMu) {
        oyuncuMermisi = oyuncununMu;
        hiz = oyuncuMermisi ? -7.0f : 4.0f;
        sekil.setSize(sf::Vector2f(4, 12));
        sekil.setFillColor(oyuncuMermisi ? sf::Color::Cyan : sf::Color::Red);
        sekil.setPosition(x, y);
        izBrakmaZamanlayicisi = 0.0f;
    }

    void guncelle(float dt) {
        sekil.move(0, hiz);

        izBrakmaZamanlayicisi += dt;
        if (izBrakmaZamanlayicisi >= 0.02f) {
            LazerIzi yeniIz;
            float ofsetY = oyuncuMermisi ? sekil.getSize().y : 0.0f;
            yeniIz.pozisyon = sf::Vector2f(sekil.getPosition().x + sekil.getSize().x / 2.0f, sekil.getPosition().y + ofsetY);

            yeniIz.renk = oyuncuMermisi ? sf::Color(0, 255, 255) : sf::Color(255, 0, 0);
            yeniIz.maksimumOmur = 0.15f;
            yeniIz.omur = yeniIz.maksimumOmur;

            izler.push_back(yeniIz);
            izBrakmaZamanlayicisi = 0.0f;
        }

        for (auto it = izler.begin(); it != izler.end();) {
            it->omur -= dt;

            float oran = it->omur / it->maksimumOmur;
            if (oran < 0) oran = 0;
            it->renk.a = static_cast<sf::Uint8>(oran * 255);

            if (it->omur <= 0) {
                it = izler.erase(it);
            }
            else {
                ++it;
            }
        }
    }
};

// Dusman Durumlari
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

        govde.setPointCount(4);
        govde.setPoint(0, sf::Vector2f(15, 2));
        govde.setPoint(1, sf::Vector2f(22, 14));
        govde.setPoint(2, sf::Vector2f(15, 28));
        govde.setPoint(3, sf::Vector2f(8, 14));
        govde.setFillColor(sf::Color(255, 190, 0));
        govde.setOrigin(15, 15);
        govde.setPosition(aktifPozisyonu);

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

// Oyuncu Sınıfı
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

        kanatlar.setPointCount(5);
        kanatlar.setPoint(0, sf::Vector2f(20, 0));
        kanatlar.setPoint(1, sf::Vector2f(44, 38));
        kanatlar.setPoint(2, sf::Vector2f(29, 30));
        kanatlar.setPoint(3, sf::Vector2f(11, 30));
        kanatlar.setPoint(4, sf::Vector2f(-4, 38));
        kanatlar.setFillColor(sf::Color(230, 235, 245));
        kanatlar.setOrigin(20, 19);
        kanatlar.setPosition(pozisyon);

        govde.setPointCount(4);
        govde.setPoint(0, sf::Vector2f(20, 4));
        govde.setPoint(1, sf::Vector2f(32, 28));
        govde.setPoint(2, sf::Vector2f(20, 22));
        govde.setPoint(3, sf::Vector2f(8, 28));
        govde.setFillColor(sf::Color(255, 175, 0));
        govde.setOrigin(20, 19);
        govde.setPosition(pozisyon);

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

    // Yıldız Havuzu Oluşturma
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
    std::vector<PatlamaParcacigi> parcaciklar;

    int mevcutSkor = 0;
    int enYuksekSkor = 236;
    bool oyunBitti = false;
    // >>> YENİ: Büyük patlamanın sadece bir kez tetiklenmesini sağlayan bayrak <<<
    bool büyükPatlamaTetiklendi = false;

    // Dusman Formasyonunun Olusturulmasi
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

        // Yıldız Pozisyonlarını Güncelleme
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

        // Patlama Parçacıklarını Güncelleme ve Ömrü Bitenleri Silme
        for (auto it = parcaciklar.begin(); it != parcaciklar.end();) {
            it->pozisyon += it->hiz;
            it->omur -= dt;

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

        // >>> YENİ: Game Over durumunda Büyük Ekran Patlaması Üretimi <<<
        if (oyunBitti && !büyükPatlamaTetiklendi) {
            // Patlamayı oyuncunun son elendiği noktadan başlatıyoruz
            sf::Vector2f merkez = oyuncu.pozisyon;

            // Ekranı kaplayacak kadar çok (250 adet) ve agresif parçacık saçıyoruz
            for (int p = 0; p < 250; ++p) {
                PatlamaParcacigi par;
                par.pozisyon = merkez;

                // Tam 360 derece rastgele saçılma açısı
                float aci = static_cast<float>(std::rand() % 360) * 3.14159f / 180.0f;
                // Ekranın tamamına yayılabilmeleri için yüksek hız çarpanı (3.0f - 12.0f)
                float h = 3.00f + (std::rand() % 90) / 10.0f;
                par.hiz = sf::Vector2f(std::cos(aci) * h, std::sin(aci) * h);

                // Devasa etki hissi için parçacık boyutları da değişken (4px ile 9px arası büyük şarapneller)
                par.boyut = 4.0f + (std::rand() % 6);

                // Yangın ve büyük infilak renk paleti (Kırmızı, Turuncu, Parlak Beyaz, Sarı)
                int renkSec = std::rand() % 4;
                if (renkSec == 0) par.renk = sf::Color(255, 40, 0);     // Canlı Kırmızı
                else if (renkSec == 1) par.renk = sf::Color(255, 130, 0);   // Yoğun Turuncu
                else if (renkSec == 2) par.renk = sf::Color(255, 230, 50);   // Parlak Sarı
                else par.renk = sf::Color(255, 255, 255);                    // Kor Beyaz

                // Ekranı kaplarken yavaş yavaş sönmeleri için daha uzun bir ömür (0.6 - 1.8 saniye)
                par.maksimumOmur = 0.6f + (std::rand() % 120) / 100.0f;
                par.omur = par.maksimumOmur;

                parcaciklar.push_back(par);
            }
            büyükPatlamaTetiklendi = true; // Patlamanın sonsuz döngüye girmesini engelle
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

                        for (int p = 0; p < 15; ++p) {
                            PatlamaParcacigi par;
                            par.pozisyon = dusmanlar[i].aktifPozisyonu;
                            float aci = static_cast<float>(std::rand() % 360) * 3.14159f / 180.0f;
                            float h = 1.0f + (std::rand() % 30) / 10.0f;
                            par.hiz = sf::Vector2f(std::cos(aci) * h, std::sin(aci) * h);
                            par.renk = (std::rand() % 2 == 0) ? sf::Color(255, 100, 0) : sf::Color(255, 200, 0);
                            par.maksimumOmur = 0.4f + (std::rand() % 40) / 100.0f;
                            par.omur = par.maksimumOmur;
                            par.boyut = 3.0f; // Normal düşman patlama boyutu
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
                it->guncelle(dt);
                if (it->sekil.getPosition().y < -50 || it->sekil.getPosition().y > EKRAN_YUKSEKLIK + 50) {
                    it = mermiler.erase(it);
                }
                else {
                    ++it;
                }
            }

            // --- CARPISMA TESTLERI ---
            for (auto mermiIt = mermiler.begin(); mermiIt != mermiler.end();) {
                bool mermiSilindi = false;

                if (mermiIt->oyuncuMermisi) {
                    for (auto dusmanIt = dusmanlar.begin(); dusmanIt != dusmanlar.end();) {
                        if (mermiIt->sekil.getGlobalBounds().intersects(dusmanIt->kanatlar.getGlobalBounds())) {

                            sf::Vector2f patlamaMerkezi = dusmanIt->aktifPozisyonu;
                            for (int p = 0; p < 25; ++p) {
                                PatlamaParcacigi par;
                                par.pozisyon = patlamaMerkezi;

                                float aci = static_cast<float>(std::rand() % 360) * 3.14159f / 180.0f;
                                float h = 1.5f + (std::rand() % 40) / 10.0f;
                                par.hiz = sf::Vector2f(std::cos(aci) * h, std::sin(aci) * h);

                                int renkSec = std::rand() % 3;
                                if (renkSec == 0) par.renk = sf::Color(255, 69, 0);
                                else if (renkSec == 1) par.renk = sf::Color(255, 165, 0);
                                else par.renk = sf::Color(255, 215, 0);

                                par.maksimumOmur = 0.3f + (std::rand() % 50) / 100.0f;
                                par.omur = par.maksimumOmur;
                                par.boyut = 3.0f;

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

            if (dusmanlar.empty()) {
                for (int satir = 0; satir < 4; ++satir) {
                    for (int sutun = 0; sutun < 6; ++sutun) {
                        dusmanlar.push_back(Dusman(100.0f + sutun * 70.0f, 100.0f + satir * 50.0f));
                    }
                }
            }
        }

        txtSkor.setString("SCORE: " + std::to_string(mevcutSkor) + "  high score=" + std::to_string(enYuksekSkor));
        txtCan.setString("LIVES: " + std::to_string(oyuncu.can > 0 ? oyuncu.can : 0));

        // --- EKRANA CIZIM ASAMASI ---
        window.clear(sf::Color(10, 10, 25));

        // Yıldızları Çiz
        for (const auto& y : yildizlar) {
            sf::RectangleShape yildizSekil(sf::Vector2f(2.0f, 2.0f));
            yildizSekil.setPosition(y.pozisyon);
            yildizSekil.setFillColor(y.renk);
            window.draw(yildizSekil);
        }

        // >>> GÜNCELLENDİ: Patlama Parçacıklarını kendi özel boyutuyla (par.boyut) çiziyoruz <<<
        for (const auto& par : parcaciklar) {
            sf::RectangleShape parSekil(sf::Vector2f(par.boyut, par.boyut));
            parSekil.setPosition(par.pozisyon);
            parSekil.setFillColor(par.renk);
            window.draw(parSekil);
        }

        // Mermilerin Lazer İzlerini Çiz
        for (const auto& mermi : mermiler) {
            for (const auto& iz : mermi.izler) {
                sf::RectangleShape izSekil(sf::Vector2f(4.0f, 4.0f));
                izSekil.setOrigin(2.0f, 2.0f);
                izSekil.setPosition(iz.pozisyon);
                izSekil.setFillColor(iz.renk);
                window.draw(izSekil);
            }
        }

        if (!oyunBitti) {
            window.draw(oyuncu.kanatlar);
            window.draw(oyuncu.govde);
            window.draw(oyuncu.kokpit);

            for (const auto& d : dusmanlar) {
                window.draw(d.kanatlar);
                window.draw(d.govde);
                window.draw(d.cekirdek);
            }

            for (const auto& m : mermiler) {
                window.draw(m.sekil);
            }
        }
        else {
            // >>> DEĞİŞTİRİLDİ: Oyun bitse de parçacıkların arka planda uçuşmaya devam etmesi için yazı ile beraber çiziliyor <<<
            window.draw(txtGameOver);
        }

        window.draw(txtSkor);
        window.draw(txtCan);

        window.display();
    }

    return 0;
}