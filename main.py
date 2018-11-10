import os
import time
from multiprocessing import Pool
import json
from vpython import *
import math
import matplotlib.pyplot as plt

import numpy as np
from crystal import Room

class pyRoom(Room):
    def __init__(self, a, b, c, Ec=1.0, Ep=1.0,b2a=0.0):
        Room.__init__(self, a, b, c, Ec, Ep,b2a)
        self.shape = np.asarray([a, b, c])
        self.Ec=Ec
        self.Ep=Ep
        self.b2a=b2a

    def py_input_one_ECC(self,a,length,direction,ty):
        #int x,int y,int z, int length, int direction, int type
        self.input_one_ECC(int(a[0]),int(a[1]),int(a[2]),int(length),int(direction),int(ty))

    def py_inputECC(self, num, length):

        # vec
        start_point = self.shape / 2
        sqrt_num = math.ceil(math.sqrt(num))
        start_point[1] -= length / 2
        start_point[0] -= int(sqrt_num/2)
        start_point[2] -= int(sqrt_num/2)
        print(start_point)
        for i in range(0,sqrt_num):
            for j in range(0,sqrt_num):
                if i * sqrt_num + j < num:
                    self.py_input_one_ECC( [start_point[0] + i, start_point[1], start_point[2] + j], length, 1, 0)

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
                    self.py_input_one_ECC([start_point[0] + i*2-1, start_point[1], start_point[2] + j*2-1], length, 1, 0)

                else:
                    return

    def py_inputECC_with_small(self):

        # vec
        start_point = [0,0,0]
        num=0
        self.layer={}
        for k in range(0, int(self.shape[2]/2), 2):
            self.layer[k]=[]
        for i in range(self.shape[0]):
            for  j in range(self.shape[1]):
                if j%2==0 or i%2==0:
                    for k in range(0,int(self.shape[2]/2),2):
                        self.layer[k].append(num)
                        self.py_input_one_ECC([i,j,k+int(self.shape[2]/4)],2,2,1)

                        num+=1
                else:
                    self.py_input_one_ECC([i, j,int(self.shape[2]/4)], self.shape[2]/2, 2, 0)
                    num+=1


                pass

    def remove_a_layer(self,k):
        for i in self.layer[k]:
            self.delete_chain(i)


    def save(self,filepath):
        with open(filepath, 'w') as file:
            file.write(json.dumps(self.get_list()))

    def py_input_stop_chain(self):
        for i in range(self.shape[0]-1):
            for j in range(2):
                self.py_input_one_ECC([i, i + j, 0], self.shape[2], 2, 1)

    def py_input_stop_chain2(self):
        for i in range(self.shape[0]):
            self.py_input_one_ECC([0, i, 0], self.shape[2], 2, 1)

    def draw_box(self):
        radius=0.05
        box_color=color.blue
        c = curve(vector(0, 0, 0), vector(0, 0, self.shape[2]), color=box_color, radius=radius)
        c = curve(vector(self.shape[0], 0, 0), vector(self.shape[0], 0, self.shape[2]), color=box_color, radius=radius)
        c = curve(vector(0, self.shape[1], 0), vector(0, self.shape[1], self.shape[2]), color=box_color, radius=radius)
        c = curve(vector(self.shape[0], self.shape[1], 0), vector(self.shape[0], self.shape[1], self.shape[2]),
                  color=box_color, radius=radius)

        c = curve(vector(0, 0, 0), vector(self.shape[0], 0,0), color=box_color, radius=radius)
        c = curve(vector(0, 0,  self.shape[2]), vector(self.shape[0], 0,  self.shape[2]), color=box_color, radius=radius)
        c = curve(vector(0, self.shape[1], 0), vector(self.shape[0], self.shape[1],0), color=box_color, radius=radius)
        c = curve(vector(0, self.shape[1], self.shape[2]), vector(self.shape[0], self.shape[1], self.shape[2]),
                  color=box_color, radius=radius)

        c = curve(vector(0, 0, 0), vector(0, self.shape[1], 0), color=box_color, radius=radius)
        c = curve(vector(self.shape[0], 0, 0), vector(self.shape[0], self.shape[1], 0), color=box_color, radius=radius)
        c = curve(vector(0, 0, self.shape[2]), vector(0, self.shape[1], self.shape[2]), color=box_color, radius=radius)
        c = curve(vector(self.shape[0], 0, self.shape[2]), vector(self.shape[0], self.shape[1], self.shape[2]),
                  color=box_color, radius=0.1)

    def draw(self,polylist=None,path=None):

        scene = canvas(title=path,width=800, height=800,
                        center=vector(self.shape[0]/2, self.shape[1]/2, self.shape[2]/2), background=color.white)
        self.draw_box()
        if polylist==None:
            polylist = self.get_list()
        for chain in polylist:
            c = curve(color=color.yellow, radius=0.2)
            if chain:
                point2 = chain[0].copy()
            else:
                continue
            for point in chain:
                if (ifp1p2(point2, point)):
                 pass
                else:
                 c = curve(color=color.yellow, radius=0.2)
                c.append(vector(point[0], point[1], point[2]))
                point2 = point.copy()

    def loadf(self):
        with open('64/f %d,%d,%d.json' % (1 * 10, 1 * 10, 10 * 10), 'r') as file:
            all_line_txt = file.readline()  # 读所有行
            # print(all_line_txt)
            t, f = json.loads(all_line_txt)
            t = np.asarray(t)
            f = np.asarray(f)
            range1 = list(range(70000, 200000))
            z1 = np.polyfit(t[range1], f[range1], 1)
            p1 = np.poly1d(z1)
            print(z1)
            print(p1)
            f1 = p1(t[range1])
            range2 = list(range(310000, 370000))
            z2 = np.polyfit(t[range2], f[range2], 1)
            p2 = np.poly1d(z2)
            f2 = p2(t[range2])
            plt.plot(t, f)
            plt.plot(t[range1], f1, color='r')
            plt.plot(t[range2], f2, color='r')
            plt.xlabel("kT/Ec")
            plt.ylabel("f")
            plt.ylim(0, 1)
            plt.show()
            print(get_cross(z1, z2))

    def loadpolymer(self,filepath):
        T=0
        time=1
        with open(filepath, 'r') as file:
            all_line_txt = file.readline()  # 读所有行
            # print(all_line_txt)
            polymerlist= json.loads(all_line_txt)
            self.draw(polymerlist,filepath)

    def stepheating(self,start,end,step,EC_max):
        E_list, Ec_list, Ep_list, t_list = [], [], [], []
        for i in np.arange(start, end, step):
            print(i)
            self.movie(1000, 5, i)
            E, Ec, Ep = self.get_result()
            E_list += E
            Ec_list += Ec
            Ep_list += Ep
            t_list += [i] * 200
            #self.save(i, i*1000)
        f = []
        for i in Ec_list:
            f.append(-i / EC_max)
        return E_list, Ec_list, Ep_list, t_list,f



def roomtask(Ec0, Ep0, T0):
    # try:
    print('Run task %f ,%f,%f(%s)...' % (Ec0, Ep0, T0, os.getpid()))
    start = time.time()

    r = pyRoom(32, 32, 128, Ec=1, Ep=Ep0,b2a=0)
    num_of_chains=31*31
    chain_length=32
    EC_max = num_of_chains*(chain_length-1)
    # r.py_inputECC(num_of_chains,chain_length)
    r.py_inputECC_with_small()
    for k in range(0, int(r.shape[2]/2), 4):
        r.remove_a_layer(k)
        r.remove_a_layer(k + 2)
        # r.remove_a_layer(k + 4)
        # r.remove_a_layer(k + 6)
        # r.remove_a_layer(k + 8)
        # r.remove_a_layer(k + 10)
        # r.remove_a_layer(k + 12)
        r.movie(10000,1000,3)
        r.save("chain-%d,%d,%d.json"%(Ep0*10,3,k))
        #r.draw()
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

def drawpictures(Ep,T,k):

    r = pyRoom(32, 32, 128, Ec=1, Ep=1, b2a=0)
    #for k in range(0, int(r.shape[2] / 2), 4):
    r.loadpolymer("chain/chain-%d,%d,%d.json"%(Ep*10,T,k))


def ifp1p2(point2,point1):
    for i in range(3):
        if(abs(point2[i]-point1[i])>1):
            return False
    return True

def draw(point1,point2):

    pass
    # with open('f %d,%d,%d.json' % (Ec0 * 10, Ep0 * 10, T0 * 10), 'r') as file:
    #     file.write(json.dumps([t_list, f]))




# TODO

#
if __name__ == '__main__':
    # roomtask(1,1,10)
    drawpictures(2.0,3,32)
    # return
    # #
    # start = time.time()
    # p = Pool(7)
    # # # print('Parent process %s.' % os.getpid())
    # #p = Pool(4)
    # for i in np.arange(0, 2.2, 0.2):
    #     p.apply_async(roomtask, args=(1, i, 10))
    # # # print('Waiting for all subprocesses done...')
    # p.close()
    # p.join()
    # # print('All subprocesses done.')
    # end = time.time()
    # print('Tasks runs %0.2f seconds.' % (end - start))
