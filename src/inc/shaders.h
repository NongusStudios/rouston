#pragma once

#define OPENGL_VERSION_STRING "#version 460 core\n"

const char* g_rect_vshd = \
    OPENGL_VERSION_STRING \
    "layout (location = 0) in vec2 a_pos;\n" \
    "layout (location = 1) in vec2 a_uv;\n" \
    \
    "layout (location = 0) uniform mat4 u_model;\n" \
    "layout (location = 1) uniform mat4 u_view;\n" \
    "layout (location = 2) uniform mat4 u_proj;\n" \
    \
    "void main(){\n" \
    "   gl_Position = u_proj * u_view * u_model * vec4(a_pos, 0.0, 1.0);\n" \
    "}";
const char* g_rect_fshd = \
    OPENGL_VERSION_STRING \
    "out vec4 frag_color;\n" \
    "layout (location = 3) uniform vec4 color;\n" \
    \
    "void main(){\n" \
    "   frag_color = color;\n" \
    "}";

const char* g_rectbatch_vshd = \
    OPENGL_VERSION_STRING \
    "layout (location = 0) in vec2 a_pos;\n" \
    "layout (location = 1) in vec2 a_uv;\n" \
    "layout(std140, binding = 0) buffer RectOffsetData {\n" \
    "   mat4 transforms[];\n" \
    "};\n" \
    "layout(std140, binding = 1) buffer RectColourData {\n" \
    "   vec4 colours[];\n" \
    "};\n" \
    \
    "layout (location = 0) uniform mat4 u_view;\n" \
    "layout (location = 1) uniform mat4 u_proj;\n" \
    \
    "out vec4 rect_colour;\n"
    \
    "void main(){\n" \
    "   gl_Position = u_proj * u_view * transforms[gl_InstanceID] * vec4(a_pos, 0.0, 1.0);\n" \
    "   rect_colour = colours[gl_InstanceID];\n" \
    "}";
const char* g_rectbatch_fshd = \
    OPENGL_VERSION_STRING \
    "in vec4 rect_colour;\n"
    "out vec4 frag_colour;\n" \
    \
    "void main(){\n" \
    "   frag_colour = rect_colour;\n" \
    "}";

const char* g_sprite_vshd = \
    OPENGL_VERSION_STRING \
    "layout (location = 0) in vec2 a_pos;\n" \
    "layout (location = 1) in vec2 a_uv;\n" \
    \
    "layout (location = 0) uniform mat4 u_model;\n" \
    "layout (location = 1) uniform mat4 u_view;\n" \
    "layout (location = 2) uniform mat4 u_proj;\n" \
    \
    "out vec2 out_uv;\n" \
    \
    "void main(){\n" \
    "   gl_Position = u_proj * u_view * u_model * vec4(a_pos, 0.0, 1.0);\n" \
    "   out_uv = a_uv;\n"\
    "}";
const char* g_sprite_fshd = \
    OPENGL_VERSION_STRING \
    "layout(location = 3) uniform vec2 pos;\n" \
    "layout(location = 4) uniform vec2 size;\n" \
    "uniform sampler2D u_texture;\n" \
    "in vec2 out_uv;\n" \
    "out vec4 frag_colour;\n" \
    \
    "void main(){\n" \
    "   float minx = pos.x;\n" \
    "   float maxx = pos.x + size.x;\n" \
    \
    "   float miny = pos.y;\n" \
    "   float maxy = pos.y + size.y;\n" \
    \
    "   vec2 correct_uv = out_uv;\n" \
    \
    "   correct_uv.x = minx + (maxx - minx) * out_uv.x;\n" \
    "   correct_uv.y = miny + (maxy - miny) * out_uv.y;\n" \
    \
    "   frag_colour = texture(u_texture, correct_uv);\n" \
    "   if(frag_colour.w <= 0.1) discard;\n" \
    "}";
