#ifndef MPE_RENDERER_HPP
#define MPE_RENDERER_HPP

#include "PointLight.hpp"
#include "Shader.hpp"

#include <vector>

namespace mpe
{
	class Camera;
	struct Material;
	struct Mesh;
	struct Transform;

	namespace Renderer
	{
		void Initialize();
		void Terminate();

		Camera* GetActiveCamera();
		void SetActiveCamera(Camera* cam);
		void SetPointLights(const std::vector<PointLight>& lights);
		
		void Render(
			const Shader& shader,
			const Mesh& mesh,
			const Transform& transform,
			const Material& material);
			
		//Called by EndFrame()
		void Draw();
	}
}

#endif