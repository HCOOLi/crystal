
import json
import math
import matplotlib.pyplot as plt
import os
import time
import numpy as np
from crystal import Room


class pyRoom(Room):
    def __init__(self, a, b, c, Ep, Eb):
        Room.__init__(self, int(a), int(b), int(c), Ep, Eb)
        self.shape = np.asarray([a, b, c])
        self.Ep = Ep
        self.Eb = Eb

    def py_input_one_ECC(self, a, length, direction, ty, movable):
        # int x,int y,int z, int length, int direction, int type
        self.input_one_ECC(int(a[0]), int(a[1]), int(a[2]), int(length), int(direction), int(ty), int(movable))

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
                        self.py_input_one_ECC([i, j, k + int(self.shape[2] / 8)], 2, 2, 2, 0)

                        num += 1
                else:
                    # self.py_input_one_ECC([i, j, int(self.shape[2] / 8)], int(3 * self.shape[2] / 4), 2, 1, 0)
                    # self.py_input_one_ECC([i, j, 1], int(self.shape[2] - 2), 2, 1, 0)
                    self.py_input_one_ECC([i, j, 1], int(self.shape[2] / 2), 2, 1, 0)
                    num += 1

                pass

    def remove_c_layer(self):

        for k in range(0, int(self.shape[2] * 3 / 4), 2):
            for i in self.c_layer[k]:
                self.delete_chain(i)
            yield
        return
        #   for i in self.c_layer[k]:
        #       self.delete_chain(i)
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
        parameters = {"Ep": self.Ep, "Eb": self.Eb}
        saving_dict = {"parameters": parameters, "data": self.get_list()}
        with open(file_path, 'w') as file:
            # file.write(json.dumps(self.get_list()))
            file.write(json.dumps(saving_dict))
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
        # print("draw a box")
        p1 = [0, 0, 0]
        p2 = [0, 0, 0]
        radius = 0.04
        if box_color == 'blue':
            box_color = color.black
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

    def draw(self, polylist=None, title=None):
        from vpython import canvas, vector, curve, color, sphere

        scene = canvas(title=title, width=800, height=800,
                       center=vector(self.shape[0] / 2, self.shape[1] / 2, self.shape[2] / 2), background=color.white)
        self.draw_box()
        nums = self.get_num_of_polymers()
        for i in range(nums):
            chain = self.get_polymer(i)
            chaintype = chain.get_type()
            # print(chaintype)

            # for chain in polylist:
            if chaintype == 1:
                this_color = color.yellow
            else:
                this_color = color.blue
            c = curve(color=this_color, radius=0.1)
            if chain:
                point2 = chain.get_list()["chain"][0].copy()['position']
            else:
                continue
            for point in chain.get_list()["chain"]:
                point = point['position']
                if (self.if_out_of_range(point2, point)):
                    pass
                else:
                    c = curve(color=this_color, radius=0.1)
                c.append(vector(point[0], point[1], point[2]))
                point2 = point.copy()
        return scene

    def load_polymer(self, filepath):
        T = 0
        time = 1
        with open(filepath, 'r') as file:
            all_line_txt = file.readline()  # 读所有行
            polymerlist = json.loads(all_line_txt)
            try:
                return polymerlist['data']
            except:
                return polymerlist

    def step_heating(self, start, end, step, EC_max):
        E_list, Ec_list, Ep_list, t_list = [], [], [], []
        for i in np.arange(start, end, step):
            self.movie(100000, 5000, i)
            E, Ec, Ep, Eb = self.get_result()
            E_list += E
            Ec_list += Ec
            Ep_list += Ep
            t_list += [i] * 20
            # self.save(i, i*1000)
        f = []
        for i in Ec_list:
            f.append(-i / EC_max)
        return E_list, Ec_list, Ep_list, t_list, f

    def if_out_of_range(self, point2, point1):
        for i in range(3):
            if (abs(point2[i] - point1[i]) > 1):
                return False

        return True

    def cal_crystal(self):

        r0 = pyRoom(self.shape[0], self.shape[2], self.shape[1], Ep=self.Ep, Eb=self.Eb)
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
        print(thicknesslist)
        plot_list = []
        sort_list = []
        for lammellar in thicknesslist:
            if lammellar:
                a = lammellar[0] - lammellar[3]
                b = lammellar[1] - lammellar[4]
                c = lammellar[2] - lammellar[5]
                self.new_draw_box([lammellar[0], lammellar[1], lammellar[2]],
                                  [lammellar[3], lammellar[4], lammellar[5]],
                                  'red')
                plot_list.append([b, c])
                sort_list.append(c)
        sort_list.sort(reverse=True)

        plt.figure()
        plt.title('Ep=%f,Eb=%f,length=%f  ' % (self.Ep, self.Eb, self.shape[2]))
        plt.xlabel('b')
        plt.ylabel('c')
        plt.scatter(x=np.asarray(plot_list)[:, 0], y=np.asarray(plot_list)[:, 1])
        plt.show()
        return np.mean(np.asarray(sort_list[:5]))


def reconstruct(parameter):
    Ep, Eb, T, length, T_anneal,steps = parameter["Ep"], parameter["Eb"], parameter["T"], \
                                  parameter["length"], parameter["T_anneal"],parameter["steps"]
    k = length * 3 / 4 - 4
    if T_anneal != 0:
        loadpath = "steps%d/chain%d/chain-%d,%d,%d,%d-annealed in%d.json" % \
                   (steps,length, Ep * 10, Eb * 10, T * 10, k, T_anneal * 10)
        # loadpath = "chain%d/chain-%3.2f,%3.2f,%3.2f,%d-annealed in%3.2f.json" % \
        #            (length, Ep, Eb, T, k, T_anneal)
    else:
        # loadpath = "steps%d/chain%d/chain-%d,%d,%d,%d.json" % (steps,length, Ep * 10, Eb * 10, T * 10, k)
        # loadpath = "k=4/steps%d/chain%d/chain-%3.2f,%3.2f,%3.2f,%d.json" % (steps, length, Ep, Eb, T, k)
        # loadpath = "k=4/steps%d/chain%d/chain-%3.2f,%3.2f,%3.2fend.json" % (steps, length, Ep, Eb, T)
        loadpath = "Complex/chain-%3.2f.json" % (T)

    try:
        print('Run task steps=%d Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (steps, Ep, Eb, T, length, os.getpid()))
        r = pyRoom(32, 32, 128, Ep=[[0, 0, 0], [0, 1, 2], [0, 2, 1]], Eb=[[0, 0, 0], [0, 0, 0], [0, 0, 0]])
        r.construct_by_pylist(r.load_polymer(filepath=loadpath))
        # r.draw(path=loadpath)
        r.draw(title=loadpath)
        print(r.py_cal_thickness())

        # r.cal_crystal()



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
    # loadpath = "chain%d/chain-%3.2f,%3.2f,%3.2f,%d.json" % (length, Ep, Eb, T, k)
    savepath = "chain%d/chain-%3.2f,%3.2f,%3.2f,%d-annealed in%3.2f.json" % \
               (length, Ep, Eb, T, k, T_anneal)
    try:
        print('Run task Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (Ep, Eb, T, length, os.getpid()))
        r = pyRoom(32, 32, length, Ep=Ep, Eb=Eb)
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
        Ep, Eb, T, length, steps = parameter["Ep"], parameter["Eb"], parameter["T"], parameter["length"], parameter[
            "steps"]
        print('Run task Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (Ep, Eb, T, length, os.getpid()))
        start = time.time()

        r = pyRoom(24, 24, length, Ep=Ep, Eb=Eb)
        r.py_inputECC_with_small()
        if not os.path.exists('steps%d' % steps):
            os.mkdir('steps%d' % steps)
        if not os.path.exists('steps%d/chain%d' % (steps, length)):
            os.mkdir('steps%d/chain%d' % (steps, length))
        k = 0
        for i, _ in enumerate(r.remove_c_layer()):
            if (i % 4) == 0:
                k = i * 2
                r.movie(steps, 20000, T)

                if k % 24 == 0:
                    print("steps%d/chain%d/chain-%3.2f,%3.2f,%3.2f,%d.json" % (steps, length, Ep, Eb, T, k))
                    r.save("steps%d/chain%d/chain-%3.2f,%3.2f,%3.2f,%d.json" % (steps, length, Ep, Eb, T, k))
                pass

        r.save("steps%d/chain%d/chain-%3.2f,%3.2f,%3.2fend.json" % (steps, length, Ep, Eb, T))
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

    r = pyRoom(32, 32, 128, Ep=Ep, Eb=Eb)
    EC_max = 16 * 16 * (64 - 1)
    r.py_inputECC_with_small()

    for k in range(0, int(r.shape[0]), 2):
        r.remove_a_layer(k)
        r.remove_a_layer(k + 1)
        r.remove_b_layer(k)
        r.remove_b_layer(k + 1)
        r.remove_c_layer()
        # r.remove_c_layer(k + 2)
        #     # r.remove_c_layer(k + 4)
        #     # r.remove_c_layer(k + 6)
        #     # r.remove_c_layer(k + 8)
        #     # r.remove_c_layer(k + 10)
        #     # r.remove_c_layer(k + 12)
        r.movie(20000, 20000, T)
        r.save("chainabc/chain-%d,%d,%d,%d.json" % (Ep * 10, Eb * 10, T * 10, k))

    end = time.time()
    print('Task%f ,%fruns %0.2f seconds.' % (Ep, (end - start)))

    return


def Inclusion_Complex(parameter):
    Ep1, Ep12, Eb, T, length = parameter["Ep1"], parameter["Ep12"], parameter["Eb"], parameter["T"], parameter["length"]

    # try:
    print('Run task Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (Ep1, Eb, T, length, os.getpid()))
    start = time.time()
    if not os.path.exists('Complex'):
        os.mkdir('Complex')

    r = pyRoom(24, 24, 24, Ep=[[0, 0, 0], [0, 0, Ep12], [0, Ep12, Ep1]], Eb=[[0, 0, 0], [0, 0, 0], [0, 0, 0]])
    r.py_inputECC_with_small()

    r.movie(200000, 1, 100)
    # r.draw(title="chain-%3.2f.json" % (T))
    # r.save("Complex/chain-%3.2raw.json" % (T))
    ###########################
    r.movie(2000000, 1, T)
    # r.draw(title="chain-%3.2f.json" % (T))
    r.save("Complex/length/chainEp-0,%3.2f,%3.2f,T=%3.2f.json" % (Ep1, Ep12, T))

    end = time.time()
    print('Task%f runs %0.2f seconds.' % (T, (end - start)))

    return
def step_heating(parameter):
    try:
        Ep, Eb, T, length = parameter["Ep"], parameter["Eb"], parameter["T"], parameter["length"]
        print('Run task %f ,%f,%f(%s)...' % (Ep, Eb, T, os.getpid()))
        start = time.time()
        EC_max = 16 * 16 * (96 - 1)

        r = pyRoom(32, 32, length, Ep=Ep, Eb=Eb)
        r.py_inputECC_with_small()
        for _ in r.remove_c_layer():
            pass
        print("removed all")
        E_list, Ec_list, Ep_list, t_list, f = r.step_heating(6 * Ep, 1 * Ep, -0.1 * Ep, EC_max)
        plt.plot(t_list, f)
        plt.savefig("stepheating%3.2f,%3.2f.png" % (Ep, Eb))
        plt.show()

    except Exception as e:
        print(e)
        raise e



    return



