#pragma once

#include <glew.h>
#include <glm.hpp>
#include <string>

class Shader
{
public:
	unsigned int Id;

	Shader(const char* VertexPath, const char* FragmentPath);

	void use() const;

	void setBool(const std::string& Name, bool Value) const;
	void setInt(const std::string& Name, int Value) const;
	void setFloat(const std::string& Name, float Value) const;
	void setVec3(const std::string& Name, const glm::vec3& Value) const;
	void setVec3(const std::string& Name, float X, float Y, float Z) const;
	void setMat4(const std::string& Name, const glm::mat4& Mat) const;

private:
	static void checkCompileErrors(unsigned int Shader, const std::string& Type);
};
