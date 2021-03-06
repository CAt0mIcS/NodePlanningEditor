#pragma once

#include "Ray/RBase.h"
#include "Ray/Components/RMesh.h"

#include "Ray/Core/RDynamicVertex.h"
#include "Ray/Graphics/Buffers/RIndexBuffer.h"


struct aiMesh;
struct aiMaterial;


namespace At0::Ray
{
	class Material;
	class Shader;

	class RAY_EXPORT Model
	{
	public:
		Model(std::string_view filepath, Ref<Material> material = nullptr);

		const Mesh::VertexData& GetVertexData() const { return m_VertexData; }

	private:
		void ParseMesh(std::string_view filepath, const aiMesh& mesh,
			const aiMaterial* const* pMaterials, Ref<Material> material);
		static Ref<Material> CreateMaterial(
			const std::string& basePath, const aiMesh& mesh, const aiMaterial* const* pMaterials);
		static DynamicVertex AssembleVertices(const aiMesh& mesh, const Shader& shader);
		static std::vector<IndexBuffer::Type> GenerateIndices(const aiMesh& mesh);

	private:
		Mesh::VertexData m_VertexData;

		bool m_ParentSet = false;
	};
}  // namespace At0::Ray
