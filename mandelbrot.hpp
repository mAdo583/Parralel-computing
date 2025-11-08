#include <vector>
#include <complex>
#include <numeric>
#include <thread>

#include "pixel.hpp"
#include "image.hpp"

class Mandelbrot {
    Image image;

    int max_iterations = 2048;

public:
    Mandelbrot(int rows, int cols, int max_iterations): image(rows, cols, {255, 255, 255}), max_iterations(max_iterations) { }

    void compute(int num_threads = 1) {
        // create and launch threads
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(&Mandelbrot::worker, this, num_threads, i);
        }

        // wait for all threads to finish
        for (auto &t : threads) {
            t.join();
        }
    }

    void worker(int num_threads, int thread_id=0)
    {
        // each thread gets its own grayscale color
        unsigned char color = (254*(thread_id+1))/num_threads % 254; // use for your parallel code

        // divide work by assigning rows based on thread_id
        for (int y = thread_id; y < image.height; y += num_threads) 
        {
            for (int x = 0; x < image.width; ++x) 
            {
                double dx = ((double)x / image.width - 0.75) * 2.0;
                double dy = ((double)y / image.height - 0.5) * 2.0;

                std::complex<double> c(dx, dy);

                if (check_pixel(c)) { 
                    image[y][x] = {color, color, color}; // {0, 0, 0} - black for a pixel inside of the mandelbrot set
                }
            }
        }
    }

    // Test if point c belongs to the Mandelbrot set
    bool check_pixel(std::complex<double> c)
    {
        std::complex<double> z(0, 0);
        for (int i=0; i<max_iterations; ++i) {
            z = z * z + c;
            if ( std::abs(z) > 4 ) return false;
        }

        return true;
    };

    void save_to_ppm(std::string filename){
        image.save_to_ppm(filename);
    }
};
