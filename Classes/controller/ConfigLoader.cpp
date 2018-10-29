#include "controller/ConfigLoader.h"
#include "ErrorLog.h"

using namespace cocos2d;

ConfigLoader::ConfigLoader()
{

}

ConfigLoader::~ConfigLoader()
{
	for (auto it = mMaterials.begin(); it != mMaterials.end(); it++) {
		it->second->release();
	}
	mMaterials.clear();

	for (u32 i = 0; i < mShaders.getNItems(); i++) {
		GLProgram* shader = (GLProgram*)mShaders.findByIndex(i);
		shader->release();
	}
	mShaders.clear(true);
}

void ConfigLoader::loadShader(const char* fragmentShaderFilename, const char* vertexShaderFilename, const char* name)
{
	GLProgram* shader = GLProgram::createWithFilenames(vertexShaderFilename, fragmentShaderFilename);
	shader->link();
	shader->updateUniforms();
	shader->retain();
	shader->addAttribute(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
	shader->addAttribute(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
	shader->addAttribute(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
	
	if (shader) {
		mShaders.addByHash(DRMakeStringHash(name), shader);
	}
	else {
		ErrorLog::printf("[ConfigLoader::loadShader] error loading shader: %s", name);
	}
}

void ConfigLoader::loadMaterial(const char* path, const char* name)
{
	Material* material = Material::createWithFilename(path);
	mMaterials.insert(std::pair<std::string, Material*>(name, material));
}
