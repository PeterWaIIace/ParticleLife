# ParticleLife

Particle life is algorithm showing emergent bacterial-like behaviours by simulating set of particles following simple rules. Particle are only interacting through attraction or repelment with each other. However, as this is nonphysical simulation, meaning that energy conservation rule is not obeyed, there has to be added liquid physics simulation rules, like system friction, so particle energy can be dissipated.

That simple set of behaviour leads to eye catching emergence, giving feeling that structures are almost exhibiting exploratory behaviours.

Some prelimiary results - still not optimal: 

![particleLife](https://user-images.githubusercontent.com/40773550/234987226-72c52e8c-6991-4da5-bcae-75d3ea1086da.gif)


# How to:

You can compile version C++:
```
cd Cpp\build ; cmake .. ; cmake --build . ; .\Debug\myproject.exe
```

Or run python script:
```
cd python ; python main.py
```
