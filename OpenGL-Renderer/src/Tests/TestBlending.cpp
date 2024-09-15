#include "TestBlending.h"
#include <imgui/imgui.h>
#include "Input/Input.h"
#include <map>


unsigned int cubeVAO, cubeVBO;

test::TestBlending::TestBlending(Window* window) :
    Test(window), mShader("res/shaders/unlit.vert", "res/shaders/unlit.frag"),
    mMarble("res/textures/marble.jpg"), mFloor("res/textures/metal.png"), mTransparent("res/textures/window.png")
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	mSort = true;

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

    mCubeVAO.Bind();
    mCubeVBO = VBO(cubeVertices, sizeof(cubeVertices));
    mCubeVAO.AddAttribute(mCubeVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    mCubeVAO.AddAttribute(mCubeVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    mPlaneVAO.Bind();
    mPlaneVBO = VBO(planeVertices, sizeof(planeVertices));
    mPlaneVAO.AddAttribute(mCubeVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    mPlaneVAO.AddAttribute(mCubeVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    mTransparentVAO.Bind();
    mTransparentVBO = VBO(transparentVertices, sizeof(transparentVertices));
    mTransparentVAO.AddAttribute(mCubeVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    mTransparentVAO.AddAttribute(mCubeVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    mWindowPositions = std::vector<glm::vec3>({
            glm::vec3(-1.5f, 0.0f, -0.48f),
            glm::vec3(1.5f, 0.0f, 0.51f),
            glm::vec3(0.0f, 0.0f, 0.7f),
            glm::vec3(-0.3f, 0.0f, -2.3f),
            glm::vec3(0.5f, 0.0f, -0.6f)
        });

    // Create camera
    mCamera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
}

test::TestBlending::~TestBlending()
{

}

void test::TestBlending::OnUpdate(float deltaTime)
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

void test::TestBlending::OnRender()
{
    // Set up view/projection
    mShader.use();
    int x, y, w, h;
    mWindow->GetViewport(x, y, w, h);
    glm::mat4 proj = glm::perspective(mCamera.Zoom, (float)w / (float)h, 0.1f, 100.0f);
    glm::mat4 view = mCamera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    // Set uniforms
    mShader.SetMat4("view", view);
    mShader.SetMat4("projection", proj);
    mShader.SetMat4("model", model);

    // Draw cubes
    mMarble.Bind(0);
    mCubeVAO.Bind();
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0));
    mShader.SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0));
    mShader.SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw floor
    mFloor.Bind(0);
    mPlaneVAO.Bind();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 4.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.f));
    mShader.SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Sort windows (optionally) and draw
    mTransparent.Bind(0);
    mTransparentVAO.Bind();
    if (mSort)
    {
        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < mWindowPositions.size(); i++)
        {
            float distance = glm::length(mCamera.Position - mWindowPositions[i]);
            sorted[distance] = mWindowPositions[i];
        }
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            mShader.SetMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
        }
    }
    else
    {
        for (unsigned int i = 0; i < mWindowPositions.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, mWindowPositions[i]);
            mShader.SetMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
}

void test::TestBlending::OnImGuiRender()
{
	ImGui::Checkbox("Enable Sorting?", &mSort);
}
