#pragma once

namespace Matrix
{
	////////////////////////////////
	//-- Stack allocated Matrix --//
	////////////////////////////////
	template<typename T, unsigned int L, unsigned int C>
	class SMatrix
	{
	private:
		unsigned int m_Lines;
		unsigned int m_Columns;

		T m_Matrix[L][C];

	public:
		/////////////////////////////////////
		//-- Constructos and destructors --//
		/////////////////////////////////////
		SMatrix() :
			m_Lines(L),
			m_Columns(C)
		{}

		SMatrix(const T mat[L][C]) :
			m_Lines(L),
			m_Columns(C)
		{
			for (unsigned int i = 0; i < L; i++)
				memcpy(m_Matrix[i], mat[i], C * sizeof(T));
		}

		SMatrix(const SMatrix& mat) :
			SMatrix(mat.m_Matrix)
		{}

		///////////////////
		//-- Accessors --//
		///////////////////
		unsigned int Lines() const
		{
			return m_Lines;
		}

		unsigned int Columns() const
		{
			return m_Columns;
		}

		T& operator()(unsigned int i, unsigned int j)
		{
			if (i >= L || j >= C)
				throw std::out_of_range("Index out of range.");

			return m_Matrix[i][j];
		}
	};
}