cpp_srcs:=$(shell find src -name "*.cpp")
cpp_objs:=$(patsubst src/%.cpp,objs/%.o,$(cpp_srcs))

so_objs:=$(filter-out objs/main.o,$(cpp_objs) )

include_dirs:=/home/mansteinlr/work/include
libs_dirs:=/home/mansteinlr/work/lib

libs:=ddd

I_options:=$(include_dirs:%=-I%)
l_options:=$(libs:%=-l%)
L_options:=$(libs_dirs:%=-L%)
r_options:=$(libs_dirs:%=-Wl,-rpath=%)

compile_op:= -g -O3 -w -fPIC $(I_options)
link_options:= $(l_options) $(L_options) $(r_options)

workspace/exec: objs/main.o
	@mkdir -p $(dir $@)
	$(CXX) $^ -o $@ $(link_options)




objs/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $^ -o $@ $(compile_op)



compile: $(cpp_objs)


lib/libddd.so:$(so_objs)
	@mkdir -p $(dir $@)
	$(CXX) -shared $^ -o $@


dynamic: lib/libddd.so





debug:
	@echo $(cpp_srcs)
	@echo $(cpp_objs)
	@echo $(so_objs)



.PHONY: debug compile dynamic