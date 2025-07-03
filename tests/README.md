# P5.h Test Suite

This directory contains comprehensive tests for the p5.h graphics library using golden image testing.

## Test Structure

### Test Files
- `test_canvas.c` - ✅ **Working** - Tests canvas creation, sizing, positioning, and window dimensions
- `test_basic_shapes.c` - 🚧 **Future** - Tests rectangle, circle, line, triangle, and other basic shapes
- `test_colors.c` - 🚧 **Future** - Tests fill, stroke, color creation, and color state management  
- `test_transforms.c` - 🚧 **Future** - Tests push/pop, translate, rotate, scale transformations

### Utilities
- `test_utils.h` - Test macros (TEST_ASSERT_TRUE, TEST_ASSERT_FALSE) and function declarations
- `test_utils.c` - Implementation of image comparison and PNG saving functions

### Golden Images
- `golden/` - Reference images for visual regression testing
- Test images are automatically created on first run if golden images don't exist

## Running Tests

### Build and run all tests:
```bash
make run_tests
```

### Build tests only:
```bash
make tests
```

### Run individual test suites:
```bash
make run_test_canvas        # ✅ Working
make run_test_basic_shapes  # 🚧 Future (requires full sokol setup)
make run_test_colors        # 🚧 Future (requires full sokol setup)  
make run_test_transforms    # 🚧 Future (requires full sokol setup)
```

### Clean test artifacts:
```bash
make clean_tests
```

## How Golden Testing Works

1. **First Run**: If no golden image exists, the test output is saved as the golden reference
2. **Subsequent Runs**: Test output is compared against the golden image
3. **Tolerance**: Small differences (< 1% pixels) are allowed to account for minor rendering variations
4. **Failure**: Tests fail if images differ significantly, indicating a regression

## Test Output

Tests generate PNG files in the `tests/` directory:
- `test_output_*.png` - Current test run output images
- These are compared against `golden/*.png` reference images

## Dependencies

The test suite requires these additional dependencies (automatically downloaded):
- `stb_image.h` - For PNG loading and image comparison
- `stb_image_write.h` - For PNG saving
- `sokol_fetch.h` - For framebuffer access (future enhancement)

## Adding New Tests

1. Create a new test file following the naming pattern `test_*.c`
2. Include the test utilities: `#include "test_utils.h"`
3. Use the testing macros: `TEST_ASSERT_TRUE()`, `TEST_ASSERT_FALSE()`
4. Add build targets to the Makefile
5. Use the test runner macros for consistent output

Example test function:
```c
void test_my_feature(void) {
    p5_init();
    
    // Setup and render test scene
    p5_background(1.0f, 1.0f, 1.0f);
    p5_fill(1.0f, 0.0f, 0.0f);
    p5_rect(50, 50, 100, 100);
    
    // Save output and compare with golden image
    render_to_offscreen_and_save("tests/test_output_my_feature.png");
    TEST_ASSERT_TRUE(compare_images("tests/test_output_my_feature.png", "tests/golden/my_feature.png"));
}
```

## Current Status

✅ **Working**: Canvas API testing with mock functions that validate:
- Canvas dimensions and positioning logic
- Window size handling  
- Bounds checking and validation
- Multiple canvas creation behavior

🚧 **Future Enhancements**: Visual regression testing requiring:
- Full Sokol graphics initialization without conflicting main() functions
- Offscreen rendering to framebuffer
- Pixel-perfect golden image comparison
- Integration with CI/CD systems

## Limitations

- Currently only canvas API tests work (no graphics rendering)
- Visual tests require complex Sokol offscreen setup
- Golden image testing infrastructure is prepared but not fully integrated
- Tests validate API behavior rather than visual output