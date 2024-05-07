.PHONY: test clean mutate coverage main analyze

main:	
	g++ BankAccount.cpp -o main

test:
	@echo "Running tests..."
	make clean
	rm -rf tests
	mkdir tests
	g++ BankAccount.cpp test.cpp -o test -ldeepstate
	./test --fuzz  --timeout 10 --output_test_dir=tests -vv

mutate:
	@echo "Running mutation tests..."
	make clean
	rm -rf mutants
	mkdir mutants
	mutate BankAccount.cpp --mutantDir mutants --cmd "g++ BankAccount.cpp -c"

analyze:
	@echo "Analyzing mutation tests..."
	analyze_mutants BankAccount.cpp --mutantDir mutants "g++ BankAccount.cpp test.cpp -o test -ldeepstate && ./test --fuzz --timeout 1" --timeout 5

coverage:
	@echo "Running coverage tests..."
	make clean
	rm -rf coverage
	mkdir coverage
	g++ -O1 -fprofile-arcs -ftest-coverage test.cpp BankAccount.cpp -o test  -ldeepstate -lgcov
	./test --fuzz --timeout 10 --output_test_dir=coverage -vv
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '/usr/include/*' '/usr/local/include/*'  -o coverage.info
	

clean:
	rm -rf test