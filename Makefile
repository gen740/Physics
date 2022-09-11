nvim_build:
	@cmake --build build

nvim_run: nvim_build
	@./build/physics_test --gtest_filter=*Fraction*
	@./build/physics_test --gtest_filter=*Quantity*
	# cmake --build build --target test
