#include "AlphaBlendingScene.h"
#include "input/Input.h"
#include <map>
#include <imgui/imgui.h>

AlphaBlendingScene::AlphaBlendingScene(Window* window):
    Scene(window)
{
    m_UnlitShader = new Shader("./shaders/unlit/unlit.vert", "./shaders/unlit/unlit.frag");

    m_WindowPositions = {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3(1.5f, 0.0f, 0.51f),
        glm::vec3(0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3(0.5f, 0.0f, -0.6f)
    };

    m_Runtime = 0.0f;
    m_Sort = true;
}

void AlphaBlendingScene::Enter()
{
    // Configure global OpenGL state
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float planeVertices[] = {
        // positions          // texture Coords 
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };
    // Set up cube
    glGenVertexArrays(1, &m_CubeVAO);
    glGenBuffers(1, &m_CubeVBO);
    glBindVertexArray(m_CubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // Set up plane
    glGenVertexArrays(1, &m_PlaneVAO);
    glGenBuffers(1, &m_PlaneVBO);
    glBindVertexArray(m_PlaneVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_PlaneVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    // Set up transparent quad
    glGenVertexArrays(1, &m_TransparentVAO);
    glGenBuffers(1, &m_TransparentVBO);
    glBindVertexArray(m_TransparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_TransparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), &transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Load textures
    m_CubeTexture = Utils::LoadTexture("./res/textures/marble.jpg");
    m_FloorTexture = Utils::LoadTexture("./res/textures/metal.png");
    m_TransparentTexture = Utils::LoadTexture("./res/textures/window.png");

    // Configure shaders
    m_UnlitShader->use();
    m_UnlitShader->setInt("texture_diffuse1", 0);
}

void AlphaBlendingScene::Exit()
{
}

void AlphaBlendingScene::Update(float dt)
{
    // Increment runtime (use this for circular movement)
    m_Runtime += dt;

    // Camera movement
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

void AlphaBlendingScene::Render()
{
    m_UnlitShader->use();
    glm::mat4 projection = glm::perspective(m_Camera.Zoom, (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), 0.1f, 1000.0f);
    glm::mat4 view = m_Camera.GetViewMatrix();
    m_UnlitShader->setMat4("projection", projection);
    m_UnlitShader->setMat4("view", view);
    
    glm::mat4 model = glm::mat4(1.0f);
    // render cubes
    glBindVertexArray(m_CubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_CubeTexture);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
    m_UnlitShader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    m_UnlitShader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // render floor
    glBindVertexArray(m_PlaneVAO);
    glBindTexture(GL_TEXTURE_2D, m_FloorTexture);
    m_UnlitShader->setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // draw windows (optionally sorted from furthest to nearest)
    glBindVertexArray(m_TransparentVAO);
    glBindTexture(GL_TEXTURE_2D, m_TransparentTexture);
    if (m_Sort)
    {
        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < m_WindowPositions.size(); i++)
        {
            float distance = glm::length(m_Camera.Position - m_WindowPositions[i]);
            sorted[distance] = m_WindowPositions[i];
        }

        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            model = glm::translate(glm::mat4(1.0f), it->second);
            m_UnlitShader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
    else
    {
        for (glm::vec3 pos : m_WindowPositions)
        {
            model = glm::translate(glm::mat4(1.0f), pos);
            m_UnlitShader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
}

void AlphaBlendingScene::RenderUI()
{
    ImGui::Text("Sort windows?");
    ImGui::Checkbox("Sort?: ", &m_Sort);
}

