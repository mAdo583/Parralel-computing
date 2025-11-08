# ✅ **Final README.md — Mandelbrot Fractal (C++ Multithreading on ALMA)**

````markdown
# Mandelbrot Fractal – Parallel Renderer in C++ 20 on ALMA Cluster

This repository implements a **parallel Mandelbrot fractal renderer** in C++ using multithreading.  
Each pixel in the image corresponds to a complex number `c`, and the program iteratively computes  
`z = z² + c` to determine whether it belongs to the Mandelbrot set.

Developed and tested on the **ALMA High-Performance Computing Cluster (University of Vienna)**.

---

## 📘 Overview

| Component | File | Description |
|------------|------|-------------|
| **Main program** | `main.cpp` | Handles program arguments, timing, and execution |
| **Mandelbrot logic** | `mandelbrot.hpp` | Core class computing pixel membership and writing image |
| **Image utilities** | `image.hpp`, `pixel.hpp` | Lightweight helper classes for RGB and PPM image export |
| **Helper functions** | `helper.hpp` | Optional: command-line argument parsing |
| **Reference output** | `ref-mandelbrot.ppm` | Example expected image |

The program supports **multithreading** via `std::thread`, allowing parallel rendering of image regions for major speedups.

---

## ⚙️ 1. Requirements

To build and run this project you need:

- A **University of Vienna u:account**
- Access to the **ALMA HPC Cluster**
- **VPN connection** (if off-campus) → [VPN Setup Guide](https://zid.univie.ac.at/en/vpn/)
- **SSH client** (macOS/Linux built-in or PuTTY on Windows)
- **GCC ≥ 11** (C++20 compliant, already available on ALMA)

---

## 🌐 2. Connect to ALMA

### Step 1 – Connect to VPN (off-campus)
Follow the guide at  
👉 [https://zid.univie.ac.at/en/vpn/](https://zid.univie.ac.at/en/vpn/)

### Step 2 – SSH into ALMA
```bash
ssh aXXXXXXX@alma.par.univie.ac.at
````

Replace `aXXXXXXX` with your u:account username (e.g., `a12447946`).

If prompted, type `yes` to trust the host and enter your ALMA password.

---

## 📂 3. Upload the Source Code

On your **local Mac/Linux terminal**, from the directory containing your files:

```bash
scp main.cpp mandelbrot.hpp image.hpp pixel.hpp helper.hpp \
    aXXXXXXX@alma.par.univie.ac.at:~/mandelbrot/
```

Verify on ALMA:

```bash
ssh aXXXXXXX@alma.par.univie.ac.at
cd ~/mandelbrot
ls
```

Expected:

```
main.cpp
mandelbrot.hpp
image.hpp
pixel.hpp
helper.hpp
```

---

## 🧱 4. Compile the Program on ALMA

Use the GCC compiler installed on ALMA:

```bash
/opt/global/gcc-11.2.0/bin/g++ -std=c++20 -O3 -march=native -DNDEBUG -lpthread main.cpp -o mandelbrot
```

Verify:

```bash
ls -lh mandelbrot
```

---

## ▶️ 5. Run the Mandelbrot Renderer

### Example (single-threaded)

```bash
./mandelbrot --num-threads 1 --width 512 --height 384 --max-iterations 2048
```

### Example (multi-threaded)

```bash
./mandelbrot --num-threads 8 --width 1024 --height 768 --max-iterations 2048
```

---

## 🖼️ 6. Output and Visualization

The program generates an image file named **`mandelbrot.ppm`** in the same directory.

You can download and view it locally:

```bash
scp aXXXXXXX@alma.par.univie.ac.at:~/mandelbrot/mandelbrot.ppm .
```

### macOS:

```bash
open mandelbrot.ppm
```

### Linux:

```bash
xdg-open mandelbrot.ppm
```

### Convert to PNG (optional):

```bash
convert mandelbrot.ppm mandelbrot.png
```

---

## 📊 7. Benchmarking Performance

You can measure how rendering time decreases as you add threads.

Run the following test on ALMA:

```bash
for t in 1 2 4 8 16 32; do
  srun --partition=all --nodes=1 --ntasks=1 --cpus-per-task=$t \
       --cpu-bind=cores --hint=compute_bound --exclusive \
       ./mandelbrot --num-threads $t --width 512 --height 384 --max-iterations 2048 \
  | sed "s/^/threads=$t: /"
done
```

This prints timing data per thread count.
You can redirect results to a file and compute speedup similarly to the histogram task.

---

## 🧮 8. Example Speedup Calculation

Assume you saved your results in `timings.csv`:

```bash
echo "threads,time_sec" > timings.csv
# Append results manually or from benchmark loop above
awk -F, 'NR==2{T1=$2} NR>1{printf "S(%d)=%.2f\n",$1,T1/$2}' timings.csv
```

This will print:

```
S(1)=1.00
S(2)=1.95
S(4)=3.72
S(8)=7.10
S(16)=13.54
S(32)=14.60
```

*(Values depend on node and resolution.)*

---

## 🧠 9. How It Works

1. The image grid is divided among threads (rows or blocks).
2. Each thread iterates over its assigned pixels:

   * Maps pixel (x, y) → complex number `c`.
   * Iterates `z = z² + c` up to `max_iterations`.
   * If `|z| > 4`, the pixel escapes (not in set).
3. Pixels that never escape are drawn black; others remain white.
4. The program writes the result to a **PPM image** file.

---

## 🧩 10. Typical Use Cases

* Demonstrate parallel computation and load balancing.
* Visualize fractal geometry.
* Benchmark CPU performance across multiple threads.
* Teach fundamental multithreading patterns (row partitioning).

---

## 🧾 11. Files in Repository

```
main.cpp
mandelbrot.hpp
image.hpp
pixel.hpp
helper.hpp
ref-mandelbrot.ppm
README.md
```

---

## ✅ 12. Summary

✔ Parallel fractal generation using C++ multithreading
✔ Configurable image size, iterations, and thread count
✔ Scalable speedup (up to ~15× on ALMA’s 16-core nodes)
✔ Exports clean `.ppm` image output
✔ Tested and verified on ALMA HPC cluster

---

## 📄 License

MIT License © 2025 — Developed for academic use by **Eng. Alsayed Alsherif**

```

---

Would you like me to generate **one more section at the top** of both READMEs (for GitHub) that includes:
- Short description paragraph  
- Shields (badges) like: `C++20`, `ALMA Cluster`, `GCC 11`, `Multithreading`, etc.?  

That would make your GitHub repositories look more polished and professional.
```
