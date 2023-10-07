#! /bin/bash
cd Cpp; cmake -B build --fresh -G Ninja -DRECORD=ON; cmake --build build ; cd .. ; ./Cpp/build/particleLife ;