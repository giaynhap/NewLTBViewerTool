#ifndef GNMATRIXLIB
#define GNMATRIXLIB
#include <math.h>
#include <stdio.h>
#include <string.h>
#define MAXPT  16
typedef struct i_v2d
{
	int x;
	int y;
} i_v2d_t;
class gnMatrix
{

public:
	gnMatrix(int x, int y);
	gnMatrix();
	~gnMatrix();
	float data[MAXPT * MAXPT];

	gnMatrix operator^(int a);

	gnMatrix operator/(float a) const;


	gnMatrix operator*(float a) const;

	gnMatrix operator=(gnMatrix a);

	gnMatrix& operator*=(float a);
	gnMatrix& operator/=(float a);


	gnMatrix operator+(gnMatrix a) const;
	gnMatrix operator-(gnMatrix a) const;

	float& operator()(int a, int b);
	float operator()(int a, int b) const;

	bool operator+=(gnMatrix a);
	bool operator-=(gnMatrix a);

	gnMatrix operator*(gnMatrix a) const;
	bool operator*=(gnMatrix a);
	 
	float det();
	gnMatrix Inverse();
	int getcol()
	{
		return size.y;
	}
	int getrow()
	{
		return size.x;
	}
	void clean_data()
	{
		//	delete [] data;

		size.x = 0;
		size.y = 0;
	};
	template <size_t size_x, size_t size_y>
	void parse(float(&Array)[size_x][size_y])
	{
		if (size_x * size_y > MAXPT * 2) return;
		this->clean_data();
		size.x = size_x; size.y = size_y;
		for (int i = 0; i < size.x; i++)
		{
			for (int j = 0; j < size.y; j++)
				data[i * MAXPT + j] = Array[i][j];
		}
	}

protected:
	gnMatrix __getM(int x, int y);
	i_v2d_t size;

};

#endif
