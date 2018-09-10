#pragma once

#include "../../Core/Common.h"
#include "../../DataStructures/String.h"
#include "../../Rendering/ShaderType.h"
#include <GL\glew.h>
#include <map>
#include "../../Rendering/ResourceTypes.h"

namespace Eunoia { namespace Rendering {

	class ShaderManagerGL : private ResourceIDAssigner
	{
	public:
		ShaderManagerGL();
		~ShaderManagerGL();

		ShaderID CreateShader(const String& shaderFile);
		void DeleteShader(ShaderID shaderID);

		void BindShader(ShaderID shaderID) const;

		void BindTextureToShader(ShaderID shaderID, TextureID textureID, const String& nameOfTextureInShader) const;
	private:
		struct ShaderInfoGL;
		static void ParseShader(const String& src, ShaderInfoGL& shader);

		static GLuint CreateShaderGL(const String& src, GLuint program, GLenum type);
		static void LinkAndValidate(GLuint program);
		static void CheckForError(GLuint shader, GLuint flag, bool isProgram, const String& errorMessage);
	private:
		struct ShaderInfoGL
		{
			GLuint program;
			GLuint shaders[NUM_SHADER_TYPES];
			std::map<String, TextureUnit> textureUnits;
			std::map<String, GLuint> uniformLocations;
		};
	private:
		std::vector<ShaderInfoGL> m_shaders;
	};

} }