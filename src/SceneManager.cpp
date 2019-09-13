#include <SceneManager.h>

#include <RenderConstants.h>
#include <Sphere.h>

SceneManager::SceneManager(SceneRenderer *pSceneRenderer, SceneSimulator *pSceneSimulator)
: m_pSceneRenderer(pSceneRenderer)
, m_pSceneSimulator(pSceneSimulator)
{
}

SceneManager::~SceneManager()
{
	delete m_pSceneRenderer;
	delete m_pSceneSimulator;

	for (std::vector<Sphere *>::iterator it = m_spheres.begin(); it != m_spheres.end(); ++it)
	{
		delete (*it);
	}
}

void SceneManager::Update(float deltaTime)
{
	m_pSceneSimulator->Update(deltaTime, m_spheres);

	size_t i = 0;
	while (i < m_spheres.size())
	{
		if (!m_pSceneRenderer->IsInFrustum(m_spheres[i]->Position))
		{
			delete m_spheres[i];
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

void SceneManager::AddSphere(Sphere * pSphere)
{
	m_spheres.push_back(pSphere);
}

void SceneManager::ShootSphere(int x, int y)
{
	glm::vec3 position = m_pSceneRenderer->UnprojectScreenCoordinateAt(x, y);
	glm::vec3 speed = glm::vec3(0, 0, -1);
	Sphere * pSphere = new Sphere(position, RenderConstants::SphereRadius);
	pSphere->Speed = speed;
	AddSphere(pSphere);
}