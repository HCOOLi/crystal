import json
import matplotlib.pyplot as plt
import os
from multiprocessing import Pool

from pyroom import *

path = r'./Data/2019-8-13/'
picpath = r'./Data/2019-8-13-1.0-3.80/'
if not os.path.exists(picpath):
    os.mkdir(picpath)

for i in range(355):
    filename = "d=0E%d=1.00,T=3.80.json" % i
    r = pyRoom(64, 64, 64, Ep=[[0, 0, 0], [0, 0, 2], [0, 2, 0]], Eb=[[0, 0, 0], [0, 0, 0], [0, 0, 0]])
    print(path + filename)
    polymerlist = r.load_polymer(filepath=path + filename)
    # r.draw(path=loadpath)50
    # r.draw_all()
    # r.draw_a_layer(1,title=str(i))
    coutlist = r.draw_a_layer_plot_json(1, polymerlist)
    plt.savefig(picpath + str(i) + "-1")
    plt.close()
    coutlist = r.draw_a_layer_plot_json(1, polymerlist)
    plt.savefig(picpath + str(i) + "-2")
    plt.close()
    # coutlist = r.draw_a_layer_plot_json(3, polymerlist)
    # plt.savefig(picpath + str(i) + "-3")
    # plt.close()

    # plt.show()
    # plt.savefig(picpath+str(i)+"3")
    plt.close()
    # plt.ylim(0,40)
    # plt.hist(coutlist,bins=list(range(16)),)
    # plt.savefig(picpath + str(i)+"hist2")
    # plt.close()

    # os.system("pause")

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
