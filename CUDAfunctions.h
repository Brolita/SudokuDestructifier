class CUDAfunctions 
{
	static __global__ assign(float* a, float* v) 
	{
		a[threadIdx.x] = (*b);
	}
	
	static __global__ add(float* a, float* b, float* out) 
	{
		out[threadIdx.x] = a[threadIdx.x] + b[threadIdx.x];
	}
	
	static __global__ sub(float* a, float* b, float* out) 
	{
		out[threadIdx.x] = a[threadIdx.x] - b[threadIdx.x];
	}
	
	
	static __global__ mult(float* a, float* b, float* out) 
	{
		out[threadIdx.x] = a[threadIdx.x] * b[threadIdx.x];
	}
	
	static __global__ cross(float* a, float* b, int* s, float* out) 
	{
		out[threadIdx.x] = a[threadIdx.x/(*s1)] * b[threadIdx.x%(*s1)];
	}
};