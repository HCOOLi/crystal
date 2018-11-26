from pyroom import *
import time
from concurrent.futures import ProcessPoolExecutor
from multiprocessing import Pool
import os
import numpy as np


def tuning_washing_small(p):
    for Ep in np.arange(0.5, 3.0, 0.2):
        for Eb in np.arange(0, 0.2, 0.2):
            try:
                p.submit(washing_small, 1, Ep, Eb, Ep * 3)
            except:
                raise Exception("something wrong")



def reconstructs(p):
    for Ep in np.arange(1.4, 2.0, 0.2):
        for Eb in np.arange(0, 1.0, 0.2):
            try:
                # p.apply_async(reconstruct,(Ep, Eb, Ep * 30+5, 188))
                p.submit(reconstruct, Ep, Eb, Ep * 30 + 5, 188)
            except:
                print("***resonstucts catch the error")

                # raise Exception("something wrong")


def draw_pictures(p):
    for Ep in np.arange(2.0, 5.0, 1.0):
        for Eb in np.arange(3, 6, 1.0):
            k=60
            # for k in range(0, 64, 16):
            p.submit(draw_picture, Ep, Eb, Ep * 4, k)


def tuning_step_heating(p):
    for Ep in np.arange(2.0, 5.0, 1.0):
        for Eb in np.arange(0, 3.2, 0.4):
            p.submit(step_heating, 1, Ep, Eb, 10)


if __name__ == '__main__':
    start = time.time()
    print('Parent process %s.' % os.getpid())
    try:
        with ProcessPoolExecutor(max_workers=10) as p:
            # with Pool(5) as p:
            #################################################
            try:
                tuning_washing_small(p)
            except:
                print("***main catch the error")

            finally:
                # raise Exception("something wrong")
                print('Waiting for all subprocesses done...')

            #################################################
    except:
        print("shutdown")
        p.shutdown(wait=False)

    print('All subprocesses done.')
    end = time.time()
    print('Tasks runs %0.2f seconds.' % (end - start))
    # tuning_washing_small()
