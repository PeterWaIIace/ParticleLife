#! /bin/bash
cmake -B build --fresh -G Ninja ; cmake --build build ; build/myproject