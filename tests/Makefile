# Tests Makefile for p5.h library
# This file contains all test-related build targets and is included by the main Makefile

# Test-specific variables (relative to project root where main Makefile is)
TEST_DEPS = $(DEPS) deps/stb_image.h deps/stb_image_write.h deps/sokol_fetch.h
TEST_DIR = tests
BUILD_DIR = .

# Test dependency compilation (two versions: with and without Sokol)
$(TEST_DIR)/test_deps_simple.o: $(TEST_DIR)/test_deps.c $(TEST_DEPS)
	clang -c $(CFLAGS) -o $(TEST_DIR)/test_deps_simple.o $(TEST_DIR)/test_deps.c

$(TEST_DIR)/test_deps_full.o: $(TEST_DIR)/test_deps.c $(TEST_DEPS)
	clang -c $(CFLAGS) -DTEST_NEEDS_SOKOL -o $(TEST_DIR)/test_deps_full.o $(TEST_DIR)/test_deps.c

$(TEST_DIR)/test_utils.o: $(TEST_DIR)/test_utils.c $(TEST_DIR)/test_utils.h $(TEST_DEPS)
	clang -c $(CFLAGS) -o $(TEST_DIR)/test_utils.o $(TEST_DIR)/test_utils.c


# Test executables (simple tests use STB only, complex tests use full Sokol)
test_simple_visual: $(TEST_DIR)/test_simple_visual.c $(TEST_DIR)/test_utils.o $(TEST_DIR)/test_deps_simple.o $(TEST_DEPS)
	clang -o $(BUILD_DIR)/test_simple_visual $(CFLAGS) $(TEST_DIR)/test_deps_simple.o $(TEST_DIR)/test_utils.o $(TEST_DIR)/test_simple_visual.c

test_basic_shapes_visual: $(TEST_DIR)/test_basic_shapes_visual.c $(TEST_DIR)/test_utils.o $(TEST_DIR)/test_deps_simple.o $(TEST_DEPS)
	clang -o $(BUILD_DIR)/test_basic_shapes_visual $(CFLAGS) $(TEST_DIR)/test_deps_simple.o $(TEST_DIR)/test_utils.o $(TEST_DIR)/test_basic_shapes_visual.c

test_canvas: $(TEST_DIR)/test_canvas.c $(TEST_DIR)/test_utils.o $(TEST_DIR)/test_deps_simple.o $(TEST_DEPS)
	clang -o $(BUILD_DIR)/test_canvas $(CFLAGS) $(TEST_DIR)/test_deps_simple.o $(TEST_DIR)/test_utils.o $(TEST_DIR)/test_canvas.c

# Legacy tests (may not work without proper sokol setup)
test_basic_shapes: $(TEST_DIR)/test_basic_shapes.c $(TEST_DIR)/test_utils.o p5.h $(TEST_DIR)/test_deps_full.o $(TEST_DEPS)
	clang -o $(BUILD_DIR)/test_basic_shapes $(CFLAGS) $(LIBS) $(TEST_DIR)/test_deps_full.o $(TEST_DIR)/test_utils.o $(TEST_DIR)/test_basic_shapes.c

test_colors: $(TEST_DIR)/test_colors.c $(TEST_DIR)/test_utils.o p5.h $(TEST_DIR)/test_deps_full.o $(TEST_DEPS)
	clang -o $(BUILD_DIR)/test_colors $(CFLAGS) $(LIBS) $(TEST_DIR)/test_deps_full.o $(TEST_DIR)/test_utils.o $(TEST_DIR)/test_colors.c

test_transforms: $(TEST_DIR)/test_transforms.c $(TEST_DIR)/test_utils.o p5.h $(TEST_DIR)/test_deps_full.o $(TEST_DEPS)
	clang -o $(BUILD_DIR)/test_transforms $(CFLAGS) $(LIBS) $(TEST_DIR)/test_deps_full.o $(TEST_DIR)/test_utils.o $(TEST_DIR)/test_transforms.c

# Individual test runners
run_test_simple_visual: test_simple_visual
	@echo "Running simple visual tests..."
	@$(BUILD_DIR)/test_simple_visual

run_test_basic_shapes_visual: test_basic_shapes_visual
	@echo "Running visual basic shapes tests..."
	@$(BUILD_DIR)/test_basic_shapes_visual

run_test_canvas: test_canvas
	@echo "Running canvas API tests..."
	@$(BUILD_DIR)/test_canvas

# Legacy test runners (may not work without proper sokol setup)
run_test_basic_shapes: test_basic_shapes
	@echo "Running basic shapes tests (legacy)..."
	@$(BUILD_DIR)/test_basic_shapes

run_test_colors: test_colors
	@echo "Running color tests (legacy)..."
	@$(BUILD_DIR)/test_colors

run_test_transforms: test_transforms
	@echo "Running transform tests (legacy)..."
	@$(BUILD_DIR)/test_transforms

# Build working tests (recommended)
tests: test_simple_visual test_canvas test_basic_shapes_visual

# Run all working tests  
run_tests: tests
	@echo "========================================="
	@echo "Running P5.h Test Suite"
	@echo "========================================="
	@$(BUILD_DIR)/test_canvas
	@echo ""
	@$(BUILD_DIR)/test_simple_visual
	@echo ""
	@$(BUILD_DIR)/test_basic_shapes_visual
	@echo ""
	@echo "========================================="
	@echo "All tests completed!"
	@echo "========================================="

# Clean test artifacts
clean_tests:
	rm -f $(BUILD_DIR)/test_basic_shapes $(BUILD_DIR)/test_colors $(BUILD_DIR)/test_transforms $(BUILD_DIR)/test_canvas $(BUILD_DIR)/test_basic_shapes_visual $(BUILD_DIR)/test_simple_visual
	rm -f $(TEST_DIR)/test_utils.o
	rm -f $(TEST_DIR)/test_deps_simple.o $(TEST_DIR)/test_deps_full.o
	rm -f $(TEST_DIR)/test_output_*.png

# Test-specific phony targets
.PHONY: tests run_tests run_test_simple_visual run_test_basic_shapes_visual run_test_basic_shapes run_test_colors run_test_transforms run_test_canvas clean_tests