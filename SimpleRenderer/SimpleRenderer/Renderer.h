#pragma once

#include <glad\glad.h>

class Scene;

class Renderer {
public:
	Renderer(const Scene&);

	// Renders the scene
	void renderFrame();

private:
	const Scene& m_scene;
	GLuint m_uniformBuffer;
	GLuint m_uniformBlockBinding;
	const GLuint m_kUniformModelOffset;
	const GLuint m_kUniformViewOffset;
	const GLuint m_kUniformProjectionOffset;
};