# MD-ML-repro
A reproduction of the code and experiments from the MD-ML (Yuan et al., 2024)

This repository reproduces the code and experiments from **MD-ML (Yuan et al., 2024)**, focusing on the fake-offline and online execution pipelines.

## Citation / Source

- **Paper**: Yuan et al., *MD-ML: Super Fast Privacy-Preserving Machine Learning for Malicious Security with a Dishonest Majority*, 2024.  
- **Original implementation (authors)**: <https://github.com/NemoYuan2008/MD-ML>

This reproduction is for academic course purposes.  
To ensure successful compilation and execution under **macOS (libc++)**, minor compatibility changes were made, including replacing `<execution>`-based parallel algorithms with sequential wrappers (e.g., `std::transform`).

## Environment

- OS: macOS
- Build system: CMake
- Compiler: clang++ (Apple Clang)

## Reproduction Steps (Quick)

```bash
cd ~/repro/MD-ML
mkdir -p build && cd build
cmake ..
cmake --build . -j


## fake-offline 

cd ~/repro/MD-ML/build
./experiments/test/test_fake_offline
echo $?

cd ~/repro/MD-ML
ls -lah fake-offline-data | sed -n '1,120p'
find . -maxdepth 4 -type f -mmin -10 | sed -n '1,200p'

## Online-test, with party-0 and party-1

## party-0
./experiments/test/test_party_0 test_job
## party-1
./experiments/test/test_party_1 test_job


## Online-dot-product

## party-0
./experiments/dot-product/dot_product_party_0 test_job
## party-1
./experiments/dot-product/dot_product_party_1 test_job


## Online-AlexNet

## party-0
./experiments/AlexNet-ImageNet/AlexNet_party0 test_job
## party-1
./experiments/AlexNet-ImageNet/AlexNet_party1 test_job
