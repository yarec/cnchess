bt_bin=nqueen_bt.exe combine_bt.exe
dp_bin=fibo_dp.exe
bin=${bt_bin} ${dp_bin}

all: ${bin}

clean: 
	rm -vf ${bin}


tmp.exe: tmp.c
	g++ $< -o $@

nqueen_bt.exe: nqueen_bt.cpp
	g++ $< -o $@

fibo_dp.exe: fibo_dp.cpp
	g++ $< -o $@

knap_bt.exe: knap_bt.cpp
	g++ $< -o $@

knap_dp.exe: knap_dp.cpp
	g++ $< -o $@

combine_bt.exe: combine_bt.cpp
	g++ $< -o $@

read.exe: read.cpp
	g++ $< -o $@

