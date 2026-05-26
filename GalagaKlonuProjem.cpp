#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>

// --- OYUN AYARLARI ---
const int EKRAN_GENISLIK = 600;
const int EKRAN_YUKSEKLIK = 800;

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
    sf::ConvexShape sekil;
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

        // Klasik Galaga tarzi ucgen/bocek gorunumu olusturma
        sekil.setPointCount(5);
        sekil.setPoint(0, sf::Vector2f(15, 0));
        sekil.setPoint(1, sf::Vector2f(30, 20));
        sekil.setPoint(2, sf::Vector2f(22, 30));
        sekil.setPoint(3, sf::Vector2f(8, 30));
        sekil.setPoint(4, sf::Vector2f(0, 20));

        sekil.setFillColor(sf::Color::Green);
        sekil.setOrigin(15, 15);
        sekil.setPosition(aktifPozisyonu);
    }

    void guncelle(float formasyonOfsetX) {
        if (durum == DusmanDurumu::Formasyon) {
            // Sinuzoidal formasyon hareketi (saga ve sola ortak salinim)
            aktifPozisyonu.x = formasyonPozisyonu.x + formasyonOfsetX;
            sekil.setPosition(aktifPozisyonu);
        }
        else if (durum == DusmanDurumu::Dalista) {
            // Dalisa gecen dusman asagi inerken hafifce yalpalar
            aktifPozisyonu.y += dalisHizi;
            sinusZamani += 0.05f;
            aktifPozisyonu.x += std::sin(sinusZamani) * 2.0f;
            sekil.setPosition(aktifPozisyonu);

            // Ekranin altindan cikan dusman tekrar uste, formasyona doner
            if (aktifPozisyonu.y > EKRAN_YUKSEKLIK + 20) {
                aktifPozisyonu.y = formasyonPozisyonu.y;
                durum = DusmanDurumu::Formasyon;
            }
        }
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

