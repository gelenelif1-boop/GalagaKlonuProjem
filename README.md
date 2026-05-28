# YZM104 - Programlama II Dönem Oyun Projesi: Galaga Klonu

Bu proje, Kocaeli Üniversitesi Mühendislik Fakültesi Yazılım Mühendisliği Bölümü YZM104 - Programlama II dersi kapsamında geliştirilmiş, arcade klasiği olan Galaga oyununun modern C++ ve SFML (Simple and Fast Multimedia Library) kütüphanesi kullanılarak kodlanmış bir klonudur.

## Proje Hakkında Genel Bilgiler

* **Geliştirilen Dil:** C++ (Modern C++ Standartları)
* **Kullanılan Grafik & Ses Kütüphanesi:** SFML v2.6+
* **Geliştirme Ortamı:** Visual Studio / C++ CLI Compiler
* **Temel Oyun Özellikleri ve Mekanikler:** Proje, nesne yönelimli programlama (OOP) prensipleri gözetilerek ve bellek yönetimi kurallarına dikkat edilerek tasarlanmıştır.

## Öne Çıkan Teknik Özellikler

* **Oyuncu Kontrolü:** `Oyuncu` sınıfı vasıtasıyla uzay gemisi klavyedeki A/D veya Sol/Sağ yön tuşları ile yatay eksende hareket ettirilir. Space (Boşluk) tuşu ile ateş edilir. Geminin ekran sınırlarının dışına çıkması engellenmiştir.
* **Dinamik Düşman Formasyonu:** `Dusman` sınıfı, ekranın üst kısmında periyodik olarak sağa ve sola salınan organize bir formasyon oluşturur. Tüm düşmanlar yok edildiğinde otomatik olarak yeni dalga üretilir.
* **Gelişmiş Yapay Zeka Mekanikleri:** Formasyondaki düşmanlar, bağımsız zamanlayıcılar aracılığıyla rastgele zamanlarda formasyondan ayrılarak oyuncuya doğru doğrusal olmayan akıllı dalışlar gerçekleştirir.
* **Çatışma ve Ateş Sistemi:** Mermilerin takibi dinamik bellek yönetimi sağlayan `std::vector` dizileri ile kontrol edilir. Ekrandan çıkan mermiler hafızadan güvenli bir şekilde temizlenir.
* **Gelişmiş Görsel Efektler & Parçacık Motoru (Particle System):**
* **Lazer İzleri:** Sıkılan mermilerin arkasında doğrusal azalan opaklığa sahip kuyruk efektleri.
* **İmha Efektleri:** Vurulan düşmanların koordinatında, rastgele saçılan 25 adet dinamik parçacık.
* **Game Over İnfilakı:** Oyuncu elendiğinde, 250 adet agresif şarapnel parçacığı ile oluşturulan devasa bir final patlaması.


* **Gelişmiş Ses Yönetimi (Audio Engine):**
* **Eşzamanlı Ses Oynatma:** `std::list<sf::Sound>` ve lambda tabanlı ses yöneticisi kullanılarak, sesler kesilmeden (bir ses bitmeden diğerinin başlaması) çalınabilmektedir.
* **Olay Bazlı Sesler:** Ateş etme, düşman imhası, oyuncu hasar alma ve Game Over efektleri için `sf::SoundBuffer` ile yönetilen yüksek kaliteli ses geri bildirimleri.


* **Uzay Ambiyansı:** Sürekli akan, parlaklıkları ve yanıp sönme hızları birbirinden bağımsız 80 adet yıldız katmanı.
* **Skor ve Arayüz Sistemi:** Anlık skor, en yüksek skor (high score) ve kalan can durumu dinamik metin bileşenleri ile gösterilir.

## Temiz Kod ve Analiz Standartları

* Projenin geliştirilmesinde C++ Core Guidelines standartlarına sıkı sıkıya bağlı kalınmıştır.
* Tüm `struct` ve `class` yapıları In-class member initializer yöntemiyle derleme aşamasında sıfırlanmıştır.
* Kod tabanı Visual Studio Code Analysis uyarılarından arındırılmış olup, "Warning-free" (Sıfır uyarı) ile temiz derleme yapmaktadır.

## Kurulum ve Çalıştırma

### Gereksinimler

* C++17 destekleyen bir derleyici (MSVC, GCC veya Clang).
* SFML Kütüphanesi (Grafik, Pencere, Sistem ve **Audio** modülleri).
* Gerekli medya dosyaları: `arial.ttf`, `atis.wav`, `patlama.wav`, `gameover.wav`.

### Derleme Adımları (Visual Studio)

1. SFML kütüphanesini sisteminize kurun.
2. Proje Özellikleri -> C/C++ -> General -> Additional Include Directories kısmına SFML include klasörünü ekleyin.
3. Proje Özellikleri -> Linker -> General -> Additional Library Directories kısmına SFML lib klasörünü ekleyin.
4. Linker -> Input -> Additional Dependencies kısmına aşağıdakileri ekleyin:
* `sfml-graphics.lib`
* `sfml-window.lib`
* `sfml-system.lib`
* `sfml-audio.lib`


5. Derleme sonrası oluşan `.exe` dosyasının bulunduğu dizine gerekli `.dll` dosyalarını ve medya dosyalarını (wav ve ttf) kopyalamayı unutmayın.

## Dosya Yapısı

* `main.cpp` - Tüm oyun mantığını, sınıfları, parçacık motorunu ve ses yöneticisini içeren ana kaynak kod dosyası.
* `arial.ttf` - Arayüz yazı tipi.
* `atis.wav` / `patlama.wav` / `gameover.wav` - Oyun içi ses efektleri.
* `README.md` - Proje dokümantasyonu.
  
