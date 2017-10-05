#include "RenderSystem.h"

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

RenderSystem::RenderSystem(GLFWwindow* glContext, const Scene& scene)
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

void RenderSystem::beginRender()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderSystem::endRender()
{
	glfwSwapBuffers(m_glContext);
}

void RenderSystem::update(size_t entityID)
{
	// Filter renderable entities
	const size_t kRenderableMask = COMPONENT_MESH | COMPONENT_MATERIAL | COMPONENT_TRANSFORM;
	if ((m_scene.componentMasks.at(entityID) & kRenderableMask) != kRenderableMask)
		return;

	// Get rendering components of entity
	const MaterialComponent& material = m_scene.materialComponents[entityID];
	const MeshComponent& mesh = m_scene.meshComponents.at(entityID);
	const mat4& transform = m_scene.transformComponents.at(entityID);

	// TODO: Add check that camera is a valid camera entity, throw error otherwise
	const mat4& cameraTransform = m_scene.transformComponents.at(m_cameraEntity);

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
	uniforms.view = glm::inverse(cameraTransform);
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

void RenderSystem::setCamera(size_t entityID)
{
	// TODO: Throw error if entity does not have a camera component
	m_cameraEntity = entityID;
}
