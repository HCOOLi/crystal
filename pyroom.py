
import json
import math
import matplotlib.pyplot as plt
import os
import time
import numpy as np
from crystal import Room


class pyRoom(Room):
    def __init__(self, a, b, c, Ec=1.0, Ep=1.0, b2a=0.0, Eb=0.0):
        Room.__init__(self, int(a), int(b), int(c), float(Ec), Ep, b2a, 0.0, 0.0, Eb)
        self.shape = np.asarray([a, b, c])
        self.Ec = Ec
        self.Ep = Ep
        self.Eb = Eb
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
        parameters = {"Ep": self.Ep, "Ec": self.Ec, "Eb": self.Eb}
        with open(file_path, 'w') as file:
            file.write(json.dumps(self.get_list()))

    def py_input_stop_chain(self):
        for i in range(self.shape[0] - 1):
            for j in range(2):
                self.py_input_one_ECC([i, i + j, 0], self.shape[2], 2, 1)

    def py_input_stop_chain2(self):
        for i in range(self.shape[0]):
            self.py_input_one_ECC([0, i, 0], self.shape[2], 2, 1)

    def new_draw_box(self, point1, point2, box_color='blue'):
        from vpython import canvas, vector, curve, color
        # def into_vector(a):
        #     return vector(a[0], a[1], a[2])

        p1 = [0, 0, 0]
        p2 = [0, 0, 0]
        radius = 0.05
        if box_color == 'blue':
            box_color = color.blue
        if box_color == 'red':
            box_color = color.red
        for a in range(2):
            for b in range(2):
                for c in range(3):
                    p1[c], p2[c] = point1[c], point2[c]

                    p1[(c + 1) % 3] = point1[(c + 1) % 3] if a == 0 else point2[(c + 1) % 3]
                    p2[(c + 1) % 3] = point1[(c + 1) % 3] if a == 0 else point2[(c + 1) % 3]
                    p1[(c + 2) % 3] = point1[(c + 2) % 3] if b == 0 else point2[(c + 2) % 3]
                    p2[(c + 2) % 3] = point1[(c + 2) % 3] if b == 0 else point2[(c + 2) % 3]
                    c = curve(vector(p1[0], p1[1], p1[2]), vector(p2[0], p2[1], p2[2]), color=box_color, radius=radius)

    def draw_box(self):
        from vpython import canvas, vector, curve, color
        self.new_draw_box([0, 0, 0], self.shape)

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

    def cal_crystal(self):

        r0 = pyRoom(self.shape[0], self.shape[2], self.shape[1], Ec=1, Ep=self.Ep, b2a=0, Eb=self.Eb)
        num_of_chains = self.shape[0] * self.shape[1] / 4
        chain_length = self.shape[2] / 4 * 3
        EC_max = num_of_chains * (chain_length - 1)
        r0.py_inputECC(num_of_chains, chain_length)
        Ep0 = r0.cal_Ep()

        thicka, thickb, thickc = self.cal_thick_by_point()
        print('Ep=%f,Eb=%f,length=%f   a' % (self.Ep, self.Eb, self.shape[2]))
        print("Ep结晶度：%3.1f%%" % (self.cal_Ep() / Ep0 * 100))
        print("Ec结晶度：%3.1f%%" % ((self.cal_Ec() / EC_max + 1) * 100))
        print("max=%d" % max(thickc))
        plt.title('Ep=%f,Eb=%f,length=%f   a' % (self.Ep, self.Eb, self.shape[2]))
        plt.hist(thicka)
        plt.show()
        plt.title('Ep=%f,Eb=%f,length=%f   b' % (self.Ep, self.Eb, self.shape[2]))
        plt.hist(thickb)
        plt.show()
        plt.title('Ep=%f,Eb=%f,length=%f   c' % (self.Ep, self.Eb, self.shape[2]))
        plt.hist(thickc)
        plt.show()

    def py_cal_thickness(self):
        thicknesslist = self.cal_thickness()
        plot_list = []
        for lammellar in thicknesslist:
            a = lammellar[0] - lammellar[3]
            b = lammellar[1] - lammellar[4]
            c = lammellar[2] - lammellar[5]
            self.new_draw_box([lammellar[0], lammellar[1], lammellar[2]], [lammellar[3], lammellar[4], lammellar[5]],
                              'red')
            plot_list.append([(a + b) / 2, c])

        plt.figure()
        plt.title('Ep=%f,Eb=%f,length=%f  ' % (self.Ep, self.Eb, self.shape[2]))
        plt.xlabel('(a+b)/2')
        plt.ylabel('c')
        plt.scatter(x=np.asarray(plot_list)[:, 0], y=np.asarray(plot_list)[:, 1])
        plt.show()


def reconstruct(parameter):
    Ep, Eb, T, length, T_anneal = parameter["Ep"], parameter["Eb"], parameter["T"], \
                                  parameter["length"], parameter["T_anneal"]
    k = length * 3 / 4 - 4
    if T_anneal != 0:
        loadpath = "step20000/chain%d/chain-%d,%d,%d,%d-annealed in%d.json" % \
                   (length, Ep * 10, Eb * 10, T * 10, k, T_anneal * 10)
    else:
        loadpath = "step20000/chain%d/chain-%d,%d,%d,%d.json" % (length, Ep * 10, Eb * 10, T * 10, k)


    try:
        print('Run task Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (Ep, Eb, T, length, os.getpid()))
        r = pyRoom(32, 32, length, Ep=Ep, b2a=0, Eb=Eb)
        r.construct_by_pylist(r.load_polymer(filepath=loadpath))
        r.draw(path=loadpath)
        # r.py_cal_thickness()
        r.cal_crystal()



        os.system("pause")
    except Exception as e:
        print(e)
        print("subprocess wrong")
        raise Exception("subprocess error ")
    # return


def anneal(parameter):
    Ep, Eb, T, length, T_anneal = parameter["Ep"], parameter["Eb"], parameter["T"], \
                                  parameter["length"], parameter["T_anneal"]

    k = length * 3 / 4 - 4
    loadpath = "chain%d/chain-%d,%d,%d,%d.json" % (length, Ep * 10, Eb * 10, T * 10, k)
    savepath = "chain%d/chain-%d,%d,%d,%d-annealed in%d.json" % \
               (length, Ep * 10, Eb * 10, T * 10, k, T_anneal * 10)
    try:
        print('Run task Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (Ep, Eb, T, length, os.getpid()))
        r = pyRoom(32, 32, length, Ep=Ep, b2a=0, Eb=Eb)
        r.construct_by_pylist(r.load_polymer(loadpath))

        r.movie(30000 * int(length / 12), 20000, T_anneal)
        r.save(savepath)
        # r.draw(path="chain%d/chain-%d,%d,%d,%d.json" % (length, Ep * 10, Eb * 10, T * 10, k))
        # r.cal_crystal()

    except Exception as e:
        print(e)
        print("subprocess wrong")
        raise Exception("subprocess error ")
    # return


def washing_small(parameter):
    try:
        Ep, Eb, T, length = parameter["Ep"], parameter["Eb"], parameter["T"], parameter["length"]
        print('Run task Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (Ep, Eb, T, length, os.getpid()))
        start = time.time()

        r = pyRoom(24, 24, length, Ep=Ep, b2a=0, Eb=Eb)
        r.py_inputECC_with_small()
        if not os.path.exists('chain%d' % length):
            os.mkdir('chain%d' % length)
        for k in range(0, int(3 * r.shape[2] / 4), 4):
            r.remove_c_layer(k)
            r.remove_c_layer(k + 2)
            # r.remove_c_layer(k + 4)
            # r.remove_c_layer(k + 6)
            #     # r.remove_c_layer(k + 8)
            #     # r.remove_c_layer(k + 10)
            #     # r.remove_c_layer(k + 12)
            # r.draw()
            r.movie(int(30000 * length / 128), 20000, T)
            r.save("chain%d/chain-%d,%d,%d,%d.json" % (length, Ep * 10, Eb * 10, T * 10, k))

        end = time.time()
        print('Task%fruns %0.2f seconds.' % (Ep, (end - start)))
    except Exception as e:
        print(e)
        raise Exception("subprocess error")

    return


def washing_small_a_b(parameter):
    Ep, Eb, T, length = parameter["Ep"], parameter["Eb"], parameter["T"], parameter["length"]

    # try:
    print('Run task Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (Ep, Eb, T, length, os.getpid()))
    start = time.time()

    r = pyRoom(32, 32, 128, Ec=1, Ep=Ep, b2a=0, Eb=Eb)
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
        r.movie(20000, 20000, T)
        r.save("chainabc/chain-%d,%d,%d,%d.json" % (Ep * 10, Eb * 10, T * 10, k))

    end = time.time()
    print('Task%f ,%fruns %0.2f seconds.' % (Ec, Ep, (end - start)))

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

    return

def draw_picture(Ep, Eb, T, k):
    r = pyRoom(32, 32, 128, Ec=1, Ep=1, b2a=0)
    filepath = "chain/chain-%d,%d,%d,%d.json" % (Ep * 10, Eb * 10, T, k)
    polymerlist = r.load_polymer(filepath)
    scence = r.draw(polymerlist, filepath)



