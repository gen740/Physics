nvim_build:
	cmake --build build

nvim_run: nvim_build
	cmake --build build --target test
