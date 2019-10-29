#pragma once

#include <Sphere.h>

#include <vector>
#include <memory>

class SceneRenderer;
class SceneSimulator;

class SceneManager
{
	std::unique_ptr<SceneRenderer> m_pSceneRenderer;
	std::unique_ptr<SceneSimulator> m_pSceneSimulator;
	std::vector<Sphere> m_spheres;

public:
	SceneManager(std::unique_ptr<SceneRenderer>&& pSceneRenderer, std::unique_ptr<SceneSimulator>&& pSceneSimulator);
	~SceneManager();
	void Update(float deltaTime);
	void Draw();
	void AddSphere(const Sphere& pSphere);
	void ShootSphere(int x, int y);
};
