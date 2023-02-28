#include <regex>

#include "nador/video/opengl3/shaders/ShaderOpenGL3.h"

namespace nador
{
	namespace ShaderOpenGL3
	{
		//==================================================
		const char* BaseVertexShader()
		{
			return R"(

					attribute vec4	aPosition;

					uniform mat4	uMVP;

					void main()
					{
						gl_Position = uMVP * aPosition;
					};

			)";
		}

		const char* BaseFragmentShader()
		{
			return R"(

					uniform vec4 uColor;

					void main()
					{
						gl_FragColor = uColor;
					};

			)";
		}

		//==================================================
		const char* TextureVertexShader()
		{
			return R"(

					attribute vec4	aPosition;
					attribute vec2	aTexCoord;

					varying vec2	vTexCoord;

					uniform mat4 uMVP;

					void main()
					{
						gl_Position = uMVP * aPosition;
						vTexCoord = aTexCoord;
					};

			)";
		}

		const char* TextureFragmentShader()
		{
			return R"(

					uniform sampler2D	uTexture;
					uniform vec4		uColor;

					varying vec2		vTexCoord;

					void main()
					{
						gl_FragColor = uColor * texture2D(uTexture, vTexCoord);
					}

			)";
		}

		//==================================================
		const char* FontVertexShader()
		{
			return R"(

					attribute vec4  aPosition;
					attribute vec2  aTexCoord;

					varying vec2    vTexCoord;

					uniform mat4    uMVP;

					void main(void)
					{
						gl_Position = uMVP * aPosition;
						vTexCoord = aTexCoord;
					}

			)";
		}

		const char* FontFragmentShader()
		{
			return R"(

					varying vec2        vTexCoord;

					uniform sampler2D   uTexture;
					uniform vec4        uColor;

					void main(void)
					{
						//gl_FragColor = vec4(1, 1, 1, texture2D(uTexture, vTexCoord).a) * uColor;


						vec4 texture = texture2D(uTexture, vTexCoord);
						if(vec3(texture) == vec3(0, 0, 0))
						{
							gl_FragColor = vec4(1, 1, 1, texture.a) * uColor;
						}
						else
						{
							gl_FragColor = uColor * texture;
						}
					}
			)";
		}

		//==================================================
		const char* RoundEdgeVertexShader()
		{
			return R"(

					attribute vec4	aPosition;
					attribute vec2  aTexCoord;

					varying vec2    vTexCoord;

					uniform mat4	uMVP;

					void main()
					{
						gl_Position = uMVP * aPosition;
						vTexCoord = aTexCoord;
					};

			)";
		}

		const char* RoundEdgeFragmentShader()
		{
			return R"(

					varying vec2    vTexCoord;

					uniform vec2	uDimensions;
					uniform float	uRadius;

					uniform vec4	uColor;

					void main()
					{
						vec2 coords = vTexCoord * uDimensions;
						
						vec2 a = vec2(0);
						vec2 b = vec2(0, uDimensions.y);
						vec2 c = vec2(uDimensions.x, 0);
						vec2 d = uDimensions;

						float l_a = length(coords - a);
						float l_b = length(coords - b);
						float l_c = length(coords - c);
						float l_d = length(coords - d);

						float length = uRadius;

						if (l_a < uRadius)
						{
							vec2 p = a + vec2(uRadius, uRadius);
							length = length(coords - p);
						}
						else if (l_b < uRadius)
						{
							vec2 p = b + vec2(uRadius, -uRadius);
							length = length(coords - p);
						}
						else if (l_c < uRadius)
						{
							vec2 p = c + vec2(-uRadius, uRadius);
							length = length(coords - p);
						}
						else if (l_d < uRadius)
						{
							vec2 p = d + vec2(-uRadius, -uRadius);
							length = length(coords - p);
						}

						if(length > uRadius)
						{
							discard;
						}

						gl_FragColor = uColor;
					};

			)";
		}

		//==================================================
		const char* BatchQuadVertexShader()
		{
			return R"(
					//#version 300 es

					precision mediump float;

					layout (location = 0) in vec4 aPosition;
					layout (location = 1) in vec4 aColor;
					layout (location = 2) in vec2 aTexCoord;
					layout (location = 3) in float aTexIndex;
					layout (location = 4) in float aIsText;					

					//varying vec2    vTexCoord;
					//varying vec4    vColor;
					//varying float   vTexIndex;
					//varying float   vIsText;

					out vec2    vTexCoord;
					out vec4    vColor;
					out float   vTexIndex;
					out float   vIsText;

					uniform mat4	uMVP;

					void main()
					{
						vTexCoord = aTexCoord;
						vColor = aColor;
						vTexIndex = aTexIndex;
						vIsText = aIsText;
						
						gl_Position = uMVP * aPosition;
					};

			)";
		}

		std::string BatchQuadFragmentShader(size_t textureSlots)
		{
			std::string result = R"(

					//#version 300 es

					precision mediump float;

					//varying vec2    vTexCoord;
					//varying vec4    vColor;
					//varying float   vTexIndex;
					//varying float   vIsText;

					in vec2    vTexCoord;
					in vec4    vColor;
					in float   vTexIndex;
					in float   vIsText;

					out vec4 outFragColor;

					uniform sampler2D	uTextures[??];

					void main()
					{	
						//vec4 texture = texture2D(uTextures[int(vTexIndex)], vTexCoord);
						vec4 texture = texture(uTextures[int(vTexIndex)], vTexCoord);
						
						if(bool(vIsText))
						{
							// To support font rendering
							//gl_FragColor = vec4(1, 1, 1, texture.a) * vColor;
							outFragColor = vec4(1, 1, 1, texture.a) * vColor;
						}
						else
						{
							//gl_FragColor = vColor * texture;
							outFragColor = vColor * texture;
						}
					};
			)";

			std::regex regex(R"(\?\?)");

			result = std::regex_replace(result, regex, std::to_string(textureSlots));

			return result;
		}
	}
}