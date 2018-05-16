

default:
	clang++ main.cc -o main -lX11 -std=c++17 -lXrandr

lint:
	clang-tidy main.cc -- -lX11 -lXrandr -std=c++17
