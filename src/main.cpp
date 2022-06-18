#include <MPE/Core/Log.hpp>
#include <MPE/Core/Window.hpp>
#include <MPE/Core/Properties.hpp>
#include <MPE/Core/Transform.hpp>
#include <MPE/Core/Time.hpp>
#include <MPE/Maths/Mat4f.hpp>
#include <MPE/Maths/Vec3f.hpp>
#include <MPE/Graphics/Shader.hpp>
#include <MPE/Graphics/Renderer.hpp>
#include <MPE/Graphics/Camera.hpp>
#include <MPE/Graphics/Texture.hpp>
#include <MPE/Graphics/Material.hpp>
#include <MPE/Graphics/Mesh.hpp>
#include <MPE/Graphics/VertexIndex.hpp>
#include <MPE/Graphics/PointLight.hpp>
#include <MPE/Importers/OBJLoader.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <cmath>

int main()
{
	using namespace mpe;
	ParsePropertiesFromFile("mpeprops.txt");
	constexpr int WIDTH = 800;
	constexpr int HEIGHT = 600;
	Window w("test", WIDTH, HEIGHT);
	w.SetClearColor(0.15f, 0.15f, 0.2f);

	Renderer::Initialize();
	auto diff_texture = LoadTexture("diff_test.png", false);
	auto spec_texture = LoadTexture("spec_test.png", false);
	auto unlit = LoadShader("unlit");
	auto phong = LoadShader("phong");
	auto blinn = LoadShader("blinn-phong");
	auto mesh = LoadOBJMesh("cube.obj");

	PointLight light;
	light.position = Vec3f();
	light.ambient= Vec3f( 0.05f, 0.05f, 0.05f);
	light.diffuse=Vec3f( 0.8f, 0.8f, 0.8f);
	light.specular=Vec3f( 1.0f, 1.0f, 1.0f);
	light.constant= 1.0f;
	light.linear = 0.09f;
	light.quadratic = 0.032f;

	constexpr float FOVY = 0.785398f;
	constexpr float RATIO = static_cast<float>(WIDTH) / static_cast<float>(HEIGHT);
	Camera camera(FOVY, RATIO, 0.15f, 1000.0f);
	Transform camera_transform;
	camera_transform.position = Vec3f(0.0f, 3.0f, -2.0f);
	camera_transform.rotation = Quaternion(Vec3f::UnitX(), Maths::PI * 0.4f);
	camera.SetTransform(camera_transform);
	Renderer::SetActiveCamera(&camera);
	Material mat0, mat1;
	mat0.textures[Material::DIFFUSE_MAP_INDEX] = diff_texture;
	mat0.textures[Material::SPECULAR_MAP_INDEX] = spec_texture;
	mat1.textures[Material::DIFFUSE_MAP_INDEX] = spec_texture;
	mat1.textures[Material::SPECULAR_MAP_INDEX] = diff_texture;
	Vec3f rotation_axis = Vec3f(0.03f, 0.8f, 0.1f).Normalized();

	float last = Time::GetTime();
	float y1, y2;
	y1 = y2 = 0.0f;
	constexpr float shininess0 = 2.5f;
	constexpr float shininess1 = 13.33f;
	mat0.floats[Material::SHININESS_INDEX] = shininess0;
	mat1.floats[Material::SHININESS_INDEX] = shininess1;

	Transform transform0;
	Transform transform1;
	Transform transform2;
	transform2.position.y = -4.0f;
	transform2.scale = 20.0f * transform2.scale;

	while (!w.ShouldClose())
	{
		float now = Time::GetTime();
		float dt = now - last;
		last = now;

		Vec3f right = camera_transform.Right();
		Vec3f up = camera_transform.Up();
		Vec3f forward = camera_transform.Forward();

		Vec3f direction;
		direction = direction + (
			((float)(glfwGetKey(w.GetNativeWindow(), 'D') == GLFW_PRESS) - (float)(glfwGetKey(w.GetNativeWindow(), 'A') == GLFW_PRESS))
			* right);
		direction = direction + (
			((float)(glfwGetKey(w.GetNativeWindow(), 'W') == GLFW_PRESS) - (float)(glfwGetKey(w.GetNativeWindow(), 'S') == GLFW_PRESS))
			* forward);

		direction.y += (float)(glfwGetKey(w.GetNativeWindow(), 'Q') == GLFW_PRESS);
		direction.y -= (float)(glfwGetKey(w.GetNativeWindow(), 'E') == GLFW_PRESS);

		Quaternion rotX(
			right,
			((float)(glfwGetKey(w.GetNativeWindow(), 'K') == GLFW_PRESS)
				- (float)(glfwGetKey(w.GetNativeWindow(), 'I') == GLFW_PRESS)) * dt * 0.5f);
		Quaternion rotY(
			up,
			((float)(glfwGetKey(w.GetNativeWindow(), 'L') == GLFW_PRESS)
				- (float)(glfwGetKey(w.GetNativeWindow(), 'J') == GLFW_PRESS)) * dt * 0.5f);

		camera_transform.rotation = rotY * rotX * camera_transform.rotation;		
		camera_transform.position = camera_transform.position + (dt * direction);
		camera.SetTransform(camera_transform);

		Shader selected_shader = blinn;
		if (glfwGetKey(w.GetNativeWindow(), 'P') == GLFW_PRESS)
			selected_shader = phong;
		else if (glfwGetKey(w.GetNativeWindow(), 'U') == GLFW_PRESS)
			selected_shader = unlit;
		else
		{
			mat0.floats[Material::SHININESS_INDEX] = shininess0 * 3.5f;
			mat1.floats[Material::SHININESS_INDEX] = shininess1 * 3.5f;
		}

		light.position.y = std::sin(now) * 0.2f + 0.5f;
		Renderer::SetPointLights(std::vector<PointLight>({ light }));

		Quaternion rot(rotation_axis, now);
		transform0.position = Vec3f(-0.2f, 0.15f, 0.0f);
		transform0.rotation = rot;
		transform1.rotation = rot;
		transform1.position = transform0.position + Vec3f(0.4f, 0.0f, 0.0f);

		Renderer::Render(
			selected_shader,
			mesh,
			transform0,
			mat0);
		Renderer::Render(
			selected_shader,
			mesh,
			transform1,
			mat1);
		Renderer::Render(
			selected_shader,
			mesh,
			transform2,
			mat0);

		Renderer::Draw();
		w.Draw();
	}
	FreeShader(unlit.id);
	FreeShader(phong.id);
	FreeShader(blinn.id);
	FreeTexture(diff_texture);
	FreeTexture(spec_texture);
	Renderer::Terminate();
	
	return 0;
}