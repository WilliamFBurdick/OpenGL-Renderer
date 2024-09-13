#include "TestBlending.h"
#include <imgui/imgui.h>
#include "Input/Input.h"
#include "Renderer.h"

test::TestBlending::TestBlending(Window* window) :
    Test(window), mShader("res/shaders/unlit.vert", "res/shaders/unlit.frag"),
    mMarble("res/textures/marble.jpg"), mFloor("res/textures/wood.png"), mTransparent("res/textures/window.png")
{
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

    // Set up Vertex Arrays (they all have the same layout, so one layout is fine)
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    mCubeVAO.AddBuffer(VertexBuffer(cubeVertices, sizeof(cubeVertices)), layout);
    mPlaneVAO.AddBuffer(VertexBuffer(planeVertices, sizeof(planeVertices)), layout);
    mTransparentVAO.AddBuffer(VertexBuffer(transparentVertices, sizeof(transparentVertices)), layout);

    // Create camera
    mCamera = Camera();
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
    glm::mat4 view = mCamera.GetViewMatrix();
    int x, y, w, h;
    mWindow->GetViewport(x, y, w, h);
    glm::mat4 proj = glm::perspective(mCamera.Zoom, (float)w / (float)h, 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);
    // Set uniforms
    mShader.SetMat4("view", view);
    mShader.SetMat4("projection", proj);

    // Draw cubes
    mMarble.Bind(0);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    mShader.SetMat4("model", model);


}

void test::TestBlending::OnImGuiRender()
{
	ImGui::Checkbox("Enable Sorting?", &mSort);
}
