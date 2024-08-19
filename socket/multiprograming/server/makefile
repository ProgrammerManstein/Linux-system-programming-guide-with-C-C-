cpps:=$(shell find src -name "*.cpp")
objs:=$(patsubst src/%.cpp,objs/%.o,$(cpps))

compile_op:=-g -O3 -w -fPIC


workspace/exec: $(objs)
	@mkdir $(dir $@)
	$(CXX) $^ -o $@


$(objs):$(cpps)
	@mkdir $(dir $@)
	$(CXX) -c $^ -o $@ $(compile_op)

debug:
	@echo $(objs)


clean:
	@rm -rf ./objs
	@rm -rf ./workspace

.PHONY:debug clean