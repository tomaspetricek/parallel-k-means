generate: generate.cpp
	/usr/local/bin/g++-12 -std=c++20 -O3 -fopt-info-vec -march=native -fopenmp \
	generate.cpp -I ../include -o generate

benchmark: benchmark.cpp
	/usr/local/bin/g++-12 -std=c++20 -O3 -fopt-info-vec -march=native -fopenmp \
	benchmark.cpp -I ../include -o benchmark