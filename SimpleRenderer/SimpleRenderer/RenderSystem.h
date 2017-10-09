#pragma once

#include <glad\glad.h>

struct Scene;
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
	GLuint m_uboUniforms;
	GLuint m_uboShaderParams;
	GLuint m_uniformBindingPoint;
	GLuint m_shaderParamsBindingPoint;
	const GLuint m_kUniformModelOffset;
	const GLuint m_kUniformViewOffset;
	const GLuint m_kUniformProjectionOffset;
	size_t m_cameraEntity;
};