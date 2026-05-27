# YZM104 - Programlama II Dönem Oyun Projesi: Galaga Klonu

Bu proje, Kocaeli Üniversitesi Mühendislik Fakültesi Yazılım Mühendisliği Bölümü YZM104 - Programlama II dersi kapsamında geliştirilmiş, arcade klasiği olan **Galaga** oyununun modern C++ ve SFML (Simple and Fast Multimedia Library) kütüphanesi kullanılarak kodlanmış bir klonudur.

## Proje Hakkında Genel Bilgiler
* **Geliştirilen Dil:** C++ (Modern C++ Standartları)
* **Kullanılan Grafik Kütüphanesi:** SFML v2.6+
* **Geliştirme Ortamı:** Visual Studio / C++ CLI Compiler

---

##  Temel Oyun Özellikleri ve Mekanikler

Proje, nesne yönelimli programlama (OOP) prensipleri gözetilerek ve bellek yönetimi kurallarına dikkat edilerek tasarlanmıştır.

1. **Oyuncu Kontrolü:** `Oyuncu` sınıfı vasıtasıyla uzay gemisi klavyedeki `A`/`D` veya `Sol`/`Sağ` yön tuşları ile yatay eksende hareket ettirilir. `Space` (Boşluk) tuşu ile ateş edilir. Geminin ekran sınırlarının dışına çıkması engellenmiştir.
2. **Dinamik Düşman Formasyonu:** `Dusman` sınıfı, ekranın üst kısmında periyodik olarak sağa ve sola salınan (sinüs dalgası matrisi tabanlı) organize bir formasyon oluşturur. Tüm düşmanlar yok edildiğinde otomatik olarak yeni dalga (wave) üretilir.
3. **Gelişmiş Yapay Zeka Mekanikleri:** Formasyondaki düşmanlar, bağımsız zamanlayıcılar (`dalisZamanlayici`) aracılığıyla rastgele zamanlarda formasyondan ayrılarak oyuncuya doğru doğrusal olmayan **akıllı dalışlar** gerçekleştirir. Dalıştaki düşmanlar oyuncuya çarparsa can azaltır.
4. **Çatışma ve Ateş Sistemi:** Düşmanlar ve oyuncu belirli periyotlarla mermi üretebilir. Mermilerin takibi dinamik bir bellek yönetimi sağlayan `std::vector` dizileri ile kontrol edilir. Ekrandan çıkan mermiler hafızadan güvenli bir şekilde temizlenir.
5. **Gelişmiş Görsel Efektler & Parçacık Motoru (Particle System):**
   * **Lazer İzleri:** Sıkılan mermilerin arkasında doğrusal azalan opaklığa (`alpha channel`) sahip kuyruk efektleri oluşur.
   * **İmha Efektleri:** Vurulan her düşman gemisinin koordinatında patlama yönü, hızı ve ömrü rastgele hesaplanan 25 adet dinamik parçacık saçılır.
   * **Game Over İnfilakı:** Oyuncu canı bittiğinde (`can <= 0`), geminin elendiği son noktadan ekranın tamamına yayılan, farklı boyutlarda (`par.boyut`) ve yangın renk paletine (kor beyaz, kırmızı, turuncu, sarı) sahip **250 adet agresif şarapnel parçacığı** devasa bir patlama etkisi oluşturur. Parçacıklar arka planda süzülürken `GAME OVER` yazısı ekrana gelir.
6. **Uzay Ambiyansı (Arka Plan):** Arka planda aşağı doğru sürekli akan, parlaklıkları ve yanıp sönme hızları (`yanipSonmeZamanlayicisi`) birbirinden bağımsız 80 adet yıldız katmanı bulunur.
7. **Skor ve Arayüz Sistemi:** Ekranın üst kısmında anlık skor, ulaşılan en yüksek skor (`high score`) ve kalan can durumu dinamik metin bileşenleri ile oyuncuya gösterilir.

---

##  Temiz Kod ve Kod Analizi (Code Analysis) Standartları

Projenin geliştirilmesinde C++ Core Guidelines standartlarına sıkı sıkıya bağlı kalınmıştır. 
* Kodun hiçbir satırında tanımsız davranışlara (undefined behavior) izin verilmemiştir.
* Yapıların (`struct Yildiz`, `struct PatlamaParcacigi`, `struct LazerIzi`) tüm üye değişkenleri **In-class member initializer** (modern sınıf içi varsayılan değer atama) yöntemiyle derleme aşamasında sıfırlanmıştır.
* Kod tabanı, Visual Studio Code Analysis (**C26495** vb.) uyarılarından tamamen arındırılmış olup, sıfır hata ve sıfır uyarı (Warning-free) ile temiz derleme (Clean Compile) gerçekleştirmektedir.

---

##  Kurulum ve Çalıştırma

### Gereksinimler
* C++17 destekleyen bir derleyici (MSVC, GCC veya Clang)
* SFML Kütüphanesi (Grafik, Pencere ve Sistem modülleri)
* `arial.ttf` yazı tipi dosyası (Proje exe dosyasının yanında bulunmalıdır)

### Derleme Adımları (Visual Studio)
1. SFML kütüphanesini sisteminize indirin.
2. Proje Özellikleri -> C/C++ -> General -> **Additional Include Directories** kısmına SFML `include` klasörünü ekleyin.
3. Proje Özellikleri -> Linker -> General -> **Additional Library Directories** kısmına SFML `lib` klasörünü ekleyin.
4. Proje Özellikleri -> Linker -> Input -> **Additional Dependencies** kısmına aşağıdaki kütüphaneleri dahil edin:
   * `sfml-graphics.lib`
   * `sfml-window.lib`
   * `sfml-system.lib`
5. Projeyi `Release` veya `Debug` modunda derleyip çalıştırın (İlgili SFML `.dll` dosyalarını derlenen `.exe` dosyasının dizinine kopyalamayı unutmayınız).

---

## Dosya Yapısı
* `main.cpp` - Tüm oyun mantığını, sınıfları, parçacık motorunu ve oyun döngüsünü içeren ana kaynak kod dosyası.
* `arial.ttf` - Arayüz metinlerinin ekrana basılması için kullanılan yazı tipi.
* `README.md` - Proje dokümantasyon dosyası.
