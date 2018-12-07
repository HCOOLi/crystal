#include"ConnectedComponent.h"

namespace matrix {
	ostream & operator<<(ostream & o, Matrix3 m)
	{
		for (int i = 0; i < m.shape[0]; i++) {
			for (int j = 0; j < m.shape[1]; j++) {
				for (int k = 0; k < m.shape[2]; k++) {
					o << m[i][j][k] << ',';
				}
				o << endl;
			}
			o << endl;
		}
		return o;
	}

	ostream & operator<<(ostream & o, Matrix2 m)
	{
		for (int i = 0; i < m.shape[0]; i++) {
			for (int j = 0; j < m.shape[1]; j++) {
				o << m[i][j] << ',';
			}
			o << endl;
		}
		return o;
	}

	py::list labeling_nearby(const Matrix3 & bitmap, Matrix3 & label, stack<vec3 > & que, int index) {
		static  vector<vec3> direction = { {-1,0,0},{1,0,0},{0,-1,0},{0,1,0},{0,0,-1} ,{0,0,1} };
		array<int, 6> a{ 0,0,0,bitmap.shape[0],bitmap.shape[1] ,bitmap.shape[2] };
		while (!que.empty())
		{
			vec3 seed = que.top();
			que.pop();
			update_min_max(a, seed);
			for (auto &dir : direction) {
				vec3 next = seed + dir;
				if (next >= vec3{ 0,0,0 } && next < bitmap.shape) {
					if (label[next] == 0) {
						if (bitmap[next] == 1) {
							label[next] = index;
							update_min_max(a, next);
							que.emplace(next);
						}
						else {
							label[next] = -1;
						}
					}
				}
			}
		}
		py::list * results = new py::list;
		for (int i = 0; i < 6; i++)
			 results->append(a[i]) ;
		return *results;

	}

	py::list ConnectedComponentLabeling(const Matrix3 & bitmap) {
		Matrix3 label(bitmap.shape);
		stack<vec3 > que;
		py::list *results = new py::list;
		int label_index = 0;
		for (int i = 0; i < bitmap.shape[0]; i++) {
			for (int j = 0; j < bitmap.shape[1]; j++) {
				for (int k = 0; k < bitmap.shape[2]; k++) {
					if (label[i][j][k] == 0) {
						if (bitmap[i][j][k] == 1) {

							label_index++;
							label[i][j][k] = label_index;
							que.emplace(vec3{ i,j,k });
							results->append(labeling_nearby(bitmap, label, que, label_index));
						}
						else {
							label[i][j][k] = -1;
						}
					}
				}

			}
		}
		return *results;
	}
}