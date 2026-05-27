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
        sekil.setFillColor(oyuncuMermisi ? sf::Color::Blue : sf::Color::Red);
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
    // >>> DEĞİŞİKLİK: Düşman gemisinin 3boyutluya çevrilmesi <<<
    sf::ConvexShape kanatlar;
    sf::ConvexShape govde;
    sf::ConvexShape cekirdek;
    // >>> DEĞİŞİKLİK SONU <<<

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

        // >>> DEĞİŞİKLİK: Düşman gemisi tasarım değişiklği <<<
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
        // >>> DEĞİŞİKLİK SONU <<<
    }

    void guncelle(float formasyonOfsetX) {
        if (durum == DusmanDurumu::Formasyon) {
            // Salınım hareketi 
            aktifPozisyonu.x = formasyonPozisyonu.x + formasyonOfsetX;
        }
        else if (durum == DusmanDurumu::Dalista) {
            // Dalisa gecen dusman asagi inerken hafifce yalpalar
            aktifPozisyonu.y += dalisHizi;
            sinusZamani += 0.05f;
            aktifPozisyonu.x += std::sin(sinusZamani) * 2.0f;

            // Ekranin altindan cikan dusman tekrar uste, formasyona doner
            if (aktifPozisyonu.y > EKRAN_YUKSEKLIK + 20) {
                aktifPozisyonu.y = formasyonPozisyonu.y;
                durum = DusmanDurumu::Formasyon;
            }
        }

        // >>> DEĞİŞİKLİK: Hareket eden geminin tüm katmanlarını senkronize şekilde taşı <<<
        kanatlar.setPosition(aktifPozisyonu);
        govde.setPosition(aktifPozisyonu);
        cekirdek.setPosition(aktifPozisyonu);
        // >>> DEĞİŞİKLİK SONU <<<
    }
};

// Oyuncu Sinifi
class Oyuncu {
public:
    sf::ConvexShape sekil;
    float hiz;
    int can;

    Oyuncu() {
        hiz = 5.5f;
        can = 3;

        // Klasik uzay gemisi sekli tasarimi
        sekil.setPointCount(4);
        sekil.setPoint(0, sf::Vector2f(20, 0));
        sekil.setPoint(1, sf::Vector2f(40, 35));
        sekil.setPoint(2, sf::Vector2f(20, 25));
        sekil.setPoint(3, sf::Vector2f(0, 35));
        sekil.setFillColor(sf::Color::White);
        sekil.setOrigin(20, 17);
        sekil.setPosition(EKRAN_GENISLIK / 2.0f, EKRAN_YUKSEKLIK - 60);
    }

    void solaHareketEt() {
        if (sekil.getPosition().x > 20) {
            sekil.move(-hiz, 0);
        }
    }

    void sagaHareketEt() {
        if (sekil.getPosition().x < EKRAN_GENISLIK - 20) {
            sekil.move(hiz, 0);
        }
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

    // Game Over yazisini ekrana tam ortalama hesabi
    sf::FloatRect metinBoyutu = txtGameOver.getLocalBounds();
    txtGameOver.setOrigin(metinBoyutu.left + metinBoyutu.width / 2.0f, metinBoyutu.top + metinBoyutu.height / 2.0f);
    txtGameOver.setPosition(sf::Vector2f(EKRAN_GENISLIK / 2.0f, EKRAN_YUKSEKLIK / 2.0f));

    // Oyun Dongusu Elemanlari
    Oyuncu oyuncu;
    std::vector<Mermi> mermiler;
    std::vector<Dusman> dusmanlar;

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

        // Zaman takibi 
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

        if (!oyunBitti) {
            // --- KLAVYE KONTROLLERİ ---
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                oyuncu.solaHareketEt();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                oyuncu.sagaHareketEt();
            }

            // Coklu Atis Mekanizmasi (Gecikmeli seri atis)
            if (atisGecikmesi > 0) atisGecikmesi -= dt;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && atisGecikmesi <= 0) {
                mermiler.push_back(Mermi(oyuncu.sekil.getPosition().x, oyuncu.sekil.getPosition().y - 20, true));
                atisGecikmesi = 0.25f;
            }

            // --- GUNCELLEMELER ---
            // Formasyonun ortak salinim hareketi icin ofset hesabi
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
                // >>> DEĞİŞİKLİK: Eski .sekil nesnesi yerine .aktifPozisyonu referans alındı <<<
                mermiler.push_back(Mermi(dusmanlar[atesEdenDusman].aktifPozisyonu.x, dusmanlar[atesEdenDusman].aktifPozisyonu.y + 15, false));
                // >>> DEĞİŞİKLİK SONU <<<
                dusmanAtisZamani = 0.0f;
            }

            // Dusman pozisyonlarinin guncellenmesi
            for (size_t i = 0; i < dusmanlar.size(); ++i) {
                dusmanlar[i].guncelle(formasyonOfsetX);

                // Dalistaki dusman gemisinin oyuncuya dogrudan carpma kontrolu
                if (dusmanlar[i].durum == DusmanDurumu::Dalista) {
                    // >>> DEĞİŞİKLİK: Çarpışma kutusu en geniş katman olan kanatlar üzerinden hesaplanır <<<
                    if (dusmanlar[i].kanatlar.getGlobalBounds().intersects(oyuncu.sekil.getGlobalBounds())) {
                        // >>> DEĞİŞİKLİK SONU <<<
                        oyuncu.can--;
                        dusmanlar[i].aktifPozisyonu.y = dusmanlar[i].formasyonPozisyonu.y;
                        dusmanlar[i].durum = DusmanDurumu::Formasyon;
                        if (oyuncu.can <= 0) {
                            oyunBitti = true;
                        }
                    }
                }
            }

            // Mermilerin guncellenmesi ve ekran disina cikanlarin silinmesi
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
                        // >>> DEĞİŞİKLİK: Oyuncu mermisinin düşman kanatlarına çarpma kontrolü <<<
                        if (mermiIt->sekil.getGlobalBounds().intersects(dusmanIt->kanatlar.getGlobalBounds())) {
                            // >>> DEĞİŞİKLİK SONU <<<
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
                    // Dusman mermisinin oyuncuya carpma durumu
                    if (mermiIt->sekil.getGlobalBounds().intersects(oyuncu.sekil.getGlobalBounds())) {
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
        window.clear(sf::Color(10, 10, 25)); // Koyu Arka Plan

        // Yıldızları Çiz
        for (const auto& y : yildizlar) {
            sf::RectangleShape yildizSekil(sf::Vector2f(2.0f, 2.0f));
            yildizSekil.setPosition(y.pozisyon);
            yildizSekil.setFillColor(y.renk);
            window.draw(yildizSekil);
        }

        if (!oyunBitti) {
            window.draw(oyuncu.sekil);

            // >>> DEĞİŞİKLİK: Düşman gemisinin tüm katmanlarını derinlik sırasına göre ekrana çiz <<<
            for (const auto& d : dusmanlar) {
                window.draw(d.kanatlar);  // Arka katman
                window.draw(d.govde);     // Orta katman
                window.draw(d.cekirdek);  // Ön parlama katmanı
            }
            // >>> DEĞİŞİKLİK SONU <<<

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
