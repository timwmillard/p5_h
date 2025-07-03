
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

# Include test targets from tests/Makefile
include tests/Makefile

# Clean everything including tests
clean: clean_tests
	rm -f demo p5style canvas shapes
	rm -f deps/deps.o
	rm -f compile_flags.txt

.PHONY: clean

