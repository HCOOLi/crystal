from vpython import *
import numpy as np
from main import drawpictures
if __name__=="__main__":
    Ep=1.6
    for i in np.arange(0, 64, 16):
            temperature=(Ep*5+0.5)*0.5*10
            drawpictures(Ep, temperature, i)