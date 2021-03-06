/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <globjects/Texture.h>

#include <gloperate/resources/Loader.h>


namespace gloperate
{


/**
*  @brief
*    Loader for glraw textures
*/
class GLOPERATE_API GlrawTextureLoader : public Loader<globjects::Texture> {


    public:
        /**
        *  @brief
        *    Constructor
        */
        GlrawTextureLoader();

        /**
        *  @brief
        *    Destructor
        */
        virtual ~GlrawTextureLoader();

        // Virtual gloperate::AbstractLoader functions
        virtual bool canLoad(const std::string & ext) const override;
        virtual std::vector<std::string> loadingTypes() const override;
        virtual std::string allLoadingTypes() const override;

        // Virtual gloperate::Loader<globjects::Texture> functions
        virtual globjects::Texture * load(const std::string & filename) const override;


};


} // namespace gloperate
