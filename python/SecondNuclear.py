from pyroom import *
import time
from concurrent.futures import ProcessPoolExecutor
from multiprocessing import Pool
import os
import numpy as np
from typing import List, Dict
from copy import deepcopy


class Simulator():
    def __init__(self):
        pass

    def install_model(self, parameter):
        pass

    def simulate(self):
        pass

    def parameters(self):
        pass


class SecondNuclear(Simulator):
    def __init__(self):
        super(Simulator, self).__init__()
        if not os.path.exists('Complex'):
            os.mkdir('Complex')
        pass

    def parameters(self):
        import itertools
        Ep = [1.0]
        length = [64]
        T = [3.3, 3.5, 3.6, 3.8]
        d = [0]
        return itertools.product(Ep, d, T)

    @staticmethod
    def install_model(r: pyRoom, d):
        for i in range(0, r.shape[1]):
            r.py_input_one_ECC([0, i, 8], 16, 2, [1] * 16, 1)
        # for i in range(0, r.shape[2], d):
        #     r.py_input_one_ECC([15, 0, i], r.shape[1], 1, [0] * r.shape[1], 1)
        # for i in range(0, r.shape[2], 3):
        #     r.py_input_one_ECC([62, 0, i], r.shape[1], 1, [0] * r.shape[1], 1)

        for i in range(2, r.shape[0] - 1):
            # if i == 15 or i == 62:
            #     continue
            for j in range(0, r.shape[1] - 1, 2):
                r.py_input_one_FCC([i, j, 0], 64, 2, 1, [1] * 64, 0)

    @staticmethod
    def simulate(parameter):

        try:
            Ep, d, T = parameter[0], parameter[1], parameter[2]
            print('Run task %f ,%f,%f(%s)...' % (Ep, 1, T, os.getpid()))
            start = time.time()
            EC_max = 31 * 31 * (31 - 1)
            if not os.path.exists('Data'):
                os.mkdir('Data')
            if not os.path.exists('Data/2019-8-13/'):
                os.mkdir('Data/2019-8-13/')
            r = pyRoom(32, 32, 32, Ep=[[0, 0], [0, Ep]], Eb=[[0, 0], [0, 0]])
            E_list, Ec_list, Ep_list, t_list = [], [], [], []

            SecondNuclear.install_model(r, d)
            print("install model")
            # r.draw_all()
            r.movie(500000, 10000, 100)
            print("end preheat")
            # r.movie(2000000, 10000, T*Ep)
            # # E_list, Ec_list, Ep_list, t_list, f = r.step_heating(6 * Ep+0.1, 1 * Ep, -0.1 * Ep,10000,5000, EC_max)
            r.save("Data/no-ECCheated%3.2f-d.json" % (Ep * T))
            # # E_list, Ec_list, Ep_list, t_list, f = r.step_heating(6 * Ep+0.1, 1 * Ep, -0.1 * Ep+0.01,10000,5000, EC_max)
            # # plt.plot(t_list, f)
            # # plt.savefig("stepheating%3.2f.png" % (Ep))
            for i in range(500):
                r.movie(50000, 1000, T * Ep)
                print("after movie%d" % (i))
                # E_result, Ec_result, Ep_result, Eb_result = r.get_result()
                # E_list += E_result
                # Ec_list += Ec_result
                # Ep_list += Ep_result
                r.save("Data/2019-8-13/d=%dE%d=%3.2f,T=%3.2f.json" % (d, i, Ep, T * Ep))

            # with open("Data/Ec_list,Ep2=%3.2f,T=%3.2f.json" % (Ep, T * Ep), 'w') as file:
            #     # file.write(json.dumps(self.get_list()))
            #     file.write(json.dumps(Ec_list))

        except Exception as e:
            print(e)
            raise e

        return


if __name__ == '__main__':
    start = time.time()
    print('Parent process %s.' % os.getpid())
    S = SecondNuclear()
    parameter_list = list(S.parameters())
    # S.simulate(parameter_list[1])
    try:
        # with ProcessPoolExecutor(max_workers=5) as p:
        with Pool(10) as p:
            p.map_async(S.simulate, parameter_list)
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
