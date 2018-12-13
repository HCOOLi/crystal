from pyroom import *
import time
from concurrent.futures import ProcessPoolExecutor
from multiprocessing import Pool
import os
import numpy as np


if __name__ == '__main__':
    start = time.time()
    print('Parent process %s.' % os.getpid())
    parameter_list = []

    for Ep in [1.3]:
        for Eb in [0.2, 0.4]:
            for length in [160]:
                for T in [3.0 * Ep]:
                    for T_anneal in [0]:
                        for steps in [5000, 10000, 15000, 20000]:
                            parameter_list.append(
                                {"Eb": Eb, "Ep": Ep, "length": length, "T": T, "T_anneal": T_anneal, "steps": steps})
    try:
        # with ProcessPoolExecutor(max_workers=5) as p:
        with Pool(1) as p:
            # p.map_async(washing_small, parameter_list)
            p.map_async(reconstruct, parameter_list)
            # p.map_async(anneal, parameter_list)
            p.close()
            p.join()
            # with Pool(5) as p:
            #################################################
            # try:

        # reconstruct(1.4,0.4,44,140)
        # except:
        #     print("***main catch the error")

        # finally:
        #     # raise Exception("something wrong")
        #     print('Waiting for all subprocesses done...')

            #################################################
    except:
        print("shutdown")
        p.terminate()
        # p.shutdown(wait=False)

    print('All subprocesses done.')
    end = time.time()
    print('Tasks runs %0.2f seconds.' % (end - start))
    # tuning_washing_small()
