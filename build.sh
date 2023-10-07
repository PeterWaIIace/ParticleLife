#! /bin/bash
cd Cpp; cmake -B build --fresh -G Ninja ; cmake --build build ; cd .. ; ./Cpp/build/myproject ;