#pragma once

#include "Ray/RBase.h"
#include "Core/RSharedBindable.h"
#include "Ray/Utils/RModel.h"
#include "Pipelines/Shader/RShader.h"

#include <string>
#include <unordered_map>
#include <mutex>


namespace At0::Ray
{
	/**
	 * Handles static bindables and models by hashing them by tag
	 */
	class RAY_EXPORT Codex
	{
	public:
		/**
		 * Checks if a bindable was already created by a previous drawable.
		 * If not, the codex will create it
		 */
		template<typename T, typename... Args>
		static Ref<T> Resolve(Args&&... args)
		{
			if constexpr (std::is_base_of_v<SharedBindable, T>)
				return Get().BindableResolve<T>(std::forward<Args>(args)...);
			else if constexpr (std::is_same_v<Shader, T>)
				return Get().ShaderResolve(std::forward<Args>(args)...);
		}

		/**
		 * Clears the codex map
		 */
		static void Shutdown() { Get().InternalShutdown(); }

	private:
		static Codex& Get()
		{
			static Codex codex;
			return codex;
		}

		template<typename T, typename... Args>
		Ref<T> BindableResolve(Args&&... args)
		{
			std::string tag = T::GetUID(args...);

			std::scoped_lock lock(m_BindableMutex);
			auto it = m_Bindables.find(tag);
			// Key does not exist, create it
			if (it == m_Bindables.end())
			{
				m_Bindables[tag] = MakeRef<T>(std::forward<Args>(args)...);
				// Log::Trace(
				//	"[Codex] SharedBindable (Tag=\"{0}\") was created because it didn't exist",
				//	tag);
				return std::static_pointer_cast<T>(m_Bindables[tag]);
			}
			// Key exists, return it
			else
			{
				// Log::Trace("[Codex] SharedBindable (Tag=\"{0}\") already exists", tag);
				return std::static_pointer_cast<T>(it->second);
			}
		}

		template<typename... Args>
		Ref<Shader> ShaderResolve(Args&&... args)
		{
			std::string tag = Shader::GetUID(args...);

			std::scoped_lock lock(m_ShaderMutex);
			auto it = m_Shaders.find(tag);
			// Key does not exist, create it
			if (it == m_Shaders.end())
			{
				m_Shaders[tag] = MakeRef<Shader>(std::forward<Args>(args)...);
				// Log::Trace("[Codex] Model (Tag=\"{0}\") was created because it didn't exist",
				// tag);
				return m_Shaders[tag];
			}
			// Key exists, return it
			else
			{
				// Log::Trace("[Codex] Model (Tag=\"{0}\") already exists", tag);
				return it->second;
			}
		}

		void InternalShutdown()
		{
			std::scoped_lock lockShaders(m_ShaderMutex);
			m_Shaders.clear();

			std::scoped_lock lockBindable(m_BindableMutex);
			m_Bindables.clear();
		}

	private:
		std::unordered_map<std::string, Ref<SharedBindable>> m_Bindables;
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
		std::mutex m_BindableMutex;
		std::mutex m_ShaderMutex;
	};
}  // namespace At0::Ray
