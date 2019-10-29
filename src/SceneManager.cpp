#include <SceneManager.h>

#include <RenderConstants.h>

SceneManager::SceneManager(std::unique_ptr<SceneRenderer> &&pSceneRenderer, std::unique_ptr<SceneSimulator> &&pSceneSimulator)
: m_pSceneRenderer(std::move(pSceneRenderer))
, m_pSceneSimulator(std::move(pSceneSimulator))
{
}

void SceneManager::Update(float deltaTime)
{
	m_pSceneSimulator->Update(deltaTime, m_spheres);

	size_t i = 0;
	while (i < m_spheres.size())
	{
		if (!m_pSceneRenderer->IsInFrustum(m_spheres[i].Position))
		{
			m_spheres.erase(m_spheres.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

void SceneManager::Draw()
{
	m_pSceneRenderer->Draw(m_spheres);
}

void SceneManager::AddSphere(const Sphere & pSphere)
{
	m_spheres.push_back(pSphere);
}

void SceneManager::ShootSphere(int x, int y)
{
	glm::vec3 position = m_pSceneRenderer->UnprojectScreenCoordinateAt(x, y);
	glm::vec3 speed = glm::vec3(0, 0, -1);
	Sphere sphere(position, RenderConstants::SphereRadius);
	sphere.Speed = speed;
	AddSphere(sphere);
}