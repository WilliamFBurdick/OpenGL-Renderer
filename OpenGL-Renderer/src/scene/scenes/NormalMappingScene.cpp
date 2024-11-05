#include "NormalMappingScene.h"
#include "input/Input.h"
#include <imgui/imgui.h>

NormalMappingScene::NormalMappingScene(Window* window):
	Scene(window)
{
    m_Camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    // positions
    glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
    glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
    glm::vec3 pos3(1.0f, -1.0f, 0.0f);
    glm::vec3 pos4(1.0f, 1.0f, 0.0f);
    // texture coordinates
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);
    glm::vec2 uv4(1.0f, 1.0f);
    // normal vector
    glm::vec3 nm(0.0f, 0.0f, 1.0f);

    // calculate tangent/bitangent vectors of both triangles
    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;
    // triangle 1
    // ----------
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

    // triangle 2
    // ----------
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


    float quadVertices[] = {
        // positions            // normal         // texcoords  // tangent                          // bitangent
        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
    };

	glGenVertexArrays(1, &m_QuadVAO);
	glGenBuffers(1, &m_QuadVBO);
	glBindVertexArray(m_QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));

    glCheckError();

    glBindVertexArray(0);

    // Load shaders
    m_NormalMapShader = new Shader("./shaders/detail-maps/normal-mapping.vs", "./shaders/detail-maps/normal-mapping.fs");
    m_ParallaxMapShader = new Shader("./shaders/detail-maps/parallax-mapping.vs", "./shaders/detail-maps/parallax-mapping.fs");
    m_ActiveShader = 0;

    // Load textures
    m_QuadDiffuseMap = Utils::LoadTexture("./res/textures/brickwall.jpg");
    m_QuadNormalMap = Utils::LoadTexture("./res/textures/brickwall_normal.jpg");
    m_BricksDiffuse = Utils::LoadTexture("./res/textures/bricks2.jpg");
    m_BricksNormal = Utils::LoadTexture("./res/textures/bricks2_normal.jpg");
    m_BricksHeight = Utils::LoadTexture("./res/textures/bricks2_disp.jpg");

    m_NormalMapShader->use();
    m_NormalMapShader->setInt("diffuseMap", 0);
    m_NormalMapShader->setInt("normalMap", 1);

    m_ParallaxMapShader->use();
    m_ParallaxMapShader->setInt("diffuseMap", 0);
    m_ParallaxMapShader->setInt("normalMap", 1);
    m_ParallaxMapShader->setInt("depthMap", 2);

    m_LightPos = glm::vec3(0.5f, 1.0f, 0.3f);

    m_HeightScale = 0.5f;
}

void NormalMappingScene::Update(float dt)
{
    if (Input::GetMouseButton(m_Window->GetWindow(), GLFW_MOUSE_BUTTON_RIGHT))
    {
        Input::SetCursorVisible(m_Window->GetWindow(), false);
        m_Camera.ProcessMouseMovement(Input::GetMouse().xDelta, Input::GetMouse().yDelta);
        if (Input::GetKey(m_Window->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
            m_Camera.ProcessKeyboard(EForward, dt);
        if (Input::GetKey(m_Window->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
            m_Camera.ProcessKeyboard(EBackward, dt);
        if (Input::GetKey(m_Window->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
            m_Camera.ProcessKeyboard(ELeft, dt);
        if (Input::GetKey(m_Window->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
            m_Camera.ProcessKeyboard(ERight, dt);
    }
    else
    {
        Input::SetCursorVisible(m_Window->GetWindow(), true);
    }
}

void NormalMappingScene::Render()
{
    Shader* shader = m_ActiveShader == 0 ? m_NormalMapShader : m_ParallaxMapShader;

    // configure matrices
    glm::mat4 projection = glm::perspective(glm::radians(m_Camera.Zoom), (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), 0.1f, 100.0f);
    glm::mat4 view = m_Camera.GetViewMatrix();
    shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    // render the quad
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    shader->setMat4("model", model);
    shader->setVec3("viewPos", m_Camera.Position);
    shader->setVec3("lightPos", m_LightPos);
    glCheckError();

    if (m_ActiveShader == 0)
    {
        // Normal mapping demo
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_QuadDiffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_QuadNormalMap);
    }
    else if (m_ActiveShader == 1)
    {
        // Parallax mapping demo
        m_ParallaxMapShader->setFloat("height_scale", m_HeightScale);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_BricksDiffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_BricksNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_BricksHeight);
    }

    glBindVertexArray(m_QuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glCheckError();
}

void NormalMappingScene::RenderUI()
{
    ImGui::RadioButton("Normal mapping", &m_ActiveShader, 0);
    ImGui::RadioButton("Parallax mapping", &m_ActiveShader, 1);
    ImGui::DragFloat("Parallax Height Scale: ", &m_HeightScale, 0.01f, 0.0f, 1.0f);
}

void NormalMappingScene::Enter()
{
}

void NormalMappingScene::Exit()
{
}
