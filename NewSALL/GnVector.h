
#ifndef GNVECTORLIB
#define GNVECTORLIB
#include <math.h>

#include "GnMatrix.h"

#ifndef MAXPT 
# define MAXPT          16
#endif

typedef unsigned char byte;
class gnVector
{

public:
	float data[MAXPT];
	byte size;
	gnVector(int size);
	gnVector();
	~gnVector();
	bool operator=(gnVector a);
	bool operator==(gnVector a);
	bool operator!=(gnVector a);
	gnVector operator+(gnVector vec) const;
	gnVector operator-(gnVector vec) const;
	gnVector operator*(float a) const;
	gnVector operator/(float a) const;
	float Length();

	float Normalize();
	float operator[](int a);
	float operator*(gnVector vec) const;

	gnVector& operator+=(gnVector vec);
	gnVector& operator-=(gnVector vec);
	gnVector& operator/=(float a);
	gnVector& operator*=(float a);
	gnMatrix toMatrix(float b)
	{
		gnMatrix rMatrix(4, 1);
		for (int i = 0; i < size; i++)
			rMatrix.data[i * MAXPT + 0] = data[i];
		rMatrix.data[3 * MAXPT + 0] = b;
		return rMatrix;
	};
	void fromMatrix(gnMatrix m)
	{
		size = m.getrow();

		for (int i = 0; i < size; i++)
			data[i] = m.data[i * MAXPT + 0];

	}
	template <size_t size_x>
	void parse(float(&arr)[size_x])
	{
		size = size_x;

		for (int i = 0; i < size_x; i++)
			data[i] = arr[i];
	};

	void clean()
	{
		size = 0;

	}
};

#endif