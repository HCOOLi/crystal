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

    for Ep in [1.0]:
        for Eb in [0.15]:
            for length in [128]:
                for T in np.arange(1.0, 3.5, 0.5):
                    for T_anneal in [0]:
                        for steps in [50000]:
                            # step_heating({"Eb": Eb, "Ep": Ep, "length": length, "T": T, "T_anneal": T_anneal, "steps": steps})
                            parameter_list.append(
                                {"Eb": Eb, "Ep": Ep, "length": length, "T": T, "T_anneal": T_anneal, "steps": steps})

    try:
        # with ProcessPoolExecutor(max_workers=5) as p:
        with Pool(10) as p:
            # p.map_async(Inclusion_Complex, parameter_list)
            p.map_async(reconstruct, parameter_list)
            # p.map_async(step_heating, parameter_list)
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
