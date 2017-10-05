#pragma once

#include <glad\glad.h>

class Scene;
struct GLFWwindow;

class Renderer {
public:
	Renderer(GLFWwindow* glContext, const Scene&);
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	// Renders the scene
	void renderFrame();
	void setCamera(size_t entityID);

private:
	GLFWwindow* m_glContext;
	const Scene& m_scene;
	GLuint m_uniformBuffer;
	GLuint m_uniformBlockBinding;
	const GLuint m_kUniformModelOffset;
	const GLuint m_kUniformViewOffset;
	const GLuint m_kUniformProjectionOffset;
	size_t m_cameraEntity;
};