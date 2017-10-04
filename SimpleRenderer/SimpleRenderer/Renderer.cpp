#include "Renderer.h"

#include "GLUtils.h"
#include "MaterialComponent.h"
#include "MeshComponent.h"
#include "Scene.h"
#include "UniformFormat.h"

#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using glm::mat4;
using glm::vec3;

Renderer::Renderer(const Scene& scene)
	: m_scene{ scene }
	, m_uniformBlockBinding{ 0 }
	, m_kUniformModelOffset{ 0 }
	, m_kUniformViewOffset{ sizeof(mat4) }
	, m_kUniformProjectionOffset{ sizeof(mat4) * 2 }
{
	// Setup 
	glGenBuffers(1, &m_uniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformFormat), nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_uniformBlockBinding, m_uniformBuffer);
}

void Renderer::renderFrame()
{
	// Loop through entities
	const size_t kRenderableMask = COMPONENT_MESH | COMPONENT_MATERIAL | COMPONENT_TRANSFORM;
	for (size_t entityID = 0; entityID < m_scene.getEntityCount(); ++entityID) {
		// Skip non-renderable entities
		if (!(m_scene.getComponentMask(entityID) & kRenderableMask))
			continue;

		// Get rendering components of entity
		const MaterialComponent& material = m_scene.getMaterialComponent(entityID);
		const MeshComponent& mesh = m_scene.getMeshComponent(entityID);
		const mat4& transform = m_scene.getTransformComponent(entityID);

		// Tell the gpu what material to use
		glUseProgram(material.shader);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, material.texture);
		
		// Get Aspect ratio (Temporary for quick camera)
		// TODO: Place in camera class
		GLFWwindow* context = glfwGetCurrentContext();
		int width, height;
		glfwGetFramebufferSize(context, &width, &height);
		float aspectRatio = width / height;

		// Temporary camera setup
		// TODO: Place in camera class
		UniformFormat uniforms;
		uniforms.view = glm::lookAt(vec3{ 0, 0, -1 }, vec3{ 0, 0, 0 }, vec3{ 0, 1, 0 });
		uniforms.projection = glm::perspective(glm::radians(60.0f), aspectRatio, 1.0f, 100.0f);

		// Send the model view and projection matrix to the gpu
		uniforms.model = transform;
		GLuint blockIndex;
		blockIndex = glGetUniformBlockIndex(material.shader, "Uniforms");
		glUniformBlockBinding(material.shader, blockIndex, m_uniformBlockBinding);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, m_kUniformModelOffset, sizeof(UniformFormat), &uniforms);

		// Draw object
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_INT, 0);
	}
}
