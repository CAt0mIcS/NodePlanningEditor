#include "Rpch.h"
#include "RFlatTextureMaterial.h"

#include "Graphics/RCodex.h"


namespace At0::Ray
{
	FlatTextureMaterial::FlatTextureMaterial(Layout layout, GraphicsPipeline::Layout pipelineLayout)
		: m_Layout(std::move(layout))
	{
		if (!pipelineLayout.shader)
			pipelineLayout.shader = Codex::Resolve<Shader>(ChooseShaders());

		m_GraphicsPipeline = Codex::Resolve<GraphicsPipeline>(std::move(pipelineLayout));
	}

	std::vector<std::string> At0::Ray::FlatTextureMaterial::ChooseShaders()
	{
		std::vector<std::string> shaders;
		std::vector<std::string> shaderCodes;

		shaderCodes.emplace_back(ShaderFileCode::DiffuseMap);

		// Sort shader codes alphabetically
		std::sort(shaderCodes.begin(), shaderCodes.end());

		std::string shaderCode =
			std::accumulate(shaderCodes.begin(), shaderCodes.end(), std::string{});
		shaders.emplace_back("Resources/Shaders/Flat" + shaderCode + ".vert");
		shaders.emplace_back("Resources/Shaders/Flat" + shaderCode + ".frag");
		return shaders;
	}
}  // namespace At0::Ray
