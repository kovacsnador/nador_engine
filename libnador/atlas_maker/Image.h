#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "IImage.h"
#include "cimage.h"

namespace am {

	class Image : public IImage {
	public:
		using img_t = cimg_library::CImg<byte_t>;

		static Image* CreateImage(const std::string& file_path);

		virtual int			GetWidth() const override;
		virtual int			GetHeight() const override;
		virtual std::string GetName() const override;
		virtual int			GetDataSize() const;

		virtual const byte_t* GetRow(const int row) const override; 
		virtual const byte_t* GetData() const override;
		virtual byte_t*		  GetData();

		const img_t&			GetImage() const { return _image; }
		virtual sp::SimpleImage GetSimpleImageClone() const override;
		virtual int				SetSprectrum(const int channel) override;
		virtual int				GetSprectrum() const override;

		virtual ~Image();

	protected:
		Image(const std::string& file_path);
		Image(const int width, const int height);
		Image(const int width, const int height, const int depth, const int channels);

		img_t		_image;
		std::string	_file_path;
	};


}


#endif //! __IMAGE_H__

