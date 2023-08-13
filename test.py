# Online Python compiler (interpreter) to run Python online.
# Write Python 3 code in this online editor and run it.
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm

row = 20
el = np.random.randint(0,row,size=(2000,2))
# print(el)

key = el[:,0] + (el[:,1] * row)
n = len(key)

rowf = 20.0
elf = np.random.random((2000,2))
# print(el)

keyf = elf[:,0] + (elf[:,1] * rowf)
nf = len(keyf)
# print(key)


fig, ax = plt.subplots()
# ax.scatter()

colors = cm.rainbow(key/np.max(key))
for i ,c in enumerate(colors):
    ax.scatter(el[i,0],el[i,1], color=c)



fig2, ax2 = plt.subplots()
colorsf = cm.rainbow(keyf/np.max(keyf))
for i ,c in enumerate(colorsf):
    ax2.scatter(elf[i,0],elf[i,1], color=c)

# for i, txt in enumerate(key):
#     ax.annotate(txt, (el[i,0],el[i,1]))


plt.show()