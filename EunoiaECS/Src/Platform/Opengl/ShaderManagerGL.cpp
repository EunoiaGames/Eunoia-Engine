#include "ShaderManagerGL.h"
#include "../../Utils/FileReader.h"
#include "../../Rendering/ResourceManager.h"

namespace Eunoia { namespace Rendering {

	ShaderManagerGL::ShaderManagerGL()
	{

	}

	ShaderManagerGL::~ShaderManagerGL()
	{
		for (uint32 i = 0; i < m_shaders.size(); i++)
		{
			const ShaderInfoGL& shader = m_shaders[i];
			for (uint32 j = 0; j < NUM_SHADER_TYPES; j++)
			{
				if (shader.shaders[j])
				{
					glDetachShader(shader.program, shader.shaders[j]);
					glDeleteShader(shader.shaders[j]);
				}
			}
			glDeleteProgram(shader.program);
		}
	}

	ShaderID ShaderManagerGL::CreateShader(const String & shaderFile)
	{
		ShaderInfoGL shader;
		shader.program = glCreateProgram();

		String shaderSrc = FileReader::ReadFile(String("Res/Shaders/GLSL/") + shaderFile + ".glsl", true);

		int32 vertexStart = shaderSrc.FindFirstOf("#vertex");
		int32 fragmentStart = shaderSrc.FindFirstOf("#fragment");

		String vertexSrc = shaderSrc.SubString(shaderSrc.FindFirstOf("#version", vertexStart), fragmentStart);
		String fragmentSrc = shaderSrc.SubString(shaderSrc.FindFirstOf("#version", fragmentStart));

		shader.shaders[SHADER_TYPE_VERTEX] = CreateShaderGL(vertexSrc, shader.program, GL_VERTEX_SHADER);
		shader.shaders[SHADER_TYPE_PIXEL] = CreateShaderGL(fragmentSrc, shader.program, GL_FRAGMENT_SHADER);

		LinkAndValidate(shader.program);

		ParseShader(shaderSrc, shader);

		ShaderID id = GetNewID();
		if (id - 1 == m_shaders.size())
			m_shaders.push_back(shader);
		else
			m_shaders[id - 1] = shader;
		return id;
	}

	void ShaderManagerGL::DeleteShader(ShaderID shaderID)
	{
		const ShaderInfoGL& shader = m_shaders[shaderID - 1];
		for (uint32 i = 0; i < NUM_SHADER_TYPES; i++)
		{
			if (shader.shaders[i])
			{
				glDetachShader(shader.program, shader.shaders[i]);
				glDeleteShader(shader.shaders[i]);
			}
		}
		glDeleteProgram(shader.program);
		FreeID(shaderID);
	}

	void ShaderManagerGL::BindShader(ShaderID shaderID) const
	{
		glUseProgram(m_shaders[shaderID - 1].program);
	}

	void ShaderManagerGL::BindTextureToShader(ShaderID shaderID, TextureID textureID, const String & nameOfTextureInShader) const
	{
		const ShaderInfoGL& shader = m_shaders[shaderID - 1];
		const auto&& it = shader.textureUnits.find(nameOfTextureInShader);
		if (it == shader.textureUnits.end())
			return;
		glUniform1i(shader.uniformLocations.at(nameOfTextureInShader), it->second);
		ResourceManager::pTextureManager->BindTexture(textureID, it->second);
	}

	void ShaderManagerGL::ParseShader(const String & src, ShaderInfoGL& shader)
	{
		String uniformKeyWord = "uniform";
		
		uint32 nextTextureUnit = 0;
		int32 uniformIndex = src.FindFirstOf(uniformKeyWord);
		while (uniformIndex != -1)
		{
			int32 typeNameIndex = src.FindFirstNotOf(" ", uniformIndex + uniformKeyWord.Length() + 1);
			int32 spaceAfterTypeNameIndex = src.FindFirstOf(" ", typeNameIndex);
			int32 semicolonIndex = src.FindFirstOf(";", spaceAfterTypeNameIndex);

			String typeName = src.SubString(typeNameIndex, spaceAfterTypeNameIndex);
			String uniformName = src.SubString(spaceAfterTypeNameIndex, semicolonIndex);
			uniformName = uniformName.TrimBeginning();
			uniformName = uniformName.TrimEnd();

			GLint location = glGetUniformLocation(shader.program, uniformName.C_Str());
			EU_ASSERT(location != -1, "Invalid opengl uniform location");
			shader.uniformLocations[uniformName] = location;

			if (typeName == "sampler2D")
			{
				shader.textureUnits[uniformName] = nextTextureUnit++;
			}

			uniformIndex = src.FindFirstOf(uniformKeyWord, semicolonIndex);
		}
	}

	GLuint ShaderManagerGL::CreateShaderGL(const String & src, GLuint program, GLenum type)
	{
		GLuint shader = glCreateShader(type);
		
		const GLchar* lines[1] = { src.C_Str() };
		GLint lengths[1] = { (int32)src.Length() };
		glShaderSource(shader, 1, lines, lengths);
		glCompileShader(shader);
		CheckForError(shader, GL_COMPILE_STATUS, false, "Error creating opengl shader");
		glAttachShader(program, shader);

		return shader;
	}

	void ShaderManagerGL::LinkAndValidate(GLuint program)
	{
		glLinkProgram(program);
		CheckForError(program, GL_LINK_STATUS, true, "Error: Failed to link shader program: ");
		glValidateProgram(program);
		CheckForError(program, GL_VALIDATE_STATUS, true, "Error: Failed to validate program: ");
	}

	void ShaderManagerGL::CheckForError(GLuint shader, GLuint flag, bool isProgram, const String& errorMessage)
	{
		GLint success = 0;
		GLchar error[1024] = { 0 };

		if (isProgram)
		{
			glGetProgramiv(shader, flag, &success);
		}
		else
		{
			glGetShaderiv(shader, flag, &success);
		}

		if (success == GL_FALSE)
		{
			if (isProgram)
			{
				glGetProgramInfoLog(shader, sizeof(error), NULL, error);
			}
			else
			{
				glGetShaderInfoLog(shader, sizeof(error), NULL, error);
			}

			std::cerr << errorMessage << ": '" << error << "'" << std::endl;
		}
	}

} }
