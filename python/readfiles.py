import json
import matplotlib.pyplot as plt
import os
from multiprocessing import Pool
import numpy as np

from pyroom import *


def drawpicture(date, Ep, T):
    path = './Data/' + date + '/'
    picpath = ('./Data/' + date + '-%3.2f-%3.2f/') % (Ep, Ep * T)
    if not os.path.exists(picpath):
        os.mkdir(picpath)

    for i in range(1, 1000):
        filename = ('d=0E%d=%3.2f,T=%3.2f.json') % (i, Ep, Ep * T)
        r = pyRoom(64, 64, 64, Ep=[[0, 0, 0], [0, 0, 2], [0, 2, 0]], Eb=[[0, 0, 0], [0, 0, 0], [0, 0, 0]], roomtype=4)
        print(path + filename)
        polymerlist = r.load_polymer(filepath=path + filename)
        # r.draw(path=loadpath)50
        # r.draw_all()
        # r.draw_a_layer(1,title=str(i))
        coutlist = r.draw_a_layer_plot_json(1, polymerlist)
        plt.savefig(picpath + str(i) + "-1")
        plt.close()
        coutlist = r.draw_a_layer_plot_json(2, polymerlist)
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
if __name__ == '__main__':
    start = time.time()
    print('Parent process %s.' % os.getpid())
    date = '2019-9-1-m=-4.0'

    try:
        # with ProcessPoolExecutor(max_workers=5) as p:
        with Pool(12) as p:

            # for T in ["4.60", "4.80", "4.40"]:"2.20","2.40","2.60","2.80",
            # for T in ["3.00","3.20","3.40","3.60","3.80","4.00","4.20","4.60", "4.80", "4.40"]:
            for Ep in [1]:
                for T in np.arange(4.0, 10.0, 1.0):
                    p.apply_async(drawpicture, (date, Ep, T))
            p.close()
            p.join()
    except:
        print("shutdown")
        p.terminate()
        p.shutdown(wait=False)
    #
    # print('All subprocesses done.')
    # end = time.time()
    # print('Tasks runs %0.2f seconds.' % (end - start))
