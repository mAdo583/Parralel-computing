
# 🧠 Parallel Computing Projects – C++ Multithreading on ALMA Cluster

This repository contains two **C++ 20 multithreaded applications** designed to demonstrate parallel programming concepts using the **ALMA High-Performance Computing (HPC) Cluster**.
Each project showcases a different approach to parallel computation and synchronization in modern C++.

---

## 📘 Project Overview

| Project                     | Folder                        | Description                                                                                                                                                               |
| --------------------------- | ----------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **1️⃣ Parallel Histogram**  | [`histogram/`](./histogram)   | Parallel histogram computation implemented in two variants — one using atomic operations and mutexes, and another using an optimized, synchronization-minimized approach. |
| **2️⃣ Mandelbrot Renderer** | [`mandelbrot/`](./mandelbrot) | Parallel fractal renderer that visualizes the Mandelbrot set using multithreading for fast image generation.                                                              |

Each subfolder includes its own detailed `README.md` explaining build, execution, and benchmarking procedures.

---

## ⚙️ Requirements

To build and run these programs you need:

* Access to the **ALMA Cluster** (or any Linux system with GCC 11+)
* **VPN connection** if off-campus → VPN
* **SSH client** (macOS/Linux Terminal or PuTTY for Windows)
* GCC ≥ 11 with pthread support (already available on ALMA)

---

## 🌐 Connecting to ALMA

1. **Connect to VPN** if you’re outside the university network.
2. **Open a terminal** and log in:

   ```bash
   ssh aXXXXXXX@alma.par.univie.ac.at
   ```

   Replace `aXXXXXXX` with your user ID.
3. (Optional) change your password upon first login:

   ```bash
   passwd
   ```

---

## 📂 Repository Structure

```
parallel-computing/
│
├── histogram/
│   ├── histogram-best.cpp
│   ├── histogram-atomic-mutex.cpp
│   ├── histogram.cpp
│   ├── helper.hpp
│   ├── README.md
│
├── mandelbrot/
│   ├── main.cpp
│   ├── mandelbrot.hpp
│   ├── image.hpp
│   ├── pixel.hpp
│   ├── helper.hpp
│   ├── README.md
│
└── README.md     ← (this file)
```

---

## 🚀 Quick Setup and Execution

### 1. Upload source code to ALMA

From your local computer:

```bash
scp -r histogram mandelbrot aXXXXXXX@alma.par.univie.ac.at:~/
```

### 2. Connect to ALMA

```bash
ssh aXXXXXXX@alma.par.univie.ac.at
```

### 3. Compile (example for histogram)

```bash
cd ~/histogram
/opt/global/gcc-11.2.0/bin/g++ -std=c++20 -O3 -lpthread histogram-best.cpp -o histogram_best
```

### 4. Run

```bash
srun --partition=all --nodes=1 --ntasks=1 --cpus-per-task=8 \
     --cpu-bind=cores --hint=compute_bound --exclusive \
     ./histogram_best --num-threads 8 --num-bins 10 --sample-size 100000000 --print-level 2
```

### 5. Verify

Expected output should include the total sample count and timing per thread configuration.

---

## 📊 Benchmarking and Performance

You can measure runtime and calculate speedup directly on ALMA.
Each project’s `README.md` includes commands to:

* Run automated timing tests for multiple thread counts
* Export results to `.csv`
* Calculate speedup ratios using `awk`

> ⚠️ **Note:**
> Timing and image result files (`.csv`, `.ppm`, `.png`) are **not included** in this repository, as they depend on each user's environment and ALMA node performance.
> Each user can generate their own results following the benchmarking steps in the respective project folders.

---

## 🧠 Learning Objectives

* Demonstrate **parallel programming** concepts in C++
* Understand **thread synchronization** and **atomic operations**
* Explore **load balancing** and **scalability**
* Measure **speedup and efficiency** across multiple cores
* Use **SLURM** to run controlled performance experiments on a cluster

---

## 🧾 License

MIT License — for educational and non-commercial use.

---

## 💡 Tips

* Always verify VPN connectivity before SSH login.
* Use `/opt/global/gcc-11.2.0/bin/g++` for consistent compiler versioning on ALMA.
* Increase `--sample-size` or image resolution for longer test runs to get more stable timing results.
* Use `--num-threads` equal to the number of **physical cores** for best speedup.
* Each subproject’s `README.md` provides step-by-step reproduction instructions.

---

Would you like me to extend this with **GitHub badges and links** (e.g., `C++20`, `ALMA Cluster`, `MIT License`, `Parallel Computing`) at the top for a polished repository header?
They give it a more professional, portfolio-ready look on GitHub.
