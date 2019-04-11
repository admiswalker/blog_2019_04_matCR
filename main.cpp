#include <iostream>
#include "./sstd_mat_c.hpp"
#include "./sstd_mat_r.hpp"

typedef unsigned int uint;

template<typename T>
void mat_print(const sstd::mat_c<T>& rhs){
	for(uint p=0; p<rhs.rows(); p++){
		std::cout << "[ ";
		for(uint q=0; q<rhs.cols(); q++){
			std::cout << rhs(p, q) << ' ';
		}
		std::cout << "]" << std::endl;
	}
	std::cout << std::endl;
}

template<typename T>
void mat_print(const sstd::mat_r<T>& rhs){
	for(uint p=0; p<rhs.rows(); p++){
		std::cout << "[ ";
		for(uint q=0; q<rhs.cols(); q++){
			std::cout << rhs(p, q) << ' ';
		}
		std::cout << "]" << std::endl;
	}
	std::cout << std::endl;
}

int main(){
	// TEST of sstd::mat_c<int>.
	{
		sstd::mat_c<int> matC2 = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
		sstd::mat_c<int> matC1 = matC2;
		sstd::mat_c<int> matC3(matC1);
		sstd::mat_c<int> matC4(std::move(matC3));
		sstd::mat_c<int> matC5(3, 3);
		for(uint i=0; i<matC5.size(); i++){ matC5[i]=matC4[i]; }
		sstd::mat_c<int> matC5_2(3, 3);
		for(uint q=0; q<matC5_2.cols(); q++){
			for(uint p=0; p<matC5_2.rows(); p++){
				matC5_2(p, q) = matC5(p, q);
			}
		}
		mat_print(matC5_2);
	}
	
	// TEST of sstd::mat_r<int>.
	{
		sstd::mat_r<int> matR2 = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
		sstd::mat_r<int> matR1 = matR2;
		sstd::mat_r<int> matR3(matR1);
		sstd::mat_r<int> matR4(std::move(matR3));
		sstd::mat_r<int> matR5(3, 3);
		for(uint i=0; i<matR5.size(); i++){ matR5[i]=matR4[i]; }
		sstd::mat_r<int> matR5_2(3, 3);
		for(uint p=0; p<matR5_2.rows(); p++){
			for(uint q=0; q<matR5_2.cols(); q++){
				matR5_2(p, q) = matR5(p, q);
			}
		}
		mat_print(matR5_2);
	}
	return 0;
}
