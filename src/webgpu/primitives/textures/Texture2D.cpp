#include "Texture2D.hpp"


tinyrender::Texture2D::Texture::Texture(Context *context, tinyrender::Texture2D::DataDelegate *data,
                           tinyrender::Texture2D::CreationDelegate *create, tinyrender::Texture2D::ViewDelegate *view,
                           tinyrender::Texture2D::SamplerDelegate *samp):
_data(data),
_create(create),
_view(view),
_samp(samp)
{
    this->context = context;

    // Data delegate gives us information on how big the texture is
    _data->initialise(this->width, this->height, this->mips);

    // Create the texture and write to the queue to populate it
    underlying = _create->createTexture(context, this->width, this->height, this->mips);
    _data->write(context, underlying);

    // Create the view needed
    textureView = _view->createView(underlying, mips);

    // Create the sampler needed
    sampler = _samp->createSampler(context, mips);
}

tinyrender::Texture2D::Texture::~Texture() {
    // Delete delegates
    delete _data;
    delete _create;
    delete _view;
    delete _samp;

    // Delete WebGPU resources
    if(sampler != nullptr)
        sampler.release();
    if(textureView != nullptr)
        textureView.release();
    underlying.destroy();
    underlying.release();
}

