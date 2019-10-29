#pragma once

#include <SceneRenderer.h>
#include <SceneSimulator.h>
#include <Sphere.h>

#include <vector>
#include <memory>

class SceneManager
{
	std::unique_ptr<SceneRenderer> m_pSceneRenderer;
	std::unique_ptr<SceneSimulator> m_pSceneSimulator;
	std::vector<Sphere> m_spheres;

public:
	SceneManager(std::unique_ptr<SceneRenderer>&& pSceneRenderer, std::unique_ptr<SceneSimulator>&& pSceneSimulator);
	void Update(float deltaTime);
	void Draw();
	void AddSphere(const Sphere& pSphere);
	void ShootSphere(int x, int y);
};
