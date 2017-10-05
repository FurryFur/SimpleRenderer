#include "Renderer.h"

#include "CameraComponent.h"
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

Renderer::Renderer(GLFWwindow* glContext, const Scene& scene)
	: m_glContext{ glContext }
	, m_scene{ scene }
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
	glClear(GL_COLOR_BUFFER_BIT);

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

		// TODO: Add check that camera is a valid camera entity, throw error otherwise
		const CameraComponent& camera = m_scene.getCameraComponent(m_cameraEntity);

		// Tell the gpu what material to use
		glUseProgram(material.shader);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, material.texture);
		
		// Get Aspect ratio (Temporary for quick camera)
		// TODO: Place in camera class
		int width, height;
		glfwGetFramebufferSize(m_glContext, &width, &height);
		float aspectRatio = static_cast<float>(width) / height;

		// Model and Temporary camera setup
		// TODO: Place in camera class
		UniformFormat uniforms;
		uniforms.model = transform;
		uniforms.view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);
		uniforms.projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.5f, 100.0f);

		// Send the model view and projection matrix to the gpu
		GLuint blockIndex;
		blockIndex = glGetUniformBlockIndex(material.shader, "Uniforms");
		glUniformBlockBinding(material.shader, blockIndex, m_uniformBlockBinding);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBuffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UniformFormat), &uniforms);

		// Draw object
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_INT, 0);
	}

	glfwSwapBuffers(m_glContext);
}

void Renderer::setCamera(size_t entityID)
{
	// TODO: Throw error if entity does not have a camera component
	m_cameraEntity = entityID;
}
