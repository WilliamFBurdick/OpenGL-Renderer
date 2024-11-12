#include "DeferredShadingScene.h"
#include "input/Input.h"

DeferredShadingScene::DeferredShadingScene(Window* window):
	Scene(window)
{
    m_CubeVAO = m_CubeVBO = m_QuadVAO = m_QuadVBO = 0;
	m_Camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
	m_Backpack = new Model("./res/models/backpack/backpack.obj");

	m_GeometryPassShader = new Shader("./shaders/deferred-shading/g-buffer.vs", "./shaders/deferred-shading/g-buffer.fs");
	m_LightingPassShader = new Shader("./shaders/deferred-shading/deferred-shading.vs", "./shaders/deferred-shading/deferred-shading.fs");
    m_LightShader = new Shader("./shaders/deferred-shading/light-box.vs", "./shaders/deferred-shading/light-box.fs");

    m_ObjectPositions.push_back(glm::vec3(-3.0, -0.5, -3.0));
    m_ObjectPositions.push_back(glm::vec3(0.0, -0.5, -3.0));
    m_ObjectPositions.push_back(glm::vec3(3.0, -0.5, -3.0));
    m_ObjectPositions.push_back(glm::vec3(-3.0, -0.5, 0.0));
    m_ObjectPositions.push_back(glm::vec3(0.0, -0.5, 0.0));
    m_ObjectPositions.push_back(glm::vec3(3.0, -0.5, 0.0));
    m_ObjectPositions.push_back(glm::vec3(-3.0, -0.5, 3.0));
    m_ObjectPositions.push_back(glm::vec3(0.0, -0.5, 3.0));
    m_ObjectPositions.push_back(glm::vec3(3.0, -0.5, 3.0));

    glGenFramebuffers(1, &m_GBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);
    // position color buffer
    glGenTextures(1, &m_GPosition);
    glBindTexture(GL_TEXTURE_2D, m_GPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GPosition, 0);
    // normal color buffer
    glGenTextures(1, &m_GNormal);
    glBindTexture(GL_TEXTURE_2D, m_GNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GNormal, 0);
    // color + specular color buffer
    glGenTextures(1, &m_GAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, m_GAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_GAlbedoSpec, 0);
    // tell openGL which color attachments to use
    glDrawBuffers(3, m_Attachments);
    // create depth buffer
    glGenRenderbuffers(1, &m_DepthRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (float)m_Window->GetWidth(), (float)m_Window->GetHeight());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);
    // check if renderbuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Framebuffer not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // light configuration
    srand(13);
    for (unsigned int i = 0; i < NR_LIGHTS; i++)
    {
        // calculate slightly random offsets
        float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
        float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        m_LightPositions.push_back(glm::vec3(xPos, yPos, zPos));
        // also calculate random color
        float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        m_LightColors.push_back(glm::vec3(rColor, gColor, bColor));
    }

    // shader configuration
    m_LightingPassShader->use();
    m_LightingPassShader->setInt("gPosition", 0);
    m_LightingPassShader->setInt("gNormal", 1);
    m_LightingPassShader->setInt("gAlbedoSpec", 2);
}

void DeferredShadingScene::Update(float dt)
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

void DeferredShadingScene::Render()
{
    // geometry pass: render geometry/color data to gbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 projection = glm::perspective(glm::radians(m_Camera.Zoom), (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), 0.1f, 100.0f);
    glm::mat4 view = m_Camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    m_GeometryPassShader->use();
    m_GeometryPassShader->setMat4("projection", projection);
    m_GeometryPassShader->setMat4("view", view);
    for (unsigned int i = 0; i < m_ObjectPositions.size(); i++)
    {
        model = glm::translate(glm::mat4(1.0f), m_ObjectPositions[i]);
        model = glm::scale(model, glm::vec3(0.5f));
        m_GeometryPassShader->setMat4("model", model);
        m_Backpack->Draw(*m_GeometryPassShader);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // lighting pass: calculate lighting by iterating over screen filled quad pixel-by-pixel using gbuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_LightingPassShader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_GPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_GNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_GAlbedoSpec);
    // send light uniforms
    for (unsigned int i = 0; i < m_LightPositions.size(); i++)
    {
        m_LightingPassShader->setVec3("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
        m_LightingPassShader->setVec3("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
        // update attenuation parameters and calculate radius
        const float linear = 0.7f;
        const float quadratic = 1.8f;
        m_LightingPassShader->setFloat("lights[" + std::to_string(i) + "].Linear", linear);
        m_LightingPassShader->setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
    }
    m_LightingPassShader->setVec3("viewPos", m_Camera.Position);
    // render the quad
    RenderQuad();

    // copy content of geometry's depth buffer to default framebuffer's depth buffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // blit depth to default framebuffer
    glBlitFramebuffer(0, 0, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0, 0, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // render lights on top of the scene
    m_LightShader->use();
    m_LightShader->setMat4("projection", projection);
    m_LightShader->setMat4("view", view);
    for (unsigned int i = 0; i < m_LightPositions.size(); i++)
    {
        model = glm::translate(glm::mat4(1.0f), m_LightPositions[i]);
        model = glm::scale(model, glm::vec3(0.125f));
        m_LightShader->setMat4("model", model);
        m_LightShader->setVec3("lightColor", m_LightColors[i]);
        RenderCube();
    }
}

void DeferredShadingScene::RenderUI()
{
}

void DeferredShadingScene::Enter()
{
}

void DeferredShadingScene::Exit()
{
}

void DeferredShadingScene::RenderQuad()
{
    if (m_QuadVAO == 0)
    {
        // Create quad
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        glGenVertexArrays(1, &m_QuadVAO);
        glGenBuffers(1, &m_QuadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
        glBindVertexArray(m_QuadVAO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(m_QuadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void DeferredShadingScene::RenderCube()
{
    if (m_CubeVAO == 0)
    {
        // Create Cube
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
             // bottom face
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             // top face
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
              1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &m_CubeVAO);
        glGenBuffers(1, &m_CubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(m_CubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    }
    glBindVertexArray(m_CubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
