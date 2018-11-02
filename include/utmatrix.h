﻿// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// utmatrix.h - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (21.04.2015)
//
// Верхнетреугольная матрица - реализация на основе шаблона вектора

#ifndef __TMATRIX_H__
#define __TMATRIX_H__

#include <iostream>
#include <algorithm>
#include <numeric>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Шаблон вектора
template <class ValType>
class TVector
{
protected:
  ValType *pVector;
  int Size;       // размер вектора
  int StartIndex; // индекс первого элемента вектора
public:
  TVector(int s = 10, int si = 0);
  TVector(const TVector &v);                // конструктор копирования
  ~TVector();
  int GetSize()      { return Size;       } // размер вектора
  int GetStartIndex(){ return StartIndex; } // индекс первого элемента
  ValType& operator[](int pos);             // доступ
  bool operator==(const TVector &v) const;  // сравнение
  bool operator!=(const TVector &v) const;  // сравнение
  TVector& operator=(const TVector &v);     // присваивание
  TVector& operator=(TVector<ValType> &&v);
  TVector& operator=(int newLen);  // изменение длины вектора

  // скалярные операции
  TVector  operator+(const ValType &val);   // прибавить скаляр
  TVector  operator-(const ValType &val);   // вычесть скаляр
  TVector  operator*(const ValType &val);   // умножить на скаляр

  // векторные операции
  TVector  operator+(const TVector &v);     // сложение
  TVector  operator-(const TVector &v);     // вычитание
  ValType  operator*(const TVector &v);     // скалярное произведение

  // ввод-вывод
  friend istream& operator>>(istream &in, TVector &v)
  {
    for (int i = 0; i < v.Size; i++)
      in >> v.pVector[i];
    return in;
  }
  friend ostream& operator<<(ostream &out, const TVector &v)
  {
    for (int i = 0; i < v.Size; i++)
      out << v.pVector[i] << ' ';
    return out;
  }
};

template <class ValType>
TVector<ValType>::TVector(int s, int si)
{
	if (s <= MAX_VECTOR_SIZE && si > -1)
	{
		Size = s;
		StartIndex = si;
		pVector = new ValType[Size];
		for (int i = 0; i < Size; i++)
		{
			pVector[i] = NULL;
		}
	}
	else
		throw "ERROR_MAX_LENGTH";
} /*-------------------------------------------------------------------------*/

template <class ValType> //конструктор копирования
TVector<ValType>::TVector(const TVector<ValType> &v)
{
	Size = v.Size;
	StartIndex = v.StartIndex;
	pVector = new ValType[Size];
	copy(v.pVector, v.pVector + Size, pVector);
} /*-------------------------------------------------------------------------*/

template <class ValType>
TVector<ValType>::~TVector()
{
	delete[] pVector;
} /*-------------------------------------------------------------------------*/

template <class ValType> // доступ
ValType& TVector<ValType>::operator[](int pos)
{
	if ((pos - StartIndex) >= Size || (pos - StartIndex) < 0)
		throw "ERROR_INDEX_OUT_OF_RANGE";

	return pVector[pos - StartIndex];
} /*-------------------------------------------------------------------------*/

template <class ValType> // сравнение
bool TVector<ValType>::operator==(const TVector &v) const
{
	if (Size == v.Size && StartIndex == v.StartIndex)
	{
		for (int i = 0; i < Size; i++)
		{
			if (pVector[i] != v.pVector[i])
				return 0;
		}
		//return equal(v.pVector, v.pVector + Size, pVector);
	}
	else
		return 0;

	return 1;
} /*-------------------------------------------------------------------------*/

template <class ValType> // сравнение
bool TVector<ValType>::operator!=(const TVector &v) const
{
	return !(operator==(v));
} /*-------------------------------------------------------------------------*/

template <class ValType> // присваивание
TVector<ValType>& TVector<ValType>::operator=(const TVector &v)
{
	if (Size != v.Size)
	{
		if (Size != v.Size)
		{
			Size = v.Size;
			delete[] pVector;
			pVector = new ValType[Size];
		}
		
		StartIndex = v.StartIndex;
		copy(v.pVector, v.pVector + Size, pVector);
	}

	return *this;
} /*-------------------------------------------------------------------------*/

template <class ValType> // прибавить скаляр
TVector<ValType> TVector<ValType>::operator+(const ValType &val)
{
	TVector<ValType> tmp(Size);

	for (int i = 0; i < Size; i++)
		tmp.pVector[i] = pVector[i] + val;

	return tmp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // вычесть скаляр
TVector<ValType> TVector<ValType>::operator-(const ValType &val)
{
	TVector<ValType> tmp(Size);

	for (int i = 0; i < Size; i++)
		tmp.pVector[i] = pVector[i] - val;

	return tmp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // умножить на скаляр
TVector<ValType> TVector<ValType>::operator*(const ValType &val)
{
	TVector<ValType> tmp(Size);

	for (int i = 0; i < Size; i++)
		tmp.pVector[i] = pVector[i] * val;

	return tmp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // сложение
TVector<ValType> TVector<ValType>::operator+(const TVector<ValType> &v)
{
	TVector<ValType> tmp(Size);

	if (Size == v.Size)
	{
		for (int i = 0; i < Size; i++)
			tmp.pVector[i] = pVector[i] + v.pVector[i];
	}
	else
		throw "ERR_DIFF_LEN";

	return tmp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // вычитание
TVector<ValType> TVector<ValType>::operator-(const TVector<ValType> &v)
{
	TVector<ValType> tmp(Size);

	if (Size == v.Size)
	{
		for (int i = 0; i < Size; i++)
			tmp.pVector[i] = pVector[i] - v.pVector[i];
	}
	else
		throw "ERR_DIFF_LEN";

	return tmp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // скалярное произведение
ValType TVector<ValType>::operator*(const TVector<ValType> &v)
{
	TVector<ValType> tmp(Size);
	ValType sum = 0;
	
	if (Size == v.Size)
	{
		for (int i = 0; i < Size; i++)
		{
			tmp.pVector[i] = pVector[i] * v.pVector[i];
			sum += tmp.pVector[i];
		}
	}
	else
		throw "ERR_DIFF_LEN";

	return sum;
} /*-------------------------------------------------------------------------*/

template <class ValType>
TVector<ValType>& TVector<ValType>::operator=(TVector<ValType> &&v)
{
	delete[] pVector;
	pVector = v.pVector;
	Size = v.Size;
	StartIndex = v.StartIndex;
	v.pVector = nullptr;
	return (*this);
} /*-------------------------------------------------------------------------*/

template <class ValType>
TVector<ValType>& TVector<ValType>::operator=(int newLen)
{
	delete[] pVector;

	Size = newLen;
	pVector = new ValType[Size];
	for (int i = 0; i < Size; i++)
		pVector[i] = 0;

	return (*this);
}


// Верхнетреугольная матрица
template <class ValType>
class TMatrix : public TVector<TVector<ValType> >
{
public:
  TMatrix(int s = 10);                           
  TMatrix(const TMatrix &mt);                    // копирование
  TMatrix(const TVector<TVector<ValType> > &mt); // преобразование типа
  bool operator==(const TMatrix &mt) const;      // сравнение
  bool operator!=(const TMatrix &mt) const;      // сравнение
  TMatrix& operator= (const TMatrix &mt);        // присваивание
  TMatrix  operator+ (const TMatrix &mt);        // сложение
  TMatrix  operator- (const TMatrix &mt);        // вычитание

  // ввод / вывод
  friend istream& operator>>(istream &in, TMatrix &mt)
  {
    for (int i = 0; i < mt.Size; i++)
      in >> mt.pVector[i];
    return in;
  }
  friend ostream & operator<<( ostream &out, const TMatrix &mt)
  {
    for (int i = 0; i < mt.Size; i++)
      out << mt.pVector[i] << endl;
    return out;
  }
};

template <class ValType>
TMatrix<ValType>::TMatrix(int s): TVector<TVector<ValType> >(s)
{
	if (s <= 0)
		throw s;
	else
		for (int i = 0; i < s; i++)
			this->pVector[i] = move(TVector<ValType>(s - i, i));
} /*-------------------------------------------------------------------------*/

template <class ValType> // конструктор копирования
TMatrix<ValType>::TMatrix(const TMatrix<ValType> &mt):
  TVector<TVector<ValType> >(mt) {}

template <class ValType> // конструктор преобразования типа
TMatrix<ValType>::TMatrix(const TVector<TVector<ValType> > &mt):
  TVector<TVector<ValType> >(mt) {}

template <class ValType> // сравнение
bool TMatrix<ValType>::operator==(const TMatrix<ValType> &mt) const
{
	if (this->Size == mt.Size)
	{
		for (int i = 0; i < this->Size; i++)
		{
			if (this->pVector[i] != mt.pVector[i])
				return false;
		}
	}
	else
		return false;

	return true;
} /*-------------------------------------------------------------------------*/

template <class ValType> // сравнение
bool TMatrix<ValType>::operator!=(const TMatrix<ValType> &mt) const
{
	return !(operator==(mt));
} /*-------------------------------------------------------------------------*/

template <class ValType> // присваивание
TMatrix<ValType>& TMatrix<ValType>::operator=(const TMatrix<ValType> &mt)
{
	if (this->Size == mt.Size)
	{
		delete[] this->pVetor;
		this->pVector = new TMatrix<ValType>[this->Size];
		this->Size = mt.Size;
		this->StartIndex = mt.StartIndex;
	}
	for (int i = 0; i < this->Size; i++)
		this->pVector[i] = mt.pVector;

	return *this;
} /*-------------------------------------------------------------------------*/

template <class ValType> // сложение
TMatrix<ValType> TMatrix<ValType>::operator+(const TMatrix<ValType> &mt)
{
	TMatrix<ValType> tmp(this->Size);

	if (this->Size == mt.Size)
	{
		for (int i = 0; i < mt.Size; i++)
			tmp.pVector[i] = this->pVector[i] + mt.pVector[i];
	}
	else
		throw "ERR_DIFF_LEN";

	return tmp;
} /*-------------------------------------------------------------------------*/

template <class ValType> // вычитание
TMatrix<ValType> TMatrix<ValType>::operator-(const TMatrix<ValType> &mt)
{
	TMatrix<ValType> tmp(this->Size);

	if (this->Size == mt.Size)
	{
		for (int i = 0; i < mt.Size; i++)
			tmp.pVector[i] = this->pVector[i] - mt.pVector[i];
	}
	else
		throw "ERR_DIFF_LEN";

	return tmp;
} /*-------------------------------------------------------------------------*/

// TVector О3 Л2 П4 С6
// TMatrix О2 Л2 П3 С3
#endif
