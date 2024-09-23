#pragma once
#include <memory>
#include "Model.h"
#include <list>

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

