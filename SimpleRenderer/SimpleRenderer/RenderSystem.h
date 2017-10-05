#pragma once

#include <glad\glad.h>

class Scene;
struct GLFWwindow;

class RenderSystem {
public:
	RenderSystem(GLFWwindow* glContext, const Scene&);
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;

	// Renders the scene
	void beginRender();
	void endRender();
	void update(size_t entityID);
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