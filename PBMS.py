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


class PBS_PBMS(Simulator):
    def __init__(self):
        super(Simulator, self).__init__()

        pass

    def parameters(self):
        pass

    def run(self, parameter):
        Ep1, Ep12, Eb, T, length = parameter["Ep1"], parameter["Ep12"], parameter["Eb"], parameter["T"], parameter[
            "length"]

        pass
