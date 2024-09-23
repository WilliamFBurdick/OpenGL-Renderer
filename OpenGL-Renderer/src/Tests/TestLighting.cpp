#include "TestLighting.h"
#include "Input/Input.h"
#include <imgui/imgui.h>

test::TestLighting::TestLighting(Window* window):
	Test(window),
	mPhongShader("res/shaders/phong.vert", "res/shaders/phong.frag"), mBlinnPhongShader("res/shaders/blinn_phong.vert", "res/shaders/blinn_phong.frag"),
    mColorShader("res/shaders/color.vert", "res/shaders/color.frag"),
    mCube_D("res/textures/container2.png"), mCube_S("res/textures/container2_specular.png"),
    mFloor_D("res/textures/metal.png")
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    float cubeVertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    float planeVertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f
    };

    mCubeVAO.Bind();
    mCubeVBO = VBO(cubeVertices, sizeof(cubeVertices));
    mCubeVAO.AddAttribute(mCubeVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    mCubeVAO.AddAttribute(mCubeVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    mCubeVAO.AddAttribute(mCubeVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    mPlaneVAO.Bind();
    mPlaneVBO = VBO(planeVertices, sizeof(planeVertices));
    mPlaneVAO.AddAttribute(mCubeVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    mPlaneVAO.AddAttribute(mCubeVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    mPlaneVAO.AddAttribute(mCubeVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    mDirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    mDirLight.ambient = glm::vec3(0.05f);
    mDirLight.diffuse = glm::vec3(0.4f);
    mDirLight.specular = glm::vec3(0.5f);

    mSelectedShader = 1;
}

test::TestLighting::~TestLighting()
{
}

void test::TestLighting::OnUpdate(float deltaTime)
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

void test::TestLighting::OnRender()
{
    // Set up view/projection
    int x, y, w, h;
    mWindow->GetViewport(x, y, w, h);

    Shader* currentShader = (mSelectedShader == 0) ? &mPhongShader : &mBlinnPhongShader;

    currentShader->use();
    glm::mat4 proj = glm::perspective(mCamera.Zoom, (float)w / (float)h, 0.1f, 100.0f);
    glm::mat4 view = mCamera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    // Set uniforms
    currentShader->SetMat4("view", view);
    currentShader->SetMat4("projection", proj);
    currentShader->SetMat4("model", model);
    currentShader->SetVec3("viewPos", mCamera.Position);

    ApplyLighting(*currentShader);

    // Draw cubes
    mCube_D.Bind(0);
    mCube_S.Bind(1);
    mCubeVAO.Bind();
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0));
    currentShader->SetMat4("model", model);
    currentShader->SetInt("material.diffuseMap", 0);
    currentShader->SetInt("material.specularMap", 1);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0));
    currentShader->SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw floor
    mFloor_D.Bind(0);
    mPlaneVAO.Bind();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 4.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.f));
    currentShader->SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void test::TestLighting::OnImGuiRender()
{
    //ImGui::Text("Light calculation method:");
    //ImGui::RadioButton("Phong", &mSelectedShader, 0);
    //ImGui::RadioButton("Blinn-Phong", &mSelectedShader, 1);
    
    ImGui::Text("Lights");

    ImGui::Text("Directional Light");
    ImGui::InputFloat3("Direction", &mDirLight.direction.x);
    ImGui::ColorPicker3("Ambient", &mDirLight.ambient.x);
    ImGui::ColorPicker3("Diffuse", &mDirLight.diffuse.x);
    ImGui::ColorPicker3("Specular", &mDirLight.specular.x);
    
    ImGui::Text("Point Lights");
    if (ImGui::Button("Create Point Light"))
    {
        mPointLights.push_back(PointLight());
        mPointLights.back().ambient = glm::vec3(0.05f, 0.05f, 0.05f);
        mPointLights.back().diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
        mPointLights.back().specular = glm::vec3(1.0f, 1.0f, 1.0f);
        mPointLights.back().constant = 1.0f;
        mPointLights.back().linear = 0.09f;
        mPointLights.back().quadratic = 0.032f;
    }
    for (int i = 0; i < mPointLights.size(); i++)
    {
        std::string name = "Point Light #" + std::to_string(i);
        ImGui::Text(name.c_str());
        ImGui::InputFloat3("Position", &mPointLights[i].position.x);
        ImGui::Text("Colors");
        ImGui::ColorPicker3("Ambient", &mPointLights[i].ambient.x);
        ImGui::ColorPicker3("Diffuse", &mPointLights[i].diffuse.x);
        ImGui::ColorPicker3("Specular", &mPointLights[i].specular.x);
        ImGui::Text("Attenuation");
        ImGui::InputFloat("Constant", &mPointLights[i].constant);
        ImGui::InputFloat("Linear", &mPointLights[i].linear);
        ImGui::InputFloat("Quadratic", &mPointLights[i].quadratic);
    }
    ImGui::Text("Spot Lights");
    if (ImGui::Button("Create Spot Light"))
    {
        mSpotLights.push_back(SpotLight());
        mSpotLights.back().position = mCamera.Position;
        mSpotLights.back().direction = mCamera.Front;
        mSpotLights.back().ambient = glm::vec3(0.0f);
        mSpotLights.back().diffuse = glm::vec3(1.0f);
        mSpotLights.back().specular = glm::vec3(1.0f);
        mSpotLights.back().constant = 1.0f;
        mSpotLights.back().linear = 0.09f;
        mSpotLights.back().quadratic = 0.032f;
        mSpotLights.back().cutOff = glm::cos(glm::radians(12.5f));
        mSpotLights.back().outerCutOff = glm::cos(glm::radians(15.0f));
    }
    for (int i = 0; i < mSpotLights.size(); i++)
    {
        std::string name = "Spot Light #" + std::to_string(i);
        ImGui::Text(name.c_str());
        ImGui::InputFloat3("Position", &mSpotLights[i].position.x);
        ImGui::Text("Colors");
        ImGui::ColorPicker3("Ambient", &mSpotLights[i].ambient.x);
        ImGui::ColorPicker3("Diffuse", &mSpotLights[i].diffuse.x);
        ImGui::ColorPicker3("Specular", &mSpotLights[i].specular.x);
        ImGui::Text("Attenuation");
        ImGui::InputFloat("Constant", &mSpotLights[i].constant);
        ImGui::InputFloat("Linear", &mSpotLights[i].linear);
        ImGui::InputFloat("Quadratic", &mSpotLights[i].quadratic);
        ImGui::InputFloat("Cut Off", &mSpotLights[i].cutOff);
        ImGui::InputFloat("Outer Cut Off", &mSpotLights[i].outerCutOff);
    }
}

void test::TestLighting::ApplyLighting(Shader& shader)
{
    shader.SetInt("numDirLights", 1);
    std::string dirLightID = "dirLights[0]";
    shader.SetVec3(dirLightID + ".direction", mDirLight.direction);
    shader.SetVec3(dirLightID + ".ambient", mDirLight.ambient);
    shader.SetVec3(dirLightID + ".diffuse", mDirLight.diffuse);
    shader.SetVec3(dirLightID + ".specular", mDirLight.specular);
    
    shader.SetInt("numPointLights", mPointLights.size());
    for (int i = 0; i < mPointLights.size(); i++)
    {
        std::string pointLightID = "pointLights[" + std::to_string(i) + "]";
        shader.SetVec3(pointLightID + ".position", mPointLights[i].position);

        shader.SetVec3(pointLightID + ".ambient", mPointLights[i].ambient);
        shader.SetVec3(pointLightID + ".diffuse", mPointLights[i].diffuse);
        shader.SetVec3(pointLightID + ".specular", mPointLights[i].specular);

        shader.SetFloat(pointLightID + ".constant", mPointLights[i].constant);
        shader.SetFloat(pointLightID + ".linear", mPointLights[i].linear);
        shader.SetFloat(pointLightID + ".quadratic", mPointLights[i].quadratic);
    }

    shader.SetInt("numSpotLights", mSpotLights.size());
    for (int i = 0; i < mSpotLights.size(); i++)
    {
        std::string spotLightID = "spotLights[" + std::to_string(i) + "]";
        shader.SetVec3(spotLightID + ".position", mSpotLights[i].position);
        shader.SetVec3(spotLightID + ".direction", mSpotLights[i].direction);

        shader.SetFloat(spotLightID + ".cutOff", mSpotLights[i].cutOff);
        shader.SetFloat(spotLightID + ".outerCutOff", mSpotLights[i].outerCutOff);

        shader.SetVec3(spotLightID + ".ambient", mSpotLights[i].ambient);
        shader.SetVec3(spotLightID + ".diffuse", mSpotLights[i].diffuse);
        shader.SetVec3(spotLightID + ".specular", mSpotLights[i].specular);

        shader.SetFloat(spotLightID + ".constant", mSpotLights[i].constant);
        shader.SetFloat(spotLightID + ".linear", mSpotLights[i].linear);
        shader.SetFloat(spotLightID + ".quadratic", mSpotLights[i].quadratic);
    }
}
