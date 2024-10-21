#include "InstancingScene.h"
#include <imgui/imgui.h>
#include "input/Input.h"

InstancingScene::InstancingScene(Window* window):
	Scene(window)
{
	m_Camera = Camera(glm::vec3(0.0f, 0.0f, 55.0f));
	m_InstancedRender = false;

	m_InstancedShader = new Shader("./shaders/instancing/instanced.vert", "./shaders/instancing/instanced.frag");
	m_NotInstancedShader = new Shader("./shaders/instancing/not-instanced.vert", "./shaders/instancing/not-instanced.frag");

	m_Planet = new Model("./res/models/planet/planet.obj");
	m_Asteroid = new Model("./res/models/rock/rock.obj");
	m_AsteroidInstanced = new Model("./res/models/rock/rock.obj");

}

void InstancingScene::Enter()
{
	glClearColor(0.01f, 0.02f, 0.02f, 1.0f);
	glGenBuffers(1, &m_InstanceMatrixVBO);
	GenerateModelMatrices(1000);
}

void InstancingScene::Exit()
{

}

void InstancingScene::GenerateModelMatrices(unsigned int count)
{
	srand(static_cast<unsigned int>(glfwGetTime()));
	float radius = 50.0f;
	if (count >= 100000)
		radius = 150.0f;
	float offset = 2.5f;
	if (count >= 100000)
		offset = 25.0f;
	m_ModelMatrices.clear();
	for (unsigned int i = 0; i < count; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// translation: displace along circle within radius range
		float angle = (float)i / (float)count * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// scale: random scale between 0.05 and 0.25
		float scale = static_cast<float>((rand() % 20) / 100.0f + 0.05f);
		model = glm::scale(model, glm::vec3(scale));

		// rotation: add random rotation around semi-random axis
		float rotAngle = static_cast<float>((rand() % 360));
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		m_ModelMatrices.push_back(model);
	}
	GenerateInstanceMatrix();
}

void InstancingScene::GenerateInstanceMatrix()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_InstanceMatrixVBO);
	glBufferData(GL_ARRAY_BUFFER, m_ModelMatrices.size() * sizeof(glm::mat4), m_ModelMatrices.data(), GL_STATIC_DRAW);
	glCheckError();

	for (unsigned int i = 0; i < m_AsteroidInstanced->meshes.size(); i++)
	{
		unsigned int VAO = m_AsteroidInstanced->meshes[0].VAO;
		glBindVertexArray(VAO);
		glCheckError();
		// vertex attributes
		// Because a Matrix4 is 4 * larger than a vec4. 4 separate vec4's must be sent containing an entire matrix's data
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

		// Set attributes to increment each draw call
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
	glCheckError();
}

void InstancingScene::Update(float dt)
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

void InstancingScene::Render()
{
	glm::mat4 projection = glm::perspective(glm::radians(m_Camera.Zoom), (float)m_Window->GetWidth() / (float)m_Window->GetHeight(), 0.1f, 1000.0f);
	glm::mat4 view = m_Camera.GetViewMatrix();

	if (m_InstancedRender)
	{
		// Render planet normally
		m_NotInstancedShader->use();
		m_NotInstancedShader->setMat4("view", view);
		m_NotInstancedShader->setMat4("projection", projection);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		m_NotInstancedShader->setMat4("model", model);
		m_Planet->Draw(*m_NotInstancedShader);

		// Render asteroids using instancing
		m_InstancedShader->use();
		m_InstancedShader->setMat4("view", view);
		m_InstancedShader->setMat4("projection", projection);
		m_InstancedShader->setInt("texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_AsteroidInstanced->textures_loaded[0].id);
		for (unsigned int i = 0; i < m_AsteroidInstanced->meshes.size(); i++)
		{
			glBindVertexArray(m_AsteroidInstanced->meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(m_AsteroidInstanced->meshes[i].indices.size()), GL_UNSIGNED_INT, 0, static_cast<unsigned int>(m_ModelMatrices.size()));
			glBindVertexArray(0);
		}
	}
	else
	{
		// Render normally
		m_NotInstancedShader->use();
		m_NotInstancedShader->setMat4("view", view);
		m_NotInstancedShader->setMat4("projection", projection);

		// Render planet
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		m_NotInstancedShader->setMat4("model", model);
		m_Planet->Draw(*m_NotInstancedShader);

		// Draw asteroids
		for (unsigned int i = 0; i < m_ModelMatrices.size(); i++)
		{
			m_NotInstancedShader->setMat4("model", m_ModelMatrices[i]);
			m_Asteroid->Draw(*m_NotInstancedShader);
		}
	}
}

void InstancingScene::RenderUI()
{
	ImGui::Checkbox("Use instanced rendering? ", &m_InstancedRender);
	unsigned int newCount = m_ModelMatrices.size();
	ImGui::InputInt("Asteroid Count ", (int*)&newCount);
	if (newCount != m_ModelMatrices.size())
	{
		GenerateModelMatrices(newCount);
	}
}

