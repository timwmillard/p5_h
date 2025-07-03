
CFLAGS +=-Ideps

LIBS +=-framework Cocoa -framework QuartzCore -framework Metal -framework MetalKit
CFLAGS +=-ObjC
DEFS +=-DSOKOL_METAL

CFLAGS += $(DEFS)

.PHONY: shader

DEPS = deps/sokol_app.h \
	   deps/sokol_gfx.h \
	   deps/sokol_glue.h \
	   deps/sokol_gp.h

deps/deps.o: deps/deps.c $(DEPS)
	clang -c $(CFLAGS) -o deps/deps.o deps/deps.c

demo: src/demo.c p5.h deps/deps.o
	clang -o demo $(CFLAGS) $(LIBS) deps/deps.o src/demo.c

p5style: src/p5_style_demo.c p5.h deps/deps.o
	clang -o p5style $(CFLAGS) $(LIBS) deps/deps.o src/p5_style_demo.c

canvas: src/canvas_demo.c p5.h deps/deps.o
	clang -o canvas $(CFLAGS) $(LIBS) deps/deps.o src/canvas_demo.c

shapes: src/shapes_demo.c p5.h deps/deps.o
	clang -o shapes $(CFLAGS) $(LIBS) deps/deps.o src/shapes_demo.c

run: demo
	@./demo

run_p5style: p5style
	@./p5style

run_canvas: canvas
	@./canvas

run_shapes: shapes
	@./shapes


compile_flags.txt: FORCE
	@echo "Generating compile_flags.txt for IDE support"
	@echo $(CFLAGS) | tr ' ' '\n' > $@

FORCE:


sokol:
	wget -O deps/sokol_app.h https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_app.h
	wget -O deps/sokol_gfx.h https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_gfx.h
	wget -O deps/sokol_glue.h https://raw.githubusercontent.com/floooh/sokol/refs/heads/master/sokol_glue.h

sokol_gp:
	wget -O deps/sokol_gp.h https://raw.githubusercontent.com/edubart/sokol_gp/master/sokol_gp.h

# Test targets
TEST_DEPS = $(DEPS) deps/stb_image.h deps/stb_image_write.h deps/sokol_fetch.h

deps/test_deps.o: deps/test_deps.c $(DEPS)
	clang -c $(CFLAGS) -o deps/test_deps.o deps/test_deps.c

tests/test_utils.o: tests/test_utils.c tests/test_utils.h $(TEST_DEPS)
	clang -c $(CFLAGS) -o tests/test_utils.o tests/test_utils.c

test_basic_shapes: tests/test_basic_shapes.c tests/test_utils.o p5.h deps/test_deps.o $(TEST_DEPS)
	clang -o test_basic_shapes $(CFLAGS) $(LIBS) deps/test_deps.o tests/test_utils.o tests/test_basic_shapes.c

test_colors: tests/test_colors.c tests/test_utils.o p5.h deps/test_deps.o $(TEST_DEPS)
	clang -o test_colors $(CFLAGS) $(LIBS) deps/test_deps.o tests/test_utils.o tests/test_colors.c

test_transforms: tests/test_transforms.c tests/test_utils.o p5.h deps/test_deps.o $(TEST_DEPS)
	clang -o test_transforms $(CFLAGS) $(LIBS) deps/test_deps.o tests/test_utils.o tests/test_transforms.c

test_canvas: tests/test_canvas.c tests/test_utils.o $(TEST_DEPS)
	clang -o test_canvas $(CFLAGS) tests/test_utils.o tests/test_canvas.c

# Individual test runners
run_test_basic_shapes: test_basic_shapes
	@echo "Running basic shapes tests..."
	@./test_basic_shapes

run_test_colors: test_colors
	@echo "Running color tests..."
	@./test_colors

run_test_transforms: test_transforms
	@echo "Running transform tests..."
	@./test_transforms

run_test_canvas: test_canvas
	@echo "Running canvas tests..."
	@./test_canvas

# Build all tests (currently only canvas test works without full sokol setup)
tests: test_canvas

# Run all tests  
run_tests: tests
	@echo "========================================="
	@echo "Running P5.h Test Suite"
	@echo "========================================="
	@./test_canvas
	@echo ""
	@echo "========================================="
	@echo "All tests completed!"
	@echo "========================================="

# Note: Other tests (basic_shapes, colors, transforms) require full sokol graphics setup
# They are included for future enhancement when proper offscreen rendering is implemented

# Clean test artifacts
clean_tests:
	rm -f test_basic_shapes test_colors test_transforms test_canvas
	rm -f tests/test_utils.o
	rm -f deps/test_deps.o
	rm -f tests/test_output_*.png

# Clean everything including tests
clean: clean_tests
	rm -f demo p5style canvas shapes
	rm -f deps/deps.o
	rm -f compile_flags.txt

.PHONY: tests run_tests run_test_basic_shapes run_test_colors run_test_transforms run_test_canvas clean_tests clean

