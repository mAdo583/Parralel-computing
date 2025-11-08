# Parallel Histogram – C++ Multithreading on ALMA Cluster

This repository implements and compares **two parallel versions** of a histogram computation using modern C++ multithreading.  
It demonstrates performance differences between synchronization-heavy and synchronization-free designs on a multicore environment.

Developed and tested on the **ALMA High-Performance Cluster (University of Vienna)**.

---

## 📘 Overview

| Version | Source file | Technique | Goal |
|----------|--------------|-----------|------|
| **V1 – Atomic/Mutex** | `histogram-atomic-mutex.cpp` | Shared global histogram updated using `std::atomic<int>` | Correct but synchronization-heavy |
| **V2 – Best Effort** | `histogram-best.cpp` | Per-thread local histograms + one-time reduction | Minimize synchronization, achieve near-linear scaling |

Both versions produce the same total count as the serial reference implementation (`histogram.cpp`).

---

## ⚙️ 1. Requirements

To build and run this project you need:

- A **University of Vienna u:account**  
- Access to the **ALMA cluster**
- **VPN connection** (if off-campus) → [VPN setup guide](https://zid.univie.ac.at/en/vpn/)
- **SSH client** (macOS/Linux terminal or PuTTY on Windows)
- **GCC ≥ 11** with pthreads (already available on ALMA)

---

## 🌐 2. Connect to ALMA

### Step 1 – Connect to VPN (if not on campus)
Use the University VPN:  
👉 [https://zid.univie.ac.at/en/vpn/](https://zid.univie.ac.at/en/vpn/)

### Step 2 – SSH into ALMA
```bash
ssh aXXXXXXX@alma.par.univie.ac.at

When prompted, enter your ALMA password.
After the first login, you may be required to change it.
```
📂 3. Upload Your Code

On your local terminal (Mac/Linux):

scp main.cpp helper.hpp histogram.cpp histogram-atomic-mutex.cpp histogram-best.cpp \
    aXXXXXXX@alma.par.univie.ac.at:~/histogram/


Then log in again and verify:

ssh aXXXXXXX@alma.par.univie.ac.at
cd ~/histogram
ls
You should see:

helper.hpp
histogram.cpp
histogram-atomic-mutex.cpp
histogram-best.cpp
4. Compile on ALMA

Use ALMA’s system GCC compiler:

/opt/global/gcc-11.2.0/bin/g++ -std=c++20 -O3 -march=native -DNDEBUG -lpthread histogram-atomic-mutex.cpp -o histogram_atomic
/opt/global/gcc-11.2.0/bin/g++ -std=c++20 -O3 -march=native -DNDEBUG -lpthread histogram-best.cpp -o histogram_best


Check binaries:

ls -lh histogram_atomic histogram_best

▶️ 5. Run the Programs
Run a single test
./histogram_best --num-threads 4 --num-bins 10 --sample-size 30000000 --print-level 1

Run parallel tests with srun

Example on ALMA:

srun --partition=all --nodes=1 --ntasks=1 --cpus-per-task=8 \
     --cpu-bind=cores --hint=compute_bound --exclusive \
     ./histogram_best --num-threads 8 --num-bins 10 --sample-size 100000000 --print-level 2
    
📊 6. Measure and Record Performance

Step 1 – Collect timing results

echo "threads,time_sec" > timings_best.csv
for t in 1 2 4 8 16 32; do
  tt=$(srun --partition=all --nodes=1 --ntasks=1 --cpus-per-task=$t \
            --cpu-bind=cores --hint=compute_bound --exclusive \
            ./histogram_best --num-threads $t --num-bins 10 --sample-size 100000000 --print-level -1 \
            | tail -n1)
  echo "$t,$tt" >> timings_best.csv
done
cat timings_best.csv


Step 2 – Compute speedup

awk -F, 'NR==2{T1=$2} NR>1{printf "S(%d)=%.2f\n",$1,T1/$2}' timings_best.csv
🧩 8. Verify Correctness

For both versions, the total count must equal the --sample-size:

./histogram_atomic --num-threads 8 --num-bins 10 --sample-size 30000000 --print-level 1
./histogram_best   --num-threads 8 --num-bins 10 --sample-size 30000000 --print-level 1


Expected output (example):

total:30000000

🧮 9. Compare Both Versions

Atomic Version (V1)
echo "threads,time_sec" > timings_atomic.csv
for t in 1 2 4 8 16 32; do
  tt=$(srun --partition=all --nodes=1 --ntasks=1 --cpus-per-task=$t \
            ./histogram_atomic --num-threads $t --num-bins 10 --sample-size 100000000 --print-level -1 \
            | tail -n1)
  echo "$t,$tt" >> timings_atomic.csv
done
awk -F, 'NR==2{T1=$2} NR>1{printf "S(%d)=%.2f\n",$1,T1/$2}' timings_atomic.csv


Expected: Very low speedup (~1×) — this version is limited by synchronization.

🧾 10. Files in Repository

helper.hpp
histogram.cpp
histogram-atomic-mutex.cpp
histogram-best.cpp
timings_atomic.csv
timings_best.csv
README.md

🧩 11. Summary

✅ Correctness verified (identical totals)
✅ Threading implemented with std::thread
✅ Parallel scaling achieved (~15× at 16 threads)
✅ Fully portable across Linux / macOS / ALMA cluster

