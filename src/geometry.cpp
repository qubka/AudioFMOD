#include "geometry.hpp"
#include "vertex.hpp"
#include "mesh.hpp"

std::shared_ptr<Mesh> geometry::cuboid(const glm::vec3& halfExtents, bool inwards, const std::shared_ptr<Texture>& texture) {
    float orientation = 1;
    if (inwards)
        orientation = -1;

    std::vector<Vertex> cuboid_vertices {
        //front
        //                                 position														 normal					      tex coord
        { {-1.f * halfExtents.x, -1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f, 0.0f,  1.0f * orientation},		{ 0.f, 0.f } },
        { { 1.f * halfExtents.x, -1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f, 0.0f,  1.0f * orientation},		{ 1.f, 0.f } },
        { { 1.f * halfExtents.x,  1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f, 0.0f,  1.0f * orientation},		{ 1.f, 1.f } },
        { {-1.f * halfExtents.x,  1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f, 0.0f,  1.0f * orientation},		{ 0.f, 1.f } },

        //right
        { { 1.f * halfExtents.x, -1.f * halfExtents.y,  1.f * halfExtents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 0.f } },
        { { 1.f * halfExtents.x, -1.f * halfExtents.y, -1.f * halfExtents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 0.f } },
        { { 1.f * halfExtents.x,  1.f * halfExtents.y, -1.f * halfExtents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 1.f } },
        { { 1.f * halfExtents.x,  1.f * halfExtents.y,  1.f * halfExtents.z },		{ 1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 1.f } },

        //back
        { { 1.f * halfExtents.x, -1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 0.f, 0.f } },
        { {-1.f * halfExtents.x, -1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 1.f, 0.f } },
        { {-1.f * halfExtents.x,  1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 1.f, 1.f } },
        { { 1.f * halfExtents.x,  1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f, 0.0f, -1.0f * orientation},		{ 0.f, 1.f } },

        //left
        { {-1.f * halfExtents.x, -1.f * halfExtents.y, -1.f * halfExtents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 0.f } },
        { {-1.f * halfExtents.x, -1.f * halfExtents.y,  1.f * halfExtents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 0.f } },
        { {-1.f * halfExtents.x,  1.f * halfExtents.y,  1.f * halfExtents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 1.f, 1.f } },
        { {-1.f * halfExtents.x,  1.f * halfExtents.y, -1.f * halfExtents.z },		{-1.0f * orientation, 0.0f, 0.0f},		{ 0.f, 1.f } },

        //top
        { {-1.f * halfExtents.x,  1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 0.f, 0.f } },
        { { 1.f * halfExtents.x,  1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 1.f, 0.f } },
        { { 1.f * halfExtents.x,  1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 1.f, 1.f } },
        { {-1.f * halfExtents.x,  1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f,  1.0f * orientation, 0.0f},		{ 0.f, 1.f } },

        //bottom
        { {-1.f * halfExtents.x, -1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 0.f, 0.f } },
        { { 1.f * halfExtents.x, -1.f * halfExtents.y, -1.f * halfExtents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 1.f, 0.f } },
        { { 1.f * halfExtents.x, -1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 1.f, 1.f } },
        { {-1.f * halfExtents.x, -1.f * halfExtents.y,  1.f * halfExtents.z },		{ 0.0f, -1.0f * orientation, 0.0f},		{ 0.f, 1.f } },
    };

    std::vector<uint32_t> cuboid_indices {
        0,  1,  2,		 0,  2,  3,  //front
        4,  5,  6,		 4,  6,  7,  //right
        8,  9, 10,		 8, 10, 11,  //back
        12, 13, 14,		12, 14, 15,  //left
        16, 17, 18,		16, 18, 19,  //upper
        20, 21, 22,		20, 22, 23   //bottom
    };

    return std::make_shared<Mesh>(std::move(cuboid_vertices), std::move(cuboid_indices), texture);
}

std::shared_ptr<Mesh> geometry::sphere(uint32_t stacks, uint32_t slices, float radius, const std::shared_ptr<Texture>& texture) {
    std::vector<Vertex> sphere_vertices;
    std::vector<uint32_t> sphere_indices;

    const float sliceStep = 2 * M_PI / slices;
    const float stackStep = M_PI / stacks;
    const float lengthInv = 1.0f / radius;

    for (uint32_t i = 0; i <= stacks; ++i) {
        const float stackAngle = M_PI_2 - i * stackStep;  // starting from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);             // r * cos(u)
        float z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sliceCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (uint32_t j = 0; j <= slices; ++j) {
            const float sliceAngle = j * sliceStep;       // starting from 0 to 2pi

            // vertex position (x, y, z)
            const float pos_x = xy * cosf(sliceAngle);    // r * cos(u) * cos(v)
            const float pos_y = xy * sinf(sliceAngle);    // r * cos(u) * sin(v)
            const float pos_z = z;

            // normalized vertex normal (x, y, z)
            float norm_x = pos_x * lengthInv;
            float norm_y = pos_y * lengthInv;
            float norm_z = pos_z * lengthInv;

            // vertex tex coord (x, y) range between [0, 1]
            float tex_y = 1.0f - static_cast<float>(j) / slices;
            float tex_x = static_cast<float>(i) / stacks;

            sphere_vertices.emplace_back(
                    glm::vec3{ pos_x, pos_y, pos_z },
                    glm::vec3{ norm_x, norm_y, norm_z },
                    glm::vec2{ tex_x, tex_y });
        }
    }

    for (uint32_t i = 0; i < stacks; ++i) {
        uint32_t k1 = i * (slices + 1);      // beginning of current stack
        uint32_t k2 = k1 + slices + 1;      // beginning of next stack

        for (uint32_t j = 0; j < slices; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0) {
                sphere_indices.push_back(k1);
                sphere_indices.push_back(k2);
                sphere_indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stacks - 1)) {
                sphere_indices.push_back(k1 + 1);
                sphere_indices.push_back(k2);
                sphere_indices.push_back(k2 + 1);
            }
        }
    }

    return std::make_shared<Mesh>(std::move(sphere_vertices), std::move(sphere_indices), texture);
}

std::shared_ptr<Mesh> geometry::quad(const glm::vec2& extent, const std::shared_ptr<Texture>& texture) {
    std::vector<glm::vec3> vertices {
        {-extent.x, -extent.y, 0.f},
        {extent.x, -extent.y, 0.f},
        {-extent.x, extent.y, 0.f},
        {extent.x, -extent.y, 0.f},
        {extent.x, extent.y, 0.f},
        {-extent.x, extent.y, 0.f}
    };

    std::vector<Vertex> quad_vertices {
        //  position                                  normal                         tex coord
        { vertices.at(0),  { 1.f, 1.f, 1.f },  { 0.f,  0.f } },
        { vertices.at(1),  { 1.f, 1.f, 1.f },  { 1.f,  0.f } },
        { vertices.at(2),  { 1.f, 1.f, 1.f },  { 0.0f, 1.f } },
        { vertices.at(3),  { 1.f, 1.f, 1.f },  { 1.f,  0.f } },
        { vertices.at(4),  { 1.f, 1.f, 1.f },  { 1.f,  1.f } },
        { vertices.at(5),  { 1.f, 1.f, 1.f },  { 0.0f, 1.f } },
    };

    return std::make_shared<Mesh>(std::move(quad_vertices), texture);
}

std::shared_ptr<Mesh> geometry::octahedron(const glm::vec3& extent, const std::shared_ptr<Texture>& texture) {
    std::vector<glm::vec3> vertices {
        {0.f, extent.y, 0.f},
        {extent.x, 0.f, extent.z},
        {-extent.x, 0.f, extent.z},
        {-extent.x, 0.f, -extent.z},
        {extent.x, 0.f, -extent.z},
        {0.f, -extent.y, 0.f}
    };
    std::vector<glm::vec3> normals;
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(1)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(2)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(4)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(4), vertices.at(0) - vertices.at(3)));
    normals.push_back(-glm::cross(vertices.at(5) - vertices.at(2), vertices.at(5) - vertices.at(1)));
    normals.push_back(-glm::cross(vertices.at(5) - vertices.at(3), vertices.at(5) - vertices.at(2)));
    normals.push_back(-glm::cross(vertices.at(5) - vertices.at(1), vertices.at(5) - vertices.at(4)));
    normals.push_back(-glm::cross(vertices.at(5) - vertices.at(4), vertices.at(5) - vertices.at(3)));

    std::vector<Vertex> octahedron_vertices {
        //  position                             normal                       tex coord
        { vertices.at(0),   normals.at(0),   { 0.f,  0.f } },
        { vertices.at(2),   normals.at(0),   { 1.f,  0.f } },
        { vertices.at(1),   normals.at(0),   { 0.5f, 1.f } },
        { vertices.at(0),   normals.at(1),   { 0.f,  0.f } },
        { vertices.at(3),   normals.at(1),   { 1.f,  0.f } },
        { vertices.at(2),   normals.at(1),   { 0.5f, 1.f } },
        { vertices.at(0),   normals.at(2),   { 0.f,  0.f } },
        { vertices.at(1),   normals.at(2),   { 1.f,  0.f } },
        { vertices.at(4),   normals.at(2),   { 0.5f, 1.f } },
        { vertices.at(0),   normals.at(3),   { 0.f,  0.f } },
        { vertices.at(4),   normals.at(3),   { 1.f,  0.f } },
        { vertices.at(3),   normals.at(3),   { 0.5f, 1.f } },
        { vertices.at(1),   normals.at(4),   { 0.f,  0.f } },
        { vertices.at(2),   normals.at(4),   { 1.f,  0.f } },
        { vertices.at(5),   normals.at(4),   { 0.5f, 1.f } },
        { vertices.at(2),   normals.at(5),   { 0.f,  0.f } },
        { vertices.at(3),   normals.at(5),   { 1.f,  0.f } },
        { vertices.at(5),   normals.at(5),   { 0.5f, 1.f } },
        { vertices.at(4),   normals.at(6),   { 0.f,  0.f } },
        { vertices.at(1),   normals.at(6),   { 1.f,  0.f } },
        { vertices.at(5),   normals.at(6),   { 0.5f, 1.f } },
        { vertices.at(3),   normals.at(7),   { 0.f,  0.f } },
        { vertices.at(4),   normals.at(7),   { 1.f,  0.f } },
        { vertices.at(5),   normals.at(7),   { 0.5f, 1.f } },
    };

    return std::make_shared<Mesh>(std::move(octahedron_vertices), texture);
}

std::shared_ptr<Mesh> geometry::tetrahedron(const glm::vec3& extent, const std::shared_ptr<Texture>& texture) {
    std::vector<glm::vec3> vertices {
        {0.f, extent.y, 0.f},
        {0.f, 0.f, extent.z},
        {-extent.x, 0.f, -extent.z},
        {extent.x, 0.f, -extent.z}
    };
    std::vector<glm::vec3> normals;
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(2), vertices.at(0) - vertices.at(1)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(3), vertices.at(0) - vertices.at(2)));
    normals.push_back(glm::cross(vertices.at(0) - vertices.at(1), vertices.at(0) - vertices.at(3)));
    normals.push_back(glm::cross(vertices.at(1) - vertices.at(2), vertices.at(1) - vertices.at(3)));

    std::vector<Vertex> tetrahedron_vertices {
        //  position			normal			         tex coord
        { vertices.at(0),   normals.at(0),	   { 0.f,  0.f } },
        { vertices.at(2),   normals.at(0),	   { 1.f,  0.f } },
        { vertices.at(1),   normals.at(0),	   { 0.5f, 1.f } },

        { vertices.at(0),   normals.at(1),	   { 0.f,  0.f } },
        { vertices.at(3),   normals.at(1),	   { 1.f,  0.f } },
        { vertices.at(2),   normals.at(1),	   { 0.5f, 1.f } },

        { vertices.at(0),   normals.at(2),	   { 0.f,  0.f } },
        { vertices.at(1),   normals.at(2),	   { 1.f,  0.f } },
        { vertices.at(3),   normals.at(2),	   { 0.5f, 1.f } },

        { vertices.at(1),   normals.at(3),	   { 0.f,  0.f } },
        { vertices.at(2),   normals.at(3),	   { 1.f,  0.f } },
        { vertices.at(3),   normals.at(3),    { 0.5f, 1.f } },
    };

    return std::make_shared<Mesh>(std::move(tetrahedron_vertices), texture);
}

std::shared_ptr<Mesh> geometry::line(const std::vector<glm::vec3>& points, const std::shared_ptr<Texture>& texture) {
    std::vector<Vertex> line_vertices;

    for (auto& p : points) {
        line_vertices.emplace_back(p, glm::vec3{1}, glm::vec2{0});
    }

    return std::make_shared<Mesh>(std::move(line_vertices), texture, GL_LINE_LOOP);
}

std::shared_ptr<Mesh> geometry::tube(const std::vector<glm::vec3>& points, float radius, int stacks, const std::shared_ptr<Texture>& texture) {
    std::vector<glm::vec2> circle_points;
    circle_points.reserve(stacks + 1);

    int i;
    float a, da = M_PI * 2.0f / stacks;
    for (a = 0.0, i = 0; i <= stacks; ++i, a += da) {
        circle_points.emplace_back(
            radius * cosf(a),
            radius * sinf(a)
        );
    }

    std::vector<Vertex> tube_vertices;

    // https://blackpawn.com/texts/pqtorus/
    int count = static_cast<int>(points.size());
    for (i = 0; i < count - 1; i++) {
        auto& curr = points[i];
        auto& next = points[i + 1];

        glm::vec3 T{ glm::normalize(next - curr) };
        glm::vec3 B{ glm::normalize(glm::cross(T, next + curr)) };
        glm::vec3 N{ glm::normalize(glm::cross(B, T)) };

        for (auto& p : circle_points) {
            //glm::vec3 tangent{ T };
            glm::vec3 normal{ -glm::normalize(B * p.x + N * p.y) };
            glm::vec3 vertex{ curr + B * p.x + N * p.y }; // note: not normalized!

            tube_vertices.emplace_back(vertex, normal, glm::vec2{0});
        }
    }

    return std::make_shared<Mesh>(std::move(tube_vertices), texture, GL_POINTS);
}

std::shared_ptr<Mesh> geometry::torus(int sides, int cs_sides, float radius, float cs_radius, const std::shared_ptr<Texture>& texture) {
    int numVertices = (sides+1) * (cs_sides+1);
    int numIndices = (2*sides+4) * cs_sides;

    std::vector<Vertex> torus_vertices;
    torus_vertices.reserve(numVertices);
    std::vector<uint32_t> torus_indices;
    torus_indices.reserve(numIndices);

    int angleincs = static_cast<int>(360.0f / sides);
    int cs_angleincs = static_cast<int>(360.0f / cs_sides);

    float D_TO_R = M_PI / 180;

    // iterate cs_sides: inner ring
    for (int j = 0; j <= 360; j += cs_angleincs) {
        float current_radius = radius + (cs_radius * cosf(j * D_TO_R));
        float zval = cs_radius * sinf(j * D_TO_R);

        // iterate sides: outer ring
        for (int i = 0; i <= 360; i += angleincs) {
            glm::vec3 vert {
                current_radius * cosf(i * D_TO_R),
                current_radius * sinf(i * D_TO_R),
                zval
            };

            float u = i / 360.0f;
            float v = 2.0f * j / 360.0f - 1;
            if (v < 0) v = -v;

            float xc = radius * cosf(i * D_TO_R);
            float yc = radius * sinf(i * D_TO_R);

            glm::vec3 norm{ vert.x - xc, vert.y - yc, vert.z };

            torus_vertices.emplace_back(
            vert,
            glm::normalize(norm),
            glm::vec2{u, v});
        }
    }

    // indices grouped by GL_TRIANGLE_STRIP, face oriented clock-wise

    // inner ring
    for (int i = 0, nextrow = sides + 1; i < cs_sides; i++) {
        // outer ring
        int j;
        for (j = 0; j < sides; j ++) {
            torus_indices.push_back((i + 1) * nextrow + j);
            torus_indices.push_back(i * nextrow + j);
        }

        /* generate dummy triangle to avoid messing next ring */
        uint32_t dummy  = i * nextrow + j;
        torus_indices.push_back(dummy + nextrow);
        torus_indices.push_back(dummy);
        torus_indices.push_back(dummy + nextrow);
        torus_indices.push_back(dummy + nextrow);
    }

    return std::make_shared<Mesh>(std::move(torus_vertices), std::move(torus_indices), texture, GL_TRIANGLE_STRIP);
}
