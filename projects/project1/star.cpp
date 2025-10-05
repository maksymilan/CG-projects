#include "star.h"
#include <cmath>
#include <vector>

Star::Star(const glm::vec2& position, float rotation, float radius, float aspect)
    : _position(position), _rotation(rotation), _radius(radius) {
    // TODO: assemble the vertex data of the star
    // write your code here
    // -------------------------------------
    // for (int i = 0; i < 5; ++i) {
    //     _vertices.push_back( ... );
    // }
    // -------------------------------------
    const int num_points = 5;
    const float radius_ratio = 0.38196601125f;
    float inner_radius = _radius * radius_ratio;

    std::vector<glm::vec2> outer_points(num_points);
    std::vector<glm::vec2> inner_points(num_points);

    float start_angle = glm::radians(_rotation);
    float angle_step = 2 * M_PI / num_points;

    for(int i=0;i<num_points;++i){
        float outer_angle = start_angle + i * angle_step;
        outer_points[i] = {_radius * cos(outer_angle) / aspect, _radius * sin(outer_angle)};

        float inner_angle = outer_angle + angle_step / 2;
        inner_points[i] = {inner_radius * cos(inner_angle) / aspect, inner_radius * sin(inner_angle)};
    }

    // assemble the inner pentagon by 3 triangles
    _vertices.push_back(inner_points[0]);
    _vertices.push_back(inner_points[1]);
    _vertices.push_back(inner_points[2]);

    _vertices.push_back(inner_points[0]);
    _vertices.push_back(inner_points[2]);
    _vertices.push_back(inner_points[3]);

    _vertices.push_back(inner_points[0]);
    _vertices.push_back(inner_points[3]);
    _vertices.push_back(inner_points[4]);

    // assemble the corner points, each point is composed of a triangle
    for(int i=0;i<num_points;++i){
        _vertices.push_back(outer_points[i]);
        _vertices.push_back(inner_points[i]);
        _vertices.push_back(inner_points[(i + num_points - 1)%num_points]);
    }

    for(auto& vertex : _vertices){
        vertex += _position;
    }
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(glm::vec2) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Star::Star(Star&& rhs) noexcept
    : _position(rhs._position), _rotation(rhs._rotation), _radius(rhs._radius), _vao(rhs._vao),
      _vbo(rhs._vbo) {
    rhs._vao = 0;
    rhs._vbo = 0;
}

Star::~Star() {
    if (_vbo) {
        glDeleteVertexArrays(1, &_vbo);
        _vbo = 0;
    }

    if (_vao) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
}

void Star::draw() const {
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(_vertices.size()));
}