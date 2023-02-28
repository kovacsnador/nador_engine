#ifndef __TEXTURE_SETTINGS_H__
#define __TEXTURE_SETTINGS_H__

namespace nador
{
	enum class TextureFilter
	{
		NONE_FILTER,
		LINEAR,
		NEAREST,
		MIPMAP_LINEAR,
		MIPMAP_NEAREST,
	};

	enum class TextureWrapping
	{
		NONE_WRAPPING,
		REPEAT,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE
	};

	enum class TextureType
	{
		TEXTURE_2D,
	};

	enum class DrawType
	{
		STATIC_DRAW,
		DYNAMINC_DRAW,
	};

	enum class ColorFormat
	{
		RGBA,
		ALPHA,
	};

	enum class PixelStore
	{
		DEFAULT_STORE,

		GL_PACK_ALIGNMENT_1,
		GL_PACK_ALIGNMENT_2,
		GL_PACK_ALIGNMENT_4,
		GL_PACK_ALIGNMENT_8,

		GL_UNPACK_ALIGNMENT_1,
		GL_UNPACK_ALIGNMENT_2,
		GL_UNPACK_ALIGNMENT_4,
		GL_UNPACK_ALIGNMENT_8
	};

	struct TextureSettings
	{
		TextureSettings()
		: target(TextureType::TEXTURE_2D)
		, filter(TextureFilter::LINEAR)
		, wrapping(TextureWrapping::CLAMP_TO_EDGE)
		, colorFormat(ColorFormat::RGBA)
		, pixelStore(PixelStore::DEFAULT_STORE)
		{
		}

		TextureSettings(TextureType target, TextureFilter filter, TextureWrapping wrapping, ColorFormat colorFormat, PixelStore pixelStore)
		: target(target)
		, filter(filter)
		, wrapping(wrapping)
		, colorFormat(colorFormat)
		, pixelStore(pixelStore)
		{
		}

		TextureType     target;
		TextureFilter   filter;
		TextureWrapping wrapping;
		ColorFormat     colorFormat;
		PixelStore		pixelStore;
	};
}

#endif // !__TEXTURE_SETTINGS_H__

