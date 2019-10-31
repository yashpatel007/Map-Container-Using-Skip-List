all:
	g++ -o Test1 -Wall -Wextra -pedantic  Test1.cpp
	g++ -o Test2 -Wall -Wextra -pedantic  Test2.cpp
	g++ -o Minimal -Wall -Wextra -pedantic Minimal.cpp
	g++ -o MorseCodeExample -Wall -Wextra -pedantic MorseCodeExample.cpp
	g++ -O -o PerformanceTest -Wall -Wextra -pedantic PerformanceTest.cpp

Test1:
	 g++ -o Test1 -Wall -Wextra -pedantic  Test1.cpp
Test2:
	 g++ -o Test2 -Wall -Wextra -pedantic  Test2.cpp
Minimal:
	g++ -o Minimal -Wall -Wextra -pedantic Minimal.cpp
MorseCodeExample:
	g++ -o MorseCodeExample -Wall -Wextra -pedantic MorseCodeExample.cpp
PerformanceTest:
	g++ -O -o PerformanceTest -Wall -Wextra -pedantic PerformanceTest.cpp

clean:
	rm Test1
	rm Test2
	rm Minimal
	rm MorseCodeExample
	rm PerformanceTest
