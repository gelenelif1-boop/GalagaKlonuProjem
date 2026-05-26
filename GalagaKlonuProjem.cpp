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
