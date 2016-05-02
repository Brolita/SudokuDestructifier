class Matrix 
{
	public:
		Matrix() {};
		void Initalize (int _x, int _y, float _i);
		Matrix(const Matrix& m);
		~Matrix();
		int x, y;
		inline float* operator[](int i) { return v[i]; }
	protected:
		float** v;
};