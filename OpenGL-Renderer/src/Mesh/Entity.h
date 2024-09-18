#pragma once
#include <memory>
#include "Model.h"
#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
protected:
	// local transform
	glm::vec3 mPos = { 0.0f, 0.0f, 0.0f };
	glm::vec3 mEulerRot = { 0.0f, 0.0f, 0.0f };
	glm::vec3 mScale = { 1.0f, 1.0f, 1.0f };

	// global space matrix
	glm::mat4 mModelMatrix = glm::mat4(1.0f);

	// dirty flag
	bool mIsDirty = true;
protected:
	glm::mat4 GetLocalModelMatrix()
	{
		const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(mEulerRot.x), glm::vec3(1.0f, 0.0f, 0.0f));
		const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(mEulerRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(mEulerRot.z), glm::vec3(0.0f, 0.0f, 1.0f));

		const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

		return glm::translate(glm::mat4(1.0f), mPos) * rotationMatrix * glm::scale(glm::mat4(1.0f), mScale);
	}
public:
	void ComputeModelMatrix()
	{
		mModelMatrix = GetLocalModelMatrix();
		mIsDirty = false;
	}
	void ComputeModelMatrix(const glm::mat4& parentGlobalModelMatrix)
	{
		mModelMatrix = parentGlobalModelMatrix * GetLocalModelMatrix();
		mIsDirty = false;
	}
	void SetLocalPosition(const glm::vec3 position)
	{
		mPos = position;
		mIsDirty = true;
	}
	inline const glm::vec3& GetLocalPosition() { return mPos; }
	inline const glm::mat4& GetModelMatrix() { return mModelMatrix; }
	inline const bool IsDirty() const { return mIsDirty; }
};

class Entity : public Model
{
public:
	// Scene graph
	std::list<std::unique_ptr<Entity>> children;
	Entity* parent = nullptr;

	// Space information
	Transform transform;

	// Constructor, expects filepath to 3D model
	Entity(const std::string& path, bool gamma = false) : Model(path.c_str()) {}

	// Add child
	template<typename... TArgs>
	void AddChild(const TArgs&... args)
	{
		children.emplace_back(std::make_unique<Entity>(args...));
		children.back()->parent = this;
	}

	// Update transform if changed
	void UpdateSelfAndChild()
	{
		if (transform.IsDirty())
		{
			ForceUpdateSelfAndChild();
			return;
		}
		for (auto&& child : children)
		{
			child->UpdateSelfAndChild();
		}
	}

	// Force update of transform even if local space didn't change
	void ForceUpdateSelfAndChild()
	{
		if (parent)
		{
			transform.ComputeModelMatrix(parent->transform.GetModelMatrix());
		}
		else
		{
			transform.ComputeModelMatrix();
		}
		for (auto&& child : children)
		{
			child->ForceUpdateSelfAndChild();
		}
	}
};

