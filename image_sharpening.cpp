#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

vector<unsigned char> readRawImage(const char* filename, int width, int height);
void saveImage(const char* filename, const vector<unsigned char>& image, int width, int height);
void applyLaplacianSharpeningFilter(const vector<unsigned char>& inputImage, vector<unsigned char>& outputImage, int width, int height, double w);

int main() {
    // Image dimensions (adjust according to your image)
    const int width = 538;
    const int height = 464;

    // Load the input image
    vector<unsigned char> inputImage = readRawImage("moon.raw", width, height);

    // Initialize output images
    vector<unsigned char> outputImage1(width * height, 0);
    vector<unsigned char> outputImage2(width * height, 0);

    // Apply the Laplacian sharpening filter with two different values of "w"
    double w1 = 1.0;
    double w2 = 0.5;

    applyLaplacianSharpeningFilter(inputImage, outputImage1, width, height, w1);
    applyLaplacianSharpeningFilter(inputImage, outputImage2, width, height, w2);

    // Save the output images
    saveImage("LaplacianSharpening_w1_output.raw", outputImage1, width, height);
    saveImage("LaplacianSharpening_w2_output.raw", outputImage2, width, height);

    return 0;
}

// Function to read a raw image file
vector<unsigned char> readRawImage(const char* filename, int width, int height) {
    vector<unsigned char> image(width * height);
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(image.data()), width * height);
        file.close();
    }
    return image;
}

// Function to save the grayscale image
void saveImage(const char* filename, const vector<unsigned char>& image, int width, int height) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(image.data()), width * height);
        file.close();
    }
}

// Apply the Laplacian sharpening filter
void applyLaplacianSharpeningFilter(const vector<unsigned char>& inputImage, vector<unsigned char>& outputImage, int width, int height, double w) {
    // Laplacian 3x3 kernel
    int laplacianKernel[9] = {
        0, -1, 0,
        -1, 4 + w, -1,
        0, -1, 0
    };

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int sum = 0;

            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    int pixel = inputImage[(y + j) * width + (x + i)];
                    int kernelValue = laplacianKernel[(j + 1) * 3 + (i + 1)];
                    sum += pixel * kernelValue;
                }
            }

            // Ensure the pixel value is within the [0, 255] range
            sum = max(0, min(255, sum));

            outputImage[y * width + x] = static_cast<unsigned char>(sum);
        }
    }
}