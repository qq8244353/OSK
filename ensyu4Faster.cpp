#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
class Complex {
    public:
    float real;
    float imag;

    Complex(float real, float imag) : real(real), imag(imag) {} 
};

Complex operator+(const Complex& lhs, const Complex& rhs) {
    return Complex(lhs.real + rhs.real, lhs.imag + rhs.imag);
}

Complex operator*(const Complex& lhs, const Complex& rhs) {
    return Complex(lhs.real * rhs.real - lhs.imag * rhs.imag,
                lhs.real * rhs.imag + lhs.imag * rhs.real);
}

float abs(const Complex& c) {
    return std::sqrt(c.real * c.real + c.imag * c.imag);
}
class RGB {
    public:
        char r;
        char g;
        char b;
    RGB (int _r, int _g, int _b) : r((char)_r), g((char)_g), b((char)_b) {}
    RGB() {}
    
};

class PicData {
    private:
        int const height;
        int const width;
        RGB* color;
        int index(int i, int j) { return i * width + j; }
    public:
        PicData(int _height, int _width) : height(_height), width(_width),
            color(new RGB[_height * _width]) {
            for (int i = 0; i < _height; i++) {
                for (int j = 0; j < _width; j++) {
                    color[index(i, j)] = { 0, 0, 0 };
                }
            }
        }
        void setPixel(int i, int j, RGB newcolor) {
            if (i < 0 || height <= i || j < 0 || width <= j) {
                std::cerr << "setPixel:out of range" << std::endl;
                return;
            }
            color[index(i, j)] = newcolor;
            return;
        }
        void output(std::string filename) {
            if(system("type convert >/dev/null 2>&1") == 1) {
            std::cerr << "output:command not found \"convert\"" << std::endl;
            exit(1);
            }
            std::ofstream file(filename+".ppm");
            file << "P6" << std::endl;
            file << width << " " << height << std::endl;
            file << "255" << std::endl;
            for (int j = 0; j < width; j++) {
                for (int i = 0; i < height; i++) {
                    file << color[index(i, j)].r << color[index(i, j)].g
                        << color[index(i, j)].b;
                }
            }
            file.close();
            system(("convert " + filename + ".ppm " + filename + ".png").c_str());
            system(("rm " + filename + ".ppm").c_str());
        }
        ~PicData() { delete [] color; }
};
int main() {
    const RGB WHITE(255, 255, 255);
    const RGB YELLOW(255, 255, 0);
    const RGB LIGHTGRAY(192, 192, 192);
    const RGB CYAN(0, 255, 255);
    const RGB YELLOWGREEN(0, 255, 0);
    const RGB GRAY(128, 128, 128);
    const RGB OCHER(128, 128, 0);
    const RGB DARKPINK(255, 0, 255);
    const RGB BLUEGREEN(0, 128, 128);
    const RGB RED(255, 0, 0);
    const RGB GREEN(0, 128, 0);
    const RGB PURPLE(128, 0, 128);
    const RGB BROWN(128, 0, 0);
    const RGB BLUE(0, 0, 255);
    const RGB NAVYBLUE(0, 0, 128);
    const RGB BLACK(0, 0, 0);
    //stripe
    // {
    //     int x, y, stripeWidth;
    //     x = 2000;
    //     y = 2000;
    //     stripeWidth = 400;
    //     PicData stripe(y, x);
    //     for (int i = 0; i < y; i++) {
    //         for (int j = 0; j < x; j++) {
    //             if ((j % stripeWidth) < (stripeWidth / 2)) 
    //                    stripe.setPixel(i, j, WHITE);
    //             else stripe.setPixel(i, j, BLACK);
    //         }
    //     }
    //     stripe.output("stripe");
    // }
    //gradation
    // {
    //     int x, y;
    //     x = 2000;
    //     y = 2000;
    //     PicData gradation(y, x);
    //     for (int i = 0; i < y; i++) {
    //         for (int j = 0; j < x; j++) {
    //             RGB color = {i / 10, j / 10, 255};
    //             gradation.setPixel(i, j, color);
    //         }
    //     }
    //     gradation.output("gradation");
    // }
    // Mandelbrot set
    {
        int y, x;
        y = 2000;
        x = 2000;
        PicData mandelbrotSet(y, x);
        int samples = 100;
        for (int i = 0; i < y; ++i) {
            for (int j = 0; j < x; ++j) {
                const float u = (2.0f * i - x / 2 * 3) / x;
                const float v = (2.0f * j - y) / y;
                Complex z(0, 0); // n = 0
                Complex c(u, v);
                bool diverge = false;
                int sampleTimes;
                for (int k = 0; k < samples; ++k) {
                    z = z * z + c;
                    if (abs(z) > 2) {
                        diverge = true;
                        sampleTimes = k;
                        break;
                    }
                }
                if (diverge) {
                    if (sampleTimes != 1) std::cerr << sampleTimes << " ";
                    sampleTimes *= 2;
                    RGB color = { sampleTimes, sampleTimes, 255 - sampleTimes};
                    mandelbrotSet.setPixel(i, j, color);
                    
                }
                else mandelbrotSet.setPixel(i, j, BLACK);
            }
        }
        mandelbrotSet.output("mandelbrotSet");
    }
    return 0;

}