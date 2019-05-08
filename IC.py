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


class Inclusion_Complex(Simulator):
    def __init__(self):
        super(Simulator, self).__init__()
        if not os.path.exists('Complex'):
            os.mkdir('Complex')
        pass

    def py_inputECC_with_small(self, r: pyRoom):
        num = 0
        for i in range(r.shape[0]):
            for j in range(r.shape[1]):
                if j % 2 == 0 or i % 2 == 0:
                    for k in range(0, int(3 * r.shape[2] / 4), 2):
                        r.py_input_one_ECC([i, j, k + int(r.shape[2] / 8)], 2, 2, [2] * 2, 0)

                        num += 1
                else:
                    r.py_input_one_ECC([i, j, 1], r.shape[2] - 1, 2, [1] * (r.shape[2] - 1), 0)
                    num += 1

                pass

    def parameters(self):
        import itertools
        Ep12 = list(range(1, 3, 1))
        # length = [32,64,128]
        # T = list(np.arange(2, 6, 0.5))
        length = [32]
        T = [1.5, 2, 2.5, 3]
        return itertools.product(Ep12, length, T)

    def simulate(self, parameter):
        Ep12, length, T = parameter[0], parameter[1], parameter[2]
        try:
            print('Run task Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (Ep12, 0, T, length, os.getpid()))
            start = time.time()

            r = pyRoom(32, 32, 32, Ep=[[0, 0, 0], [0, 0, Ep12], [0, Ep12, 0]], Eb=[[0, 0, 0], [0, 0, 0], [0, 0, 0]])

            self.py_inputECC_with_small(r)
            print("installed model")
            r.movie(200000, 10000, 100)
            # r.draw(title="chain-%3.2f.json" % (T))
            # r.save("Complex/chain-%3.2raw.json" % (T))
            ###########################
            r.movie(1000000, 10000, T)
            # r.draw(title="chain-%3.2f.json" % (T))
            # r.save("Complex/chainEp-0,%3.2f,%3.2f,T=%3.2f.json" % (Ep1, Ep12, T))
            r.save("Complex/chainEp-0,%3.2f,%3.2f,Eb=0,0,%3.2f,T=%3.2f.json" % (0, Ep12, 0, T))

            end = time.time()
            print('Task%f runs %0.2f seconds.' % (T, (end - start)))
        except Exception as e:
            print(e)
            raise e
        return


if __name__ == '__main__':
    start = time.time()
    print('Parent process %s.' % os.getpid())
    S = Inclusion_Complex()
    parameter_list = list(S.parameters())
    # S.simulate(parameter_list[1])
    try:
        # with ProcessPoolExecutor(max_workers=5) as p:
        with Pool(8) as p:
            p.map_async(S.simulate, parameter_list)
            p.close()
            p.join()
    except:
        print("shutdown")
        p.terminate()
        p.shutdown(wait=False)

    print('All subprocesses done.')
    end = time.time()
    print('Tasks runs %0.2f seconds.' % (end - start))
