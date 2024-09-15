#include "TestPostProcessing.h"
#include "Input/Input.h"
#include <imgui/imgui.h>

unsigned int myFBO, myRBO;
unsigned int myColorbuffer;

test::TestPostProcessing::TestPostProcessing(Window* window): Test(window),
	mObjectShader("res/shaders/unlit.vert", "res/shaders/unlit.frag"),
	mPostProcessingShader("res/shaders/postprocessing.vert", "res/shaders/postprocessing.frag"),
    mMarble("res/textures/container.jpg"), mFloor("res/textures/metal.png")
{
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
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    mCubeVAO.Bind();
    mCubeVBO = VBO(cubeVertices, sizeof(cubeVertices));
    mCubeVAO.AddAttribute(mCubeVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    mCubeVAO.AddAttribute(mCubeVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    mPlaneVAO.Bind();
    mPlaneVBO = VBO(planeVertices, sizeof(planeVertices));
    mPlaneVAO.AddAttribute(mCubeVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    mPlaneVAO.AddAttribute(mCubeVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    mQuadVAO.Bind();
    mQuadVBO = VBO(quadVertices, sizeof(quadVertices));
    mQuadVAO.AddAttribute(mQuadVBO, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
    mQuadVAO.AddAttribute(mQuadVBO, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));



    mCamera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    // Create Framebuffer and Renderbuffer
    // Renderbuffer format: 24 bits for depth 8 bits for stencil
    int x, y, w, h;
    mWindow->GetViewport(x, y, w, h);

    glGenFramebuffers(1, &myFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, myFBO);

    glGenTextures(1, &myColorbuffer);
    glBindTexture(GL_TEXTURE_2D, myColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, myColorbuffer, 0);

    glGenRenderbuffers(1, &myRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, myRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, myRBO);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("Framebuffer is not complete!");
}

test::TestPostProcessing::~TestPostProcessing()
{

}

void test::TestPostProcessing::OnUpdate(float deltaTime)
{
    if (Input::GetMouseButton(mWindow->GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        mCamera.ProcessMouseMovement(Input::Cursor.xDelta, Input::Cursor.yDelta);
        if (Input::GetKey(mWindow->GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
            mCamera.ProcessKeyboard(EForward, deltaTime);
        if (Input::GetKey(mWindow->GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
            mCamera.ProcessKeyboard(EBackward, deltaTime);
        if (Input::GetKey(mWindow->GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
            mCamera.ProcessKeyboard(ELeft, deltaTime);
        if (Input::GetKey(mWindow->GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
            mCamera.ProcessKeyboard(ERight, deltaTime);
        if (Input::GetKey(mWindow->GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
            mCamera.ProcessKeyboard(EUp, deltaTime);
        if (Input::GetKey(mWindow->GetWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            mCamera.ProcessKeyboard(EDown, deltaTime);

        Input::SetMouseVisible(mWindow->GetWindow(), false);
    }
    else
    {
        Input::SetMouseVisible(mWindow->GetWindow(), true);
    }
}

void test::TestPostProcessing::OnRender()
{
    // First pass (render to mFBO buffer)
    glBindFramebuffer(GL_FRAMEBUFFER, myFBO);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up view/projection
    int x, y, w, h;
    mWindow->GetViewport(x, y, w, h);
    mObjectShader.use();
    glm::mat4 proj = glm::perspective(mCamera.Zoom, (float)w / (float)h, 0.1f, 100.0f);
    glm::mat4 view = mCamera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    // Set uniforms
    mObjectShader.SetMat4("view", view);
    mObjectShader.SetMat4("projection", proj);
    mObjectShader.SetMat4("model", model);

    // Draw cubes
    mMarble.Bind(0);
    mCubeVAO.Bind();
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0));
    mObjectShader.SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0));
    mObjectShader.SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw floor
    mFloor.Bind(0);
    mPlaneVAO.Bind();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 4.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.f));
    mObjectShader.SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Second pass (render texture to screen)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mPostProcessingShader.use();
    mPostProcessingShader.SetInt("diffuse", 0);
    mPostProcessingShader.SetInt("enabledEffect", mEnabledEffect);
    mQuadVAO.Bind();
    glBindTexture(GL_TEXTURE_2D, myColorbuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void test::TestPostProcessing::OnImGuiRender()
{
    ImGui::Text("Select a post-processing effect:");
    ImGui::RadioButton("None", &mEnabledEffect, 0);
    ImGui::RadioButton("Invert", &mEnabledEffect, 1);
    ImGui::RadioButton("Greyscale", &mEnabledEffect, 2);
    ImGui::RadioButton("Sharpen", &mEnabledEffect, 3);
    ImGui::RadioButton("Blur", &mEnabledEffect, 4);
    ImGui::RadioButton("Edge Detect", &mEnabledEffect, 5);
}
