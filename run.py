from pyroom import *
import time
from multiprocessing import Pool
import os
import numpy as np
def tuning_washing_small():
    start = time.time()
    p = Pool(11)
    print('Parent process %s.' % os.getpid())
    for Ep in np.arange(2.0, 6.0, 1.0):
        for Eb in np.arange(0, 3.2, 0.2):
            p.apply_async(washing_small, args=(1, Ep, Eb, Ep*4))
    print('Waiting for all subprocesses done...')
    p.close()
    p.join()
    print('All subprocesses done.')
    end = time.time()
    print('Tasks runs %0.2f seconds.' % (end - start))

def draw_pictures():
    start = time.time()
    p = Pool(2)
    print('Parent process %s.' % os.getpid())
    for Ep in np.arange(2.0, 5.0, 1.0):
        for Eb in np.arange(3, 6, 1.0):
            k=60
            # for k in range(0, 64, 16):
            p.apply_async(draw_picture, args=(Ep, Eb, Ep * 4, k))
    print('Waiting for all subprocesses done...')
    p.close()
    p.join()
    print('All subprocesses done.')
    end = time.time()
    print('Tasks runs %0.2f seconds.' % (end - start))

def tuning_step_heating():
    start = time.time()
    p = Pool(11)
    print('Parent process %s.' % os.getpid())
    for Ep in np.arange(2.0, 5.0, 1.0):
        for Eb in np.arange(0, 3.2, 0.4):
            p.apply_async(step_heating, args=(1, Ep, Eb, 10))
    print('Waiting for all subprocesses done...')
    p.close()
    p.join()
    print('All subprocesses done.')
    end = time.time()
    print('Tasks runs %0.2f seconds.' % (end - start))

if __name__ == '__main__':

    # washing_small(1, 3, 1, 3 * 4)
    tuning_washing_small()

