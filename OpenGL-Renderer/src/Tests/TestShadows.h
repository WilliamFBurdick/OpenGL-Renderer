#pragma once
#include "Test.h"
#include "Camera/Camera.h"
#include "VertexArray/VAO.h"
#include "Lights/Lights.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"

namespace test
{
    class TestShadows : public Test
    {
    public:
        TestShadows(Window* window);
        ~TestShadows();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        Camera mCamera;
        VAO mCubeVAO, mPlaneVAO, mQuadVAO;
        VBO mCubeVBO, mPlaneVBO, mQuadVBO;

        DirectionalLight mDirLight;
        std::vector<PointLight> mPointLights;
        std::vector<SpotLight> mSpotLights;

        Shader mSimpleDepth, mObjectShader, mDebugQuad;
        Texture mWood;

        unsigned int mShadowMapFBO;
        unsigned int mShadowWidth, mShadowHeight;
        unsigned int mDepthMap;

        unsigned int GenerateShadowmap(int width, int height);
        void RenderScene(const Shader& shader);
    };
}

