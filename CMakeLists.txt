cmake_minimum_required(VERSION 3.10)
project(GalagaClone)

# Modern C++ standardını belirliyoruz
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# SFML kütüphanesini ve gerekli modüllerini (grafik, pencere, sistem ve SES) buluyoruz
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Ana dizindeki (veya src klasöründeki) tüm .cpp dosyalarını projeye dahil et
file(GLOB SOURCES "*.cpp" "src/*.cpp")

# Çalıştırılabilir dosyayı oluştur
add_executable(GalagaClone ${SOURCES})

# SFML kütüphanelerini projeye bağla
target_link_libraries(GalagaClone sfml-graphics sfml-window sfml-system sfml-audio)
