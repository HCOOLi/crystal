from pyroom import *
import time
from multiprocessing import Pool
import os
import numpy as np


def tuning_washing_small(p):
    for Ep in np.arange(0, 2, 0.1):
        for Eb in np.arange(0, 1, 0.1):
            p.apply_async(washing_small, args=(1, Ep, Eb, Ep * 3+0.3))


def reconstructs(p):

    for Ep in np.arange(1, 4.0, 0.3):
        for Eb in np.arange(0.2, 3.1, 0.3):
            p.apply_async(reconstruct, args=(Ep, Eb, Ep * 3, 92))


def draw_pictures(p):
    for Ep in np.arange(2.0, 5.0, 1.0):
        for Eb in np.arange(3, 6, 1.0):
            k=60
            # for k in range(0, 64, 16):
            p.apply_async(draw_picture, args=(Ep, Eb, Ep * 4, k))


def tuning_step_heating(p):
    for Ep in np.arange(2.0, 5.0, 1.0):
        for Eb in np.arange(0, 3.2, 0.4):
            p.apply_async(step_heating, args=(1, Ep, Eb, 10))


if __name__ == '__main__':
    start = time.time()
    p = Pool(11)
    print('Parent process %s.' % os.getpid())
    #################################################
    # reconstructs(p)
    tuning_washing_small(p)

    #################################################
    print('Waiting for all subprocesses done...')
    p.close()
    p.join()
    print('All subprocesses done.')
    end = time.time()
    print('Tasks runs %0.2f seconds.' % (end - start))
    # tuning_washing_small()
