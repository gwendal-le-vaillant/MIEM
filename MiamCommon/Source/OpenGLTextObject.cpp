/*
  ==============================================================================

    OpenGLTextObject.cpp
    Created: 5 Jul 2018 11:38:16am
    Author:  ayup1

  ==============================================================================
*/

#include "OpenGLTextObject.h"
using namespace std;

OpenGLTextObject::OpenGLTextObject(String path, float _x, float _y, float _characterWidth, float _characterHeight, int _maxSize) : 
	textX(_x), textY(_y), characterWidth(_characterWidth), characterHeight(_characterHeight), maxSize(_maxSize)
{
	textTexture = std::unique_ptr<OpenGLTexture>( new OpenGLTexture());

	bool isBinary = false;
	int resourceId = 0;
	for (int i = 0; i < BinaryData::namedResourceListSize; ++i)
	{
		if (path == BinaryData::namedResourceList[i])
		{
			isBinary = true;
			resourceId = i;
		}
	}

	if (isBinary)
	{
		int dataSize = 0;
		const void * srcData = BinaryData::getNamedResource(BinaryData::namedResourceList[resourceId], dataSize);
		image = resizeImageToPowerOfTwo(ImageCache::getFromMemory(srcData,dataSize));
	}
	else
		image = resizeImageToPowerOfTwo(ImageFileFormat::loadFrom(File(path)));

	g_vertex_buffer_data = new GLfloat[maxSize * 6 * 3];
	g_UV_buffer_data = new GLfloat[maxSize * 6 * 2];
	computeVertices();
	//std::string testString = "                      ";
	for(int i = 0; i < maxSize; ++i)
		computeUV(i,' ');
}

OpenGLTextObject::~OpenGLTextObject()
{
	delete[] g_vertex_buffer_data;
	delete[] g_UV_buffer_data;
}

void OpenGLTextObject::initialiseText(OpenGLContext& context)
{
	initialiseShaderProgram(context);
	initialiseBuffer(context);
	initialiseAttribute();
}

void OpenGLTextObject::initialiseBuffer(OpenGLContext& context)
{
	context.extensions.glGenBuffers(1, &vertexBuffer);
	context.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	context.extensions.glBufferData(GL_ARRAY_BUFFER, maxSize * 6 * 3 * sizeof(GLfloat), g_vertex_buffer_data, GL_STATIC_DRAW);

	context.extensions.glGenBuffers(1, &UVBuffer);
	context.extensions.glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	context.extensions.glBufferData(GL_ARRAY_BUFFER, maxSize * 6 * 2 * sizeof(GLfloat), g_UV_buffer_data, GL_STATIC_DRAW);
}

void OpenGLTextObject::initialiseAttribute()
{
	positionText = new OpenGLShaderProgram::Attribute(*textShaderProgram, "position");
	vertexUV = new OpenGLShaderProgram::Attribute(*textShaderProgram, "uvCoord");

	textProjectionMatrix = new OpenGLShaderProgram::Uniform(*textShaderProgram, "projectionMatrix");
	textViewMatrix = new OpenGLShaderProgram::Uniform(*textShaderProgram, "viewMatrix");
	textModelMatrix = new OpenGLShaderProgram::Uniform(*textShaderProgram, "modelMatrix");
	texture = new OpenGLShaderProgram::Uniform(*textShaderProgram, "demoTexture");

	textTexture->loadImage(image);
}

void OpenGLTextObject::initialiseShaderProgram(OpenGLContext &context)
{
	textShaderProgram = new OpenGLShaderProgram(context);
	textShaderProgram->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(myTextVertexShader));
	textShaderProgram->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(myTextFragmentShader));
	textShaderProgram->link();

	textShaderProgram->use();
}

void OpenGLTextObject::drawOneTexturedRectangle(OpenGLContext &context, juce::Matrix3D<float> &model, juce::Matrix3D<float> &testView, juce::Matrix3D<float> &testPerspective, std::wstring &stringToDraw)
{
	//std::string stringToDraw = "BB";
	computeVertices();
	for (int i = 0; i < stringToDraw.length(); ++i)
		computeUV(i, wchar_t(stringToDraw[i]));
	for(int i = stringToDraw.length(); i < maxSize;++i)
		computeUV(i, ' ');
	context.extensions.glBindBuffer(GL_ARRAY_BUFFER, 0);
	context.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	textShaderProgram->use();
	/*model = Matrix3D<float>(1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	3.0f, 4.0f, 0.0f, 1.0f);*/

	if (textModelMatrix != nullptr)
		textModelMatrix->setMatrix4(model.mat, 1, false);

	if (textProjectionMatrix != nullptr)
		textProjectionMatrix->setMatrix4(testPerspective.mat, 1, false);

	if (textViewMatrix != nullptr)
		textViewMatrix->setMatrix4(testView.mat, 1, false);

	if (textTexture != nullptr)
		textTexture->loadImage(image);
	if (texture != nullptr)
		texture->set(0);



	//model = Matrix3D<float>(1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	-3.0f, 4.0f, 0.0f, 1.0f);

	context.extensions.glEnableVertexAttribArray(positionText->attributeID);
	context.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//context.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, maxSize * 6 * 3 * sizeof(GLfloat), g_oneVertex_buffer_data);
	context.extensions.glVertexAttribPointer(positionText->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(float[3]), 0);

	context.extensions.glEnableVertexAttribArray(vertexUV->attributeID);
	context.extensions.glBindBuffer(GL_ARRAY_BUFFER, UVBuffer); // changer �a!
	context.extensions.glBufferSubData(GL_ARRAY_BUFFER, 0, maxSize * 6 * 2 * sizeof(GLfloat), g_UV_buffer_data);
	context.extensions.glVertexAttribPointer(vertexUV->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(float[2]), 0);

	glDrawArrays(GL_TRIANGLES, 0, maxSize * 2 * 3);

	context.extensions.glDisableVertexAttribArray(positionText->attributeID);
	context.extensions.glDisableVertexAttribArray(vertexUV->attributeID);

	if(textTexture != nullptr)
		textTexture->loadImage(image);
}

void OpenGLTextObject::computeVertices()
{
	for (int i = 0; i < maxSize; ++i)
	{
		g_vertex_buffer_data[i * 18 + 0] = textX + i * characterWidth;
		g_vertex_buffer_data[i * 18 + 1] = textY + 0.0f;
		g_vertex_buffer_data[i * 18 + 2] = 0.0f;

		g_vertex_buffer_data[i * 18 + 3] = textX + (i + 1) * characterWidth;
		g_vertex_buffer_data[i * 18 + 4] = textY + 0.0f;
		g_vertex_buffer_data[i * 18 + 5] = 0.0f;

		g_vertex_buffer_data[i * 18 + 6] = textX + (i + 1) * characterWidth;
		g_vertex_buffer_data[i * 18 + 7] = textY + characterHeight;
		g_vertex_buffer_data[i * 18 + 8] = 0.0f;

		g_vertex_buffer_data[i * 18 + 9] = textX + i * characterWidth;
		g_vertex_buffer_data[i * 18 + 10] = textY + characterHeight;
		g_vertex_buffer_data[i * 18 + 11] = 0.0f;

		g_vertex_buffer_data[i * 18 + 12] = textX + (i + 1) * characterWidth;
		g_vertex_buffer_data[i * 18 + 13] = textY + characterHeight;
		g_vertex_buffer_data[i * 18 + 14] = 0.0f;

		g_vertex_buffer_data[i * 18 + 15] = textX + i * characterWidth;
		g_vertex_buffer_data[i * 18 + 16] = textY + 0.0f;
		g_vertex_buffer_data[i * 18 + 17] = 0.0f;
	}
}

void OpenGLTextObject::computeUV(int idx, wchar_t character)
{
	float wRatio = (39.0f / 1024.0f); // width of character in % of texture width
	float hRatio = 1.0f / 16.0f; // height of character in % of texture height

	unsigned char test = '�';
	float xNew = wRatio * float(character % 16);//float(character % 16) * wRatio;//float(character % 16) / 8.0f;
	float yNew = 1.0f - float(int(character / 16) - 2) * hRatio;//1 - float(int(character / 16)) * hRatio;//(7 - (float(character / 8) - 4)) / 8.0f;//(float(character / 16)-1.0f) / 8.0f;

	std::wstring testString = L"���";
	int strSz = testString.length();
	wchar_t firstChar = testString[0];
	wchar_t scndChar = testString[1];
	wchar_t lastChar = testString[2];

	// UV :30------>1
	//     | \      |
	//     |  \     |
	//     |   \    |
	//     |    \   |
	//     |     \  |
	//     |      V V
	//     5------>42



	// coin inferieur droit

	g_UV_buffer_data[idx * 12 + 0] = xNew;//xNew + 0.0f;
	g_UV_buffer_data[idx * 12 + 1] = yNew - hRatio;//0.0f;//yNew - hRatio;

	g_UV_buffer_data[idx * 12 + 2] = xNew + wRatio;//1.0f;//xNew + wRatio;
	g_UV_buffer_data[idx * 12 + 3] = yNew - hRatio; //0.0f;//yNew - hRatio;

	g_UV_buffer_data[idx * 12 + 4] = xNew + wRatio; //1.0f;//xNew + wRatio;
	g_UV_buffer_data[idx * 12 + 5] = yNew; //1.0f;//yNew - 2 * hRatio;


	//coin superieur gauche

	g_UV_buffer_data[idx * 12 + 6] = xNew;//xNew + 0.0f;
	g_UV_buffer_data[idx * 12 + 7] = yNew;//1.0f;//yNew - hRatio;

	g_UV_buffer_data[idx * 12 + 8] = xNew + wRatio; //1.0f;//xNew + wRatio;
	g_UV_buffer_data[idx * 12 + 9] = yNew; //1.0f;//yNew - 2 * hRatio;

	g_UV_buffer_data[idx * 12 + 10] = xNew;//xNew + 0.0f;
	g_UV_buffer_data[idx * 12 + 11] = yNew - hRatio;;//0.0f;//yNew - 2 * hRatio;
}

void OpenGLTextObject::release()
{
	textTexture->release();
	textTexture = nullptr;
}