# import json
# import matplotlib.pyplot as plt
#
# Ecmax = 64 * 31 * (31 - 1)
# with open("Data/Ec_list,Ep2=1.00,T=4.00.json", 'r') as f:
#     for line in f.readlines():
#         Ec_list = json.loads(line)
#         Ec_list = list(map(lambda x: 1 + x / Ecmax, Ec_list))
#         plt.title("Ep2=1.00,T=4.00")
#         plt.ylabel("f")
#         plt.xlabel("100000MC cycle")
#         plt.plot(Ec_list)
#         plt.show()
#         print(Ec_list)
import os, re

sets = set()
for filename in os.listdir('.\\Complex'):
    filename = re.sub("chain[0-9]*", "", filename)
    filename = re.sub(".json", "", filename)
    sets.add(filename)
for x in sets:
    print(x)
