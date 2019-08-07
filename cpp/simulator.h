#pragma once
#include<vector>
#include<functional>
#include"room.h"
using namespace std;
class Simulator {
public:
	/*template<class F, class... Args>*/
	/*vector<tuple<Args...> > parameter(F &&f,Args && ... args) {
		vector<tuple<Args...>> result;
		result.push_back(tuple<Args...>(args...));
		return result;
	}*/
	/*template<class F, class... Args>*/
	/*virtual void install_model(tuple<Args...> parameter) = 0;*/
	virtual void simulate()=0;

};
class SecondNuclear :public Simulator {
public:
	SecondNuclear(){}
	void install_model(Room &pyRoom) {
		//for i in range(0, r.shape[1]) :
		//	r.py_input_one_ECC([0, i, 8], 16, 2, [1] * 16, 1)
		//	# for i in range(0, r.shape[2], d) :
		//	#     r.py_input_one_ECC([15, 0, i], r.shape[1], 1, [0] * r.shape[1], 1)
		//	# for i in range(0, r.shape[2], 3) :
		//	#     r.py_input_one_ECC([62, 0, i], r.shape[1], 1, [0] * r.shape[1], 1)


		//	for i in range(2, r.shape[0] - 1) :
		//		# if i == 15 or i == 62:
		//		#     continue
		//		for j in range(0, r.shape[1] - 1, 2) :
		//			r.py_input_one_FCC([i, j, 0], 128, 2, 1, [1] * 128, 0)
	}
	void simulate(){}
	/*def __init__(self) :
		super(Simulator, self).__init__()
		if not os.path.exists('Complex') :
			os.mkdir('Complex')
			pass

			def parameters(self) :
			import itertools
			Ep = [0.5, 1.0, 2.0]
			length = [64]
			T = [4.5, 5, 5.2, 5.5]
			d = [0]
			return itertools.product(Ep, d, T)

			@staticmethod
			def install_model(r: pyRoom, d) :
			for i in range(0, r.shape[1]) :
				r.py_input_one_ECC([0, i, 8], 16, 2, [1] * 16, 1)
				# for i in range(0, r.shape[2], d) :
				#     r.py_input_one_ECC([15, 0, i], r.shape[1], 1, [0] * r.shape[1], 1)
				# for i in range(0, r.shape[2], 3) :
				#     r.py_input_one_ECC([62, 0, i], r.shape[1], 1, [0] * r.shape[1], 1)


				for i in range(2, r.shape[0] - 1) :
# if i == 15 or i == 62:
					#     continue
					for j in range(0, r.shape[1] - 1, 2) :
						r.py_input_one_FCC([i, j, 0], 128, 2, 1, [1] * 128, 0)


						@staticmethod
						def simulate(parameter) :

						try :
						Ep, d, T = parameter[0], parameter[1], parameter[2]
						print('Run task %f ,%f,%f(%s)...' % (Ep, 1, T, os.getpid()))
						start = time.time()
						EC_max = 31 * 31 * (31 - 1)
						if not os.path.exists('Data') :
							os.mkdir('Data')
							if not os.path.exists('Data/2019-6-18/') :
								os.mkdir('Data/2019-6-18/')
								r = pyRoom(64, 64, 64, Ep = [[0, 0], [0, Ep]], Eb = [[0, 0], [0, 0]])
								E_list, Ec_list, Ep_list, t_list = [], [], [], []

								SecondNuclear.install_model(r, d)
								# r.draw_all()
								r.movie(5000000, 100000, 100)
								# r.movie(2000000, 10000, T*Ep)
# # E_list, Ec_list, Ep_list, t_list, f = r.step_heating(6 * Ep+0.1, 1 * Ep, -0.1 * Ep,10000,5000, EC_max)
								r.save("Data/no-ECCheated%3.2f-d.json" % (Ep*T))
# # E_list, Ec_list, Ep_list, t_list, f = r.step_heating(6 * Ep+0.1, 1 * Ep, -0.1 * Ep+0.01,10000,5000, EC_max)
# # plt.plot(t_list, f)
# # plt.savefig("stepheating%3.2f.png" % (Ep))
								for i in range(100) :
									r.movie(20000, 1000, T * Ep)
									print("after movie%d" % (i))
									# E_result, Ec_result, Ep_result, Eb_result = r.get_result()
									# E_list += E_result
									# Ec_list += Ec_result
									# Ep_list += Ep_result
									r.save("Data/2019-6-18/d=%dE%d=%3.2f,T=%3.2f.json" % (d, i, Ep, T * Ep))

									# with open("Data/Ec_list,Ep2=%3.2f,T=%3.2f.json" % (Ep, T * Ep), 'w') as file :
#     # file.write(json.dumps(self.get_list()))
	#     file.write(json.dumps(Ec_list))

		except Exception as e :
	print(e)
		raise e

		return*/
};