#pragma once
#include <initializer_list>

//-----------------------------------------------------------------------------------------------------------------------------------------------

typedef unsigned int uint;

//-----------------------------------------------------------------------------------------------------------------------------------------------

namespace sstd{
	template <typename T> class mat_r;
	template <typename T> void copy(sstd::mat_r<T>& lhs, const sstd::mat_r<T>& rhs);
	template <typename T> void move(sstd::mat_r<T>& lhs,       sstd::mat_r<T>& rhs);
	template <typename T> void swap(sstd::mat_r<T>& lhs,       sstd::mat_r<T>& rhs);
	
	template <typename T> sstd::mat_r<T> Tr       (const sstd::mat_r<T>& rhs); // lhs = Transpose(rhs)
	template <typename T> void           Tr_myself(      sstd::mat_r<T>& rhs); // Transpose(rhs)
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

// type martix (row major)
template <typename T>
class sstd::mat_r{
private:
	T* _pMatT;
	uint _rows; // row size
	uint _cols; // column size
	uint _size; // number of elements

public:
	inline mat_r(){ _rows=0; _cols=0; _size=0; _pMatT=0; }
	inline mat_r(const std::initializer_list<std::initializer_list<T>>& rhs){ // called by "sstd::mat_r<T> mat = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};".
		_rows = rhs.size();
		if(_rows==0){ _cols=0; _size=0; return; }
		_cols  = (rhs.begin()[0]).size();
		_size  = _rows*_cols;
		_pMatT = new T[_size];
		
		const std::initializer_list<T>* pRhs=rhs.begin();
		for(uint p=0; p<_rows; p++){
			const T* ppRhs=pRhs[p].begin();
			for(uint q=0; q<_cols; q++){
				if(q>=pRhs[p].size()){break;}
				_pMatT[p*_cols + q]=ppRhs[q];
			}
		}
	}
	inline mat_r(const class mat_r&  rhs){ _rows=0; _cols=0; _size=0; _pMatT=0; sstd::copy<T>(*this, rhs); } // called by "sstd::mat_r<T> buf1(N, N); sstd::mat_r<T> buf2(buf1);"
	inline mat_r(      class mat_r&& rhs){ _rows=0; _cols=0; _size=0; _pMatT=0; sstd::move<T>(*this, rhs); } // called by "return std::move(rhs);" or "std::swap(buf1, buf2)".
	inline mat_r(const uint row_size, const uint col_size){
		_rows = row_size;
		_cols = col_size;
		_size = _rows * _cols;
		_pMatT = new T[_size];
		if(_pMatT==0){ printf("ERROR: sstd::mat_r<T>: This pointer is not allocated.\n"); exit(-1); }
	}
	inline ~mat_r(){ delete[] _pMatT; }
	
	// R: read only
	inline const uint rows() const { return _rows; }
	inline const uint cols() const { return _cols; }
	inline const uint size() const { return _size; }
	
	// RW: read and write
	inline T*& pMatT_RW(){ return _pMatT; }
	inline uint& rows_RW(){ return _rows; }
	inline uint& cols_RW(){ return _cols; }
	inline uint& size_RW(){ return _size; }
	
	class mat_r& operator=(const class mat_r& rhs){ sstd::copy<T>(*this, rhs); return *this; } // called by "lhs = sstd::mat_r<T>(3, 3);".
	
	inline       T& operator[](const uint i)       { return _pMatT[      i]; }
	inline       T& operator[](const  int i)       { return _pMatT[(uint)i]; }
	inline const T& operator[](const uint i) const { return _pMatT[      i]; }
	inline const T& operator[](const  int i) const { return _pMatT[(uint)i]; }

	inline       T& operator()(const uint p, const uint q)       { return _pMatT[_cols*      p +       q]; }
	inline       T& operator()(const  int p, const uint q)       { return _pMatT[_cols*(uint)p +       q]; }
	inline       T& operator()(const uint p, const  int q)       { return _pMatT[_cols*      p + (uint)q]; }
	inline       T& operator()(const  int p, const  int q)       { return _pMatT[_cols*(uint)p + (uint)q]; }
	inline const T& operator()(const uint p, const uint q) const { return _pMatT[_cols*      p +       q]; }
	inline const T& operator()(const  int p, const uint q) const { return _pMatT[_cols*(uint)p +       q]; }
	inline const T& operator()(const uint p, const  int q) const { return _pMatT[_cols*      p + (uint)q]; }
	inline const T& operator()(const  int p, const  int q) const { return _pMatT[_cols*(uint)p + (uint)q]; }
};

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline void copy_withoutAllocation(sstd::mat_r<T>& lhs, const sstd::mat_r<T>& rhs){
	for(uint p=0; p<rhs.rows(); p++){
		for(uint q=0; q<rhs.cols(); q++){
			lhs(p, q) = rhs(p, q);
		}
	}
}
template <typename T>
inline void sstd::copy(sstd::mat_r<T>& lhs, const sstd::mat_r<T>& rhs){

	if(lhs.size() != rhs.size()){
		delete[] lhs.pMatT_RW(); lhs.pMatT_RW() = 0;
		if(rhs.size()!=0){ lhs.pMatT_RW() = new T[rhs.size()]; }
	}

	lhs.rows_RW() = rhs.rows();
	lhs.cols_RW() = rhs.cols();
	lhs.size_RW() = rhs.size();

	copy_withoutAllocation<T>(lhs, rhs);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
inline void sstd::move(sstd::mat_r<T>& lhs, sstd::mat_r<T>& rhs){	
	lhs.rows_RW() = rhs.rows(); rhs.rows_RW() = 0;
	lhs.cols_RW() = rhs.cols(); rhs.cols_RW() = 0;
	lhs.size_RW() = rhs.size(); rhs.size_RW() = 0;

	delete[] lhs.pMatT_RW();
	lhs.pMatT_RW() = rhs.pMatT_RW();
	rhs.pMatT_RW() = 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void swap(sstd::mat_r<T>& lhs, sstd::mat_r<T>& rhs){
	uint rowsBuf=lhs.rows(); lhs.rows_RW()=rhs.rows(); rhs.rows_RW()=rowsBuf;
	uint colsBuf=lhs.cols(); lhs.cols_RW()=rhs.cols(); rhs.cols_RW()=colsBuf;
	uint sizeBuf=lhs.size(); lhs.size_RW()=rhs.size(); rhs.size_RW()=sizeBuf;
	
	T* pMatBuf=lhs.pMat(); lhs.pMatT_RW()=rhs.pMat(); rhs.pMatT_RW()=pMatBuf;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
sstd::mat_r<T> sstd::Tr(const sstd::mat_r<T>& rhs){
	sstd::mat_r<T> lhs(rhs.cols(), rhs.rows());
	for(uint p=0; p<rhs.rows(); p++){
		for(uint q=0; q<rhs.cols(); q++){
			lhs(q, p) = rhs(p, q);
		}
	}
	return lhs;
}
template <typename T>
void sstd::Tr_myself(sstd::mat_r<T>& rhs){
	sstd::mat_r<T> lhs = sstd::Tr(rhs);
	sstd::move(rhs, lhs);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

