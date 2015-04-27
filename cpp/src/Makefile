OUTPUT=../output/
all:
	g++ -DTEST_13P8P2 cpp_primer.cpp -o $(OUTPUT)/test_13.8.2.bin
	#g++ -DTEST_6P2 -DTEST_5P5P1 cpp_primer.cpp -lm
	#g++ -DTEST_11P5P2 cpp_primer.cpp -o $(OUTPUT)/test_11.5.2.bin
test:
	g++ test.cpp
	#g++ -DTEST_10P3P2 cpp_primer.cpp -o $(OUTPUT)/test_10.3.2.bin
test6.2:
	g++ -DTEST_6P2 cpp_primer.cpp -o $(OUTPUT)/test_6.2.bin
test10.3.2:
	g++ -DTEST_10P3P2 cpp_primer.cpp -o $(OUTPUT)/test_10.3.2.bin
test11.5.2:
	g++ -DTEST_11P5P2 cpp_primer.cpp -o $(OUTPUT)/test_11.5.2.bin
test13.8.2:
	g++ -DTEST_13P8P2 cpp_primer.cpp -o $(OUTPUT)/test_13.8.2.bin

test_libevent:
	g++ libevent.cpp -levent
clean:
	rm -f a.out
	rm -f ../output/*
