from pyroom import *
import time
from concurrent.futures import ProcessPoolExecutor
from multiprocessing import Pool
import os
import numpy as np


def parameter_generator():
    for Ep1 in np.arange(0, 1, 0.5):
        for Ep12 in np.arange(3, 8, 0.5):
            for Eb in [1.0]:
                for length in [128]:
                    for T in [3.5]:
                        for T_anneal in [0]:
                            for steps in [50000]:
                                # step_heating({"Eb": Eb, "Ep": Ep, "length": length, "T": T, "T_anneal": T_anneal, "steps": steps})
                                yield {"Eb": Eb, "Ep1": Ep1, "Ep12": Ep12, "length": length, "T": T,
                                       "T_anneal": T_anneal,
                                       "steps": steps}



if __name__ == '__main__':
    start = time.time()
    print('Parent process %s.' % os.getpid())
    parameter_list = list(parameter_generator())

    try:
        # with ProcessPoolExecutor(max_workers=5) as p:
        with Pool(9) as p:
            p.map_async(Inclusion_Complex, parameter_list)
            # p.map_async(reconstruct, parameter_list)
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
