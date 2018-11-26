
import json
import math
import matplotlib.pyplot as plt
import os
import time
import numpy as np
from crystal import Room


class pyRoom(Room):
    def __init__(self, a, b, c, Ec=1.0, Ep=1.0, b2a=0.0, Eb=0.0):
        Room.__init__(self, a, b, c, Ec, Ep, b2a, 0.0, 0.0, Eb)
        self.shape = np.asarray([a, b, c])
        self.Ec = Ec
        self.Ep = Ep
        self.b2a = b2a

    def py_input_one_ECC(self, a, length, direction, ty):
        # int x,int y,int z, int length, int direction, int type
        self.input_one_ECC(int(a[0]), int(a[1]), int(a[2]), int(length), int(direction), int(ty))

    def py_inputECC(self, num, length):

        # vec
        start_point = self.shape / 2
        sqrt_num = math.ceil(math.sqrt(num))
        start_point[1] -= length / 2
        start_point[0] -= int(sqrt_num / 2)
        start_point[2] -= int(sqrt_num / 2)
        print(start_point)
        for i in range(0, sqrt_num):
            for j in range(0, sqrt_num):
                if i * sqrt_num + j < num:
                    self.py_input_one_ECC([start_point[0] + i, start_point[1], start_point[2] + j], length, 1, 0)

                else:
                    return

    def py_inputECC2(self, num, length):

        # vec
        start_point = self.shape / 2
        sqrt_num = math.ceil(math.sqrt(num))
        start_point[1] -= length / 2
        start_point[0] -= sqrt_num
        start_point[2] -= sqrt_num
        print(start_point)
        for i in range(0, sqrt_num):
            for j in range(0, sqrt_num):
                if i * sqrt_num + j < num:
                    self.py_input_one_ECC([start_point[0] + i * 2 - 1, start_point[1], start_point[2] + j * 2 - 1],
                                          length, 1, 0)

                else:
                    return

    def py_inputECC_with_small(self):

        # vec
        start_point = [0, 0, 0]
        num = 0
        self.a_layer = {}
        self.b_layer = {}
        self.c_layer = {}

        for k in range(0, int(self.shape[2] * 3 / 4), 2):
            self.c_layer[k] = []
        for i in range(0, int(self.shape[0])):
            self.a_layer[i] = []
        for j in range(0, int(self.shape[1])):
            self.b_layer[j] = []

        for i in range(self.shape[0]):
            for j in range(self.shape[1]):
                if j % 2 == 0 or i % 2 == 0:
                    for k in range(0, int(3 * self.shape[2] / 4), 2):
                        self.a_layer[i].append(num)
                        self.b_layer[j].append(num)
                        self.c_layer[k].append(num)
                        self.py_input_one_ECC([i, j, k + int(self.shape[2] / 8)], 2, 2, 1)

                        num += 1
                else:
                    self.py_input_one_ECC([i, j, int(self.shape[2] / 8)], int(3 * self.shape[2] / 4), 2, 0)
                    num += 1

                pass

    def remove_c_layer(self, k):
        for i in self.c_layer[k]:
            self.delete_chain(i)

    def remove_a_layer(self, k):
        if k >= self.shape[0]:
            return
        for i in self.a_layer[k]:
            self.delete_chain(i)

    def remove_b_layer(self, k):
        if k >= self.shape[1]:
            return
        for i in self.b_layer[k]:
            self.delete_chain(i)

    def save(self, file_path):
        with open(file_path, 'w') as file:
            file.write(json.dumps(self.get_list()))

    def py_input_stop_chain(self):
        for i in range(self.shape[0] - 1):
            for j in range(2):
                self.py_input_one_ECC([i, i + j, 0], self.shape[2], 2, 1)

    def py_input_stop_chain2(self):
        for i in range(self.shape[0]):
            self.py_input_one_ECC([0, i, 0], self.shape[2], 2, 1)

    def draw_box(self):
        from vpython import canvas, vector, curve, color
        radius = 0.05
        box_color = color.blue
        c = curve(vector(0, 0, 0), vector(0, 0, self.shape[2]), color=box_color, radius=radius)
        c = curve(vector(self.shape[0], 0, 0), vector(self.shape[0], 0, self.shape[2]), color=box_color, radius=radius)
        c = curve(vector(0, self.shape[1], 0), vector(0, self.shape[1], self.shape[2]), color=box_color, radius=radius)
        c = curve(vector(self.shape[0], self.shape[1], 0), vector(self.shape[0], self.shape[1], self.shape[2]),
                  color=box_color, radius=radius)

        c = curve(vector(0, 0, 0), vector(self.shape[0], 0, 0), color=box_color, radius=radius)
        c = curve(vector(0, 0, self.shape[2]), vector(self.shape[0], 0, self.shape[2]), color=box_color, radius=radius)
        c = curve(vector(0, self.shape[1], 0), vector(self.shape[0], self.shape[1], 0), color=box_color, radius=radius)
        c = curve(vector(0, self.shape[1], self.shape[2]), vector(self.shape[0], self.shape[1], self.shape[2]),
                  color=box_color, radius=radius)

        c = curve(vector(0, 0, 0), vector(0, self.shape[1], 0), color=box_color, radius=radius)
        c = curve(vector(self.shape[0], 0, 0), vector(self.shape[0], self.shape[1], 0), color=box_color, radius=radius)
        c = curve(vector(0, 0, self.shape[2]), vector(0, self.shape[1], self.shape[2]), color=box_color, radius=radius)
        c = curve(vector(self.shape[0], 0, self.shape[2]), vector(self.shape[0], self.shape[1], self.shape[2]),
                  color=box_color, radius=0.1)

    def draw(self, polylist=None, path=None):
        from vpython import canvas, vector, curve, color
        scene = canvas(title=path, width=800, height=800,
                       center=vector(self.shape[0] / 2, self.shape[1] / 2, self.shape[2] / 2), background=color.white)
        self.draw_box()
        if polylist == None:
            polylist = self.get_list()
        for chain in polylist:
            c = curve(color=color.yellow, radius=0.2)
            if chain:
                point2 = chain[0].copy()
            else:
                continue
            for point in chain:
                if (self.ifp1p2(point2, point)):
                    pass
                else:
                    c = curve(color=color.yellow, radius=0.2)
                c.append(vector(point[0], point[1], point[2]))
                point2 = point.copy()
        return scene

    def load_polymer(self, filepath):
        T = 0
        time = 1
        with open(filepath, 'r') as file:
            all_line_txt = file.readline()  # 读所有行
            polymerlist = json.loads(all_line_txt)
            return polymerlist

    def step_heating(self, start, end, step, EC_max):
        E_list, Ec_list, Ep_list, t_list = [], [], [], []
        for i in np.arange(start, end, step):
            print(i)
            self.movie(1000, 5, i)
            E, Ec, Ep = self.get_result()
            E_list += E
            Ec_list += Ec
            Ep_list += Ep
            t_list += [i] * 200
            # self.save(i, i*1000)
        f = []
        for i in Ec_list:
            f.append(-i / EC_max)
        return E_list, Ec_list, Ep_list, t_list, f

    def ifp1p2(self,point2, point1):
        for i in range(3):
            if (abs(point2[i] - point1[i]) > 1):
                return False

        return True


def reconstruct(Ep, Eb, T, k):
    try:
        r0 = pyRoom(32, 64, 32, Ec=1, Ep=Ep, b2a=0, Eb=Eb)
        num_of_chains = 16 * 16
        chain_length = 64
        EC_max = num_of_chains * (chain_length - 1)
        r0.py_inputECC(num_of_chains, chain_length)
        Ep0 = r0.cal_Ep()

        r = pyRoom(24, 24, 192, Ec=1, Ep=Ep, b2a=0, Eb=Eb)
        r.construct_by_pylist(r.load_polymer("chain356/chain-%d,%d,%d,%d.json" % (Ep * 10, Eb * 10, T, k)))
        r.draw(path="chain356/chain-%d,%d,%d,%d.json" % (Ep * 10, Eb * 10, T, k))
        thicka, thickb, thickc = r.cal_thick_by_point()

        print("结晶度：%3.1f%%" % (r.cal_Ep() / Ep0 * 100))
        print("f=%0.3f" % (r.cal_Ec() / EC_max))
        plt.title('a')
        thick_num_a, bins, _ = plt.hist(thicka)
        thick_weight_a = np.asarray(thick_num_a) * np.asarray(bins[1:])
        print(thick_weight_a)
        plt.bar(bins[1:], thick_weight_a)
        plt.show()
        plt.title('b')
        plt.hist(thickb)
        plt.show()
        plt.title('c')
        plt.hist(thickc)
        plt.show()
        os.system("pause")
    except:
        print("subprocess wrong")
        raise Exception("something wrong ")
    return







def room_task(Ec0, Ep0, Eb0, T0):
    # try:
    print('Run task %f ,%f,%f(%s)...' % (Ec0, Ep0, T0, os.getpid()))
    start = time.time()

    r0 = pyRoom(32, 64, 32, Ec=1, Ep=3, b2a=0, Eb=3)
    num_of_chains = 16 * 16
    chain_length = 64
    EC_max = num_of_chains * (chain_length - 1)
    r0.py_inputECC(num_of_chains, chain_length)
    print(r0.cal_Ep())

    r = pyRoom(32, 32, 128, Ec=1, Ep=3, b2a=0, Eb=3)

    r.construct_by_pylist(r.load_polymer("chain/chain-%d,%d,%d,%d.json" % (3 * 10, 10, 12, 60)))
    r.draw()
    thicka, thickb, thickc = r.cal_thick_by_point()

    print(r.cal_Ep())
    print(r.cal_Ec() / EC_max)
    # print(thicka),print(thickb),print(thickc)
    plt.hist(thickc)
    plt.show()
    return


def washing_small(Ec0, Ep0, Eb0, T0):
    print('Run task %f ,%f,%f(%s)...' % (Ep0, Eb0, T0, os.getpid()))
    start = time.time()

    r = pyRoom(24, 24, 192, Ec=Ec0, Ep=Ep0, b2a=0, Eb=Eb0)
    EC_max = 16*16 * (64 - 1)
    r.py_inputECC_with_small()

    for k in range(0, int(3*r.shape[2]/4), 4):
        r.remove_c_layer(k)
        r.remove_c_layer(k + 2)
        # r.remove_c_layer(k + 4)
        # r.remove_c_layer(k + 6)
        #     # r.remove_c_layer(k + 8)
        #     # r.remove_c_layer(k + 10)
        #     # r.remove_c_layer(k + 12)
        # r.draw()
        r.movie(30000, 20000, T0)
        r.save("chain256/chain-%d,%d,%d,%d.json" % (Ep0 * 10, Eb0 * 10, T0*10, k))

    end = time.time()
    print('Task%f ,%fruns %0.2f seconds.' % (Ec0, Ep0, (end - start)))

    return


def washing_small_a_b(Ec0, Ep0, Eb0, T0):
    # try:
    print('Run task a %f ,%f,%f(%s)...' % (Ep0, Eb0, T0, os.getpid()))
    start = time.time()

    r = pyRoom(32, 32, 128, Ec=Ec0, Ep=Ep0, b2a=0, Eb=Eb0)
    EC_max = 16 * 16 * (64 - 1)
    r.py_inputECC_with_small()

    for k in range(0, int(r.shape[0]), 2):
        r.remove_a_layer(k)
        r.remove_a_layer(k + 1)
        r.remove_b_layer(k)
        r.remove_b_layer(k + 1)
        r.remove_c_layer(k)
        # r.remove_c_layer(k + 2)
        #     # r.remove_c_layer(k + 4)
        #     # r.remove_c_layer(k + 6)
        #     # r.remove_c_layer(k + 8)
        #     # r.remove_c_layer(k + 10)
        #     # r.remove_c_layer(k + 12)
        r.movie(20000, 20000, T0)
        r.save("chainabc/chain-%d,%d,%d,%d.json" % (Ep0 * 10, Eb0 * 10, T0 * 10, k))

    end = time.time()
    print('Task%f ,%fruns %0.2f seconds.' % (Ec0, Ep0, (end - start)))

    return


def step_heating(Ec0, Ep0, Eb0, T0):
    # try:
    print('Run task %f ,%f,%f(%s)...' % (Ec0, Ep0, T0, os.getpid()))
    start = time.time()

    r0 = pyRoom(32, 64, 32, Ec=1, Ep=3, b2a=0, Eb=3)
    num_of_chains = 16 * 16
    chain_length = 64
    EC_max = num_of_chains * (chain_length - 1)
    r0.py_inputECC(num_of_chains, chain_length)
    print(r0.cal_Ep())

    r = pyRoom(32, 32, 128, Ec=1, Ep=3, b2a=0, Eb=3)

    # r.py_inputECC2(16*16,31)
    # r.draw()
    # r.py_inputECC_with_small()

    r.construct_by_pylist(r.load_polymer("chain/chain-%d,%d,%d,%d.json" % (3 * 10, 10, 12, 60)))
    r.draw()
    thicka, thickb, thickc = r.cal_thick_by_point()
    r
    print(r.cal_Ep())
    print(r.cal_Ec() / EC_max)
    # print(thicka),print(thickb),print(thickc)
    plt.hist(thickc)
    plt.show()
    # for k in range(0, int(r.shape[2]/2), 4):
    #     r.remove_c_layer(k)
    #     r.remove_c_layer(k + 2)
    # #     # r.remove_c_layer(k + 4)
    # #     # r.remove_c_layer(k + 6)
    # #     # r.remove_c_layer(k + 8)
    # #     # r.remove_c_layer(k + 10)
    # #     # r.remove_c_layer(k + 12)
    #     r.movie(5000,1000,T0)
    #     r.save("chain/chain-%d,%d,%d,%d.json"%(Ep0*10,Eb0*10,T0,k))
    #     #r.draw()
    # return
    # E_list, Ec_list, Ep_list, t_list,f=r.stepheating(1,8,0.1,EC_max)
    # fig2 = plt.figure()
    #
    # plt.plot(t_list,f)
    # #r.draw()
    # plt.savefig('f%d,%d,%d.png' % (Ec0 * 10, Ep0 * 10, T0 * 10))
    # plt.show()
    #
    # with open('f%d,%d,%d.json' % (Ec0 * 10, Ep0 * 10, T0 * 10), 'w') as file:
    #     file.write(json.dumps([t_list, f]))
    # end = time.time()
    # print('Task%f ,%fruns %0.2f seconds.' % (Ec0, Ep0, (end - start)))

    return

def draw_picture(Ep, Eb, T, k):
    r = pyRoom(32, 32, 128, Ec=1, Ep=1, b2a=0)
    filepath = "chain/chain-%d,%d,%d,%d.json" % (Ep * 10, Eb * 10, T, k)
    polymerlist = r.load_polymer(filepath)
    scence = r.draw(polymerlist, filepath)



