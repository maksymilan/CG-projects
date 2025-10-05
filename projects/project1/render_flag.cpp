#include "render_flag.h"

RenderFlag::RenderFlag(const Options& options) : Application(options) {
    // create star shader
    const char* vsCode =
        "#version 330 core\n"
        "layout(location = 0) in vec2 aPosition;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPosition, 0.0f, 1.0f);\n"
        "}\n";

    const char* fsCode =
        "#version 330 core\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "    fragColor = vec4(1.0f, 0.870f, 0.0f, 1.0f);\n"
        "}\n";

    _starShader.reset(new GLSLProgram);
    _starShader->attachVertexShader(vsCode);
    _starShader->attachFragmentShader(fsCode);
    _starShader->link();

    // TODO: create 5 stars
    // hint: aspect_of_the_window = _windowWidth / _windowHeight
    // write your code here
    // ---------------------------------------------------------------
    // _stars[i].reset(new Star(ndc_position, rotation_in_radians, size_of_star,
    // aspect_of_the_window));
    // ---------------------------------------------------------------

    float aspect = static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight);
    const float grid_width = 30.0;
    const float grid_height = 20.0;

    glm::vec2 large_star_pos(5.0,5.0);
    float large_star_radius = 3.0;
    glm::vec2 small_star_positions[] = {
        glm::vec2(10.0f, 2.0f),
        glm::vec2(12.0f, 4.0f),
        glm::vec2(12.0f, 7.0f),
        glm::vec2(10.0f, 9.0f)
    };
    float small_star_radius = 1.0;
    auto map_to_NDC = [&](const glm::vec2& grid_pos) -> glm::vec2{
        float ndcX = (grid_pos.x / grid_width) * 2 - 1;
        float ndcY = -((grid_pos.y / grid_height) * 2 - 1);
        return {ndcX,ndcY};
    };
    float large_star_ndc_radius = large_star_radius / grid_height;
    float small_star_ndc_radius = small_star_radius / grid_height;
    
    glm::vec2 large_star_ndc_pos = map_to_NDC(large_star_pos);
    _stars[0].reset(new Star(large_star_ndc_pos,90,large_star_ndc_radius,aspect));

    for(int i=0;i<4;++i){
        glm::vec2 cur_small_star_pos = small_star_positions[i];
        glm::vec2 cur_small_star_ndc_pos = map_to_NDC(cur_small_star_pos);

        glm::vec2 direction = large_star_pos - cur_small_star_pos;
        float angle = atan2(direction.y,direction.x);

        float rotation = angle - glm::radians(90.0);
        _stars[i+1].reset(new Star(cur_small_star_ndc_pos,rotation,small_star_ndc_radius,aspect));
    }
}

void RenderFlag::handleInput() {
    if (_input.keyboard.keyStates[GLFW_KEY_ESCAPE] != GLFW_RELEASE) {
        glfwSetWindowShouldClose(_window, true);
        return;
    }
}

void RenderFlag::renderFrame() {
    showFpsInWindowTitle();

    // we use background as the flag
    glClearColor(0.87f, 0.161f, 0.063f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    _starShader->use();
    for (int i = 0; i < 5; ++i) {
        if (_stars[i] != nullptr) {
            _stars[i]->draw();
        }
    }
}