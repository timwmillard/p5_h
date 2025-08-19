@ctype mat4 p5_mat4
@ctype vec4 p5_vec4
@ctype vec2 p5_vec2 

@vs vs
layout(binding=0) uniform vs_params {
    mat4 mvp;
    vec2 position;
    vec2 size;  // For circles: (radius, radius), for rects: (width/2, height/2)
    vec2 screen_size;
};

in vec2 pos;
out vec2 uv;

void main() {
    // Scale the quad by size and translate to position
    vec2 world_pos = pos * size + position;
    
    // Convert to NDC coordinates
    vec2 ndc_pos = (world_pos / screen_size) * 2.0 - 1.0;
    ndc_pos.y = -ndc_pos.y; // Flip Y for screen coordinates
    
    gl_Position = vec4(ndc_pos, 0.0, 1.0);
    uv = pos; // Pass normalized quad coordinates (-1 to 1)
}
@end

@fs fs
layout(binding=1) uniform fs_params {
    vec4 fill_color;
    vec4 stroke_color;
    float smoothness;
    float stroke_width;  // In normalized coordinates (0-1)
    int shape_type;      // 0=circle, 1=rect, 2=triangle, 3=quad
    float corner_radius; // For rounded rectangles
    vec2 quad_skew;      // For skewed quads (optional)
};

in vec2 uv;
out vec4 frag_color;

// Signed Distance Functions
float circle_sdf(vec2 p) {
    return length(p) - 1.0;
}

float rect_sdf(vec2 p, vec2 size) {
    vec2 d = abs(p) - size;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

float rounded_rect_sdf(vec2 p, vec2 size, float radius) {
    vec2 d = abs(p) - size + radius;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0) - radius;
}

float triangle_sdf(vec2 p) {
    // Equilateral triangle pointing up, inscribed in unit circle
    const float k = sqrt(3.0);
    p.x = abs(p.x) - 1.0;
    p.y = p.y + 1.0/k;
    if (p.x + k*p.y > 0.0) p = vec2(p.x - k*p.y, -k*p.x - p.y)/2.0;
    p.x -= clamp(p.x, -2.0, 0.0);
    return -length(p)*sign(p.y);
}

float quad_sdf(vec2 p, vec2 skew) {
    // Simple quad - can be skewed using the skew parameter
    vec2 skewed_p = p + vec2(p.y * skew.x, p.x * skew.y);
    return rect_sdf(skewed_p, vec2(1.0, 1.0));
}

void main() {
    float dist;
    
    // Calculate distance based on shape type
    if (shape_type == 0) {
        // Circle
        dist = circle_sdf(uv);
    } else if (shape_type == 1) {
        // Rectangle (with optional corner radius)
        if (corner_radius > 0.0) {
            dist = rounded_rect_sdf(uv, vec2(1.0, 1.0), corner_radius);
        } else {
            dist = rect_sdf(uv, vec2(1.0, 1.0));
        }
    } else if (shape_type == 2) {
        // Triangle
        dist = triangle_sdf(uv);
    } else if (shape_type == 3) {
        // Quad
        dist = quad_sdf(uv, quad_skew);
    } else {
        // Fallback to circle
        dist = circle_sdf(uv);
    }
    
    // Calculate fill and stroke
    float fill_alpha = 1.0 - smoothstep(-smoothness, smoothness, dist);
    float stroke_alpha = 0.0;
    
    // If we have a stroke width, calculate stroke
    if (stroke_width > 0.0) {
        float stroke_dist = abs(dist) - stroke_width * 0.5;
        stroke_alpha = 1.0 - smoothstep(-smoothness, smoothness, stroke_dist);
        
        // Remove stroke from inside the shape (only stroke the outline)
        stroke_alpha *= (1.0 - smoothstep(-smoothness, smoothness, -abs(dist) + stroke_width * 0.5));
    }
    
    // Combine fill and stroke colors
    vec4 final_color = fill_color * fill_alpha + stroke_color * stroke_alpha;
    
    // Handle overlap between fill and stroke
    final_color.a = clamp(fill_alpha + stroke_alpha, 0.0, 1.0);
    
    frag_color = final_color;
}

@end

@program shape vs fs
